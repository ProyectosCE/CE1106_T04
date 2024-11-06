package org.proyectosce.comunicaciones;

import java.io.IOException;
import java.nio.channels.SocketChannel;

import java.util.UUID;

/*
 * Class: Cliente
 *
 * Almacena la información relacionada con los clientes conectados.
 */
public class Cliente {
    private final SocketChannel channel;
    private final String id;  // Nuevo identificador único

    public Cliente(SocketChannel channel) {
        this.channel = channel;
        this.id = UUID.randomUUID().toString(); // Genera un ID único
    }

    public SocketChannel getChannel() {
        return channel;
    }

    public String getDireccion() throws IOException {
        return channel.getRemoteAddress().toString();
    }

    public String getId() {
        return id;
    }
}
