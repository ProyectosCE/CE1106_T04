package org.proyectosce.ui;

import javax.swing.*;
import java.awt.*;
import java.util.List;
import org.proyectosce.comunicaciones.ComServer;
import org.proyectosce.comandos.CommandHandler;

public class MainWindow extends JFrame {
    private ClientListPanel clientListPanel;
    private ComServer comServer;
    private CommandHandler commandHandler;

    public MainWindow() {
        setTitle("Servidor de Juego");
        setSize(600, 500);  // Ajustar el tamaño de la ventana
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);

        this.comServer = ComServer.getInstance();
        this.comServer.setUpdateCallback(this::updateClientLists); // Configura la función de callback
        this.commandHandler = new CommandHandler();

        clientListPanel = new ClientListPanel();
        add(clientListPanel, BorderLayout.CENTER);

        // Configurar el botón de enviar comando
        clientListPanel.getSendCommandButton().addActionListener(e -> {
            String inputCommand = clientListPanel.getCommandInputField().getText().trim();
            if (!inputCommand.isEmpty()) {
                processCommand(inputCommand);
            } else {
                JOptionPane.showMessageDialog(this, "Por favor ingrese un comando válido.", "Error", JOptionPane.WARNING_MESSAGE);
            }
            clientListPanel.getCommandInputField().setText("");
        });
    }

    private void processCommand(String inputCommand) {
        // Lógica para procesar el comando (puedes agregar la implementación específica aquí)
        System.out.println("Comando procesado: " + inputCommand);
    }

    // Método para actualizar las listas de JComboBox de forma segura en el hilo de la GUI
    public void updateClientLists(List<String> jugadores, List<String> espectadores) {
        SwingUtilities.invokeLater(() -> {
            clientListPanel.updatePlayersComboBox(jugadores);
            clientListPanel.updateSpectatorsComboBox(espectadores);
        });
    }

    public void showWindow() {
        setVisible(true);
    }
}
