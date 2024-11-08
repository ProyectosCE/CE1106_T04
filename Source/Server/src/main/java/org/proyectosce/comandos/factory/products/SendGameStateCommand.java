/*
================================== LICENCIA =================
=================================
MIT License
								Copyright (c) 2024 José Bernardo Barquero Bonilla,
								Jose Eduardo Campos Salazar,
								Jimmy Feng Feng,
								Alexander Montero Vargas
Consulta el archivo LICENSE para más detalles.
=============================================================
Cambios y Configuraciones del Proyecto3=================================
*/

package org.proyectosce.comandos.factory.products;

import org.proyectosce.comunicaciones.Cliente;
import org.proyectosce.comunicaciones.ComServer;
import org.proyectosce.comunicaciones.SocketServer;
import java.util.Map;
import java.util.Set;

/*
 * Class: SendGameStateCommand
 * Representa un comando que envía el estado del juego a los observadores de un jugador específico.
 *
 * Attributes:
 *     - gameStateJson: String - El estado del juego en formato JSON que se enviará a los observadores.
 *     - jugador: Cliente - El jugador cuyo estado del juego se enviará a sus observadores.
 *     - comServer: ComServer - Referencia al servidor de comunicaciones para obtener información sobre los observadores.
 *     - socketServer: SocketServer - Referencia al servidor de sockets para enviar el mensaje a los observadores.
 *
 * Methods:
 *     - ejecutar(): Ejecuta el comando enviando el estado del juego a los observadores del jugador.
 *     - getType(): Devuelve el tipo de comando (en este caso, "sendGameState").
 *     - toMap(): Convierte el comando en un mapa para su serialización.
 *     - configure(Map<String, Object>): Configura el comando con los parámetros proporcionados.
 *
 * Example:
 *     Map<String, Object> params = Map.of(
 *         "gameStateJson", "{\"state\":\"game over\"}",
 *         "emisor", jugador,
 *         "comServer", comServerInstance,
 *         "socketServer", socketServerInstance
 *     );
 *     Command command = new SendGameStateCommand();
 *     command.configure(params);
 *     command.ejecutar(); // Envía el estado del juego a los observadores.
 *
 * Problems:
 *     - No se han reportado problemas conocidos en la implementación.
 *
 * References:
 */
public class SendGameStateCommand implements Command {
    private String gameStateJson; // Estado del juego en formato JSON
    private Cliente jugador; // Jugador cuyo estado será enviado a los observadores
    private ComServer comServer; // Servidor de comunicaciones
    private SocketServer socketServer; // Servidor de sockets para enviar los mensajes

    /* Constructor: SendGameStateCommand
        Permite la configuración dinámica del comando.
    */
    public SendGameStateCommand() {}

    /* Function: ejecutar
        Ejecuta el comando enviando el estado del juego a todos los observadores del jugador.

        Example:
            Si el estado del juego es "{\"state\":\"game over\"}" y el jugador tiene observadores, el estado se enviará
            a todos los clientes observadores.
    */
    @Override
    public void ejecutar() {
        if (gameStateJson == null || jugador == null || comServer == null || socketServer == null) {
            throw new IllegalStateException("SendGameStateCommand no está configurado correctamente.");
        }

        // Obtener los observadores del jugador
        Set<Cliente> observadores = comServer.obtenerObservadores(jugador);

        // Enviar el estado del juego a cada observador
        for (Cliente espectador : observadores) {
            socketServer.enviarMensaje(espectador, gameStateJson);
        }
    }

    /* Function: getType
        Devuelve el tipo de comando.

        Returns:
            - String: El tipo de comando, en este caso, "sendGameState".
    */
    @Override
    public String getType() {
        return "sendGameState";
    }

    /* Function: toMap
        Convierte el comando en un mapa para su serialización o almacenamiento.

        Returns:
            - Map<String, Object>: Mapa con los detalles del comando, como el ID del jugador y el estado del juego en JSON.
    */
    @Override
    public Map<String, Object> toMap() {
        return Map.of(
                "type", getType(),
                "jugadorId", jugador != null ? jugador.getId() : null,
                "gameStateJson", gameStateJson
        );
    }

    /* Function: configure
        Configura el comando con los parámetros proporcionados.

        Params:
            - params: Map<String, Object> - Mapa con los parámetros necesarios para configurar el comando.
              Debe contener "gameStateJson", "emisor", "comServer" y "socketServer".
    */
    @Override
    public void configure(Map<String, Object> params) {
        this.gameStateJson = (String) params.get("gameStateJson"); // JSON completo del estado del juego
        this.jugador = (Cliente) params.get("emisor"); // El jugador que emite el estado
        this.comServer = (ComServer) params.get("comServer");
        this.socketServer = (SocketServer) params.get("socketServer");
    }
}
