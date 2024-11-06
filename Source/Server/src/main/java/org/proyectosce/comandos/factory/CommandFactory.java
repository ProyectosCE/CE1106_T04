package org.proyectosce.comandos.factory;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.ObjectMapper;
import org.proyectosce.comandos.factory.products.*;
import org.proyectosce.comunicaciones.Cliente;
import org.proyectosce.comunicaciones.ComServer;
import org.proyectosce.comunicaciones.SocketServer;

import java.util.Map;

public class CommandFactory {
    private static CommandFactory instance; // Singleton instance
    private final ComServer comServer;
    private final SocketServer socketServer;
    private final ObjectMapper objectMapper;

    // Constructor privado para Singleton
    private CommandFactory(ComServer comServer, SocketServer socketServer, ObjectMapper objectMapper) {
        this.comServer = comServer;
        this.socketServer = socketServer;
        this.objectMapper = objectMapper;
    }

    // Singleton: Obtener la instancia única de CommandFactory
    public static synchronized CommandFactory getInstance() {
        if (instance == null) {
            ComServer comServer = ComServer.getInstance(); // Singleton
            SocketServer socketServer = SocketServer.getInstance(); // Singleton
            ObjectMapper objectMapper = new ObjectMapper();
            instance = new CommandFactory(comServer, socketServer, objectMapper);
        }
        return instance;
    }

    // Método principal para crear comandos
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


    // Método genérico para validar parámetros requeridos
    private void validarParametros(Map<String, Object> params, String... requiredKeys) {
        for (String key : requiredKeys) {
            if (!params.containsKey(key) || params.get(key) == null) {
                throw new IllegalArgumentException("Falta el parámetro requerido: " + key);
            }
        }
    }

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

    private Command crearDisconnectCommand(Map<String, Object> params) {
        DisconnectCommand command = new DisconnectCommand(); // Usar constructor vacío
        command.configure(params); // Configuración dinámica
        return command;
    }
}
