package org.proyectosce;

import com.fasterxml.jackson.databind.ObjectMapper;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;
import java.nio.channels.ServerSocketChannel;
import java.nio.channels.SocketChannel;
import java.util.Map;
import java.util.*;
import java.util.LinkedList;
import java.util.List;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/* Class: Server
    Esta clase representa el servidor de la aplicación. El servidor acepta conexiones de clientes y maneja la comunicación con ellos.
    El servidor utiliza un pool de hilos para manejar múltiples conexiones de clientes simultáneamente.

    Fields:
      - instance: La instancia única de la clase Server.
      - PORT: El puerto en el que el servidor escucha las conexiones de los clientes.
      - objectMapper: Un objeto ObjectMapper para serializar y deserializar JSON.
      - executorService: Un pool de hilos para manejar las conexiones de los clientes.
      - clients: Un conjunto de SocketChannels que representan a los clientes conectados.

    Methods:
      - getInstance: Este metodo estático se utiliza para obtener la instancia única de la clase Server.
      - startServer: Este metodo inicia el servidor y acepta conexiones de clientes.
      - handleClient: Este metodo maneja la comunicación con un cliente específico.
      - getCommandFromJson: Este metodo convierte un JSON en un comando concreto.
      - sendMessageToClient: Este metodo envía un mensaje JSON a un cliente específico.
      - sendMessageToAllClients: Este metodo envía un mensaje JSON a todos los clientes conectados.
      - sendSetNumberMessageToClient: Este metodo envía un mensaje "setNumber" a un cliente recién conectado.
*/
public class Server {

    private static Server instance;
    private static final int PORT = 12345;
    static final ObjectMapper objectMapper = new ObjectMapper(); // Manejo de JSON
    private static final ExecutorService executorService = Executors.newCachedThreadPool(); // Pool de hilos
    private static final Set<SocketChannel> clients = ConcurrentHashMap.newKeySet(); // Almacenar clientes

    private static final Set<SocketChannel> players = ConcurrentHashMap.newKeySet(); // Lista para jugadores
    private static final Set<SocketChannel> spectators = ConcurrentHashMap.newKeySet(); // Lista para espectadores


    // Constructor privado
    /* Constructor: Server
        El constructor de la clase Server es privado para evitar que se creen instancias de la clase directamente.
    */
    private Server() {
    }

    // Metodo para obtener la instancia única (Patrón Singleton)
    /* Method: getInstance
        Este metodo estático se utiliza para obtener la instancia única de la clase Server. Si la instancia aún no se ha creado, se crea una nueva instancia y se devuelve.
    */
    public static synchronized Server getInstance() {
        if (instance == null) {
            instance = new Server();
        }
        return instance;
    }


    /* Method: startServer
        Este metodo inicia el servidor y acepta conexiones de clientes. Utiliza un ServerSocketChannel para aceptar conexiones de clientes y un pool de hilos para manejar las conexiones de los clientes.
    */
    void startServer() {
        try (ServerSocketChannel serverSocketChannel = ServerSocketChannel.open()) {
            serverSocketChannel.bind(new InetSocketAddress(PORT));
            System.out.println("Servidor escuchando en el puerto " + PORT);

            while (true) {
                // Aceptar conexiones de clientes
                SocketChannel clientChannel = serverSocketChannel.accept();
                System.out.println("Cliente conectado desde " + clientChannel.getRemoteAddress());

                // Agregar el cliente a la lista
                clients.add(clientChannel);

                // Manejar la conexión del cliente en un nuevo hilo
                executorService.submit(() -> handleClient(clientChannel));
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /* Method: handleClient
        Este metodo maneja la comunicación con un cliente específico. Lee los mensajes del cliente, los procesa y envía respuestas al cliente.
    */
    private static void handleClient(SocketChannel clientChannel) {
        try {
            ByteBuffer buffer = ByteBuffer.allocate(1024);

            while (true) {
                int bytesRead = clientChannel.read(buffer);

                if (bytesRead == -1) {
                    // Remover el cliente de la lista de jugadores o espectadores
                    if (players.contains(clientChannel)) {
                        System.out.println("Jugador desconectado: " + clientChannel.getRemoteAddress());
                        players.remove(clientChannel);
                    } else if (spectators.contains(clientChannel)) {
                        System.out.println("Espectador desconectado: " + clientChannel.getRemoteAddress());
                        spectators.remove(clientChannel);
                    }

                    // Cerrar el canal del cliente
                    clientChannel.close();
                    clients.remove(clientChannel); // Remover el cliente de la lista en caso de error


                    return;
                }

                String message = new String(buffer.array()).trim();
                System.out.println("Mensaje recibido (JSON): " + message);

                // Deserializar JSON recibido
                Map<String, Object> jsonData = objectMapper.readValue(message, Map.class);
                System.out.println("Datos recibidos: " + jsonData);

                // Procesar el comando con el patrón Command
                Command command = getCommandFromJson(jsonData, clientChannel);
                if (command != null) {
                    command.execute();
                } else {
                    System.out.println("Comando no reconocido.");
                }

            }
        } catch (IOException e) {
            System.out.println("Error manejando el cliente: " + e.getMessage());
            clients.remove(clientChannel); // Remover el cliente de la lista en caso de error
            try {
                clientChannel.close();
            } catch (IOException closeException) {
                closeException.printStackTrace();
            }
        }
    }


    /* Method: getCommandFromJson
        Este metodo convierte un JSON en un comando concreto. El metodo analiza el JSON para determinar el tipo de comando y crea una instancia del comando correspondiente.
    */
    private static Command getCommandFromJson(Map<String, Object> jsonData, SocketChannel clientChannel) throws IOException {
        String commandType = (String) jsonData.get("command");

        if ("print".equals(commandType)) {
            String message = (String) jsonData.get("message");
            return new PrintCommand(message);
        } else if ("connect".equals(commandType)){
            String role = (String) jsonData.get("role");
            if ("player".equals(role)) {
                players.add(clientChannel);
                return new PrintCommand("Jugador conectado en:" + clientChannel.getRemoteAddress());
            } else if ("spectator".equals(role)) {
                spectators.add(clientChannel);
                return new PrintCommand("Espectador conectado en:" + clientChannel.getRemoteAddress());
            }
        }
        return null;
    }

    /* Method: sendMessageToClient
        Este metodo envía un mensaje JSON a un cliente específico. El metodo convierte el mensaje JSON en bytes y lo envía al cliente a través del SocketChannel.
    */
    static void sendMessageToClient(SocketChannel clientChannel, String jsonMessage) {
        try {
            ByteBuffer buffer = ByteBuffer.allocate(1024);
            buffer.put(jsonMessage.getBytes());
            buffer.flip();
            clientChannel.write(buffer);
            System.out.println("Mensaje enviado al cliente " + clientChannel.getRemoteAddress() + ": " + jsonMessage);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /* Method: sendMessageToAllClients
        Este metodo envía un mensaje JSON a todos los clientes conectados. El metodo itera sobre la lista de clientes y envía el mensaje JSON a cada cliente.
    */
    private static void sendMessageToAllClients(String jsonMessage) {
        for (SocketChannel client : clients) {
            sendMessageToClient(client, jsonMessage);
        }
    }

    /* Method: sendMessagetoPlayers
        Este metodo envía un mensaje JSON a todos los jugadores conectados. El metodo itera sobre la lista de jugadores y envía el mensaje JSON a cada jugador.
    */
    private static void sendMessagetoPlayers(String jsonMessage) {
        for (SocketChannel player : players) {
            sendMessageToClient(player, jsonMessage);
            // print
            try {
                System.out.println("Mensaje enviado al jugador " + player.getRemoteAddress() + ": " + jsonMessage);
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }
    }

    /* Method: sendMessagetoSpectators
        Este metodo envía un mensaje JSON a todos los espectadores conectados. El metodo itera sobre la lista de espectadores y envía el mensaje JSON a cada espectador.
    */
    private static void sendMessagetoSpectators(String jsonMessage) {
        executorService.submit(() -> {
            for (SocketChannel spectator : spectators) {
                sendMessageToClient(spectator, jsonMessage);
                // print
                try {
                    System.out.println("Mensaje enviado al espectador " + spectator.getRemoteAddress() + ": " + jsonMessage);
                } catch (IOException e) {
                    throw new RuntimeException(e);
                }
            }
        });
    }

    /* Method: sendSetNumberMessage
        Este metodo envía un mensaje "setNumber" a todos los clientes conectados. El metodo crea un mensaje JSON con el comando "setNumber" y el número especificado y lo envía a todos los clientes.
    */
    private static void sendSetNumberMessage(int number) {
        try {
            String jsonMessage = objectMapper.writeValueAsString(Map.of(
                    "command", "setNumber",
                    "number", number
            ));
            sendMessageToAllClients(jsonMessage);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
