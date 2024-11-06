package org.proyectosce.comunicaciones;

import java.util.*;
import org.proyectosce.comandos.factory.products.Command;
import org.proyectosce.comandos.CommandHandler;
import org.proyectosce.comandos.factory.CommandFactory;
import org.proyectosce.comandos.factory.products.PowerCommand;
import java.util.Scanner;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

/*
 * Class: ComServer
 *
 * Clase que maneja la lógica general de comunicaciones.
 * Se encarga de iniciar el servidor de escucha, manejar clientes y ejecutar comandos.
 */
public class ComServer {
    private volatile boolean servidorActivo = true;
    private static ComServer instance;
    private final Set<Cliente> clients = ConcurrentHashMap.newKeySet();
    private final Set<Cliente> jugadores = ConcurrentHashMap.newKeySet();  // Lista de jugadores
    private final Map<Cliente, Set<Cliente>> observadores = new ConcurrentHashMap<>();  // Observadores de cada jugador
    private final SocketServer socketServer = SocketServer.getInstance();
    private final CommandHandler commandHandler = new CommandHandler();
    private final CommandFactory commandFactory = new CommandFactory();
    private final Set<String> powersAllowed = Set.of("add_life", "add_ball", "double_racket_size", "half_racket_size", "add_speed", "reduce_speed");

    // Singleton
    private ComServer() {}

    public static synchronized ComServer getInstance() {
        if (instance == null) {
            instance = new ComServer();
        }
        return instance;
    }



    public void iniciarServidor() {
        SocketServer socketServer = SocketServer.getInstance();
        socketServer.abrirPuerto();
        new Thread(this::iniciarConsola).start();

        while (servidorActivo) {
            Cliente nuevoCliente = socketServer.esperarCliente();
            if (nuevoCliente != null) {
                clients.add(nuevoCliente);
                ClientHandler clientHandler = new ClientHandler(nuevoCliente);
                Thread clientThread = new Thread(clientHandler);
                clientThread.start();
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

            // Verificar si el poder está permitido
            if (!powersAllowed.contains(power)) {
                System.out.println("Poder inválido. Poderes permitidos: " + powersAllowed);
                return;
            }

            // Crear el comando usando la fábrica
            Command comando = (Command) commandFactory.crearComando("brick_pwr", fStr, cStr, power);

            // Obtener el primer cliente conectado del set
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

    public void detenerServidor() {
        servidorActivo = false;
        // Otras acciones para cerrar conexiones y liberar recursos
    }

    public Cliente obtenerClientePorId(String id) {
        for (Cliente cliente : jugadores) {
            if (cliente.getId().equals(id)) {  // Usa el ID único de cada cliente
                return cliente;
            }
        }
        return null;  // Retorna null si no encuentra el cliente
    }

    // Método para enviar un mensaje a todos los clientes conectados
    public void enviarMensajeATodos(String mensajeJson) {
        SocketServer socketServer = SocketServer.getInstance();
        for (Cliente cliente : clients) {
            socketServer.enviarMensaje(cliente, mensajeJson);
        }
    }

    // Registrar un cliente como jugador y crear su lista de observadores
    public void registrarJugador(Cliente cliente) {
        jugadores.add(cliente);
        observadores.putIfAbsent(cliente, ConcurrentHashMap.newKeySet());
    }

    // Registrar un espectador temporal (se añade al seleccionar un jugador para observar)
    public void registrarEspectadorTemporal(Cliente cliente) {
        // Este método puede gestionar a espectadores en espera si es necesario
    }

    // Enviar la lista de jugadores a un espectador
    public void enviarListaDeJugadores(Cliente espectador) {
        SocketServer socketServer = SocketServer.getInstance();
        List<String> listaDeJugadores = new ArrayList<>();

        for (Cliente jugador : jugadores) {
            listaDeJugadores.add(jugador.getId()); // Asume que `toString()` devuelve el ID o nombre del jugador
        }

        // Convertir la lista de jugadores a JSON
        String jugadoresJson = JsonProcessor.getInstance().crearMensajeClientesLista(listaDeJugadores);
        socketServer.enviarMensaje(espectador, jugadoresJson);
    }

    // Añadir un espectador a la lista de observadores de un jugador específico
    public void registrarObservador(Cliente jugador, Cliente espectador) {
        observadores.getOrDefault(jugador, ConcurrentHashMap.newKeySet()).add(espectador);
    }

    // Obtener la lista de observadores de un jugador específico
    public Set<Cliente> obtenerObservadores(Cliente jugador) {
        return observadores.getOrDefault(jugador, Collections.emptySet());
    }
}
