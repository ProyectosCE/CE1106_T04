package org.proyectosce.comandos.factory.products;

import org.proyectosce.comunicaciones.ComServer;
import org.proyectosce.comunicaciones.SocketServer;
import org.proyectosce.comunicaciones.Cliente;

import java.util.Map;
import java.util.HashMap;

public class PowerCommand implements Command {

    private String type; // Tipo de poder
    private int f; // Posición i
    private int c; // Posición j
    private String jugadorId; // ID del jugador al que se enviará el comando
    private ComServer comServer; // Referencia al ComServer para obtener el cliente
    private SocketServer socketServer; // Referencia al SocketServer para enviar el mensaje

    // Constructor vacío para permitir configuración dinámica
    public PowerCommand() {}

    @Override
    public void ejecutar() {
        if (type == null || jugadorId == null || comServer == null || socketServer == null) {
            throw new IllegalStateException("PowerCommand no está configurado correctamente.");
        }

        // Obtener el cliente jugador por ID
        Cliente jugador = comServer.obtenerClientePorId(jugadorId);
        if (jugador == null) {
            System.err.println("Jugador no encontrado con ID: " + jugadorId);
            return;
        }

        // Crear el JSON del comando
        String comandoJson = String.format(
                "{\"command\": \"brickUpdate\", \"row\": %d, \"column\": %d, \"power\": \"%s\"}",
                f, c, type
        );

        // Enviar el mensaje al cliente
        try {
            socketServer.enviarMensaje(jugador, comandoJson);
            System.out.println("Comando enviado: " + comandoJson);
        } catch (Exception e) {
            System.err.println("Error al enviar comando al jugador: " + e.getMessage());
            e.printStackTrace();
        }
    }

    @Override
    public String getType() {
        return type;
    }

    public int getF() {
        return f;
    }

    public int getC() {
        return c;
    }

    @Override
    public Map<String, Object> toMap() {
        Map<String, Object> jsonMessage = new HashMap<>();
        jsonMessage.put("command", type);

        Map<String, Integer> position = new HashMap<>();
        position.put("i", f);
        position.put("j", c);

        jsonMessage.put("position", position);
        return jsonMessage;
    }

    @Override
    public void configure(Map<String, Object> params) {
        this.type = (String) params.get("command");
        this.f = (int) params.get("f");
        this.c = (int) params.get("c");
        this.jugadorId = (String) params.get("jugadorId");
        this.comServer = (ComServer) params.get("comServer");
        this.socketServer = (SocketServer) params.get("socketServer");
    }
}
