package org.proyectosce.comunicaciones;

import java.util.*;
import java.util.concurrent.ConcurrentHashMap;

/*
 * Class: ComServer
 *
 * Clase que maneja la lógica general de comunicaciones.
 * Se encarga de iniciar el servidor de escucha, manejar clientes y ejecutar comandos.
 */
public class ComServer {
    private static ComServer instance;
    private final Set<Cliente> clients = ConcurrentHashMap.newKeySet();
    private final Set<Cliente> jugadores = ConcurrentHashMap.newKeySet();  // Lista de jugadores
    private final Map<Cliente, Set<Cliente>> observadores = new ConcurrentHashMap<>();  // Observadores de cada jugador

    // Singleton
    private ComServer() {}

    public static synchronized ComServer getInstance() {
        if (instance == null) {
            instance = new ComServer();
        }
        return instance;
    }

    // Método que inicia el servidor de comunicaciones
    public void iniciarServidor() {
        SocketServer socketServer = SocketServer.getInstance();
        socketServer.abrirPuerto();

        while (true) {
            // Esperar comunicaciones del socket
            Cliente nuevoCliente = socketServer.esperarCliente();
            if (nuevoCliente != null) {
                clients.add(nuevoCliente);
                // Crear un nuevo hilo para manejar la comunicación con este cliente
                ClientHandler clientHandler = new ClientHandler(nuevoCliente);
                Thread clientThread = new Thread(clientHandler);
                clientThread.start();
            }
        }
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
            listaDeJugadores.add(jugador.toString()); // Asume que `toString()` devuelve el ID o nombre del jugador
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
