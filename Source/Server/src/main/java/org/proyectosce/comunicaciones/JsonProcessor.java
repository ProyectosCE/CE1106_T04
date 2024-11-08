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

package org.proyectosce.comunicaciones;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.core.JsonProcessingException;
import org.proyectosce.comandos.factory.CommandFactory;
import org.proyectosce.comandos.factory.products.Command;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/* Class: JsonProcessor
    Clase Singleton para procesar mensajes en formato JSON. Convierte objetos Java a JSON y viceversa,
    además de gestionar comandos basados en mensajes JSON.

    Attributes:
        - instance: JsonProcessor - Instancia única del procesador JSON (Singleton).
        - objectMapper: ObjectMapper - Objeto para la serialización y deserialización JSON.

    Constructor:
        - JsonProcessor: Constructor privado para implementar el patrón Singleton.

    Methods:
        - getInstance: Retorna la instancia única de JsonProcessor.
        - getObjectMapper: Retorna el objeto ObjectMapper para procesamiento JSON.
        - crearMensajeSalida: Crea un mensaje JSON a partir de un comando y un objeto.
        - crearMensajeClientesLista: Genera un JSON con una lista de clientes a partir de IDs y nombres.
        - procesarComando: Procesa un mensaje JSON para generar y ejecutar un comando.

    Example:
        JsonProcessor procesador = JsonProcessor.getInstance();
        Map<String, Object> data = Map.of("key", "value");
        String mensaje = procesador.crearMensajeSalida("comandoEjemplo", data);
*/
public class JsonProcessor {
    private static JsonProcessor instance;
    private final ObjectMapper objectMapper = new ObjectMapper();

    /* Function: JsonProcessor
        Constructor privado para la implementación del patrón Singleton.
    */
    private JsonProcessor() {}

    /* Function: getInstance
        Retorna la instancia única de JsonProcessor.

        Returns:
            - JsonProcessor - Instancia única del procesador JSON.
    */
    public static synchronized JsonProcessor getInstance() {
        if (instance == null) {
            instance = new JsonProcessor();
        }
        return instance;
    }

    /* Function: getObjectMapper
        Retorna el objeto ObjectMapper utilizado para la manipulación JSON.

        Returns:
            - ObjectMapper - Objeto para la serialización y deserialización JSON.
    */
    public ObjectMapper getObjectMapper() {
        return objectMapper;
    }

    /* Function: crearMensajeSalida
        Crea un mensaje JSON que contiene un comando y datos asociados.

        Params:
            - comando: String - Nombre del comando.
            - objeto: Map<String, Object> - Datos asociados al comando.

        Returns:
            - String - Mensaje JSON generado o "{}" en caso de error.
    */
    public String crearMensajeSalida(String comando, Map<String, Object> objeto) {
        try {
            return objectMapper.writeValueAsString(Map.of("command", comando, "data", objeto));
        } catch (JsonProcessingException e) {
            System.err.println("Error al crear mensaje JSON: " + e.getMessage());
            return "{}";
        }
    }

    /* Function: crearMensajeClientesLista
        Crea un mensaje JSON con una lista de clientes a partir de sus IDs y nombres.

        Params:
            - ids: List<String> - Lista de IDs de los clientes.
            - nombres: List<String> - Lista de nombres de los clientes.

        Returns:
            - String - JSON con la lista de clientes o null en caso de error.
    */
    public String crearMensajeClientesLista(List<String> ids, List<String> nombres) {
        try {
            // Validar que las listas tengan el mismo tamaño
            if (ids.size() != nombres.size()) {
                throw new IllegalArgumentException("Las listas de IDs y nombres deben tener el mismo tamaño.");
            }

            // Construir la lista de jugadores
            List<Map<String, Object>> jugadores = new ArrayList<>();
            for (int i = 0; i < ids.size(); i++) {
                Map<String, Object> jugador = new HashMap<>();
                jugador.put("id", ids.get(i));
                jugador.put("nombre", nombres.get(i));
                jugadores.add(jugador);
            }

            // Crear el mensaje final
            Map<String, Object> mensaje = Map.of(
                    "command", "ClientesLista",
                    "data", jugadores
            );

            return objectMapper.writeValueAsString(mensaje);

        } catch (JsonProcessingException e) {
            System.err.println("Error al crear JSON para lista de clientes: " + e.getMessage());
            return null;
        } catch (IllegalArgumentException e) {
            System.err.println("Error: " + e.getMessage());
            return null;
        }
    }

    /* Function: procesarComando
        Procesa un mensaje JSON, extrayendo el comando y delegando su ejecución
        a CommandFactory.

        Params:
            - mensajeJson: String - Mensaje JSON que contiene el comando.
            - emisor: Cliente - Cliente que envió el mensaje.

        Returns:
            - Command - Comando generado a partir del mensaje JSON, o null en caso de error.
    */
    public Command procesarComando(String mensajeJson, Cliente emisor) {
        try {
            // Deserializar el JSON a un mapa
            Map<String, Object> jsonData = objectMapper.readValue(mensajeJson, Map.class);

            // Obtener el comando del JSON
            String commandType = (String) jsonData.get("command");

            // Construir parámetros para la creación del comando
            Map<String, Object> params = Map.of(
                    "emisor", emisor,
                    "jsonCompleto", mensajeJson
            );

            // Delegar la creación del comando a CommandFactory
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

        return null;
    }
}
