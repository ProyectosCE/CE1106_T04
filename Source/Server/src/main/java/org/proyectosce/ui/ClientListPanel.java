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

import org.proyectosce.comunicaciones.Cliente;
import javax.swing.*;
import java.awt.*;
import java.util.List;

/* Class: ClientListPanel
    Componente de interfaz gráfica que muestra una lista de jugadores y espectadores.
    Proporciona opciones para seleccionar clientes y enviar comandos asociados.

    Attributes:
        - playersComboBox: JComboBox<Cliente> - Componente para seleccionar jugadores.
        - spectatorsComboBox: JComboBox<String> - Componente para seleccionar espectadores.
        - commandInputField: JTextField - Campo de entrada para comandos (futuro uso).
        - sendCommandButton: JButton - Botón para enviar comandos seleccionados.

    Constructor:
        - ClientListPanel: Configura la interfaz gráfica con listas de jugadores y espectadores.

    Methods:
        - getSendCommandButton: Retorna el botón para enviar comandos.
        - getSelectedPlayer: Obtiene el jugador seleccionado de la lista.
        - updatePlayersComboBox: Actualiza la lista de jugadores disponibles.
        - updateSpectatorsComboBox: Actualiza la lista de espectadores disponibles.

    Example:
        ClientListPanel panel = new ClientListPanel();
        panel.updatePlayersComboBox(listaJugadores);
        panel.updateSpectatorsComboBox(listaEspectadores);
*/

public class ClientListPanel extends JPanel {

    private JComboBox<Cliente> playersComboBox;
    private JComboBox<String> spectatorsComboBox;
    private JTextField commandInputField; // Campo para ingresar el comando (futuro uso)
    private JButton sendCommandButton;    // Botón para enviar el comando

    /* Function: ClientListPanel
        Constructor que inicializa los componentes gráficos para listas de jugadores y espectadores.
    */
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

    /* Function: getSendCommandButton
        Retorna el botón utilizado para enviar comandos.

        Returns:
            - JButton - Botón para enviar comandos.
    */
    public JButton getSendCommandButton() {
        return sendCommandButton;
    }

    /* Function: getSelectedPlayer
        Obtiene el identificador del jugador seleccionado en el JComboBox.

        Returns:
            - String - ID del cliente seleccionado, o null si no hay selección.
    */
    public String getSelectedPlayer() {
        Cliente seleccionado = (Cliente) playersComboBox.getSelectedItem();
        if (seleccionado != null) {
            return seleccionado.getId(); // Devuelve el ID del cliente seleccionado
        }
        return null; // Retorna null si no hay selección
    }

    /* Function: updatePlayersComboBox
        Actualiza los elementos del JComboBox con la lista de jugadores proporcionada.

        Params:
            - jugadores: List<Cliente> - Lista de jugadores a mostrar en el JComboBox.
    */
    public void updatePlayersComboBox(List<Cliente> jugadores) {
        playersComboBox.removeAllItems(); // Limpia el JComboBox
        for (Cliente jugador : jugadores) {
            playersComboBox.addItem(jugador); // Agrega objetos Cliente directamente
        }
    }

    /* Function: updateSpectatorsComboBox
        Actualiza los elementos del JComboBox con la lista de espectadores proporcionada.

        Params:
            - espectadores: List<String> - Lista de espectadores a mostrar en el JComboBox.
    */
    public void updateSpectatorsComboBox(List<String> espectadores) {
        spectatorsComboBox.removeAllItems();
        for (String espectador : espectadores) {
            spectatorsComboBox.addItem(espectador);
        }
    }
}
