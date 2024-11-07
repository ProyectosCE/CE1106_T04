package org.proyectosce;

import org.proyectosce.comunicaciones.ComServer;
import org.proyectosce.ui.MainWindow;

import javax.swing.*;
import java.util.ArrayList;
import java.util.List;

public class Main {
    public static void main(String[] args) {
        // Ejecutar ComServer en un nuevo thread
        Thread serverThread = new Thread(() -> ComServer.getInstance().iniciarServidor());
        serverThread.start();

        // Crear la ventana principal
        SwingUtilities.invokeLater(() -> {
            MainWindow mainWindow = new MainWindow();
            mainWindow.showWindow();

            // Ejemplo de actualización de las listas (esto se haría al recibir los clientes)
            List<String> jugadores = new ArrayList<>();
            jugadores.add("Jugador 1");
            jugadores.add("Jugador 2");

            List<String> espectadores = new ArrayList<>();
            espectadores.add("Espectador 1");
            espectadores.add("Espectador 2");

            // Actualizar la interfaz con las listas de clientes
            mainWindow.updateClientLists(jugadores, espectadores);
        });

    }
}
