package org.proyectosce;

import org.proyectosce.comandos.CommandHandler;
import org.proyectosce.comunicaciones.ComServer;
import org.proyectosce.comunicaciones.SocketServer;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        // Ejecutar ComServer en un nuevo thread
        Thread serverThread = new Thread(() -> ComServer.getInstance().iniciarServidor());
        serverThread.start();

        // Crear instancia de CommandHandler
        CommandHandler commandHandler = new CommandHandler();
        Scanner scanner = new Scanner(System.in);

        // Agregar un shutdown hook para cerrar el servidor correctamente
        Runtime.getRuntime().addShutdownHook(new Thread(() -> {
            SocketServer.getInstance().cerrarServidor();
            System.out.println("Servidor cerrado correctamente.");
        }));

        // Bucle para permitir el envío de múltiples comandos
        while (true) {
            System.out.print("Ingrese comando (ejemplo: brick_pwr (f,c,power)): ");
            String input = scanner.nextLine();
            commandHandler.processInputCommand(input, SocketServer.getInstance().getClienteConectado());
        }
    }
}
