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
        // Configurar el panel
        setLayout(new BorderLayout());

        // Crear JComboBox para jugadores y espectadores
        playersComboBox = new JComboBox<>();
        spectatorsComboBox = new JComboBox<>();

        // Crear el panel superior con los ComboBox
        JPanel comboPanel = new JPanel();
        comboPanel.setLayout(new GridLayout(2, 1));  // Dos filas: una para jugadores, otra para espectadores
        comboPanel.add(new JLabel("Jugadores"));
        comboPanel.add(playersComboBox);
        comboPanel.add(new JLabel("Espectadores"));
        comboPanel.add(spectatorsComboBox);
        add(comboPanel, BorderLayout.NORTH);

        // Crear panel de comandos
        JPanel commandPanel = new JPanel();
        commandPanel.setLayout(new BorderLayout());

        // Crear campo para ingresar el comando
        commandInputField = new JTextField();
        commandPanel.add(commandInputField, BorderLayout.NORTH);

        // Crear botón para enviar el comando
        sendCommandButton = new JButton("Enviar Comando");
        commandPanel.add(sendCommandButton, BorderLayout.SOUTH);

        add(commandPanel, BorderLayout.SOUTH);
    }

    // Métodos para acceder a los componentes desde MainWindow
    public JTextField getCommandInputField() {
        return commandInputField;
    }

    public JButton getSendCommandButton() {
        return sendCommandButton;
    }

    // Métodos para actualizar las listas de jugadores y espectadores
    public void updatePlayersComboBox(List<String> jugadores) {
        System.out.println("actualizando lista de jugadores en ListPanel");
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
