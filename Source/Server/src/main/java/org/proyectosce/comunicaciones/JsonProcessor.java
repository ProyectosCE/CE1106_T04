package org.proyectosce.comunicaciones;

import com.fasterxml.jackson.databind.ObjectMapper;
import org.proyectosce.comandos.factory.CommandFactory;
import org.proyectosce.comandos.factory.products.*;
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
    private static final JsonProcessor jsonInstance = new JsonProcessor();

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
            return objectMapper.writeValueAsString(Map.of("comando", comando, "objeto", objeto));
        } catch (IOException e) {
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
        } catch (IOException e) {
            e.printStackTrace();
            return null;
        }
    }

    public void enviarMensaje(Cliente cliente, String mensaje) {
        cliente.enviarMensaje(mensaje);
    }

    public void cerrarConexion(Cliente cliente) {
        cliente.cerrarConexion();
    }

    //IMPLEMENTAR FACTORY A PARTIR DE AQUÍ
    public Command procesarComando(String mensajeJson, Cliente emisor) {
        try {
            Map<String, Object> jsonData = objectMapper.readValue(mensajeJson, Map.class);
            String commandType = (String) jsonData.get("command");
            System.out.println(commandType);

            // Delegamos la creación del comando a la fábrica
            CommandFactory commandFactory = new CommandFactory();
            return commandFactory.crearComando(commandType, mensajeJson, emisor, jsonData);

        } catch (IOException e) {
            e.printStackTrace();
            System.out.println("Error al procesar el comando JSON.");
        } catch (IllegalArgumentException e) {
            System.err.println(e.getMessage());
        }
        return null;
    }

}
