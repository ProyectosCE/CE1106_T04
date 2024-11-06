package org.proyectosce.comunicaciones;

import org.proyectosce.comandos.Command;
import org.proyectosce.comunicaciones.JsonProcessor;
import org.proyectosce.comandos.TipoClienteCommand;
import org.proyectosce.comandos.GameSpectatorCommand;
import org.proyectosce.comandos.SendGameStateCommand;

public class ClientHandler implements Runnable {
    private final Cliente cliente;
    private final JsonProcessor jsonProcessor;
    private final SocketServer socketServer;
    private boolean clienteIdentificado = false;  // Flag para verificar si el cliente ya fue identificado

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

                // Procesar el primer mensaje para identificar el tipo de cliente
                if (!clienteIdentificado) {
                    Command comando = jsonProcessor.procesarComando(mensajeEntrante, cliente);
                    if (comando instanceof TipoClienteCommand) {
                        comando.ejecutar(cliente);
                        clienteIdentificado = true;  // Marcar cliente como identificado
                    } else {
                        System.out.println("Comando inesperado; se esperaba identificación de tipo de cliente.");
                        continue;  // Ignorar otros comandos hasta que el cliente esté identificado
                    }
                } else {
                    // Procesar mensajes después de la identificación
                    Command comando = jsonProcessor.procesarComando(mensajeEntrante, cliente);
                    if (comando != null) {
                        comando.ejecutar(cliente);
                    } else {
                        System.out.println("Comando no reconocido del cliente: " + cliente);
                    }
                }
            }
        } finally {
            // Limpiar y remover al cliente de la lista
            socketServer.cerrarConexion(cliente);
            System.out.println("Cliente removido: " + cliente);
        }
    }
}
