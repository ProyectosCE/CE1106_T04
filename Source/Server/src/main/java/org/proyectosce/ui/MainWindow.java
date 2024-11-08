/*
================================== LICENCIA =================
=================================
MIT License
Copyright (c) 2024  José Bernardo Barquero Bonilla,
                    Jose Eduardo Campos Salazar,
                    Jimmy Feng Feng,
                    Alexander Montero Vargas
Consulta el archivo LICENSE para más detalles.
=============================================================
Cambios y Configuraciones del Proyecto3=================================
*/

package org.proyectosce.ui;

import javax.swing.*;
import java.awt.*;
import java.util.List;
import java.util.Map;

import org.proyectosce.comandos.factory.CommandFactory;
import org.proyectosce.comandos.factory.products.Command;
import org.proyectosce.comunicaciones.Cliente;
import org.proyectosce.comunicaciones.ComServer;
import org.proyectosce.comandos.CommandHandler;

/* Class: MainWindow
   Ventana principal de la interfaz gráfica del servidor, que permite la interacción con los comandos
   y la gestión de jugadores y espectadores.
   Attributes:
       - clientListPanel: ClientListPanel - Panel que gestiona la lista de jugadores y espectadores.
       - comServer: ComServer - Instancia del servidor de comunicaciones.
       - commandHandler: CommandHandler - Manejador para los comandos enviados desde la ventana.
       - commandFactory: CommandFactory - Fábrica para crear comandos dinámicamente.
       - validCommands: List<String> - Lista de comandos válidos que se pueden enviar.
       - powerComboBox: JComboBox<String> - Selector de comandos.
       - field1: JTextField - Campo dinámico para ingresar valores.
       - field2: JTextField - Campo dinámico para ingresar valores.
       - label1: JLabel - Etiqueta dinámica asociada a field1.
       - label2: JLabel - Etiqueta dinámica asociada a field2.
   Constructor:
       - MainWindow: Inicializa y configura la ventana principal del servidor.
   Methods:
       - updateFieldLabels: Cambia las etiquetas dinámicas dependiendo del comando seleccionado.
       - processCommand: Procesa y ejecuta un comando basado en los parámetros ingresados.
       - updateClientLists: Actualiza las listas de jugadores y espectadores en la interfaz gráfica.
       - showWindow: Muestra la ventana principal.
   Example:
       MainWindow mainWindow = new MainWindow();
       mainWindow.showWindow();
   Problems:

   References:

*/
public class MainWindow extends JFrame {
    private ClientListPanel clientListPanel;
    private ComServer comServer;
    private CommandHandler commandHandler;
    CommandFactory commandFactory;

    private final List<String> validCommands = List.of(
            "ADD_LIFE",
            "ADD_BALL",
            "DOUBLE_RACKET",
            "HALF_RACKET",
            "SPEED_UP",
            "SPEED_DOWN",
            "UPDATE_POINTS"
    );

    private JComboBox<String> powerComboBox;
    private JTextField field1;
    private JTextField field2;
    private JLabel label1;
    private JLabel label2;

    public MainWindow() {
        setTitle("Servidor de Juego");
        setSize(600, 500);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);

        this.comServer = ComServer.getInstance();
        this.commandFactory = CommandFactory.getInstance();
        this.comServer.setUpdateCallback(this::updateClientLists);
        this.commandHandler = new CommandHandler();

        clientListPanel = new ClientListPanel();
        add(clientListPanel, BorderLayout.CENTER);

        JPanel commandPanel = new JPanel(new GridLayout(3, 2, 10, 10));
        powerComboBox = new JComboBox<>(validCommands.toArray(new String[0]));
        label1 = new JLabel("Row:");
        label2 = new JLabel("Col:");
        field1 = new JTextField();
        field2 = new JTextField();

        powerComboBox.addActionListener(e -> updateFieldLabels());

        commandPanel.add(new JLabel("Poder:"));
        commandPanel.add(powerComboBox);
        commandPanel.add(label1);
        commandPanel.add(field1);
        commandPanel.add(label2);
        commandPanel.add(field2);

        add(commandPanel, BorderLayout.SOUTH);

        clientListPanel.getSendCommandButton().addActionListener(e -> {
            if (clientListPanel.getSelectedPlayer() == null) {
                JOptionPane.showMessageDialog(this, "No se ha seleccionado un cliente jugador.\nSeleccione un cliente de la lista de jugadores para enviar el comando.",
                        "Error de Cliente Jugador", JOptionPane.ERROR_MESSAGE);
                return;
            }

            String selectedPower = (String) powerComboBox.getSelectedItem();
            String value1 = field1.getText().trim();
            String value2 = field2.getText().trim();

            if (!selectedPower.isEmpty()) {
                try {
                    int field1Value = Integer.parseInt(value1);
                    int field2Value = Integer.parseInt(value2);

                    if (field1Value > 0 && field2Value > 0) {
                        processCommand(selectedPower, field1Value, field2Value, clientListPanel.getSelectedPlayer());
                    } else {
                        JOptionPane.showMessageDialog(this, "Ambos campos deben ser mayores que cero.", "Error", JOptionPane.WARNING_MESSAGE);
                    }
                } catch (NumberFormatException ex) {
                    JOptionPane.showMessageDialog(this, "Por favor ingrese números enteros válidos en los campos.", "Error", JOptionPane.WARNING_MESSAGE);
                }
            } else {
                JOptionPane.showMessageDialog(this, "Por favor ingrese un comando válido.", "Error", JOptionPane.WARNING_MESSAGE);
            }
        });
    }

    /* Function: updateFieldLabels
       Cambia las etiquetas dinámicas dependiendo del comando seleccionado.
       Params:
           - Ninguno.
       Returns:
           - void - Sin valor de retorno.
       Example:
           updateFieldLabels();
       Problems:

       References:

    */
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

    /* Function: processCommand
       Procesa y ejecuta un comando basado en los parámetros ingresados.
       Params:
           - power: String - Nombre del comando.
           - row: int - Valor de fila o nivel.
           - col: int - Valor de columna o puntos.
           - jugadorId: String - Identificador del jugador.
       Returns:
           - void - Sin valor de retorno.
       Example:
           processCommand("ADD_LIFE", 1, 2, "jugador123");
       Problems:

       References:

    */
    private void processCommand(String power, int row, int col, String jugadorId) {
        try {
            Map<String, Object> params = Map.of(
                    "command", power,
                    "f", row,
                    "c", col,
                    "jugadorId", jugadorId
            );

            Command powerCommand = commandFactory.crearComando("brick_pwr", params);
            powerCommand.ejecutar();

            JOptionPane.showMessageDialog(this, "Comando ejecutado y enviado correctamente.", "Éxito", JOptionPane.INFORMATION_MESSAGE);
        } catch (Exception e) {
            JOptionPane.showMessageDialog(this, "Error al procesar el comando: " + e.getMessage(), "Error", JOptionPane.ERROR_MESSAGE);
            e.printStackTrace();
        }
    }

    /* Function: updateClientLists
       Actualiza las listas de jugadores y espectadores en la interfaz gráfica.
       Params:
           - jugadores: List<Cliente> - Lista de jugadores conectados.
           - espectadores: List<String> - Lista de espectadores conectados.
       Returns:
           - void - Sin valor de retorno.
       Example:
           updateClientLists(listaJugadores, listaEspectadores);
       Problems:

       References:

    */
    public void updateClientLists(List<Cliente> jugadores, List<String> espectadores) {
        SwingUtilities.invokeLater(() -> {
            clientListPanel.updatePlayersComboBox(jugadores);
            clientListPanel.updateSpectatorsComboBox(espectadores);
        });
    }

    /* Function: showWindow
       Muestra la ventana principal del servidor.
       Params:
           - Ninguno.
       Returns:
           - void - Sin valor de retorno.
       Example:
           mainWindow.showWindow();
       Problems:

       References:

    */
    public void showWindow() {
        setVisible(true);
    }
}
