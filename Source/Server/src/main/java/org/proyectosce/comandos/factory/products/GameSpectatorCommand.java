package org.proyectosce.comandos.factory.products;

import org.proyectosce.comunicaciones.Cliente;
import org.proyectosce.comunicaciones.ComServer;

import java.util.Map;

public class GameSpectatorCommand implements Command {
    private Cliente jugadorAObservar;
    private Cliente espectador;
    private ComServer comServer;

    // Constructor vacío para permitir configuración dinámica
    public GameSpectatorCommand() {}

    @Override
    public void ejecutar() {
        if (jugadorAObservar == null || espectador == null || comServer == null) {
            throw new IllegalStateException("GameSpectatorCommand no está configurado correctamente.");
        }

        comServer.registrarObservador(jugadorAObservar, espectador);
        System.out.println("Espectador registrado para observar al jugador: " + jugadorAObservar.getId());
    }

    @Override
    public String getType() {
        return "GameSpectator";
    }

    @Override
    public Map<String, Object> toMap() {
        return Map.of(
                "type", getType(),
                "jugadorId", jugadorAObservar != null ? jugadorAObservar.getId() : null,
                "espectadorId", espectador != null ? espectador.getId() : null
        );
    }

    @Override
    public void configure(Map<String, Object> params) {
        this.jugadorAObservar = (Cliente) params.get("jugadorAObservar");
        this.espectador = (Cliente) params.get("espectador");
        this.comServer = (ComServer) params.get("comServer");
    }
}
