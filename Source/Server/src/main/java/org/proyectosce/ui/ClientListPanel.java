package org.proyectosce.ui;

import org.proyectosce.comunicaciones.Cliente;

import javax.swing.*;
import java.awt.*;
import java.util.List;

public class ClientListPanel extends JPanel {
    private JComboBox<Cliente> playersComboBox;
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
        Cliente seleccionado = (Cliente) playersComboBox.getSelectedItem();
        if (seleccionado != null) {
            return seleccionado.getId(); // Devuelve el ID del cliente seleccionado
        }
        return null; // Retorna null si no hay selección
    }

    public void updatePlayersComboBox(List<Cliente> jugadores) {
        playersComboBox.removeAllItems(); // Limpia el JComboBox
        for (Cliente jugador : jugadores) {
            playersComboBox.addItem(jugador); // Agrega objetos Cliente directamente
        }
    }

    public void updateSpectatorsComboBox(List<String> espectadores) {
        spectatorsComboBox.removeAllItems();
        for (String espectador : espectadores) {
            spectatorsComboBox.addItem(espectador);
        }
    }
}
