package org.proyectosce.comunicaciones;

import java.io.IOException;
import java.nio.channels.SocketChannel;

/*
 * Class: Cliente
 *
 * Almacena la información relacionada con los clientes conectados.
 */
public class Cliente {
    private final SocketChannel channel;

    public Cliente(SocketChannel channel) {
        this.channel = channel;
    }

    public SocketChannel getChannel() {
        return channel;
    }

    public String getDireccion() throws IOException {
        return channel.getRemoteAddress().toString();
    }
}
