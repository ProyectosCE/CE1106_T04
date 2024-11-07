package org.proyectosce.comandos.factory;

import com.fasterxml.jackson.databind.ObjectMapper;  // Importa el ObjectMapper de Jackson
import org.proyectosce.comandos.factory.products.*;
import org.proyectosce.comunicaciones.Cliente;
import org.proyectosce.comunicaciones.ComServer;
import java.io.IOException;
import java.util.Map;

public class CommandFactory {

    private static final ObjectMapper objectMapper = new ObjectMapper();  // Instancia de ObjectMapper

    public Command crearComando(String tipoComando, String mensajeJson, Cliente emisor, Map<String, Object> jsonData) {
        switch (tipoComando) {
            case "hola":
                return new HolaCommand();

            case "sendGameState":
                return new SendGameStateCommand(mensajeJson, emisor);

            case "tipoCliente":
                String tipoCliente = (String) jsonData.get("tipoCliente");
                return new TipoClienteCommand(tipoCliente, emisor);

            case "GameSpectator":
                String jugadorId = (String) jsonData.get("jugadorId");
                Cliente jugadorAObservar = ComServer.getInstance().obtenerClientePorId(jugadorId);
                if (jugadorAObservar != null) {
                    return new GameSpectatorCommand(jugadorAObservar, emisor);
                } else {
                    System.out.println("NO HAY JUGADOR PARA OBSERVAR :(");
                }
                break;

            case "brick_pwr":
                return crearPowerCommand(mensajeJson);

            default:
                throw new IllegalArgumentException("Comando desconocido: " + tipoComando);
        }
        return null;
    }

    private PowerCommand crearPowerCommand(String mensajeJson) {
        try {
            Map<String, Object> map = objectMapper.readValue(mensajeJson, Map.class);  // Usa objectMapper para convertir JSON
            String type = (String) map.get("command");
            Map<String, Integer> position = (Map<String, Integer>) map.get("position");
            int f = position.get("y");
            int c = position.get("x");
            return new PowerCommand(f, c, type);
        } catch (IOException | ClassCastException e) {
            System.err.println("Error al procesar comando JSON: " + e.getMessage());
            return null;
        }
    }
}
