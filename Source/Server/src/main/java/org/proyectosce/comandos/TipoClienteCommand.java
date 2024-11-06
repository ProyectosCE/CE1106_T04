package org.proyectosce.comandos;

import org.proyectosce.comunicaciones.Cliente;
import org.proyectosce.comunicaciones.ComServer;

public class TipoClienteCommand implements Command{
    private final String tipoCliente;
    private  Cliente cliente;

    public TipoClienteCommand(String tipoCliente, Cliente cliente) {
        this.tipoCliente = tipoCliente;
        this.cliente = cliente;
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
}
