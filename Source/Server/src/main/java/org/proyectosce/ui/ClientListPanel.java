package org.proyectosce.ui;

import javax.swing.*;
import java.awt.*;
import java.util.List;

public class ClientListPanel extends JPanel {

    private JComboBox<String> playersComboBox;
    private JComboBox<String> spectatorsComboBox;

    public ClientListPanel() {
        setLayout(new GridLayout(2, 1, 0, 20)); // Añadido espacio entre filas (20px)

        // Crear los JComboBox para los jugadores y espectadores
        playersComboBox = new JComboBox<>();
        spectatorsComboBox = new JComboBox<>();

        // Panel para los jugadores
        JPanel playersPanel = new JPanel();
        playersPanel.setLayout(new BorderLayout());

        JLabel playersLabel = new JLabel("Jugadores Conectados:");
        playersLabel.setHorizontalAlignment(SwingConstants.CENTER);  // Centrado del texto
        playersPanel.add(playersLabel, BorderLayout.NORTH);
        playersPanel.add(playersComboBox, BorderLayout.CENTER);

        // Panel para los espectadores
        JPanel spectatorsPanel = new JPanel();
        spectatorsPanel.setLayout(new BorderLayout());

        JLabel spectatorsLabel = new JLabel("Espectadores Conectados:");
        spectatorsLabel.setHorizontalAlignment(SwingConstants.CENTER);  // Centrado del texto
        spectatorsPanel.add(spectatorsLabel, BorderLayout.NORTH);
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
