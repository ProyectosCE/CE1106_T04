package org.proyectosce.comunicaciones;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

/*
 * Class: SocketServer
 *
 * Clase que maneja el servidor de socket, enviar y recibir mensajes.
 */
public class SocketServer {
    private static SocketServer instance;
    private static final int PORT = 12345;
    private ServerSocketChannel serverSocketChannel;
    private final Set<Cliente> clientesActivos = ConcurrentHashMap.newKeySet();

    // Singleton
    private SocketServer() {}

    public static synchronized SocketServer getInstance() {
        if (instance == null) {
            instance = new SocketServer();
        }
        return instance;
    }

    // Método para abrir el puerto de escucha
    public void abrirPuerto() {
        try {
            serverSocketChannel = ServerSocketChannel.open();
            serverSocketChannel.bind(new InetSocketAddress(PORT));
            System.out.println("Servidor escuchando en el puerto " + PORT);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    // Método para esperar a que un cliente se conecte
    public Cliente esperarCliente() {
        try {
            SocketChannel clientChannel = serverSocketChannel.accept();
            return new Cliente(clientChannel);
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }
    }

    // Método para enviar un mensaje a un cliente
    public void enviarMensaje(Cliente cliente, String mensaje) {
        try {
            ByteBuffer buffer = ByteBuffer.wrap(mensaje.getBytes());
            cliente.getChannel().write(buffer);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    // Método para recibir un mensaje de un cliente
    public String recibirMensaje(Cliente cliente) {
        try {
            ByteBuffer buffer = ByteBuffer.allocate(1024);
            int bytesRead = cliente.getChannel().read(buffer);

            // Verificar si el cliente se ha desconectado
            if (bytesRead == -1) {
                System.out.println("El cliente se ha desconectado: " + cliente);
                this.cerrarConexion(cliente); // Cerrar la conexión con el cliente
                return null;  // Retornar null para indicar que el cliente se desconectó
            }

            // Si no hay datos en el buffer, retornar null también
            if (bytesRead == 0) {
                System.out.println("Buffer vacío, sin datos disponibles del cliente: " + cliente);
                return null;
            }

            // Convertir el buffer en cadena de texto y retornar el mensaje
            return new String(buffer.array(), 0, bytesRead).trim();

        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }
    }


    /*
     * Método para cerrar la conexión de un cliente
     */
    public void cerrarConexion(Cliente cliente) {
        try {
            if (cliente != null && !cliente.getChannel().isOpen()) {
                // Remover el cliente de la lista de clientes activos
                System.out.println("El cliente cerró la conexion");
                clientesActivos.remove(cliente);

            }
            else if (cliente != null && cliente.getChannel().isOpen()) {
                // Cerrar el canal del socket del cliente
                cliente.getChannel().close();
                System.out.println("Conexión cerrada con el cliente: " + cliente.getChannel().getRemoteAddress());

                // Remover el cliente de la lista de clientes activos
                clientesActivos.remove(cliente);
            }
        } catch (IOException e) {
            System.out.println("Error al cerrar la conexión del cliente: " + e.getMessage());
        }
    }
}