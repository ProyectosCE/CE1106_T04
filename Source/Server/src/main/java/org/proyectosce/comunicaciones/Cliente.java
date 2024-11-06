package org.proyectosce.comunicaciones;

import java.io.IOException;
import java.nio.channels.SocketChannel;
import java.nio.ByteBuffer;
import java.util.UUID;

/*
 * Class: Cliente
 *
 * Almacena la información relacionada con los clientes conectados.
 */
public class Cliente {
    private final SocketChannel channel;
    private final String id;  // Nuevo identificador único
    private String nombre;

    public Cliente(SocketChannel channel) {
        this.channel = channel;
        this.id = UUID.randomUUID().toString(); // Genera un ID único
        this.nombre = "Jugador"; // Genera un ID único
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

    public String getNombre() {
        return nombre;
    }

    public void setNombre(String nombre) {
        this.nombre=nombre;
    }

    public void enviarMensaje(String mensaje) {
        try {
            ByteBuffer buffer = ByteBuffer.wrap(mensaje.getBytes());
            channel.write(buffer);
            System.out.println("Mensaje enviado al cliente: " + mensaje);
        } catch (IOException e) {
            System.err.println("Error al enviar mensaje al cliente: " + e.getMessage());
        }
    }

    public String recibirMensaje() {
        try {
            ByteBuffer buffer = ByteBuffer.allocate(4096); // Tamaño del buffer para recibir datos
            int bytesRead = channel.read(buffer); // Lee datos del canal
            if (bytesRead == -1) {
                cerrarConexion(); // Cierra la conexión si no hay más datos
                return null; // Retorna null si se ha cerrado el canal
            }
            buffer.flip(); // Prepara el buffer para leer
            byte[] bytes = new byte[buffer.remaining()]; // Crea un array para almacenar los datos leídos
            buffer.get(bytes); // Copia los datos del buffer al array
            return new String(bytes).trim(); // Retorna el mensaje como string, eliminando espacios en blanco
        } catch (IOException e) {
            System.err.println("Error al recibir mensaje del cliente: " + e.getMessage());
            cerrarConexion(); // Cierra la conexión en caso de error
            return null; // Retorna null en caso de error
        }
    }

    public void cerrarConexion() {
        try {
            if (channel.isOpen()) {
                channel.close(); // Cierra el canal si está abierto
                System.out.println("Conexión cerrada con el cliente.");
            }
        } catch (IOException e) {
            System.err.println("Error al cerrar la conexión del cliente: " + e.getMessage());
        }
    }
}
