package org.proyectosce.comunicaciones;

import java.util.*;
import org.proyectosce.comandos.factory.products.Command;
import org.proyectosce.comandos.CommandHandler;
import org.proyectosce.comandos.factory.CommandFactory;
import org.proyectosce.comandos.factory.products.PowerCommand;
import org.proyectosce.ui.MainWindow;

import javax.swing.*;
import java.io.IOException;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.concurrent.ConcurrentHashMap;
import java.util.function.BiConsumer;

/*
 * Clase ComServer: Maneja la lógica general de comunicaciones.
 * Se encarga de iniciar el servidor, manejar clientes, y ejecutar comandos.
 */
public class ComServer {
    private volatile boolean servidorActivo = true;
    private static ComServer instance;
    private final Set<Cliente> clients = ConcurrentHashMap.newKeySet();  // Clientes conectados
    private final Set<Cliente> jugadores = ConcurrentHashMap.newKeySet();  // Lista de jugadores
    private final Map<Cliente, Set<Cliente>> observadores = new ConcurrentHashMap<>();  // Observadores de cada jugador
    private final Set<Cliente> espectadoresTemporales = ConcurrentHashMap.newKeySet();  // Espectadores temporales

    private final SocketServer socketServer = SocketServer.getInstance();
    private final CommandHandler commandHandler = new CommandHandler();
    private final CommandFactory commandFactory = CommandFactory.getInstance();
    private final Set<String> powersAllowed = Set.of("add_life", "add_ball", "double_racket_size", "half_racket_size", "add_speed", "reduce_speed");


    private ServerSocketChannel serverSocketChannel;
    private BiConsumer<List<String>, List<String>> updateCallback;
    private MainWindow mainWindow;

    private ComServer() {}

    public static synchronized ComServer getInstance() {
        if (instance == null) {
            instance = new ComServer();
        }
        return instance;
    }

    public void setMainWindow(MainWindow mainWindow) {
        this.mainWindow = mainWindow;
    }

    public void setUpdateCallback(BiConsumer<List<String>, List<String>> callback) {
        this.updateCallback = callback;
    }

    public void iniciarServidor() {
        socketServer.abrirPuerto();
        new Thread(this::iniciarConsola).start();

        while (servidorActivo) {
            Cliente nuevoCliente = socketServer.esperarCliente();
            if (nuevoCliente != null) {
                clients.add(nuevoCliente);
                new Thread(new ClientHandler(nuevoCliente)).start();
            }
        }
    }

    private void iniciarConsola() {
        Scanner scanner = new Scanner(System.in);
        while (scanner.hasNextLine()) {
            String input = scanner.nextLine();
            if (input.startsWith("brick_pwr")) {
                procesarComandoConsola(input);
            } else {
                System.out.println("Comando no reconocido. Intente con: brick_pwr (f,c,power)");
            }
        }
    }

    private void procesarComandoConsola(String input) {
        try {
            String[] partes = input.replace("brick_pwr", "").replace("(", "").replace(")", "").split(",");
            if (partes.length != 3) {
                System.out.println("Formato de comando incorrecto. Use: brick_pwr (f,c,power)");
                return;
            }

            String fStr = partes[0].trim();
            String cStr = partes[1].trim();
            String power = partes[2].trim();

            if (!powersAllowed.contains(power)) {
                System.out.println("Poder inválido. Poderes permitidos: " + powersAllowed);
                return;
            }

            Map<String, Object> jsonData = Map.of(
                    "x", Integer.parseInt(cStr),
                    "y", Integer.parseInt(fStr),
                    "power", power
            );

            Command comando = commandFactory.crearComando("brick_pwr", "", null, jsonData);

            Cliente primerCliente = clients.stream().findFirst().orElse(null);
            if (primerCliente != null) {
                commandHandler.handleCommand((PowerCommand) comando, primerCliente);
            } else {
                System.out.println("No hay clientes conectados para enviar el comando.");
            }
        } catch (NumberFormatException e) {
            System.out.println("Las coordenadas f y c deben ser enteros.");
        }
    }

    public void eliminarCliente(Cliente cliente) {
        clients.remove(cliente);
        jugadores.remove(cliente);
        observadores.remove(cliente);
        actualizarListas();
    }



    public void registrarEspectadorTemporal(Cliente cliente) {
        // Agrega al cliente a la lista de espectadores temporales
        espectadoresTemporales.add(cliente);
        // Si el cliente es un espectador, le enviamos la lista de jugadores
        enviarListaDeJugadores(cliente);
    }

    public void eliminarObservador(Cliente jugador, Cliente espectador) {
        observadores.getOrDefault(jugador, ConcurrentHashMap.newKeySet()).remove(espectador);
        actualizarListas();
    }

    public void registrarJugador(Cliente cliente) {
        jugadores.add(cliente);
        observadores.putIfAbsent(cliente, ConcurrentHashMap.newKeySet());
        actualizarListas();
    }

    public void registrarObservador(Cliente jugador, Cliente espectador) {
        observadores.getOrDefault(jugador, ConcurrentHashMap.newKeySet()).add(espectador);
        actualizarListas();
    }

    private List<String> obtenerNombresClientes() {
        return jugadores.stream().map(Cliente::getId).toList();
    }

    private List<String> obtenerNombresEspectadores() {
        return observadores.values().stream()
                .flatMap(Set::stream)
                .map(Cliente::getId)
                .distinct()
                .toList();
    }

    private void actualizarListas() {
        List<String> jugadoresNames = obtenerNombresClientes();
        List<String> espectadoresNames = obtenerNombresEspectadores();

        if (mainWindow != null) {
            SwingUtilities.invokeLater(() -> mainWindow.updateClientLists(jugadoresNames, espectadoresNames));
        }
    }

    public Cliente obtenerClientePorId(String id) {
        return jugadores.stream()
                .filter(cliente -> cliente.getId().equals(id))
                .findFirst()
                .orElse(null);
    }

    public void enviarListaDeJugadores(Cliente espectador) {
        String jugadoresJson = JsonProcessor.getInstance().crearMensajeClientesLista(
                jugadores.stream().map(Cliente::getId).toList()
        );
        socketServer.enviarMensaje(espectador, jugadoresJson);
    }

    // Verificar si el cliente es un jugador
    public boolean esJugador(Cliente cliente) {
        return jugadores.contains(cliente);
    }

    // Eliminar un jugador de la lista de jugadores
    public void eliminarJugador(Cliente cliente) {
        jugadores.remove(cliente);
    }

    // Obtener los observadores de un jugador
    public Set<Cliente> obtenerObservadores(Cliente cliente) {
        return observadores.getOrDefault(cliente, new HashSet<>());
    }

    // Eliminar todos los observadores de un jugador
    public void eliminarObservadores(Cliente cliente) {
        Set<Cliente> observadoresDelJugador = obtenerObservadores(cliente);
        for (Cliente espectador : observadoresDelJugador) {
            // Aquí puedes agregar la lógica para notificar a los observadores si es necesario
            // Por ejemplo, desconectar a un espectador
            eliminarEspectador(espectador);
        }
        observadores.remove(cliente);  // Eliminar la relación jugador-observadores
    }

    // Eliminar un espectador de la lista de espectadores
    public void eliminarEspectador(Cliente cliente) {
        observadores.remove(cliente);
    }



}
