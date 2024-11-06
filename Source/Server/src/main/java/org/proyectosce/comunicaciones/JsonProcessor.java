package org.proyectosce.comunicaciones;

import com.fasterxml.jackson.databind.ObjectMapper;
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
            
            if ("hola".equals(commandType)) {
                return new HolaCommand();

            } else if ("sendGameState".equals(commandType)) {
                return new SendGameStateCommand(mensajeJson, emisor);

            } else if ("tipoCliente".equals(commandType)) {
                String tipoCliente = (String) jsonData.get("tipoCliente");
                return new TipoClienteCommand(tipoCliente, emisor);

            } else if ("GameSpectator".equals(commandType)) {
                System.out.println("entramos a comando GameSpectator");
                String jugadorId = (String) jsonData.get("jugadorId");
                System.out.println("jugador id enviado desde espectador:"+jugadorId);
                Cliente jugadorAObservar = ComServer.getInstance().obtenerClientePorId(jugadorId);
                if (jugadorAObservar != null) {
                    return new GameSpectatorCommand(jugadorAObservar, emisor);
                }
                else{
                    System.out.println("NO HAY JUGADOR PARA OBSERVAR :(");
                }
            } else if ("brick_pwr".equals(commandType)) {
                try {
                    Map<String, Object> map = objectMapper.readValue(mensajeJson, Map.class);
                    String type = (String) map.get("command");
                    Map<String, Integer> position = (Map<String, Integer>) map.get("position");
                    int f = position.get("y");
                    int c = position.get("x");

                    return new PowerCommand(f, c, type); //CUIDADO
                } catch (IOException | ClassCastException e) {
                    System.err.println("Error al procesar comando JSON: " + e.getMessage());
                    return null;
                }
            }

        } catch (IOException e) {
            e.printStackTrace();
            System.out.println("tuvimos un error");
        }
        return null;
    }
}
