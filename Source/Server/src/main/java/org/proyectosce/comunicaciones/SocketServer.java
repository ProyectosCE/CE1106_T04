package org.proyectosce.comunicaciones;

import org.proyectosce.comandos.factory.products.DisconnectCommand;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.StandardSocketOptions;
import java.nio.ByteBuffer;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

/**
 * Singleton SocketServer: maneja la conexión de clientes y la comunicación mediante mensajes.
 */
public class SocketServer {
    private static SocketServer instance;
    private static final int PORT = 12346;
    private ServerSocketChannel serverSocketChannel;
    private final Set<Cliente> clientesActivos = ConcurrentHashMap.newKeySet();

    // Constructor privado para Singleton
    private SocketServer() {}

    public static synchronized SocketServer getInstance() {
        if (instance == null) {
            instance = new SocketServer();
        }
        return instance;
    }

    /**
     * Abre el puerto del servidor para escuchar nuevas conexiones.
     */
    public void abrirPuerto() {
        try {
            serverSocketChannel = ServerSocketChannel.open();
            serverSocketChannel.setOption(StandardSocketOptions.SO_REUSEADDR, true);
            serverSocketChannel.bind(new InetSocketAddress(PORT));
            serverSocketChannel.configureBlocking(true);
            System.out.println("Servidor escuchando en el puerto " + PORT);
        } catch (IOException e) {
            manejarExcepcion("No se pudo abrir el puerto del servidor", e);
        }
    }

    /**
     * Espera la conexión de un cliente y lo añade a la lista de clientes activos.
     * @return Cliente conectado o null si hubo error
     */
    public Cliente esperarCliente() {
        try {
            SocketChannel clientChannel = serverSocketChannel.accept();
            Cliente nuevoCliente = new Cliente(clientChannel);
            clientesActivos.add(nuevoCliente);
            System.out.println("Cliente conectado desde: " + clientChannel.getRemoteAddress());
            return nuevoCliente;
        } catch (IOException e) {
            manejarExcepcion("Error al aceptar cliente", e);
            return null;
        }
    }

    /**
     * Envía un mensaje a un cliente específico.
     * @param cliente Cliente al que se envía el mensaje
     * @param mensaje Mensaje a enviar
     */
    public void enviarMensaje(Cliente cliente, String mensaje) {
        try {
            ByteBuffer buffer = ByteBuffer.wrap(mensaje.getBytes());
            cliente.getChannel().write(buffer);
        } catch (IOException e) {
            manejarExcepcion("Fallo al enviar mensaje", e);
        }
    }

    // Método para recibir un mensaje de un cliente
    public String recibirMensaje(Cliente cliente) {
        try {
            ByteBuffer buffer = ByteBuffer.allocate(4096);

            // Leer datos en un ciclo para asegurarse de recibir el mensaje completo
            int bytesRead = cliente.getChannel().read(buffer);

            // Verificar si el cliente se ha desconectado
            if (bytesRead == -1) {
                System.out.println("El cliente se ha desconectado: " + cliente);
                DisconnectCommand DisconnectCommand = new DisconnectCommand(cliente);
                DisconnectCommand.ejecutar();
                return null;
            }

            // Limpiar el buffer antes de cada lectura para evitar residuos
            buffer.flip();

            // Convertir el buffer en cadena de texto y retornar el mensaje
            return new String(buffer.array(), 0, bytesRead).trim();

        } catch (IOException e) {
            //e.printStackTrace();
            System.out.println("Fallo al recibir mensaje en read buffer"+cliente);
            DisconnectCommand DisconnectCommand = new DisconnectCommand(cliente);
            DisconnectCommand.ejecutar();
            this.cerrarConexion(cliente); // Cerrar la conexión en caso de error
            return null;
        }
    }


    /**
     * Cierra la conexión de un cliente.
     * @param cliente Cliente a desconectar
     */
    public void cerrarConexion(Cliente cliente) {
        try {
            if (cliente != null && cliente.getChannel().isOpen()) {
                cliente.getChannel().close();
                System.out.println("Conexión cerrada con el cliente: " + cliente.getChannel().getRemoteAddress());
            }
            notificarDesconexion(cliente);
        } catch (IOException e) {
            manejarExcepcion("Error al cerrar la conexión del cliente", e);
        }
    }

    /**
     * Notifica y maneja la desconexión de un cliente.
     * @param cliente Cliente desconectado
     */
    private void notificarDesconexion(Cliente cliente) {
        if (cliente != null) {
            clientesActivos.remove(cliente);
            DisconnectCommand disconnectCommand = new DisconnectCommand(cliente);
            disconnectCommand.ejecutar();
            System.out.println("Cliente desconectado: " + cliente);
        }
    }

    /**
     * Maneja excepciones centralizadamente.
     * @param mensaje Mensaje de error
     * @param e Excepción lanzada
     */
    private void manejarExcepcion(String mensaje, Exception e) {
        System.err.println(mensaje + ": " + e.getMessage());
        e.printStackTrace();
    }
}
