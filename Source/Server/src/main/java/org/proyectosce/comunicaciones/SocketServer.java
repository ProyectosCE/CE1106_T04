package org.proyectosce.comunicaciones;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import com.fasterxml.jackson.databind.ObjectMapper;

public class SocketServer {
    private static SocketServer instance;
    private static final int PORT = 8321;
    private ServerSocketChannel serverSocketChannel;
    private final Set<Cliente> clientesActivos = ConcurrentHashMap.newKeySet();
    private final ObjectMapper objectMapper = new ObjectMapper();

    // Singleton
    private SocketServer() {}

    public static synchronized SocketServer getInstance() {
        if (instance == null) {
            instance = new SocketServer();
        }
        return instance;
    }

    public ObjectMapper getObjectMapper() {
        return objectMapper;
    }

    public void abrirPuerto() {
        try {
            serverSocketChannel = ServerSocketChannel.open();
            serverSocketChannel.bind(new InetSocketAddress(PORT));
            serverSocketChannel.configureBlocking(true);
            System.out.println("Servidor escuchando en el puerto " + PORT);
        } catch (IOException e) {
            System.out.println("Error al abrir el puerto: " + e.getMessage());
        }
    }

    public Cliente esperarCliente() {
        try {
            SocketChannel clientChannel = serverSocketChannel.accept();
            if (clientChannel != null) {
                System.out.println("Cliente conectado desde: " + clientChannel.getRemoteAddress());
                Cliente nuevoCliente = new Cliente(clientChannel);
                clientesActivos.add(nuevoCliente);
                return nuevoCliente;
            }
        } catch (IOException e) {
            System.out.println("Error al aceptar cliente: " + e.getMessage());
        }
        return null;
    }

    public void enviarMensaje(Cliente cliente, String mensaje) {
        try {
            ByteBuffer buffer = ByteBuffer.wrap(mensaje.getBytes());
            cliente.getChannel().write(buffer);
            System.out.println("Mensaje enviado al cliente: " + mensaje);
        } catch (IOException e) {
            System.out.println("Error al enviar mensaje al cliente: " + e.getMessage());
        }
    }

    public String recibirMensaje(Cliente cliente) {
        try {
            ByteBuffer buffer = ByteBuffer.allocate(1024);
            int bytesRead = cliente.getChannel().read(buffer);

            if (bytesRead == -1) {
                System.out.println("El cliente se ha desconectado: " + cliente);
                this.cerrarConexion(cliente);
                return null;
            }

            buffer.flip(); // Cambiar a modo de lectura
            byte[] bytes = new byte[bytesRead];
            buffer.get(bytes);
            return new String(bytes).trim();
        } catch (IOException e) {
            System.out.println("Error al recibir mensaje del cliente: " + e.getMessage());
            this.cerrarConexion(cliente);
            return null;
        }
    }

    public void cerrarConexion(Cliente cliente) {
        try {
            if (cliente != null && cliente.getChannel().isOpen()) {
                cliente.getChannel().close();
                System.out.println("Conexión cerrada con el cliente: " + cliente.getChannel().getRemoteAddress());
            }
        } catch (IOException e) {
            System.out.println("Error al cerrar la conexión del cliente: " + e.getMessage());
        } finally {
            clientesActivos.remove(cliente);
        }
    }

    public void cerrarServidor() {
        try {
            for (Cliente cliente : clientesActivos) {
                cerrarConexion(cliente);
            }
            if (serverSocketChannel != null && serverSocketChannel.isOpen()) {
                serverSocketChannel.close();
                System.out.println("Servidor cerrado.");
            }
        } catch (IOException e) {
            System.out.println("Error al cerrar el servidor: " + e.getMessage());
        }
    }

    public void shutdown() {
        cerrarServidor(); // Asegúrate de cerrar todas las conexiones antes de cerrar el servidor
    }
}
