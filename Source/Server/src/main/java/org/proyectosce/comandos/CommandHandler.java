package org.proyectosce.comandos;

import java.util.HashMap;
import java.util.Map;

import com.fasterxml.jackson.core.JsonProcessingException;
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

    // Método para manejar un comando específico y enviar el poder al cliente
    // Método para manejar un comando específico y enviar el poder al cliente
    public void handleCommand(PowerCommand comando, Cliente cliente) {
        // Crear el mensaje JSON
        Map<String, Object> jsonMessage = new HashMap<>();
        jsonMessage.put("command", comando.getType()); // El tipo de poder como comando

        // Crear el mapa para la posición
        Map<String, Integer> position = new HashMap<>();
        position.put("i", comando.getC()); // Columna
        position.put("j", comando.getF()); // Fila

        jsonMessage.put("position", position); // Añadir la posición al mensaje JSON

        // Enviar el mensaje JSON al cliente
        try {
            String mensajeSalida = jsonProcessor.getObjectMapper().writeValueAsString(jsonMessage);
            socketServer.enviarMensaje(cliente, mensajeSalida);
            System.out.println("Comando enviado correctamente: " + jsonMessage);
        } catch (JsonProcessingException e) {
            System.err.println("Error al procesar el JSON: " + e.getMessage());
            e.printStackTrace(); // O maneja la excepción de acuerdo a tu lógica de error
        }
    }



    // Método para procesar comandos desde la terminal
    public void processInputCommand(String input, Cliente cliente) {
        if (input.startsWith("brick_pwr")) {
            try {
                // Extraer valores entre paréntesis y separarlos
                String[] parts = input.substring(input.indexOf('(') + 1, input.indexOf(')')).split(",");
                int f = Integer.parseInt(parts[0].trim());
                int c = Integer.parseInt(parts[1].trim());
                String powerType = parts[2].trim();

                // Validar el tipo de poder
                if (powerType.equals("add_life") || powerType.equals("add_ball") ||
                        powerType.equals("double_racket_size") || powerType.equals("half_racket_size") ||
                        powerType.equals("add_speed") || powerType.equals("reduce_speed")) {

                    // Crear y manejar el comando si es válido
                    PowerCommand comando = new PowerCommand(powerType);
                    comando.setParams(new int[]{f, c}); // Establecer las posiciones
                    handleCommand(comando, cliente);
                } else {
                    System.out.println("Poder no válido. Ingréselo de esta forma: brick_pwr (f,c,power_type)");
                }
            } catch (Exception e) {
                System.out.println("Comando incorrecto. Ingréselo de esta forma: brick_pwr (f,c,power_type)");
            }
        } else {
            System.out.println("Comando incorrecto. Ingréselo de esta forma: brick_pwr (f,c,power_type)");
        }
    }
}
