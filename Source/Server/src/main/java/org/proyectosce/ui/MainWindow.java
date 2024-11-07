package org.proyectosce.ui;

import javax.swing.*;
import java.awt.*;

public class MainWindow extends JFrame {

    private ClientListPanel clientListPanel;

    public MainWindow() {
        // Configurando la ventana principal
        setTitle("Servidor de Juego");
        setSize(400, 300);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);

        // Crear y agregar el panel de la lista de clientes
        clientListPanel = new ClientListPanel();
        add(clientListPanel, BorderLayout.CENTER);
    }

    // Método para actualizar los JComboBox con los jugadores y espectadores
    public void updateClientLists(java.util.List<String> jugadores, java.util.List<String> espectadores) {
        clientListPanel.updatePlayersComboBox(jugadores);
        clientListPanel.updateSpectatorsComboBox(espectadores);
    }

    // Mostrar la ventana
    public void showWindow() {
        setVisible(true);
    }
}
