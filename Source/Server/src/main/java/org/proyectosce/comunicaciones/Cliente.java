package org.proyectosce.comunicaciones;

import java.io.IOException;
import java.nio.channels.SocketChannel;
import java.nio.ByteBuffer;

public class Cliente {
    private final SocketChannel clientChannel;

    public Cliente(SocketChannel clientChannel) {
        this.clientChannel = clientChannel;
    }

    public SocketChannel getChannel() {
        return clientChannel;
    }

    // Método para enviar un mensaje al cliente
    public void enviarMensaje(String mensaje) {
        try {
            ByteBuffer buffer = ByteBuffer.wrap(mensaje.getBytes());
            clientChannel.write(buffer);
            System.out.println("Mensaje enviado al cliente: " + mensaje);
        } catch (IOException e) {
            System.out.println("Error al enviar mensaje al cliente: " + e.getMessage());
        }
    }
}
