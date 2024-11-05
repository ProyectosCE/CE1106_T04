package org.proyectosce.comunicaciones;

import org.proyectosce.comandos.Command;
import org.proyectosce.comandos.CommandHandler;
import org.proyectosce.comandos.PowerCommand;

public class ClientHandler implements Runnable {
    private final Cliente cliente;
    private final JsonProcessor jsonProcessor;
    private final CommandHandler commandHandler;
    private final SocketServer socketServer;

    public ClientHandler(Cliente cliente) {
        this.cliente = cliente;
        this.jsonProcessor = JsonProcessor.getInstance();
        this.commandHandler = new CommandHandler();
        this.socketServer = SocketServer.getInstance(); // Inicializar socketServer
    }

    @Override
    public void run() {
        try {
            while (true) {
                String mensajeEntrante = socketServer.recibirMensaje(cliente); // Cambiar jsonProcessor a socketServer
                if (mensajeEntrante == null) {
                    System.out.println("Cliente desconectado: " + cliente);
                    break;
                }

                Command comando = jsonProcessor.procesarComando(mensajeEntrante);
                if (comando instanceof PowerCommand) {
                    commandHandler.handleCommand((PowerCommand) comando, cliente);
                } else {
                    System.out.println("Comando no válido o no reconocido.");
                }

                String mensajeSalida = jsonProcessor.crearMensajeSalida("Conexión exitosa", null);
                socketServer.enviarMensaje(cliente, mensajeSalida); // Cambiar jsonProcessor a socketServer
            }
        } finally {
            socketServer.cerrarConexion(cliente); // Cambiar jsonProcessor a socketServer
            System.out.println("Cliente removido: " + cliente);
        }
    }
}
