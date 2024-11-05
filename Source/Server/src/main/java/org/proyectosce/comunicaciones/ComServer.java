package org.proyectosce.comunicaciones;

import org.proyectosce.comandos.CommandHandler;
import org.proyectosce.comandos.factory.products.PowerCommand;
import java.util.Scanner;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

public class ComServer {
    private static ComServer instance;
    private final Set<Cliente> clients = ConcurrentHashMap.newKeySet();
    private final SocketServer socketServer = SocketServer.getInstance();
    private final CommandHandler commandHandler = new CommandHandler();
    private final Set<String> powersAllowed = Set.of("add_life", "add_ball", "double_racket_size", "half_racket_size", "add_speed", "reduce_speed");

    private ComServer() {}

    public static synchronized ComServer getInstance() {
        if (instance == null) {
            instance = new ComServer();
        }
        return instance;
    }

    public void iniciarServidor() {
        socketServer.abrirPuerto();
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

    private void iniciarConsola() {
        Scanner scanner = new Scanner(System.in);

        while (scanner.hasNextLine()) {
            String input = scanner.nextLine();
            if (input.startsWith("brick_pwr")) {
                procesarComandoConsola(input);
            } else {
                System.out.println("Comando no reconocido. Intente con: brick_pwr (f,c,power)");
            }
        }
    }

    private void procesarComandoConsola(String input) {
        try {
            String[] partes = input.replace("brick_pwr", "").replace("(", "").replace(")", "").split(",");
            if (partes.length != 3) {
                System.out.println("Formato de comando incorrecto. Use: brick_pwr (f,c,power)");
                return;
            }

            int f = Integer.parseInt(partes[0].trim());
            int c = Integer.parseInt(partes[1].trim());
            String power = partes[2].trim();

            if (!powersAllowed.contains(power)) {
                System.out.println("Poder inválido. Poderes permitidos: " + powersAllowed);
                return;
            }

            PowerCommand powerCommand = new PowerCommand(f, c, power);

            // Obtener el primer cliente conectado del set
            // Cambiar más adelante (commandHandler.handleCommand(powerCommand, (Cliente) clients);) para obtener el cliente según su ID
            Cliente primerCliente = clients.stream().findFirst().orElse(null);
            if (primerCliente != null) {
                commandHandler.handleCommand(powerCommand, primerCliente);
            } else {
                System.out.println("No hay clientes conectados para enviar el comando.");
            }
        } catch (NumberFormatException e) {
            System.out.println("Las coordenadas f y c deben ser enteros.");
        }
    }

}

