// GameSpectatorCommand.java
package org.proyectosce.comandos;

import org.proyectosce.comunicaciones.Cliente;
import org.proyectosce.comunicaciones.ComServer;

public class GameSpectatorCommand implements Command<Cliente> {
    private final Cliente jugadorAObservar;

    public GameSpectatorCommand(Cliente jugadorAObservar) {
        this.jugadorAObservar = jugadorAObservar;
    }

    @Override
    public void ejecutar(Cliente espectador) {
        ComServer.getInstance().registrarObservador(jugadorAObservar, espectador);
        System.out.println("Espectador registrado para observar al jugador: " + jugadorAObservar);
    }
}
