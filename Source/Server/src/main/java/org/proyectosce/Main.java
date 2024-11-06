package org.proyectosce;

import org.proyectosce.comunicaciones.ComServer;

public class Main {
    public static void main(String[] args) {
        // Ejecutar ComServer en un nuevo thread
        Thread serverThread = new Thread(() -> ComServer.getInstance().iniciarServidor());
        serverThread.start();
    }
}
