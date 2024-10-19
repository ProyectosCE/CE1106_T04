package org.proyectosce.comunicaciones;

import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

import org.proyectosce.comandos.Command;

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
                manejarCliente(nuevoCliente);
            }
        }
    }

    // Maneja las comunicaciones del cliente
    private void manejarCliente(Cliente cliente) {
        String mensajeEntrante = SocketServer.getInstance().recibirMensaje(cliente);

        if (mensajeEntrante != null) {
            JsonProcessor jsonProcessor = JsonProcessor.getInstance();
            Command comando = jsonProcessor.procesarComando(mensajeEntrante);

            if (comando != null) {
                comando.ejecutar(cliente);
            }

            // Respuesta de salida al cliente confirmando la conexión
            String mensajeSalida = jsonProcessor.crearMensajeSalida("Conexión exitosa");
            SocketServer.getInstance().enviarMensaje(cliente, mensajeSalida);
        }
    }
}
