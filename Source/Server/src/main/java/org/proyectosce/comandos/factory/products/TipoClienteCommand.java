package org.proyectosce.comandos.factory.products;

import org.proyectosce.comunicaciones.Cliente;
import org.proyectosce.comunicaciones.ComServer;

import java.util.Map;

public class TipoClienteCommand implements Command {
    private String tipoCliente;
    private Cliente cliente;
    private ComServer comServer;
    private String playerName;

    // Constructor vacío para facilitar la creación desde la fábrica
    public TipoClienteCommand() {}

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

    @Override
    public String getType() {
        return "tipoCliente";
    }

    @Override
    public Map<String, Object> toMap() {
        return Map.of(
                "type", getType(),
                "tipoCliente", tipoCliente,
                "playerName", playerName
        );
    }

    @Override
    public void configure(Map<String, Object> params) {
        this.tipoCliente = (String) params.get("tipoCliente");
        this.cliente = (Cliente) params.get("emisor");
        this.playerName = (String) params.get("playerName");
        this.comServer = (ComServer) params.get("comServer");
    }
}
