package org.proyectosce.comunicaciones;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;

/*
 * Class: SocketServer
 *
 * Clase que maneja el servidor de socket, enviar y recibir mensajes.
 */
public class SocketServer {
    private static SocketServer instance;
    private static final int PORT = 12345;
    private ServerSocketChannel serverSocketChannel;

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
            cliente.getChannel().read(buffer);
            return new String(buffer.array()).trim();
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }
    }
}
