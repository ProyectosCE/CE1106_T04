package org.proyectosce.comunicaciones;

import com.fasterxml.jackson.databind.ObjectMapper;
import org.proyectosce.comandos.Command;
import org.proyectosce.comandos.HolaCommand;

import java.io.IOException;
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

    // Método para procesar el JSON recibido y devolver un comando
    public Command procesarComando(String mensajeJson) {
        try {
            Map<String, Object> jsonData = objectMapper.readValue(mensajeJson, Map.class);
            String commandType = (String) jsonData.get("comando");

            if ("hola".equals(commandType)) {
                return new HolaCommand();
            }

        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }
}
