package org.proyectosce.comandos;

import org.proyectosce.comunicaciones.Cliente;
import org.proyectosce.comunicaciones.ComServer;

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
    }
}
