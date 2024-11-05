package org.proyectosce.comunicaciones;

import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

public class ComServer {
    private static ComServer instance;
    private final Set<Cliente> clients = ConcurrentHashMap.newKeySet();
    private final SocketServer socketServer = SocketServer.getInstance(); // Instancia de SocketServer

    // Lista de poderes permitidos
    private final Set<String> powersAllowed = Set.of("add_life", "add_ball", "double_racket_size", "half_racket_size", "add_speed", "reduce_speed");

    // Singleton
    private ComServer() {}

    public static synchronized ComServer getInstance() {
        if (instance == null) {
            instance = new ComServer();
        }
        return instance;
    }

    // Método que inicia el servidor de comunicaciones
    public void iniciarServidor() {
        socketServer.abrirPuerto();

        // Iniciar la consola en un hilo aparte para permitir comandos de usuario
        new Thread(this::iniciarConsola).start();

        while (true) {
            Cliente nuevoCliente = socketServer.esperarCliente();
            if (nuevoCliente != null) {
                clients.add(nuevoCliente);
                ClientHandler clientHandler = new ClientHandler(nuevoCliente);
                Thread clientThread = new Thread(clientHandler);
                clientThread.start();
            }
        }
    }

    // Método para iniciar la consola y procesar comandos
    private void iniciarConsola() {
        Scanner scanner = new Scanner(System.in);
        System.out.println("Ingrese el comando:");

        while (scanner.hasNextLine()) {
            String input = scanner.nextLine();
            if (input.startsWith("brick_pwr")) {
                procesarComandoConsola(input);
            } else {
                System.out.println("Comando no reconocido. Intente con: brick_pwr (f,c,power)");
            }
        }
    }

    // Procesar el comando desde la consola
    private void procesarComandoConsola(String input) {
        try {
            // Extraer valores del formato "brick_pwr (f,c,power)"
            String[] partes = input.replace("brick_pwr", "").replace("(", "").replace(")", "").split(",");
            if (partes.length != 3) {
                System.out.println("Formato de comando incorrecto. Use: brick_pwr (f,c,power)");
                return;
            }

            // Validar y parsear los valores de f y c
            int f = Integer.parseInt(partes[0].trim());
            int c = Integer.parseInt(partes[1].trim());
            String power = partes[2].trim();

            // Validar el poder
            if (!powersAllowed.contains(power)) {
                System.out.println("Poder inválido. Poderes permitidos: " + powersAllowed);
                return;
            }

            // Crear el JSON y enviarlo a todos los clientes
            Map<String, Object> jsonMessage = new HashMap<>();
            jsonMessage.put("command", "brick-pwr");
            jsonMessage.put("position", new int[]{f, c}); // Lista en lugar de objeto
            jsonMessage.put("power", power);

            enviarMensajeJsonATodos(jsonMessage);
        } catch (NumberFormatException e) {
            System.out.println("Las coordenadas f y c deben ser enteros.");
        }
    }

    // Método para enviar un mensaje JSON a un cliente específico
    public void enviarMensajeJson(Cliente cliente, Map<String, Object> jsonMessage) {
        try {
            String json = socketServer.getObjectMapper().writeValueAsString(jsonMessage);
            cliente.enviarMensaje(json);
            System.out.println("Mensaje JSON enviado al cliente: " + json);
        } catch (Exception e) {
            System.out.println("Error al enviar mensaje JSON: " + e.getMessage());
        }
    }

    // Método para enviar un mensaje JSON a todos los clientes conectados
    public void enviarMensajeJsonATodos(Map<String, Object> jsonMessage) {
        try {
            String json = socketServer.getObjectMapper().writeValueAsString(jsonMessage);
            for (Cliente client : clients) {
                client.enviarMensaje(json);
                System.out.println("Mensaje JSON enviado al cliente: " + json);
            }
        } catch (Exception e) {
            System.out.println("Error al enviar mensaje JSON a todos los clientes: " + e.getMessage());
        }
    }
}

