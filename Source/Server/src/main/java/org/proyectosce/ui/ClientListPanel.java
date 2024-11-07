package org.proyectosce.ui;

import javax.swing.*;
import java.awt.*;
import java.util.List;

public class ClientListPanel extends JPanel {

    private JComboBox<String> playersComboBox;
    private JComboBox<String> spectatorsComboBox;
    private JButton sendCommandButton;
    private JTextField commandTextField;

    public ClientListPanel() {
        setLayout(new BorderLayout());

        // Panel de clientes (Jugadores y Espectadores)
        JPanel clientPanels = new JPanel();
        clientPanels.setLayout(new GridLayout(2, 1, 0, 30)); // Espacio entre los JComboBox

        // Panel para los jugadores
        JPanel playersPanel = new JPanel();
        playersPanel.setLayout(new BorderLayout());

        JLabel playersLabel = new JLabel("Jugadores Conectados:");
        playersLabel.setHorizontalAlignment(SwingConstants.CENTER);  // Centrado
        playersPanel.add(playersLabel, BorderLayout.NORTH);

        playersComboBox = new JComboBox<>();
        playersPanel.add(playersComboBox, BorderLayout.CENTER);

        // Panel para los espectadores
        JPanel spectatorsPanel = new JPanel();
        spectatorsPanel.setLayout(new BorderLayout());

        JLabel spectatorsLabel = new JLabel("Espectadores Conectados:");
        spectatorsLabel.setHorizontalAlignment(SwingConstants.CENTER);  // Centrado
        spectatorsPanel.add(spectatorsLabel, BorderLayout.NORTH);

        spectatorsComboBox = new JComboBox<>();
        spectatorsPanel.add(spectatorsComboBox, BorderLayout.CENTER);

        clientPanels.add(playersPanel);
        clientPanels.add(spectatorsPanel);

        // Panel de comando
        JPanel commandPanel = new JPanel();
        commandPanel.setLayout(new FlowLayout(FlowLayout.LEFT, 10, 10));

        // Botón "Enviar Comando"
        sendCommandButton = new JButton("Enviar Comando");

        // Área para ingresar texto
        commandTextField = new JTextField(20);  // Ancho del cuadro de texto
        commandTextField.setPreferredSize(new Dimension(200, 30));  // Tamaño adecuado
        commandTextField.setBorder(BorderFactory.createLineBorder(Color.BLACK));  // Recuadro

        // Agregar el botón y el cuadro de texto en el panel de comandos
        commandPanel.add(sendCommandButton);
        commandPanel.add(commandTextField);

        // Agregar los paneles a la interfaz
        add(clientPanels, BorderLayout.CENTER);
        add(commandPanel, BorderLayout.SOUTH);  // Colocamos el panel de comandos abajo
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
