package org.proyectosce.ui;

import javax.swing.*;
import java.awt.*;
import java.util.List;
import org.proyectosce.comunicaciones.ComServer;
import org.proyectosce.comunicaciones.Cliente;
import org.proyectosce.comandos.CommandHandler;

public class MainWindow extends JFrame {
    private ClientListPanel clientListPanel;
    private JTextArea commandOutputArea;
    private ComServer comServer;
    private CommandHandler commandHandler;

    public MainWindow() {
        setTitle("Servidor de Juego");
        setSize(600, 500);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);

        this.comServer = ComServer.getInstance();
        this.comServer.setUpdateCallback(this::updateClientLists); // Configura la función de callback
        this.commandHandler = new CommandHandler();

        clientListPanel = new ClientListPanel();
        add(clientListPanel, BorderLayout.CENTER);

        commandOutputArea = new JTextArea();
        commandOutputArea.setEditable(false);
        JScrollPane scrollPane = new JScrollPane(commandOutputArea);
        add(scrollPane, BorderLayout.SOUTH);

        clientListPanel.getSendCommandButton().addActionListener(e -> {
            String inputCommand = clientListPanel.getCommandInputField().getText().trim();
            if (!inputCommand.isEmpty()) {
                processCommand(inputCommand);
            } else {
                commandOutputArea.append("Por favor ingrese un comando válido.\n");
            }
            clientListPanel.getCommandInputField().setText("");
        });
    }

    private void processCommand(String inputCommand) {
        // Lógica para procesar el comando
        commandOutputArea.append("Comando procesado: " + inputCommand + "\n");
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
