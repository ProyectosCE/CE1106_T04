package org.proyectosce;

import org.proyectosce.comandos.CommandHandler;
import org.proyectosce.comunicaciones.ComServer;
import org.proyectosce.comunicaciones.Cliente;
import org.proyectosce.comunicaciones.SocketServer;
import java.util.Scanner;

public class Main {
    public static void main(String[] args) {
        // Ejecutar ComServer en un nuevo thread
        Thread serverThread = new Thread(() -> ComServer.getInstance().iniciarServidor());
        serverThread.start();

        // Crear instancia de SocketServer y abrir el puerto
        SocketServer socketServer = SocketServer.getInstance();
        socketServer.abrirPuerto();

        // Esperar a que un cliente se conecte y obtener el SocketChannel
        Cliente cliente = socketServer.esperarCliente(); // Aquí obtienes el cliente que contiene el SocketChannel
        if (cliente == null) {
            System.out.println("No se pudo conectar un cliente.");
            return; // Salir si no se pudo conectar un cliente
        }

        // Agregar un shutdown hook para cerrar el servidor correctamente
        Runtime.getRuntime().addShutdownHook(new Thread(() -> {
            socketServer.cerrarServidor();
            System.out.println("Servidor cerrado correctamente.");
        }));

        // Crear instancia de CommandHandler
        CommandHandler commandHandler = new CommandHandler();
        Scanner scanner = new Scanner(System.in);

        // Bucle para permitir el envío de múltiples comandos
        while (true) {
            System.out.println("Ingrese comando (ejemplo: brick_pwr (f,c,power)):");
            String input = scanner.nextLine();

            // Procesar comando ingresado
            commandHandler.processInputCommand(input, cliente); // Aquí usa el cliente conectado
        }
    }
}
