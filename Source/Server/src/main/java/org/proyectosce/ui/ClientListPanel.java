package org.proyectosce.ui;

import javax.swing.*;
import java.awt.*;
import java.util.List;

public class ClientListPanel extends JPanel {
    private JComboBox<String> playersComboBox;
    private JComboBox<String> spectatorsComboBox;
    private JTextField commandInputField;  // Campo para ingresar el comando
    private JButton sendCommandButton;     // Botón para enviar el comando

    public ClientListPanel() {
        setLayout(new BorderLayout());

        JPanel comboPanel = new JPanel();
        comboPanel.setLayout(new GridLayout(4, 1));

        comboPanel.add(new JLabel("Jugadores"));
        playersComboBox = new JComboBox<>();
        comboPanel.add(playersComboBox);
        comboPanel.add(new JLabel("Espectadores"));
        spectatorsComboBox = new JComboBox<>();
        comboPanel.add(spectatorsComboBox);

        add(comboPanel, BorderLayout.CENTER);

        JPanel commandPanel = new JPanel();
        commandPanel.setLayout(new BorderLayout());

        commandInputField = new JTextField();
        commandPanel.add(commandInputField, BorderLayout.CENTER);

        sendCommandButton = new JButton("Enviar Comando");
        commandPanel.add(sendCommandButton, BorderLayout.EAST);

        add(commandPanel, BorderLayout.SOUTH);
    }

    public JTextField getCommandInputField() {
        return commandInputField;
    }

    public JButton getSendCommandButton() {
        return sendCommandButton;
    }

    public String getSelectedPlayer() {
        return (String) playersComboBox.getSelectedItem();
    }

    public void updatePlayersComboBox(List<String> jugadores) {
        playersComboBox.removeAllItems();
        for (String jugador : jugadores) {
            playersComboBox.addItem(jugador);
        }
    }

    public void updateSpectatorsComboBox(List<String> espectadores) {
        spectatorsComboBox.removeAllItems();
        for (String espectador : espectadores) {
            spectatorsComboBox.addItem(espectador);
        }
    }
}
