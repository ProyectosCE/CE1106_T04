package org.proyectosce.ui;

import javax.swing.*;
import java.awt.*;
import java.util.List;
import java.util.Map;

import org.proyectosce.comandos.factory.CommandFactory;
import org.proyectosce.comandos.factory.products.Command;
import org.proyectosce.comunicaciones.ComServer;
import org.proyectosce.comandos.CommandHandler;

public class MainWindow extends JFrame {
    private ClientListPanel clientListPanel;
    private ComServer comServer;
    private CommandHandler commandHandler;
    CommandFactory commandFactory;

    // Lista de comandos válidos
    private final List<String> validCommands = List.of(
            "ADD_LIFE",
            "ADD_BALL",
            "DOUBLE_RACKET",
            "HALF_RACKET",
            "SPEED_UP",
            "SPEED_DOWN",
            "UPDATE_POINTS"
    );

    // Componentes dinámicos
    private JComboBox<String> powerComboBox;
    private JTextField field1;
    private JTextField field2;
    private JLabel label1;
    private JLabel label2;

    public MainWindow() {
        setTitle("Servidor de Juego");
        setSize(600, 500);  // Ajustar el tamaño de la ventana
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);

        this.comServer = ComServer.getInstance();
        this.commandFactory = CommandFactory.getInstance();
        this.comServer.setUpdateCallback(this::updateClientLists); // Configura la función de callback
        this.commandHandler = new CommandHandler();

        clientListPanel = new ClientListPanel();
        add(clientListPanel, BorderLayout.CENTER);

        // Configurar panel inferior para los nuevos controles
        JPanel commandPanel = new JPanel(new GridLayout(3, 2, 10, 10));
        powerComboBox = new JComboBox<>(validCommands.toArray(new String[0]));
        label1 = new JLabel("Row:");
        label2 = new JLabel("Col:");
        field1 = new JTextField();
        field2 = new JTextField();

        // Cambiar dinámicamente las etiquetas y campos según el poder seleccionado
        powerComboBox.addActionListener(e -> updateFieldLabels());

        commandPanel.add(new JLabel("Poder:"));
        commandPanel.add(powerComboBox);
        commandPanel.add(label1);
        commandPanel.add(field1);
        commandPanel.add(label2);
        commandPanel.add(field2);

        add(commandPanel, BorderLayout.SOUTH);

        // Configurar el botón de enviar comando
        // Configurar el botón de enviar comando
        clientListPanel.getSendCommandButton().addActionListener(e -> {
            // Validar que el JComboBox de jugadores no esté vacío
            if (clientListPanel.getSelectedPlayer() == null) {
                JOptionPane.showMessageDialog(this, "No se ha seleccionado un cliente jugador.\nSeleccione un cliente de la lista de jugadores para enviar el comando.",
                        "Error de Cliente Jugador", JOptionPane.ERROR_MESSAGE);
                return;
            }

            String selectedPower = (String) powerComboBox.getSelectedItem();
            String value1 = field1.getText().trim();
            String value2 = field2.getText().trim();

            if (!selectedPower.isEmpty()) {
                // Validar que field1 y field2 sean enteros positivos mayores que cero
                try {
                    int field1Value = Integer.parseInt(value1);
                    int field2Value = Integer.parseInt(value2);

                    if (field1Value > 0 && field2Value > 0) {
                        // Si ambas validaciones son correctas, procesar el comando
                        processCommand(selectedPower, field1Value, field2Value,clientListPanel.getSelectedPlayer());
                    } else {
                        JOptionPane.showMessageDialog(this, "Ambos campos deben ser mayores que cero.", "Error", JOptionPane.WARNING_MESSAGE);
                    }
                } catch (NumberFormatException ex) {
                    // Si ocurre un error al intentar convertir a entero
                    JOptionPane.showMessageDialog(this, "Por favor ingrese números enteros válidos en los campos.", "Error", JOptionPane.WARNING_MESSAGE);
                }
            } else {
                JOptionPane.showMessageDialog(this, "Por favor ingrese un comando válido.", "Error", JOptionPane.WARNING_MESSAGE);
            }
        });

    }

    private void updateFieldLabels() {
        String selectedPower = (String) powerComboBox.getSelectedItem();
        if ("UPDATE_POINTS".equals(selectedPower)) {
            label1.setText("Level:");
            label2.setText("Points:");
        } else {
            label1.setText("Row:");
            label2.setText("Col:");
        }
    }

    private void processCommand(String power, int row, int col, String jugadorId) {
        try {
            // Crear el comando usando CommandFactory
            Map<String, Object> params = Map.of(
                    "command", power,
                    "f", row,
                    "c", col,
                    "jugadorId", jugadorId
            );


            Command powerCommand = commandFactory.crearComando("brick_pwr", params);

            // Ejecutar el comando
            powerCommand.ejecutar();

            JOptionPane.showMessageDialog(this, "Comando ejecutado y enviado correctamente.", "Éxito", JOptionPane.INFORMATION_MESSAGE);
        } catch (Exception e) {
            JOptionPane.showMessageDialog(this, "Error al procesar el comando: " + e.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);
            e.printStackTrace();
        }
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
