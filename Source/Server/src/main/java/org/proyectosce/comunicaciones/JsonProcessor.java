package org.proyectosce.comunicaciones;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.core.JsonProcessingException;
import org.proyectosce.comandos.factory.CommandFactory;
import org.proyectosce.comandos.factory.products.Command;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
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

    // Método para crear JSON con lista de clientes, incluyendo tanto los IDs como los nombres
    public String crearMensajeClientesLista(List<String> ids, List<String> nombres) {
        try {
            // Verificar que ambas listas tengan el mismo tamaño
            if (ids.size() != nombres.size()) {
                throw new IllegalArgumentException("Las listas de IDs y nombres deben tener el mismo tamaño.");
            }

            // Crear una lista de mapas para representar cada jugador con su ID y nombre
            List<Map<String, Object>> jugadores = new ArrayList<>();
            for (int i = 0; i < ids.size(); i++) {
                Map<String, Object> jugador = new HashMap<>();
                jugador.put("id", ids.get(i));
                jugador.put("nombre", nombres.get(i));
                jugadores.add(jugador);
            }

            // Crear el mapa final que contendrá el comando y la lista de jugadores
            Map<String, Object> mensaje = Map.of(
                    "command", "ClientesLista",
                    "data", jugadores
            );

            // Convertir el mapa a JSON
            return objectMapper.writeValueAsString(mensaje);
        } catch (JsonProcessingException e) {
            System.err.println("Error al crear JSON para lista de clientes: " + e.getMessage());
            return null;
        } catch (IllegalArgumentException e) {
            System.err.println("Error: " + e.getMessage());
            return null;
        }
    }

    // Método para procesar comandos con JSON y delegación a CommandFactory
    public Command procesarComando(String mensajeJson, Cliente emisor) {
        try {


            // Convertir el mensaje JSON en un mapa para extraer el comando
            Map<String, Object> jsonData = objectMapper.readValue(mensajeJson, Map.class);

            // Validar que el comando esté presente en el JSON
            String commandType = (String) jsonData.get("command");

            // Crear un nuevo mapa con el emisor y el JSON completo
            Map<String, Object> params = Map.of(
                    "emisor", emisor,
                    "jsonCompleto", mensajeJson // JSON original como una cadena
            );

            // Obtener la instancia de CommandFactory y delegar la creación del comando
            CommandFactory commandFactory = CommandFactory.getInstance();

            return commandFactory.crearComando(commandType, params);

        } catch (JsonProcessingException e) {
            System.err.println("Error al procesar JSON del comando: " + e.getMessage());
        } catch (IllegalArgumentException | IllegalStateException e) {
            System.err.println("Error al procesar el comando: " + e.getMessage());
        } catch (Exception e) {
            System.err.println("Error inesperado al procesar el comando: " + e.getMessage());
            e.printStackTrace();
        }

        // Si ocurre algún error, retornar nulo
        return null;
    }
}
