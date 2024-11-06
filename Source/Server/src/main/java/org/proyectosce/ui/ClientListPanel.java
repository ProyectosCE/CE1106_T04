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
        // Configurar el panel con un layout de BorderLayout
        setLayout(new BorderLayout());

        // Crear JPanel para los ComboBox y sus etiquetas
        JPanel comboPanel = new JPanel();
        comboPanel.setLayout(new GridLayout(4, 1));  // Cuatro filas: dos etiquetas y dos JComboBox

        // Crear y agregar componentes al panel de ComboBox
        comboPanel.add(new JLabel("Jugadores"));
        playersComboBox = new JComboBox<>();
        comboPanel.add(playersComboBox);
        comboPanel.add(new JLabel("Espectadores"));
        spectatorsComboBox = new JComboBox<>();
        comboPanel.add(spectatorsComboBox);

        // Agregar el panel de ComboBox a la parte central
        add(comboPanel, BorderLayout.CENTER);

        // Crear un panel inferior para el campo de texto y el botón
        JPanel commandPanel = new JPanel();
        commandPanel.setLayout(new BorderLayout());

        // Crear campo para ingresar el comando
        commandInputField = new JTextField();
        commandPanel.add(commandInputField, BorderLayout.CENTER);

        // Crear botón para enviar el comando
        sendCommandButton = new JButton("Enviar Comando");
        commandPanel.add(sendCommandButton, BorderLayout.EAST);

        // Agregar el panel de comandos en la parte inferior
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
