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

        while (servidorActivo) {
            Cliente nuevoCliente = socketServer.esperarCliente();
            if (nuevoCliente != null) {
                clients.add(nuevoCliente);
                new Thread(new ClientHandler(nuevoCliente)).start();
            }
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
        observadores.computeIfAbsent(jugador, k -> ConcurrentHashMap.newKeySet()).add(espectador);
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
        // Crear el JSON con IDs y nombres de los jugadores
        String jugadoresJson = JsonProcessor.getInstance().crearMensajeClientesLista(
                jugadores.stream().map(Cliente::getId).toList(),
                jugadores.stream().map(Cliente::getNombre).toList()
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

    public void eliminarEspectadorPorId(String idObservador) {
        for (Map.Entry<Cliente, Set<Cliente>> entry : observadores.entrySet()) {
            Cliente jugador = entry.getKey();
            Set<Cliente> observadoresDelJugador = entry.getValue();

            // Buscar el observador en el conjunto de observadores de este jugador
            observadoresDelJugador.removeIf(observador -> observador.getId().equals(idObservador));

            // Si después de eliminar el observador el conjunto queda vacío, puedes decidir si quieres eliminar la entrada
            if (observadoresDelJugador.isEmpty()) {
                observadores.remove(jugador);
            }
        }
    }


    public Object getSocketServer() {
        return this.socketServer;
    }
}
