package org.proyectosce.comunicaciones;

import org.proyectosce.comandos.Command;
import org.proyectosce.comandos.CommandHandler;
import org.proyectosce.comandos.PowerCommand;

public class ClientHandler implements Runnable {
    private final Cliente cliente;
    private final JsonProcessor jsonProcessor;
    private final SocketServer socketServer;
    private final CommandHandler commandHandler;

    public ClientHandler(Cliente cliente) {
        this.cliente = cliente;
        this.jsonProcessor = JsonProcessor.getInstance();
        this.socketServer = SocketServer.getInstance();
        this.commandHandler = new CommandHandler();
    }

    @Override
    public void run() {
        try {
            while (true) {
                String mensajeEntrante = socketServer.recibirMensaje(cliente);
                if (mensajeEntrante == null) {
                    System.out.println("Cliente desconectado: " + cliente);
                    break;
                }

                Command comando = jsonProcessor.procesarComando(mensajeEntrante);
                if (comando != null) {
                    if (comando instanceof PowerCommand) {
                        commandHandler.handleCommand((PowerCommand) comando, cliente); // Casting seguro
                    } else {
                        System.out.println("Comando no válido: " + comando.getType());
                    }
                }

                String mensajeSalida = jsonProcessor.crearMensajeSalida("Conexión exitosa");
                socketServer.enviarMensaje(cliente, mensajeSalida);
            }
        } finally {
            socketServer.cerrarConexion(cliente);
            System.out.println("Cliente removido: " + cliente);
        }
    }
}

