package org.proyectosce.comunicaciones;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.core.JsonProcessingException;
import org.proyectosce.comandos.factory.CommandFactory;
import org.proyectosce.comandos.factory.products.Command;
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

    public ObjectMapper getObjectMapper() {
        return objectMapper;
    }

    // Método para convertir un objeto Java a JSON
    public String crearMensajeSalida(String comando, Map<String, Object> objeto) {
        try {
            return objectMapper.writeValueAsString(Map.of("command", comando, "data", objeto));
        } catch (JsonProcessingException e) {
            System.err.println("Error al crear mensaje JSON: " + e.getMessage());
            return "{}";
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
        } catch (JsonProcessingException e) {
            System.err.println("Error al crear JSON para lista de clientes: " + e.getMessage());
            return null;
        }
    }

    // Método para procesar comandos con JSON y delegación a CommandFactory
    public Command procesarComando(String mensajeJson, Cliente emisor) {
        try {
            Map<String, Object> jsonData = objectMapper.readValue(mensajeJson, Map.class);
            String commandType = (String) jsonData.get("command");

            if (commandType == null || commandType.isEmpty()) {
                throw new IllegalArgumentException("El tipo de comando es nulo o vacío en el JSON recibido.");
            }

            // Delegamos la creación del comando a la fábrica
            CommandFactory commandFactory = CommandFactory.getInstance();
            return commandFactory.crearComando(commandType, mensajeJson, emisor, jsonData);

        } catch (JsonProcessingException e) {
            System.err.println("Error al procesar JSON del comando: " + e.getMessage());
        } catch (IllegalArgumentException e) {
            System.err.println("Error en el comando JSON recibido: " + e.getMessage());
        }
        return null;
    }
}
