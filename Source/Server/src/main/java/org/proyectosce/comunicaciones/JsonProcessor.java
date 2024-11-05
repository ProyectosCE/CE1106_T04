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

    // Método para obtener el ObjectMapper
    public ObjectMapper getObjectMapper() {
        return objectMapper;
    }

    // Método original para crear una respuesta JSON simple
    public String crearMensajeSalida(String mensaje) {
        try {
            return objectMapper.writeValueAsString(Map.of("respuesta", mensaje));
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }
    }

    // Método para crear un mensaje JSON complejo con state, comando y objeto
    public String crearMensajeSalida(String comando, Map<String, Object> objeto) {
        try {
            Map<String, Object> message = Map.of(
                    "comando", comando,
                    "objeto", objeto
            );
            return objectMapper.writeValueAsString(message);
        } catch (IOException e) {
            e.printStackTrace();
            return "{}";
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
            // Aquí puedes añadir más comandos si es necesario
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }
}
