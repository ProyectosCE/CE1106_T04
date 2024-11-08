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
package org.proyectosce.comandos;

import org.proyectosce.comunicaciones.JsonProcessor;
import org.proyectosce.comunicaciones.SocketServer;

/*
 * Class: CommandHandler
 * Esta clase maneja los comandos que se reciben y procesan en el servidor.
 * Se encarga de la integración con el procesador de JSON y el servidor de socket.
 *
 * Attributes:
 *     - jsonProcessor: JsonProcessor - Instancia de la clase JsonProcessor para procesar los mensajes en formato JSON.
 *     - socketServer: SocketServer - Instancia de la clase SocketServer que gestiona las conexiones y mensajes.
 *
 * Constructor:
 *     - CommandHandler(): Inicializa las instancias de JsonProcessor y SocketServer.
 *
 * Example:
 *     CommandHandler commandHandler = new CommandHandler();
 *
 * Problems:
 *
 * References:
 */
public class CommandHandler {
    private final JsonProcessor jsonProcessor;
    private final SocketServer socketServer;

    /* Constructor: CommandHandler
        Inicializa las instancias de JsonProcessor y SocketServer.

        Example:
            CommandHandler commandHandler = new CommandHandler();
    */
    public CommandHandler() {
        this.jsonProcessor = JsonProcessor.getInstance();
        this.socketServer = SocketServer.getInstance();
    }
}
