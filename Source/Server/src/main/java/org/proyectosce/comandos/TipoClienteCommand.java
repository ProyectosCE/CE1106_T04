package org.proyectosce.comandos;

import org.proyectosce.comunicaciones.Cliente;
import org.proyectosce.comunicaciones.ComServer;

public class TipoClienteCommand implements Command<Cliente> {
    private final String tipoCliente;

    public TipoClienteCommand(String tipoCliente) {
        this.tipoCliente = tipoCliente;
    }

    @Override
    public void ejecutar(Cliente cliente) {
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
