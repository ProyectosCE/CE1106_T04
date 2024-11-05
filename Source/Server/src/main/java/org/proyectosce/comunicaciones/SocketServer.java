package org.proyectosce.comunicaciones;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.StandardSocketOptions;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

public class SocketServer {
    private static final int PORT = 1234;
    private static SocketServer instance;
    private ServerSocketChannel serverSocketChannel;
    private final Set<Cliente> clientesActivos = ConcurrentHashMap.newKeySet();

    private SocketServer() {}

    public static synchronized SocketServer getInstance() {
        if (instance == null) {
            instance = new SocketServer();
        }
        return instance;
    }

    public void abrirPuerto() {
        try {
            serverSocketChannel = ServerSocketChannel.open();
            serverSocketChannel.setOption(StandardSocketOptions.SO_REUSEADDR, true);
            serverSocketChannel.bind(new InetSocketAddress(PORT));
            serverSocketChannel.configureBlocking(true);
            System.out.println("Servidor escuchando en el puerto " + PORT);
        } catch (IOException e) {
            throw new RuntimeException("No se pudo abrir el puerto del servidor: " + e.getMessage(), e);
        }
    }

    public Cliente esperarCliente() {
        try {
            SocketChannel clientChannel = serverSocketChannel.accept();
            Cliente nuevoCliente = new Cliente(clientChannel);
            clientesActivos.add(nuevoCliente);
            System.out.println("Cliente conectado desde: " + clientChannel.getRemoteAddress());
            return nuevoCliente;
        } catch (IOException e) {
            System.err.println("Error al aceptar cliente: " + e.getMessage());
            return null;
        }
    }

    public void enviarMensaje(Cliente cliente, String mensaje) {
        cliente.enviarMensaje(mensaje);
    }

    public String recibirMensaje(Cliente cliente) {
        // Delegar la recepción de mensajes al método de la clase Cliente
        return cliente.recibirMensaje();
    }

    public void cerrarConexion(Cliente cliente) {
        cliente.cerrarConexion();
        clientesActivos.remove(cliente);
        System.out.println("Cliente desconectado: " + cliente);
    }

    public void cerrarServidor() {
        try {
            for (Cliente cliente : clientesActivos) {
                cerrarConexion(cliente);
            }
            if (serverSocketChannel != null && serverSocketChannel.isOpen()) {
                serverSocketChannel.close();
            }
        } catch (IOException e) {
            System.err.println("Error al cerrar el servidor: " + e.getMessage());
        }
    }

    public Cliente getClienteConectado() {
        return clientesActivos.stream().findFirst().orElse(null);
    }
}
