/*
================================== LICENCIA =================
=================================
MIT License
Copyright (c) 2024  José Bernardo Barquero Bonilla,
                    Jose Eduardo Campos Salazar,
                    Jimmy Feng Feng,
                    Alexander Montero Vargas
Consulta el archivo LICENSE para más detalles.
=============================================================
Cambios y Configuraciones del Proyecto3=================================
*/

package org.proyectosce.comunicaciones;

import org.proyectosce.SettingsReader;
import org.proyectosce.comandos.factory.CommandFactory;
import org.proyectosce.comandos.factory.products.Command;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.StandardSocketOptions;
import java.nio.ByteBuffer;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

/* Class: SocketServer
    Clase Singleton que maneja las conexiones de clientes y la comunicación mediante mensajes.

    Attributes:
        - instance: SocketServer - Instancia única de la clase (Singleton).
        - serverSocketChannel: ServerSocketChannel - Canal del servidor para aceptar conexiones.
        - clientesActivos: Set<Cliente> - Conjunto de clientes conectados activamente.

    Constructor:
        - SocketServer: Constructor privado para implementar el patrón Singleton.

    Methods:
        - getInstance: Retorna la instancia única de SocketServer.
        - abrirPuerto: Abre el puerto para escuchar conexiones.
        - esperarCliente: Espera y acepta conexiones entrantes de clientes.
        - enviarMensaje: Envía un mensaje a un cliente específico.
        - recibirMensaje: Recibe un mensaje de un cliente conectado.
        - cerrarConexion: Cierra la conexión con un cliente.
        - notificarDesconexion: Maneja la desconexión de un cliente.
        - manejarExcepcion: Maneja excepciones y muestra mensajes de error.

    Example:
        SocketServer servidor = SocketServer.getInstance();
        servidor.abrirPuerto();
        Cliente cliente = servidor.esperarCliente();
        servidor.enviarMensaje(cliente, "Bienvenido al servidor.");
*/
public class SocketServer {
    private static SocketServer instance;
    private ServerSocketChannel serverSocketChannel;
    private final Set<Cliente> clientesActivos = ConcurrentHashMap.newKeySet();

    /* Function: SocketServer
        Constructor privado para la implementación del patrón Singleton.
    */
    private SocketServer() {}

    /* Function: getInstance
        Retorna la instancia única de la clase SocketServer.

        Returns:
            - SocketServer - Instancia única del servidor.
    */
    public static synchronized SocketServer getInstance() {
        if (instance == null) {
            instance = new SocketServer();
        }
        return instance;
    }

    /* Function: abrirPuerto
        Configura el servidor para escuchar nuevas conexiones en la dirección y puerto especificados.
    */
    public void abrirPuerto() {
        try {
            SettingsReader settings = SettingsReader.getInstance();
            String address = settings.getSocketAddress();
            int port = settings.getSocketPort();

            serverSocketChannel = ServerSocketChannel.open();
            serverSocketChannel.setOption(StandardSocketOptions.SO_REUSEADDR, true);
            serverSocketChannel.bind(new InetSocketAddress(address, port));
            serverSocketChannel.configureBlocking(true);
            System.out.println("Servidor escuchando en " + address + ":" + port);
        } catch (IOException e) {
            manejarExcepcion("No se pudo abrir el puerto del servidor", e);
        }
    }

    /* Function: esperarCliente
        Acepta la conexión de un cliente y lo añade a la lista de clientes activos.

        Returns:
            - Cliente - Objeto Cliente conectado, o null si hubo un error.
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

    /* Function: enviarMensaje
        Envía un mensaje a un cliente específico.

        Params:
            - cliente: Cliente - Cliente al que se envía el mensaje.
            - mensaje: String - Mensaje a enviar.
    */
    public void enviarMensaje(Cliente cliente, String mensaje) {
        try {
            ByteBuffer buffer = ByteBuffer.wrap(mensaje.getBytes());
            cliente.getChannel().write(buffer);
        } catch (IOException e) {
            manejarExcepcion("Fallo al enviar mensaje", e);
        }
    }

    /* Function: recibirMensaje
        Recibe un mensaje de un cliente conectado.

        Params:
            - cliente: Cliente - Cliente del que se recibe el mensaje.

        Returns:
            - String - Mensaje recibido o null si hubo un error o desconexión.
    */
    public String recibirMensaje(Cliente cliente) {
        try {
            ByteBuffer buffer = ByteBuffer.allocate(4096);

            // Leer datos en un ciclo para asegurarse de recibir el mensaje completo
            int bytesRead = cliente.getChannel().read(buffer);

            // Verificar si el cliente se ha desconectado
            if (bytesRead == -1) {
                System.out.println("El cliente se ha desconectado: " + cliente);
                // Crear y ejecutar DisconnectCommand usando CommandFactory
                CommandFactory commandFactory = CommandFactory.getInstance();
                Map<String, Object> params = Map.of(
                        "cliente", cliente, // Cliente desconectado
                        "comServer", ComServer.getInstance(), // Instancia del ComServer
                        "socketServer", SocketServer.getInstance() // Instancia del SocketServer
                );
                Command disconnectCommand = commandFactory.crearComando("disconnect", params);
                disconnectCommand.ejecutar();
                return null;
            }

            // Limpiar el buffer antes de cada lectura para evitar residuos
            buffer.flip();

            // Convertir el buffer en cadena de texto y retornar el mensaje
            return new String(buffer.array(), 0, bytesRead).trim();

        } catch (IOException e) {
            //e.printStackTrace();
            System.out.println("Fallo al recibir mensaje en read buffer"+cliente);
            // Crear y ejecutar DisconnectCommand usando CommandFactory
            CommandFactory commandFactory = CommandFactory.getInstance();
            Map<String, Object> params = Map.of(
                    "cliente", cliente, // Cliente desconectado
                    "comServer", ComServer.getInstance(), // Instancia del ComServer
                    "socketServer", SocketServer.getInstance() // Instancia del SocketServer
            );
            Command disconnectCommand = commandFactory.crearComando("disconnect", params);
            disconnectCommand.ejecutar();
            this.cerrarConexion(cliente); // Cerrar la conexión en caso de error
            return null;
        }
    }

    /* Function: cerrarConexion
        Cierra la conexión de un cliente.

        Params:
            - cliente: Cliente - Cliente a desconectar.
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

    /* Function: notificarDesconexion
        Notifica y maneja la desconexión de un cliente.

        Params:
            - cliente: Cliente - Cliente desconectado.
    */
    private void notificarDesconexion(Cliente cliente) {
        if (cliente != null) {
            clientesActivos.remove(cliente);
            CommandFactory commandFactory = CommandFactory.getInstance();
            Map<String, Object> params = Map.of(
                    "cliente", cliente,
                    "comServer", ComServer.getInstance(),
                    "socketServer", SocketServer.getInstance()
            );
            Command disconnectCommand = commandFactory.crearComando("disconnect", params);
            disconnectCommand.ejecutar();
            System.out.println("Cliente desconectado: " + cliente);
        }
    }

    /* Function: manejarExcepcion
        Maneja excepciones centralizadamente y muestra un mensaje de error.

        Params:
            - mensaje: String - Mensaje de error descriptivo.
            - e: Exception - Excepción lanzada.
    */
    private void manejarExcepcion(String mensaje, Exception e) {
        System.err.println(mensaje + ": " + e.getMessage());
        e.printStackTrace();
    }
}
