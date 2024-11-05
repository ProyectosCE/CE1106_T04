package org.proyectosce.comunicaciones;


import org.proyectosce.comandos.Command;

public class ClientHandler implements Runnable {
    private final Cliente cliente;
    private final JsonProcessor jsonProcessor;
    private final SocketServer socketServer;

    public ClientHandler(Cliente cliente) {
        this.cliente = cliente;
        this.jsonProcessor = JsonProcessor.getInstance();
        this.socketServer = SocketServer.getInstance();
    }

    @Override
    public void run() {
        try {
            while (true) {
                // Recibir el mensaje del cliente
                String mensajeEntrante = socketServer.recibirMensaje(cliente);
                if (mensajeEntrante == null) {
                    System.out.println("Cliente desconectado: " + cliente);
                    break;  // Termina el bucle si el cliente se desconecta
                }

                // Procesar el mensaje recibido
                Command comando = jsonProcessor.procesarComando(mensajeEntrante);
                if (comando != null) {
                    comando.ejecutar(cliente);
                }

                // Enviar una respuesta al cliente
                String mensajeSalida = jsonProcessor.crearMensajeSalida("Conexión exitosa");
                socketServer.enviarMensaje(cliente, mensajeSalida);
            }
        } finally {
            // Limpiar y remover al cliente de la lista
            socketServer.cerrarConexion(cliente);
            System.out.println("Cliente removido: " + cliente);
        }
    }
}
