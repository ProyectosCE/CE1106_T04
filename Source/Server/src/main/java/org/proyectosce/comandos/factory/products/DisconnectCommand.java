package org.proyectosce.comandos.factory.products;

import org.proyectosce.comunicaciones.Cliente;
import org.proyectosce.comunicaciones.ComServer;
import org.proyectosce.comunicaciones.SocketServer;

import java.util.Map;
import java.util.Set;

public class DisconnectCommand implements Command {
    private final Cliente cliente;

    public DisconnectCommand(Cliente cliente) {
        this.cliente = cliente;
    }

    @Override
    public void ejecutar() {
        ComServer comServer = ComServer.getInstance();
        SocketServer socketServer = SocketServer.getInstance();

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
            comServer.eliminarEspectador(cliente);
        }

        // Eliminar de la lista general de clientes
        comServer.eliminarCliente(cliente);
    }

    @Override
    public String getType() {
        return "";
    }

    @Override
    public Map<String, Object> toMap() {
        return Map.of();
    }
}
