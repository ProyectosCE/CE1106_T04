package org.proyectosce.comandos.factory.products;

import org.proyectosce.comunicaciones.Cliente;
import org.proyectosce.comunicaciones.ComServer;

import java.util.Map;

public class GameSpectatorCommand implements Command {
    private final Cliente jugadorAObservar;
    private final Cliente espectador;

    public GameSpectatorCommand(Cliente jugadorAObservar, Cliente espectador) {
        this.jugadorAObservar = jugadorAObservar;
        this.espectador = espectador;
    }

    @Override
    public void ejecutar() {
        ComServer.getInstance().registrarObservador(jugadorAObservar, espectador);
        System.out.println("Espectador registrado para observar al jugador: " + jugadorAObservar);
        ComServer.getInstance().actualizarListas();
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
