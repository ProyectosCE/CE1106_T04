package org.proyectosce.ui;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
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
        playersComboBox.addItem("");  // Campo vacío al inicio
        playersPanel.add(playersComboBox, BorderLayout.CENTER);

        // Panel para los espectadores
        JPanel spectatorsPanel = new JPanel();
        spectatorsPanel.setLayout(new BorderLayout());

        JLabel spectatorsLabel = new JLabel("Espectadores Conectados:");
        spectatorsLabel.setHorizontalAlignment(SwingConstants.CENTER);  // Centrado
        spectatorsPanel.add(spectatorsLabel, BorderLayout.NORTH);

        spectatorsComboBox = new JComboBox<>();
        spectatorsComboBox.addItem("");  // Campo vacío al inicio
        spectatorsPanel.add(spectatorsComboBox, BorderLayout.CENTER);

        clientPanels.add(playersPanel);
        clientPanels.add(spectatorsPanel);

        // Panel de comando
        JPanel commandPanel = new JPanel();
        commandPanel.setLayout(new GridBagLayout());  // Para centrar los componentes
        GridBagConstraints gbc = new GridBagConstraints();
        gbc.fill = GridBagConstraints.HORIZONTAL;
        gbc.insets = new Insets(10, 10, 10, 10);  // Espacio alrededor de los componentes

        // Botón "Enviar Comando Jugador"
        sendCommandButton = new JButton("Enviar Comando Jugador");

        // Acción para el botón
        sendCommandButton.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                String selectedPlayer = (String) playersComboBox.getSelectedItem();
                String commandText = commandTextField.getText();

                // Validación de selección de jugador
                if (selectedPlayer == null || selectedPlayer.isEmpty()) {
                    JOptionPane.showMessageDialog(null, "Debe seleccionar un jugador antes de enviar el comando.", "Advertencia", JOptionPane.WARNING_MESSAGE);
                    return;
                }

                // Validación de que el campo de texto no esté vacío
                if (commandText == null || commandText.trim().isEmpty()) {
                    JOptionPane.showMessageDialog(null, "El comando no puede estar vacío.", "Advertencia", JOptionPane.WARNING_MESSAGE);
                    return;
                }

                // Aquí iría la lógica de enviar el comando, ya que ambas validaciones pasaron
                JOptionPane.showMessageDialog(null, "Comando enviado al jugador: " + selectedPlayer, "Comando Enviado", JOptionPane.INFORMATION_MESSAGE);
            }
        });

        // Área para ingresar texto
        commandTextField = new JTextField(20);  // Ancho del cuadro de texto
        commandTextField.setPreferredSize(new Dimension(200, 30));  // Tamaño adecuado
        commandTextField.setBorder(BorderFactory.createLineBorder(Color.BLACK));  // Recuadro

        // Agregar el botón y el cuadro de texto en el panel de comandos
        gbc.gridx = 0;
        gbc.gridy = 0;
        gbc.gridwidth = 2;
        commandPanel.add(sendCommandButton, gbc);

        gbc.gridx = 0;
        gbc.gridy = 1;
        gbc.gridwidth = 2;
        commandPanel.add(commandTextField, gbc);

        // Agregar los paneles a la interfaz
        add(clientPanels, BorderLayout.CENTER);
        add(commandPanel, BorderLayout.SOUTH);  // Colocamos el panel de comandos abajo
    }

    // Método para actualizar el JComboBox de jugadores
    public void updatePlayersComboBox(List<String> players) {
        DefaultComboBoxModel<String> model = new DefaultComboBoxModel<>();
        model.addElement("");  // Campo vacío al inicio
        for (String player : players) {
            model.addElement(player);
        }
        playersComboBox.setModel(model);
    }

    // Método para actualizar el JComboBox de espectadores
    public void updateSpectatorsComboBox(List<String> spectators) {
        DefaultComboBoxModel<String> model = new DefaultComboBoxModel<>();
        model.addElement("");  // Campo vacío al inicio
        for (String spectator : spectators) {
            model.addElement(spectator);
        }
        spectatorsComboBox.setModel(model);
    }
}
