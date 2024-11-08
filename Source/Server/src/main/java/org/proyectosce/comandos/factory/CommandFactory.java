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
package org.proyectosce.comandos.factory;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.proyectosce.comandos.factory.products.*;
import org.proyectosce.comunicaciones.Cliente;
import org.proyectosce.comunicaciones.ComServer;
import org.proyectosce.comunicaciones.SocketServer;

import java.util.Map;

/*
 * Class: CommandFactory
 * Esta clase sigue el patrón Singleton y se encarga de la creación de comandos en función de los datos proporcionados.
 * Según el tipo de comando solicitado, crea y configura el comando adecuado.
 *
 * Attributes:
 *     - instance: CommandFactory - Instancia única de CommandFactory (Singleton).
 *     - comServer: ComServer - Instancia de la clase ComServer para interactuar con la lógica del servidor.
 *     - socketServer: SocketServer - Instancia de la clase SocketServer para gestionar las conexiones y mensajes.
 *     - objectMapper: ObjectMapper - Instancia para convertir JSON en objetos Java.
 *
 * Constructor:
 *     - CommandFactory(ComServer, SocketServer, ObjectMapper): Constructor privado para inicializar la instancia única del CommandFactory.
 *
 * Methods:
 *     - getInstance(): Devuelve la instancia única del CommandFactory (Singleton).
 *     - crearComando(String, Map<String, Object>): Crea el comando adecuado según el tipo proporcionado.
 *     - crearSendGameStateCommand(Map<String, Object>): Crea un comando SendGameStateCommand con los parámetros proporcionados.
 *     - crearTipoClienteCommand(Map<String, Object>): Crea un comando TipoClienteCommand con los parámetros proporcionados.
 *     - crearGameSpectatorCommand(Map<String, Object>): Crea un comando GameSpectatorCommand con los parámetros proporcionados.
 *     - crearPowerCommand(Map<String, Object>): Crea un comando PowerCommand con los parámetros proporcionados.
 *     - crearDisconnectCommand(Map<String, Object>): Crea un comando DisconnectCommand con los parámetros proporcionados.
 *     - validarParametros(Map<String, Object>, String...): Valida que los parámetros requeridos estén presentes y no sean nulos.
 *
 * Example:
 *     CommandFactory factory = CommandFactory.getInstance();
 *     Command command = factory.crearComando("hola", params);
 *
 * Problems:
 *
 * References:
 */
public class CommandFactory {
    private static CommandFactory instance; // Singleton instance
    private final ComServer comServer;
    private final SocketServer socketServer;
    private final ObjectMapper objectMapper;

    /* Constructor: CommandFactory
        Constructor privado para Singleton. Inicializa las instancias de ComServer, SocketServer y ObjectMapper.

        Params:
            - comServer: ComServer - Instancia del servidor de comunicación.
            - socketServer: SocketServer - Instancia del servidor de socket.
            - objectMapper: ObjectMapper - Instancia para procesar JSON.
    */
    private CommandFactory(ComServer comServer, SocketServer socketServer, ObjectMapper objectMapper) {
        this.comServer = comServer;
        this.socketServer = socketServer;
        this.objectMapper = objectMapper;
    }

    /* Function: getInstance
        Devuelve la instancia única de CommandFactory (Singleton).
    */
    public static synchronized CommandFactory getInstance() {
        if (instance == null) {
            ComServer comServer = ComServer.getInstance(); // Singleton
            SocketServer socketServer = SocketServer.getInstance(); // Singleton
            ObjectMapper objectMapper = new ObjectMapper();
            instance = new CommandFactory(comServer, socketServer, objectMapper);
        }
        return instance;
    }

    /* Function: crearComando
        Crea el comando adecuado basado en el tipo proporcionado.

        Params:
            - tipoComando: String - El tipo de comando a crear.
            - params: Map<String, Object> - Los parámetros para configurar el comando.

        Returns:
            - Command - El comando creado y configurado.

        Example:
            Command comando = commandFactory.crearComando("hola", params);
    */
    public Command crearComando(String tipoComando, Map<String, Object> params) {
        try {
            switch (tipoComando) {
                case "hola":
                    return new HolaCommand();

                case "sendGameState":
                    return crearSendGameStateCommand(params);

                case "tipoCliente":
                    return crearTipoClienteCommand(params);

                case "GameSpectator":
                    return crearGameSpectatorCommand(params);

                case "brick_pwr":
                    return crearPowerCommand(params);

                case "disconnect":
                    return crearDisconnectCommand(params);

                default:
                    throw new IllegalArgumentException("Comando desconocido: " + tipoComando);
            }
        } catch (Exception e) {
            System.err.println("Error al crear comando de tipo '" + tipoComando + "': " + e.getMessage());
            e.printStackTrace();
            return null; // Retorna nulo si ocurre algún error
        }
    }

    /* Function: crearSendGameStateCommand
        Crea un comando SendGameStateCommand y lo configura con los parámetros.

        Params:
            - params: Map<String, Object> - Parámetros necesarios para configurar el comando.

        Returns:
            - Command - El comando SendGameStateCommand creado y configurado.
    */
    private Command crearSendGameStateCommand(Map<String, Object> params) {
        validarParametros(params, "jsonCompleto", "emisor");
        Command command = new SendGameStateCommand();
        command.configure(Map.of(
                "gameStateJson", params.get("jsonCompleto"), // Usar "jsonCompleto"
                "emisor", params.get("emisor"), // Cliente que envió el estado
                "comServer", comServer, // Instancia de ComServer
                "socketServer", socketServer // Instancia de SocketServer
        ));
        return command;
    }

    /* Function: crearTipoClienteCommand
        Crea un comando TipoClienteCommand y lo configura con los parámetros proporcionados.

        Params:
            - params: Map<String, Object> - Parámetros necesarios para configurar el comando.

        Returns:
            - Command - El comando TipoClienteCommand creado y configurado.
    */
    private Command crearTipoClienteCommand(Map<String, Object> params) {
        validarParametros(params, "jsonCompleto", "emisor");

        // Convertir el JSON completo en un mapa
        String jsonCompleto = (String) params.get("jsonCompleto");
        Map<String, Object> jsonData;
        try {
            jsonData = objectMapper.readValue(jsonCompleto, Map.class);
        } catch (JsonProcessingException e) {
            throw new IllegalArgumentException("El JSON no es válido para TipoClienteCommand: " + e.getMessage());
        }

        // Crear y configurar el comando
        Command command = new TipoClienteCommand();
        command.configure(Map.of(
                "tipoCliente", jsonData.get("tipoCliente"),
                "playerName", jsonData.get("playerName"),
                "emisor", params.get("emisor"),
                "comServer", comServer
        ));
        return command;
    }

    /* Function: crearGameSpectatorCommand
        Crea un comando GameSpectatorCommand y lo configura con los parámetros.

        Params:
            - params: Map<String, Object> - Parámetros necesarios para configurar el comando.

        Returns:
            - Command - El comando GameSpectatorCommand creado y configurado.
    */
    private Command crearGameSpectatorCommand(Map<String, Object> params) {
        // Obtener el JSON completo y el cliente emisor
        String jsonCompleto = (String) params.get("jsonCompleto");
        Cliente espectador = (Cliente) params.get("emisor");

        if (jsonCompleto == null || espectador == null) {
            throw new IllegalArgumentException("El JSON completo o el espectador no pueden ser nulos.");
        }

        // Deserializar el JSON para extraer jugadorId
        Map<String, Object> jsonData;
        try {
            jsonData = objectMapper.readValue(jsonCompleto, Map.class);
        } catch (JsonProcessingException e) {
            throw new IllegalArgumentException("El JSON no es válido: " + e.getMessage());
        }

        String jugadorId = (String) jsonData.get("jugadorId");
        if (jugadorId == null || jugadorId.isEmpty()) {
            throw new IllegalArgumentException("El JSON no contiene un jugadorId válido.");
        }

        // Obtener el jugador a observar
        Cliente jugadorAObservar = comServer.obtenerClientePorId(jugadorId);
        if (jugadorAObservar == null) {
            throw new IllegalArgumentException("Jugador no encontrado para ID: " + jugadorId);
        }

        // Crear y configurar el comando
        Command command = new GameSpectatorCommand();
        command.configure(Map.of(
                "jugadorAObservar", jugadorAObservar,
                "espectador", espectador,
                "comServer", comServer
        ));
        return command;
    }

    /* Function: crearPowerCommand
        Crea un comando PowerCommand y lo configura con los parámetros.

        Params:
            - params: Map<String, Object> - Parámetros necesarios para configurar el comando.

        Returns:
            - Command - El comando PowerCommand creado y configurado.
    */
    private Command crearPowerCommand(Map<String, Object> params) {
        Command command = new PowerCommand();
        command.configure(Map.of(
                "command", params.get("command"),
                "f", params.get("f"),
                "c", params.get("c"),
                "jugadorId", params.get("jugadorId"),
                "comServer", comServer,
                "socketServer", comServer.getSocketServer()
        ));
        return command;
    }

    /* Function: crearDisconnectCommand
        Crea un comando DisconnectCommand y lo configura con los parámetros.

        Params:
            - params: Map<String, Object> - Parámetros necesarios para configurar el comando.

        Returns:
            - Command - El comando DisconnectCommand creado y configurado.
    */
    private Command crearDisconnectCommand(Map<String, Object> params) {
        DisconnectCommand command = new DisconnectCommand(); // Usar constructor vacío
        command.configure(params); // Configuración dinámica
        return command;
    }

    /* Function: validarParametros
        Valida que los parámetros requeridos estén presentes y no sean nulos.

        Params:
            - params: Map<String, Object> - El mapa de parámetros a validar.
            - requiredKeys: String... - Claves que deben estar presentes en los parámetros.

        Throws:
            - IllegalArgumentException: Si falta algún parámetro requerido.
    */
    private void validarParametros(Map<String, Object> params, String... requiredKeys) {
        for (String key : requiredKeys) {
            if (!params.containsKey(key) || params.get(key) == null) {
                throw new IllegalArgumentException("Falta el parámetro requerido: " + key);
            }
        }
    }
}
