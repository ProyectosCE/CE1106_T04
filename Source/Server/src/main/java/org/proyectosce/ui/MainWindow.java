package org.proyectosce.ui;

import javax.swing.*;
import java.awt.*;
import java.util.List;
import org.proyectosce.comunicaciones.ComServer;
import org.proyectosce.comandos.CommandHandler;

public class MainWindow extends JFrame {
    private ClientListPanel clientListPanel;
    private ComServer comServer;
    private CommandHandler commandHandler;

    // Lista de comandos válidos
    private final List<String> validCommands = List.of("add_life", "add_ball", "double_racket_size", "half_racket_size", "add_speed", "reduce_speed");

    public MainWindow() {
        setTitle("Servidor de Juego");
        setSize(600, 500);  // Ajustar el tamaño de la ventana
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);

        this.comServer = ComServer.getInstance();
        this.comServer.setUpdateCallback(this::updateClientLists); // Configura la función de callback
        this.commandHandler = new CommandHandler();

        clientListPanel = new ClientListPanel();
        add(clientListPanel, BorderLayout.CENTER);

        // Configurar el botón de enviar comando
        clientListPanel.getSendCommandButton().addActionListener(e -> {
            // Validar que el JComboBox de jugadores no esté vacío
            if (clientListPanel.getSelectedPlayer() == null) {
                JOptionPane.showMessageDialog(this, "No se ha seleccionado un cliente jugador.\nSeleccione un cliente de la lista de jugadores para enviar el comando.",
                        "Error de Cliente Jugador", JOptionPane.ERROR_MESSAGE);
                return;
            }

            String inputCommand = clientListPanel.getCommandInputField().getText().trim();
            if (!inputCommand.isEmpty()) {
                if (isValidCommand(inputCommand)) {
                    processCommand(inputCommand);
                } else {
                    // Mostrar mensaje de error si el comando es inválido
                    JOptionPane.showMessageDialog(this, "Comando inválido. Asegúrese de usar el formato:\n" +
                                    "brick_pwr (i,j,comando)\n" +
                                    "Donde:\n" +
                                    "- i y j son enteros positivos que están en el rango de la matriz del cliente\n" +
                                    "- Comando debe ser uno de estos: add_life, add_ball, double_racket_size, half_racket_size, add_speed, reduce_speed",
                            "Error de Comando", JOptionPane.ERROR_MESSAGE);
                }
            } else {
                JOptionPane.showMessageDialog(this, "Por favor ingrese un comando válido.", "Error", JOptionPane.WARNING_MESSAGE);
            }
            clientListPanel.getCommandInputField().setText("");
        });
    }

    private void processCommand(String inputCommand) {
        // Lógica para procesar el comando
        System.out.println("Comando procesado: " + inputCommand);
    }

    // Método para verificar si el comando es válido
    private boolean isValidCommand(String command) {
        // Verificar que el comando comience con "brick_pwr ("
        if (!command.startsWith("brick_pwr (") || !command.endsWith(")")) {
            return false;
        }

        // Extraer los parámetros entre paréntesis
        String params = command.substring(command.indexOf('(') + 1, command.lastIndexOf(')'));
        String[] parts = params.split(",");

        // Verificar que haya tres partes y que las coordenadas sean enteros entre 0 y 8
        if (parts.length != 3) {
            return false;
        }

        try {
            int i = Integer.parseInt(parts[0].trim());
            int j = Integer.parseInt(parts[1].trim());
            String commandType = parts[2].trim();

            // Verificar que i y j estén en el rango de 0 a 8
            if (i < 0 || i > 8 || j < 0 || j > 8) {
                return false;
            }

            // Verificar que el comando sea uno de los válidos
            return validCommands.contains(commandType);
        } catch (NumberFormatException e) {
            return false; // Si no se pueden parsear i o j, el comando es inválido
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
