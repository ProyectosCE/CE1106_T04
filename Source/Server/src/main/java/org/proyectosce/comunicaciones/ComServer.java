package org.proyectosce.comunicaciones;

import java.util.Set;
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

    public void enviarMensajeATodos(String mensajeJson) {
        SocketServer socketServer = SocketServer.getInstance();
        for (Cliente cliente : clients) {
            socketServer.enviarMensaje(cliente, mensajeJson);
        }
    }
}
