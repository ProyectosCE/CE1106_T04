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
package org.proyectosce.comandos.factory.products;

import org.proyectosce.comunicaciones.Cliente;
import org.proyectosce.comunicaciones.ComServer;
import org.proyectosce.comunicaciones.SocketServer;
import java.util.Map;
import java.util.Set;

/*
 * Class: DisconnectCommand
 * Implementa el comando de desconexión de un cliente. Este comando maneja la eliminación de un cliente, ya sea un jugador o espectador,
 * de las listas correspondientes en el servidor. También notifica a los observadores (espectadores) que el jugador se ha desconectado.
 *
 * Attributes:
 *     - cliente: Cliente - El cliente que se va a desconectar.
 *     - comServer: ComServer - Instancia del servidor de comunicación.
 *     - socketServer: SocketServer - Instancia del servidor de sockets.
 *
 * Methods:
 *     - ejecutar(): Ejecuta el comando de desconexión.
 *     - getType(): Devuelve el tipo de comando ("disconnect").
 *     - toMap(): Convierte el comando a un mapa con detalles de la desconexión.
 *     - configure(Map<String, Object>): Configura el comando con los parámetros proporcionados.
 *
 * Example:
 *     Map<String, Object> params = Map.of("cliente", cliente, "comServer", comServer, "socketServer", socketServer);
 *     Command command = new DisconnectCommand();
 *     command.configure(params);
 *     command.ejecutar();
 *
 * Problems:
 *     - Si el comando no está configurado correctamente, lanzará una excepción.
 *
 * References:
 */
public class DisconnectCommand implements Command {
    private Cliente cliente;
    private ComServer comServer;
    private SocketServer socketServer;

    /* Constructor: DisconnectCommand
        Constructor vacío para permitir configuración dinámica del comando.
    */
    public DisconnectCommand() {}

    /* Function: ejecutar
        Ejecuta el comando de desconexión del cliente. El cliente puede ser un jugador o espectador, y el comando
        lo elimina de las listas correspondientes. Si el cliente es un jugador, se notifican a los observadores
        que el jugador se ha desconectado.

        Throws:
            - IllegalStateException: Si el comando no ha sido configurado correctamente.
    */
    @Override
    public void ejecutar() {
        if (cliente == null || comServer == null || socketServer == null) {
            throw new IllegalStateException("DisconnectCommand no está configurado correctamente.");
        }

        // Verificar si el cliente es un jugador y eliminarlo de las listas
        if (comServer.esJugador(cliente)) {
            comServer.eliminarJugador(cliente);

            // Notificar a los observadores del jugador que se ha desconectado
            Set<Cliente> observadores = comServer.obtenerObservadores(cliente);
            for (Cliente espectador : observadores) {
                String mensaje = "El jugador que estás observando se ha desconectado.";
                socketServer.enviarMensaje(espectador, mensaje);
            }
            // Eliminar todos los observadores del jugador
            comServer.eliminarObservadores(cliente);

        } else {
            // Si es un espectador, eliminarlo de las listas de observadores
            comServer.eliminarEspectadorPorId(cliente.getId());
        }

        // Eliminar de la lista general de clientes
        comServer.eliminarCliente(cliente);
    }

    /* Function: getType
        Devuelve el tipo de comando.

        Returns:
            - String: "disconnect"
    */
    @Override
    public String getType() {
        return "disconnect";
    }

    /* Function: toMap
        Convierte el comando en un mapa para su serialización o almacenamiento.

        Returns:
            - Map<String, Object>: Mapa con detalles del comando.
    */
    @Override
    public Map<String, Object> toMap() {
        return Map.of(
                "type", getType(),
                "clienteId", cliente != null ? cliente.getId() : null
        );
    }

    /* Function: configure
        Configura el comando con los parámetros proporcionados.

        Params:
            - params: Map<String, Object> - Mapa con los parámetros necesarios para configurar el comando.
    */
    @Override
    public void configure(Map<String, Object> params) {
        this.cliente = (Cliente) params.get("cliente");
        this.comServer = (ComServer) params.get("comServer");
        this.socketServer = (SocketServer) params.get("socketServer");
    }
}
