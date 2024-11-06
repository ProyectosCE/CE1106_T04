package org.proyectosce.comandos.factory.products;

import org.proyectosce.comunicaciones.Cliente;
import org.proyectosce.comunicaciones.ComServer;
import org.proyectosce.comunicaciones.SocketServer;

import java.util.Map;
import java.util.Set;

public class DisconnectCommand implements Command {
    private Cliente cliente;
    private ComServer comServer;
    private SocketServer socketServer;

    // Constructor vacío para permitir configuración dinámica
    public DisconnectCommand() {}

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

    @Override
    public String getType() {
        return "disconnect";
    }

    @Override
    public Map<String, Object> toMap() {
        return Map.of(
                "type", getType(),
                "clienteId", cliente != null ? cliente.getId() : null
        );
    }

    @Override
    public void configure(Map<String, Object> params) {
        this.cliente = (Cliente) params.get("cliente");
        this.comServer = (ComServer) params.get("comServer");
        this.socketServer = (SocketServer) params.get("socketServer");
    }
}
