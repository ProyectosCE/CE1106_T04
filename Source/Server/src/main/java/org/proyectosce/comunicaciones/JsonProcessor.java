package org.proyectosce.comunicaciones;

import com.fasterxml.jackson.databind.ObjectMapper;
import org.proyectosce.comandos.*;

import java.io.IOException;
import java.util.List;
import java.util.Map;

/*
 * Class: JsonProcessor
 *
 * Procesa los mensajes en formato JSON, convirtiendo objetos Java a JSON y viceversa.
 */
public class JsonProcessor {
    private static JsonProcessor instance;
    private final ObjectMapper objectMapper = new ObjectMapper();

    // Singleton
    private JsonProcessor() {}

    public static synchronized JsonProcessor getInstance() {
        if (instance == null) {
            instance = new JsonProcessor();
        }
        return instance;
    }

    // Método para convertir un objeto Java a JSON
    public String crearMensajeSalida(String mensaje) {
        try {
            return objectMapper.writeValueAsString(Map.of("respuesta", mensaje));
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }
    }

    // Método para crear JSON con lista de clientes
    public String crearMensajeClientesLista(List<String> listaDeClientes) {
        try {
            Map<String, Object> mensaje = Map.of(
                    "command", "ClientesLista",
                    "data", listaDeClientes
            );
            return objectMapper.writeValueAsString(mensaje);
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }
    }

    public Command procesarComando(String mensajeJson, Cliente emisor) {
        try {
            Map<String, Object> jsonData = objectMapper.readValue(mensajeJson, Map.class);
            String commandType = (String) jsonData.get("command");


            if ("hola".equals(commandType)) {
                return new HolaCommand();

            } else if ("sendGameState".equals(commandType)) {
                return new SendGameStateCommand(mensajeJson, emisor);

            } else if ("tipoCliente".equals(commandType)) {
                String tipoCliente = (String) jsonData.get("tipoCliente");
                return new TipoClienteCommand(tipoCliente);

            } else if ("GameSpectator".equals(commandType)) {
                String jugadorId = (String) jsonData.get("jugadorId");
                Cliente jugadorAObservar = ComServer.getInstance().obtenerClientePorId(jugadorId);
                if (jugadorAObservar != null) {
                    return new GameSpectatorCommand(jugadorAObservar);
                }
            }

        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }
}
