package org.proyectosce.comunicaciones;

import com.fasterxml.jackson.databind.ObjectMapper;
import org.proyectosce.comandos.Command;
import org.proyectosce.comandos.PowerCommand;

import java.io.IOException;
import java.util.Map;

public class JsonProcessor {
    private static final JsonProcessor instance = new JsonProcessor();
    private final ObjectMapper objectMapper = new ObjectMapper();

    private JsonProcessor() {}

    public static JsonProcessor getInstance() {
        return instance;
    }

    public ObjectMapper getObjectMapper() {
        return objectMapper;
    }

    public String crearMensajeSalida(String comando, Map<String, Object> objeto) {
        try {
            return objectMapper.writeValueAsString(Map.of("comando", comando, "objeto", objeto));
        } catch (IOException e) {
            System.err.println("Error al crear mensaje JSON: " + e.getMessage());
            return "{}";
        }
    }

    public Command procesarComando(String mensaje) {
        try {
            Map<String, Object> map = objectMapper.readValue(mensaje, Map.class);
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

    public void enviarMensaje(Cliente cliente, String mensaje) {
        cliente.enviarMensaje(mensaje);
    }

    public void cerrarConexion(Cliente cliente) {
        cliente.cerrarConexion();
    }
}
