package org.proyectosce;

import org.proyectosce.comunicaciones.ComServer;
import org.proyectosce.ui.MainWindow;
import javax.swing.*;

public class Main {
    public static void main(String[] args) {
        // Ejecutar ComServer en un nuevo thread
        Thread serverThread = new Thread(() -> ComServer.getInstance().iniciarServidor());
        serverThread.start();

        // Crear la ventana principal
        SwingUtilities.invokeLater(() -> {
            MainWindow mainWindow = new MainWindow();
            ComServer.getInstance().setMainWindow(mainWindow);
            mainWindow.showWindow();

        });

    }
}
