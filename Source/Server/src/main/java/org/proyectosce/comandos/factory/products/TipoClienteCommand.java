package org.proyectosce.comandos.factory.products;

import org.proyectosce.comunicaciones.Cliente;
import org.proyectosce.comunicaciones.ComServer;

import java.util.Map;

public class TipoClienteCommand implements Command {
    private final String tipoCliente;
    private  Cliente cliente;

    public TipoClienteCommand(String tipoCliente, Cliente cliente, String nombre) {
        this.tipoCliente = tipoCliente;
        this.cliente = cliente;
        this.cliente.setNombre(nombre);
    }

    @Override
    public void ejecutar() {
        ComServer comServer = ComServer.getInstance();

        if ("player".equals(tipoCliente)) {
            comServer.registrarJugador(cliente);
            System.out.println("Cliente registrado como jugador: " + cliente);

        } else if ("spectador".equals(tipoCliente)) {
            comServer.registrarEspectadorTemporal(cliente);
            comServer.enviarListaDeJugadores(cliente);
            System.out.println("Cliente registrado como espectador: " + cliente);
        }
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
