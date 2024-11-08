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
package org.proyectosce.comandos.factory.products;

import org.proyectosce.comunicaciones.ComServer;
import org.proyectosce.comunicaciones.SocketServer;
import org.proyectosce.comunicaciones.Cliente;
import java.util.Map;
import java.util.HashMap;

/*
 * Class: PowerCommand
 * Representa un comando que envía un "poder" a un jugador en una ubicación específica en el juego.
 *
 * Attributes:
 *     - type: String - Tipo de poder a aplicar (por ejemplo, "brick_pwr").
 *     - f: int - Posición i (fila) en la que aplicar el poder.
 *     - c: int - Posición j (columna) en la que aplicar el poder.
 *     - jugadorId: String - ID del jugador al que se enviará el comando.
 *     - comServer: ComServer - Referencia al servidor de comunicaciones para obtener información del cliente.
 *     - socketServer: SocketServer - Referencia al servidor de sockets para enviar el mensaje.
 *
 * Methods:
 *     - ejecutar(): Ejecuta el comando enviando un mensaje al jugador especificado.
 *     - getType(): Devuelve el tipo de comando (el tipo de poder).
 *     - toMap(): Convierte el comando a un mapa con detalles para serialización.
 *     - configure(Map<String, Object>): Configura el comando con los parámetros proporcionados.
 *
 * Example:
 *     Map<String, Object> params = Map.of(
 *         "command", "brick_pwr",
 *         "f", 2,
 *         "c", 3,
 *         "jugadorId", "player123",
 *         "comServer", comServerInstance,
 *         "socketServer", socketServerInstance
 *     );
 *     Command command = new PowerCommand();
 *     command.configure(params);
 *     command.ejecutar(); // Envia el comando al jugador.
 *
 * Problems:
 *     - No se han reportado problemas conocidos en la implementación.
 *
 * References:
 */
public class PowerCommand implements Command {

    private String type; // Tipo de poder
    private int f; // Posición i
    private int c; // Posición j
    private String jugadorId; // ID del jugador al que se enviará el comando
    private ComServer comServer; // Referencia al ComServer para obtener el cliente
    private SocketServer socketServer; // Referencia al SocketServer para enviar el mensaje

    /* Constructor: PowerCommand
        Permite la configuración dinámica del comando.
    */
    public PowerCommand() {}

    /* Function: ejecutar
        Ejecuta el comando enviando un mensaje al jugador correspondiente con el poder y la posición especificada.

        Example:
            Si el tipo de poder es "brick_pwr" y la posición es (2, 3), se enviará un mensaje con el formato:
            {"command": "brickUpdate", "row": 2, "column": 3, "power": "brick_pwr"}
    */
    @Override
    public void ejecutar() {
        if (type == null || jugadorId == null || comServer == null || socketServer == null) {
            throw new IllegalStateException("PowerCommand no está configurado correctamente.");
        }

        // Obtener el cliente jugador por ID
        Cliente jugador = comServer.obtenerClientePorId(jugadorId);
        if (jugador == null) {
            System.err.println("Jugador no encontrado con ID: " + jugadorId);
            return;
        }

        // Crear el JSON del comando
        String comandoJson = String.format(
                "{\"command\": \"brickUpdate\", \"row\": %d, \"column\": %d, \"power\": \"%s\"}",
                f, c, type
        );

        // Enviar el mensaje al cliente
        try {
            socketServer.enviarMensaje(jugador, comandoJson);
            System.out.println("Comando enviado: " + comandoJson);
        } catch (Exception e) {
            System.err.println("Error al enviar comando al jugador: " + e.getMessage());
            e.printStackTrace();
        }
    }

    /* Function: getType
        Devuelve el tipo de comando.

        Returns:
            - String: El tipo de poder (por ejemplo, "brick_pwr").
    */
    @Override
    public String getType() {
        return type;
    }

    /* Function: toMap
        Convierte el comando en un mapa para su serialización o almacenamiento.

        Returns:
            - Map<String, Object>: Mapa con los detalles del comando.
    */
    @Override
    public Map<String, Object> toMap() {
        Map<String, Object> jsonMessage = new HashMap<>();
        jsonMessage.put("command", type);

        Map<String, Integer> position = new HashMap<>();
        position.put("i", f);
        position.put("j", c);

        jsonMessage.put("position", position);
        return jsonMessage;
    }

    /* Function: configure
        Configura el comando con los parámetros proporcionados.

        Params:
            - params: Map<String, Object> - Mapa con los parámetros necesarios para configurar el comando.
              Debe contener "command", "f", "c", "jugadorId", "comServer" y "socketServer".
    */
    @Override
    public void configure(Map<String, Object> params) {
        this.type = (String) params.get("command");
        this.f = (int) params.get("f");
        this.c = (int) params.get("c");
        this.jugadorId = (String) params.get("jugadorId");
        this.comServer = (ComServer) params.get("comServer");
        this.socketServer = (SocketServer) params.get("socketServer");
    }
}
