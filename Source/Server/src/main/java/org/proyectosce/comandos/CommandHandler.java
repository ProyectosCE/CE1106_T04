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

    public void processInputCommand(String input, Cliente cliente) {
        if (input.startsWith("brick_pwr")) {
            try {
                String[] parts = input.substring(input.indexOf('(') + 1, input.indexOf(')')).split(",");
                int f = Integer.parseInt(parts[0].trim());
                int c = Integer.parseInt(parts[1].trim());
                String powerType = parts[2].trim();

                if (powerType.equals("add_life") || powerType.equals("add_ball") ||
                        powerType.equals("double_racket_size") || powerType.equals("half_racket_size") ||
                        powerType.equals("add_speed") || powerType.equals("reduce_speed")) {

                    PowerCommand comando = new PowerCommand(f, c, powerType);
                    handleCommand(comando, cliente);
                } else {
                    System.out.println("Poder no válido. Use: brick_pwr (f,c,power_type)");
                }
            } catch (Exception e) {
                System.out.println("Comando incorrecto. Use: brick_pwr (f,c,power_type)");
            }
        } else {
            System.out.println("Comando incorrecto. Use: brick_pwr (f,c,power_type)");
        }
    }
}
