package org.proyectosce.comandos;

import java.util.Map;
import com.fasterxml.jackson.core.JsonProcessingException;
import org.proyectosce.comandos.factory.products.PowerCommand;
import org.proyectosce.comunicaciones.Cliente;
import org.proyectosce.comunicaciones.JsonProcessor;
import org.proyectosce.comunicaciones.SocketServer;

public class CommandHandler {
    private final JsonProcessor jsonProcessor;
    private final SocketServer socketServer;

    public CommandHandler() {
        this.jsonProcessor = JsonProcessor.getInstance();
        this.socketServer = SocketServer.getInstance();
    }

    public void handleCommand(PowerCommand comando, Cliente cliente) {
        Map<String, Object> jsonMessage = comando.toMap();

        try {
            String mensajeSalida = jsonProcessor.getObjectMapper().writeValueAsString(jsonMessage);
            socketServer.enviarMensaje(cliente, mensajeSalida);
        } catch (JsonProcessingException e) {
            System.err.println("Error al procesar el JSON: " + e.getMessage());
        }
    }
}