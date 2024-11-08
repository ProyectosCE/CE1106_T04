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

import java.nio.channels.SocketChannel;
import java.util.UUID;

/*
 * Class: Cliente
 * Esta clase almacena la información relacionada con los clientes conectados al servidor.
 * Cada cliente está identificado de forma única mediante un ID generado aleatoriamente,
 * y se le asigna un canal de comunicación a través de un SocketChannel.
 *
 * Attributes:
 *     - channel: SocketChannel - Canal de comunicación asociado al cliente.
 *     - id: String - Identificador único generado para cada cliente.
 *     - nombre: String - Nombre del cliente.
 *
 * Constructor:
 *     - Cliente(SocketChannel channel): Inicializa un cliente con el canal de comunicación y
 *       genera un ID único.
 *
 * Methods:
 *     - getChannel(): Devuelve el canal de comunicación asociado al cliente.
 *     - getId(): Devuelve el identificador único del cliente.
 *     - getNombre(): Devuelve el nombre del cliente.
 *     - setNombre(String nombre): Establece el nombre del cliente.
 *     - toString(): Devuelve una representación en cadena del nombre del cliente.
 *
 * Example:
 *     Cliente cliente = new Cliente(socketChannel);
 *     cliente.setNombre("Juan");
 *     System.out.println(cliente.toString()); // Imprime el nombre del cliente
 *
 * Problems:
 *
 * References:
 */
public class Cliente {
    private final SocketChannel channel;
    private final String id;  // Nuevo identificador único
    private String nombre;

    /* Constructor: Cliente
        Inicializa un cliente con un canal de comunicación y un ID único.

        Params:
            - channel: SocketChannel - Canal de comunicación para este cliente.
    */
    public Cliente(SocketChannel channel) {
        this.channel = channel;
        this.id = UUID.randomUUID().toString(); // Genera un ID único
        this.nombre = "Jugador"; // Nombre por defecto
    }

    /* Function: getChannel
        Devuelve el canal de comunicación del cliente.

        Returns:
            - SocketChannel: El canal de comunicación asociado al cliente.
    */
    public SocketChannel getChannel() {
        return channel;
    }

    /* Function: getId
        Devuelve el identificador único del cliente.

        Returns:
            - String: El ID único del cliente.
    */
    public String getId() {
        return id;
    }

    /* Function: getNombre
        Devuelve el nombre del cliente.

        Returns:
            - String: El nombre del cliente.
    */
    public String getNombre() {
        return nombre;
    }

    /* Function: setNombre
        Establece el nombre del cliente.

        Params:
            - nombre: String - El nombre que se asignará al cliente.
    */
    public void setNombre(String nombre) {
        this.nombre = nombre;
    }

    /* Function: toString
        Devuelve una representación en cadena del cliente, en este caso su nombre.

        Returns:
            - String: El nombre del cliente.
    */
    public String toString() {
        return getNombre(); // Devuelve el nombre del cliente
    }
}
