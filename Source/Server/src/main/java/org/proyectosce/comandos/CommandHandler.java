package org.proyectosce.comandos;

import java.util.HashMap;
import java.util.Map;
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
    public void handleCommand(Command comando, Cliente cliente) {
        Map<String, Object> powerData = new HashMap<>();

        // Crear JSON basado en el tipo de comando
        switch (comando.getType()) {
            case "add_life":
                powerData.put("addLife", true);
                break;
            case "add_ball":
                powerData.put("addBall", true);
                break;
            case "double_racket_size":
                powerData.put("doubleRacket", true);
                break;
            case "half_racket_size":
                powerData.put("halfRacket", true);
                break;
            case "add_speed":
                powerData.put("speedUp", true);
                break;
            case "reduce_speed":
                powerData.put("speedDown", true);
                break;
            default:
                System.out.println("Comando desconocido: " + comando.getType());
                return;
        }

        // Crear el mensaje JSON
        String mensajeSalida = jsonProcessor.crearMensajeSalida("OK", comando.getType(), powerData);

        // Enviar el mensaje JSON al cliente
        socketServer.enviarMensaje(cliente, mensajeSalida);
        System.out.println("Comando enviado correctamente.");
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
