package org.proyectosce.ui;

import javax.swing.*;
import java.awt.*;
import java.util.List;

public class ClientListPanel extends JPanel {

    private JComboBox<String> playersComboBox;
    private JComboBox<String> spectatorsComboBox;

    public ClientListPanel() {
        setLayout(new GridLayout(2, 1));

        // Crear los JComboBox para los jugadores y espectadores
        playersComboBox = new JComboBox<>();
        spectatorsComboBox = new JComboBox<>();

        // Panel para los jugadores
        JPanel playersPanel = new JPanel();
        playersPanel.setLayout(new BorderLayout());
        playersPanel.add(new JLabel("Jugadores Conectados:"), BorderLayout.NORTH);
        playersPanel.add(playersComboBox, BorderLayout.CENTER);

        // Panel para los espectadores
        JPanel spectatorsPanel = new JPanel();
        spectatorsPanel.setLayout(new BorderLayout());
        spectatorsPanel.add(new JLabel("Espectadores Conectados:"), BorderLayout.NORTH);
        spectatorsPanel.add(spectatorsComboBox, BorderLayout.CENTER);

        // Agregar los paneles al panel principal
        add(playersPanel);
        add(spectatorsPanel);
    }

    // Método para actualizar el JComboBox de jugadores
    public void updatePlayersComboBox(List<String> players) {
        DefaultComboBoxModel<String> model = new DefaultComboBoxModel<>();
        for (String player : players) {
            model.addElement(player);
        }
        playersComboBox.setModel(model);
    }

    // Método para actualizar el JComboBox de espectadores
    public void updateSpectatorsComboBox(List<String> spectators) {
        DefaultComboBoxModel<String> model = new DefaultComboBoxModel<>();
        for (String spectator : spectators) {
            model.addElement(spectator);
        }
        spectatorsComboBox.setModel(model);
    }
}
