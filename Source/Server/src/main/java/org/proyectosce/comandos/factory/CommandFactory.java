package org.proyectosce.comandos.factory;

import com.fasterxml.jackson.databind.ObjectMapper;
import org.proyectosce.comandos.factory.products.*;
import org.proyectosce.comunicaciones.Cliente;
import org.proyectosce.comunicaciones.ComServer;
import java.io.IOException;
import java.util.Map;

public class CommandFactory {
    private static CommandFactory instance;
    private static final ObjectMapper objectMapper = new ObjectMapper();

    // Singleton
    private CommandFactory() {}

    public static CommandFactory getInstance() {
        if (instance == null) {
            instance = new CommandFactory();
        }
        return instance;
    }

    public Command crearComando(String tipoComando, String mensajeJson, Cliente emisor, Map<String, Object> jsonData) {
        switch (tipoComando) {
            case "hola":
                return new HolaCommand();

            case "sendGameState":
                return new SendGameStateCommand(mensajeJson, emisor);

            case "tipoCliente":
                return crearTipoClienteCommand(jsonData, emisor);

            case "GameSpectator":
                return crearGameSpectatorCommand(jsonData, emisor);

            case "brick_pwr":
                return crearPowerCommand(mensajeJson);

            default:
                throw new IllegalArgumentException("Comando desconocido: " + tipoComando);
        }
    }

    private TipoClienteCommand crearTipoClienteCommand(Map<String, Object> jsonData, Cliente emisor) {
        String tipoCliente = (String) jsonData.get("tipoCliente");
        return new TipoClienteCommand(tipoCliente, emisor, (String) jsonData.get("playerName"));
    }

    private Command crearGameSpectatorCommand(Map<String, Object> jsonData, Cliente emisor) {
        String jugadorId = (String) jsonData.get("jugadorId");
        Cliente jugadorAObservar = ComServer.getInstance().obtenerClientePorId(jugadorId);
        if (jugadorAObservar != null) {
            return new GameSpectatorCommand(jugadorAObservar, emisor);
        } else {
            System.out.println("No hay jugador para observar.");
            return null;
        }
    }

    private PowerCommand crearPowerCommand(String mensajeJson) {
        try {
            Map<String, Object> map = objectMapper.readValue(mensajeJson, Map.class);
            String type = (String) map.get("command");
            Map<String, Integer> position = (Map<String, Integer>) map.get("position");

            if (position == null || !position.containsKey("y") || !position.containsKey("x")) {
                System.err.println("La posición es inválida o incompleta en el JSON.");
                return null;
            }

            int f = position.get("y");
            int c = position.get("x");
            return new PowerCommand(f, c, type);

        } catch (IOException | ClassCastException e) {
            System.err.println("Error al procesar comando JSON en PowerCommand: " + e.getMessage());
            return null;
        }
    }
}
