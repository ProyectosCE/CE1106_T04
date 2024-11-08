/*
================================== LICENCIA ================================
MIT License
Copyright (c) 2024 José Bernardo Barquero Bonilla,
                   Jose Eduardo Campos Salazar,
                   Jimmy Feng Feng,
                   Alexander Montero Vargas
Consulta el archivo LICENSE para más detalles.
============================================================================
*/
package org.proyectosce.comunicaciones;

import java.util.*;
import org.proyectosce.ui.MainWindow;
import javax.swing.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.function.BiConsumer;

/* Class: ComServer
    Clase principal que gestiona la lógica de comunicaciones entre clientes y servidores.
    Permite registrar, eliminar y gestionar clientes, jugadores y observadores. Además,
    interactúa con la interfaz gráfica (MainWindow) para mantener actualizadas las listas
    de jugadores y observadores.

    Attributes:
        - servidorActivo: boolean - Indica si el servidor está activo.
        - instance: ComServer - Instancia única de la clase (Singleton).
        - clients: Set<Cliente> - Conjunto de todos los clientes conectados.
        - jugadores: Set<Cliente> - Conjunto de jugadores registrados.
        - observadores: Map<Cliente, Set<Cliente>> - Mapeo entre jugadores y sus observadores.
        - espectadoresTemporales: Set<Cliente> - Conjunto de espectadores temporales.
        - socketServer: SocketServer - Instancia del servidor de sockets.
        - updateCallback: BiConsumer<List<Cliente>, List<String>> - Función de actualización para listas.
        - mainWindow: MainWindow - Referencia a la ventana principal de la interfaz gráfica.

    Constructor:
        - ComServer: Constructor privado para implementar el patrón Singleton.

    Methods:
        - getInstance: Devuelve la instancia única de la clase ComServer.
        - setMainWindow: Asigna la ventana principal (MainWindow) para interacción con la interfaz gráfica.
        - setUpdateCallback: Asigna la función de callback para actualizar listas de clientes.
        - iniciarServidor: Inicia el servidor y escucha nuevas conexiones de clientes.
        - eliminarCliente: Elimina un cliente de todas las listas y actualiza la interfaz.
        - registrarEspectadorTemporal: Registra a un cliente como espectador temporal.
        - registrarJugador: Registra a un cliente como jugador.
        - registrarObservador: Asocia un cliente como observador de un jugador.
        - obtenerClientes: Devuelve la lista de jugadores registrados.
        - obtenerNombresEspectadores: Devuelve los IDs de los espectadores únicos.
        - actualizarListas: Actualiza las listas de jugadores y espectadores en la interfaz gráfica.
        - obtenerClientePorId: Busca y devuelve un cliente según su ID.
        - enviarListaDeJugadores: Envía la lista de jugadores a un espectador.
        - esJugador: Verifica si un cliente es un jugador registrado.
        - eliminarJugador: Elimina a un cliente de la lista de jugadores.
        - obtenerObservadores: Devuelve los observadores de un jugador específico.
        - eliminarObservadores: Elimina todos los observadores de un jugador.
        - eliminarEspectador: Elimina a un cliente como espectador.
        - eliminarEspectadorPorId: Elimina a un espectador según su ID.
        - getSocketServer: Devuelve la instancia del servidor de sockets.

    Example:
        // Instanciar el servidor y comenzar a escuchar conexiones
        ComServer server = ComServer.getInstance();
        server.iniciarServidor();

    Problems:

    References:

*/
public class ComServer {
    private volatile boolean servidorActivo = true;
    private static ComServer instance;
    private final Set<Cliente> clients = ConcurrentHashMap.newKeySet();
    private final Set<Cliente> jugadores = ConcurrentHashMap.newKeySet();
    private final Map<Cliente, Set<Cliente>> observadores = new ConcurrentHashMap<>();
    private final Set<Cliente> espectadoresTemporales = ConcurrentHashMap.newKeySet();
    private final SocketServer socketServer = SocketServer.getInstance();
    private BiConsumer<List<Cliente>, List<String>> updateCallback;
    private MainWindow mainWindow;

    // Constructor privado para implementar Singleton
    private ComServer() {}

    /* Function: getInstance
        Devuelve la instancia única de la clase ComServer.

        Returns:
            - ComServer - Instancia única de la clase.
    */
    public static synchronized ComServer getInstance() {
        if (instance == null) {
            instance = new ComServer();
        }
        return instance;
    }

    /* Function: setMainWindow
        Asigna la ventana principal (MainWindow) para interacción con la interfaz gráfica.

        Params:
            - mainWindow: MainWindow - Referencia a la ventana principal.
    */
    public void setMainWindow(MainWindow mainWindow) {
        this.mainWindow = mainWindow;
    }

    /* Function: setUpdateCallback
        Asigna la función de callback para actualizar listas de clientes.

        Params:
            - callback: BiConsumer<List<Cliente>, List<String>> - Callback de actualización.
    */
    public void setUpdateCallback(BiConsumer<List<Cliente>, List<String>> callback) {
        this.updateCallback = callback;
    }

    /* Function: iniciarServidor
        Inicia el servidor y escucha nuevas conexiones de clientes. Por cada cliente, se crea un nuevo hilo
        para manejar la comunicación.
    */
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

    /* Function: eliminarCliente
        Elimina un cliente de todas las listas y actualiza la interfaz gráfica.

        Params:
            - cliente: Cliente - Cliente a eliminar.
    */
    public void eliminarCliente(Cliente cliente) {
        clients.remove(cliente);
        jugadores.remove(cliente);
        observadores.remove(cliente);
        actualizarListas();
    }

    /* Function: registrarEspectadorTemporal
        Registra un cliente como espectador temporal y envía la lista de jugadores.

        Params:
            - cliente: Cliente - Cliente a registrar como espectador temporal.
    */
    public void registrarEspectadorTemporal(Cliente cliente) {
        espectadoresTemporales.add(cliente);
        enviarListaDeJugadores(cliente);
    }

    /* Function: registrarJugador
        Registra un cliente como jugador y lo asocia con un conjunto de observadores vacío.

        Params:
            - cliente: Cliente - Cliente a registrar como jugador.
    */
    public void registrarJugador(Cliente cliente) {
        jugadores.add(cliente);
        observadores.putIfAbsent(cliente, ConcurrentHashMap.newKeySet());
        actualizarListas();
    }

    /* Function: registrarObservador
        Asocia un cliente como observador de un jugador específico.

        Params:
            - jugador: Cliente - Cliente que actúa como jugador.
            - espectador: Cliente - Cliente que será registrado como observador del jugador.
    */
    public void registrarObservador(Cliente jugador, Cliente espectador) {
        observadores.computeIfAbsent(jugador, k -> ConcurrentHashMap.newKeySet()).add(espectador);
        actualizarListas();
    }

    /* Function: obtenerClientes
        Devuelve la lista de jugadores registrados.

        Returns:
            - List<Cliente> - Lista de jugadores.
    */
    private List<Cliente> obtenerClientes() {
        return new ArrayList<>(jugadores);
    }

    /* Function: obtenerNombresEspectadores
        Devuelve una lista con los IDs únicos de los observadores registrados.

        Returns:
            - List<String> - Lista de IDs de observadores.
    */
    private List<String> obtenerNombresEspectadores() {
        return observadores.values().stream()
                .flatMap(Set::stream)
                .map(Cliente::getId)
                .distinct()
                .toList();
    }

    /* Function: actualizarListas
        Actualiza las listas de jugadores y espectadores en la interfaz gráfica.

        Restriction:
            - Requiere que `mainWindow` no sea nulo para realizar la actualización.
    */
    private void actualizarListas() {
        List<Cliente> jugadoresClientes = obtenerClientes();
        List<String> espectadoresClientes = obtenerNombresEspectadores();

        if (mainWindow != null) {
            SwingUtilities.invokeLater(() -> mainWindow.updateClientLists(jugadoresClientes, espectadoresClientes));
        }
    }

    /* Function: obtenerClientePorId
        Busca y devuelve un cliente según su ID.

        Params:
            - id: String - ID del cliente a buscar.

        Returns:
            - Cliente - Cliente encontrado, o null si no existe.
    */
    public Cliente obtenerClientePorId(String id) {
        return jugadores.stream()
                .filter(cliente -> cliente.getId().equals(id))
                .findFirst()
                .orElse(null);
    }

    /* Function: enviarListaDeJugadores
        Envía la lista de jugadores (IDs y nombres) a un cliente espectador.

        Params:
            - espectador: Cliente - Cliente espectador al que se enviará la lista.
    */
    public void enviarListaDeJugadores(Cliente espectador) {
        String jugadoresJson = JsonProcessor.getInstance().crearMensajeClientesLista(
                jugadores.stream().map(Cliente::getId).toList(),
                jugadores.stream().map(Cliente::getNombre).toList()
        );
        socketServer.enviarMensaje(espectador, jugadoresJson);
    }

    /* Function: esJugador
        Verifica si un cliente está registrado como jugador.

        Params:
            - cliente: Cliente - Cliente a verificar.

        Returns:
            - boolean - `true` si el cliente es un jugador, `false` en caso contrario.
    */
    public boolean esJugador(Cliente cliente) {
        return jugadores.contains(cliente);
    }

    /* Function: eliminarJugador
        Elimina a un cliente de la lista de jugadores.

        Params:
            - cliente: Cliente - Cliente a eliminar.
    */
    public void eliminarJugador(Cliente cliente) {
        jugadores.remove(cliente);
    }

    /* Function: obtenerObservadores
        Obtiene los observadores asociados a un jugador.

        Params:
            - cliente: Cliente - Jugador cuyos observadores se desean obtener.

        Returns:
            - Set<Cliente> - Conjunto de observadores del jugador.
    */
    public Set<Cliente> obtenerObservadores(Cliente cliente) {
        return observadores.getOrDefault(cliente, new HashSet<>());
    }

    /* Function: eliminarObservadores
        Elimina todos los observadores de un jugador específico.

        Params:
            - cliente: Cliente - Jugador cuyos observadores serán eliminados.
    */
    public void eliminarObservadores(Cliente cliente) {
        Set<Cliente> observadoresDelJugador = obtenerObservadores(cliente);
        for (Cliente espectador : observadoresDelJugador) {
            eliminarEspectador(espectador);
        }
        observadores.remove(cliente);
    }

    /* Function: eliminarEspectador
        Elimina a un cliente como espectador.

        Params:
            - cliente: Cliente - Cliente a eliminar como espectador.
    */
    public void eliminarEspectador(Cliente cliente) {
        observadores.remove(cliente);
    }

    /* Function: eliminarEspectadorPorId
        Elimina a un observador de todos los jugadores según su ID.

        Params:
            - idObservador: String - ID del observador a eliminar.
    */
    public void eliminarEspectadorPorId(String idObservador) {
        for (Map.Entry<Cliente, Set<Cliente>> entry : observadores.entrySet()) {
            Cliente jugador = entry.getKey();
            Set<Cliente> observadoresDelJugador = entry.getValue();

            observadoresDelJugador.removeIf(observador -> observador.getId().equals(idObservador));

            if (observadoresDelJugador.isEmpty()) {
                observadores.remove(jugador);
            }
        }
    }

    /* Function: getSocketServer
        Devuelve la instancia del servidor de sockets.

        Returns:
            - Object - Instancia del servidor de sockets.
    */
    public Object getSocketServer() {
        return this.socketServer;
    }
}
