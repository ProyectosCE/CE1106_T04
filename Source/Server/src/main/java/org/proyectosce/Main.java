/*
================================== LICENCIA =================
=================================
MIT License
Copyright (c) 2024  José Bernardo Barquero Bonilla,
                    Jose Eduardo Campos Salazar,
                    Jimmy Feng Feng,
                    Alexander Montero Vargas
Consulta el archivo LICENSE para más detalles.
=============================================================
Cambios y Configuraciones del Proyecto3=================================
*/
package org.proyectosce;

import org.proyectosce.comunicaciones.ComServer;
import org.proyectosce.ui.MainWindow;
import javax.swing.*;

/* Class: Main
Clase principal encargada de iniciar el servidor de comunicaciones y la ventana principal del sistema.
Attributes:
    (No aplica para esta clase).
Constructor:
    (No aplica para esta clase).
Methods:
    - main: Método principal para ejecutar el servidor y la interfaz gráfica.
Example:
    No se aplica para esta clase.
Problems:

References:

*/
public class Main {
    /* Function: main
    Método principal para iniciar la ejecución del servidor de comunicaciones y de la ventana principal.
    Params:
        - args: String[] - argumentos pasados por línea de comandos (no utilizados en esta implementación).
    Returns:
        - void - este método no retorna valor.
    Restriction:
        - Asegúrese de que `ComServer` y `MainWindow` estén correctamente configurados.
    Example:
        No aplica.
    Problems:

    References:

    */
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
