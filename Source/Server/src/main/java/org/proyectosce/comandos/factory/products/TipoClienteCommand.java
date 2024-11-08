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
import java.util.Map;

/*
 * Class: TipoClienteCommand
 * Representa un comando que establece el tipo de un cliente (jugador o espectador) en el sistema.
 *
 * Attributes:
 *     - tipoCliente: String - El tipo de cliente, puede ser "player" o "spectador".
 *     - cliente: Cliente - El cliente que se está registrando o modificando.
 *     - comServer: ComServer - Referencia al servidor de comunicaciones para registrar al cliente.
 *     - playerName: String - El nombre del jugador, si el cliente es un jugador.
 *
 * Methods:
 *     - ejecutar(): Ejecuta el comando, registrando al cliente como jugador o espectador.
 *     - getType(): Devuelve el tipo de comando (en este caso, "tipoCliente").
 *     - toMap(): Convierte el comando en un mapa para su serialización.
 *     - configure(Map<String, Object>): Configura el comando con los parámetros proporcionados.
 *
 * Example:
 *     Map<String, Object> params = Map.of(
 *         "tipoCliente", "player",
 *         "playerName", "JugadorUno",
 *         "emisor", clienteInstance,
 *         "comServer", comServerInstance
 *     );
 *     Command command = new TipoClienteCommand();
 *     command.configure(params);
 *     command.ejecutar(); // Registra el cliente como jugador con el nombre proporcionado.
 *
 * Problems:
 *     - No se han reportado problemas conocidos en la implementación.
 *
 * References:
 */
public class TipoClienteCommand implements Command {
    private String tipoCliente; // Tipo de cliente (jugador o espectador)
    private Cliente cliente; // Cliente que será registrado o modificado
    private ComServer comServer; // Servidor de comunicaciones para registrar al cliente
    private String playerName; // Nombre del jugador, si el cliente es un jugador

    /* Constructor: TipoClienteCommand
        Facilita la creación del comando desde la fábrica.
    */
    public TipoClienteCommand() {}

    /* Function: ejecutar
        Ejecuta el comando, registrando al cliente como jugador o espectador en el sistema.

        - Si el tipo de cliente es "player", se registra al cliente como jugador.
        - Si el tipo de cliente es "spectador", se registra al cliente como espectador temporal y se le envía la lista de jugadores.
        - Si el tipo de cliente no es reconocido, se muestra un mensaje de error.

        Example:
            Si el tipo de cliente es "player" y se proporciona un nombre, el cliente se registrará como jugador.
    */
    @Override
    public void ejecutar() {
        if ("player".equals(tipoCliente)) {
            cliente.setNombre(playerName);
            comServer.registrarJugador(cliente);
            System.out.println("Cliente registrado como jugador: " + cliente);
        } else if ("spectador".equals(tipoCliente)) {
            comServer.registrarEspectadorTemporal(cliente);
            comServer.enviarListaDeJugadores(cliente);
        } else {
            System.err.println("Tipo de cliente desconocido: " + tipoCliente);
        }
    }

    /* Function: getType
        Devuelve el tipo de comando.

        Returns:
            - String: El tipo de comando, en este caso, "tipoCliente".
    */
    @Override
    public String getType() {
        return "tipoCliente";
    }

    /* Function: toMap
        Convierte el comando en un mapa para su serialización o almacenamiento.

        Returns:
            - Map<String, Object>: Mapa con los detalles del comando, incluyendo el tipo de cliente y el nombre del jugador.
    */
    @Override
    public Map<String, Object> toMap() {
        return Map.of(
                "type", getType(),
                "tipoCliente", tipoCliente,
                "playerName", playerName
        );
    }

    /* Function: configure
        Configura el comando con los parámetros proporcionados.

        Params:
            - params: Map<String, Object> - Mapa con los parámetros necesarios para configurar el comando.
              Debe contener "tipoCliente", "emisor", "playerName" y "comServer".
    */
    @Override
    public void configure(Map<String, Object> params) {
        this.tipoCliente = (String) params.get("tipoCliente"); // Tipo de cliente (player o spectador)
        this.cliente = (Cliente) params.get("emisor"); // Cliente a registrar
        this.playerName = (String) params.get("playerName"); // Nombre del jugador si el cliente es un jugador
        this.comServer = (ComServer) params.get("comServer");
    }
}
