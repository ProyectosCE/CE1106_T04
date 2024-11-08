/*
================================== LICENCIA =================
=================================
MIT License
								Copyright (c) 2024 José Bernardo Barquero Bonilla,
								Jose Eduardo Campos Salazar,
								Jimmy Feng Feng,
								Alexander Montero Vargas
Consulta el archivo LICENSE para más detalles.
=============================================================
Cambios y Configuraciones del Proyecto3=================================
*/
package org.proyectosce.comunicaciones;

import org.proyectosce.comandos.factory.products.Command;
import org.proyectosce.comandos.factory.products.TipoClienteCommand;

/*
 * Class: ClientHandler
 * Esta clase maneja la lógica de comunicación de un cliente en el servidor. Procesa los mensajes
 * entrantes, identifica el tipo de cliente y ejecuta comandos.
 * La clase implementa la interfaz Runnable para ser ejecutada en un hilo independiente.
 *
 * Attributes:
 *     - cliente: Cliente - Cliente al que se le gestionan los mensajes.
 *     - jsonProcessor: JsonProcessor - Procesa los mensajes JSON recibidos.
 *     - socketServer: SocketServer - Servidor de sockets para enviar y recibir mensajes.
 *     - clienteIdentificado: boolean - Flag que indica si el cliente ha sido identificado.
 *
 * Constructor:
 *     - ClientHandler(Cliente cliente): Inicializa el manejador con el cliente y el servidor de sockets.
 *
 * Methods:
 *     - run: Procesa los mensajes del cliente en un bucle, identificando y ejecutando los comandos.
 *
 * Example:
 *     ClientHandler handler = new ClientHandler(cliente);
 *     new Thread(handler).start(); // Ejecuta el handler en un hilo independiente.
 *
 * Problems:
 *
 * References:
 *
 */
public class ClientHandler implements Runnable {
    private final Cliente cliente;
    private final JsonProcessor jsonProcessor;
    private final SocketServer socketServer;
    private boolean clienteIdentificado = false;  // Flag para verificar si el cliente ya fue identificado

    /* Constructor: ClientHandler
        Inicializa un nuevo manejador para el cliente.

        Params:
            - cliente: Cliente - Cliente que será manejado.
    */
    public ClientHandler(Cliente cliente) {
        this.cliente = cliente;
        this.jsonProcessor = JsonProcessor.getInstance();
        this.socketServer = SocketServer.getInstance();
    }

    /* Function: run
        Ejecuta la lógica de manejo del cliente, procesando los mensajes entrantes
        e identificando al cliente si es necesario.

        Este método es ejecutado en un hilo independiente.

        Example:
            new Thread(new ClientHandler(cliente)).start();
    */
    @Override
    public void run() {
        try {
            while (true) {
                try {
                    // Recibir el mensaje del cliente
                    String mensajeEntrante = socketServer.recibirMensaje(cliente);
                    if (mensajeEntrante == null) {
                        System.out.println("Cliente desconectado: " + cliente);
                        break;  // Termina el bucle si el cliente se desconecta
                    }

                    // Procesar el primer mensaje para identificar el tipo de cliente
                    if (!clienteIdentificado) {
                        Command comando = jsonProcessor.procesarComando(mensajeEntrante, cliente);
                        if (comando instanceof TipoClienteCommand) {
                            comando.ejecutar();
                            clienteIdentificado = true;  // Marcar cliente como identificado
                        } else {
                            System.out.println("Comando inesperado; se esperaba identificación de tipo de cliente.");
                        }
                    } else {
                        // Procesar mensajes después de la identificación
                        Command comando = jsonProcessor.procesarComando(mensajeEntrante, cliente);
                        if (comando != null) {
                            comando.ejecutar();
                        } else {
                            System.out.println("Comando no reconocido del cliente: " + cliente);
                        }
                    }
                } catch (Exception e) {
                    System.err.println("Error al procesar el mensaje del cliente: " + e.getMessage());
                    break; // Sale del bucle si ocurre una excepción inesperada
                }
            }
        } finally {
            // Limpiar y remover al cliente de la lista
            System.out.println("Cliente removido: " + cliente);
        }
    }
}
