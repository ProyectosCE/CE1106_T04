package org.proyectosce.comandos.factory.products;

import org.proyectosce.comunicaciones.Cliente;
import org.proyectosce.comunicaciones.ComServer;
import org.proyectosce.comunicaciones.SocketServer;

import java.util.Map;
import java.util.Set;

public class SendGameStateCommand implements Command {
    private String gameStateJson;
    private Cliente jugador;
    private ComServer comServer;
    private SocketServer socketServer;

    // Constructor vacío para permitir configuración dinámica
    public SendGameStateCommand() {}

    @Override
    public void ejecutar() {
        if (gameStateJson == null || jugador == null || comServer == null || socketServer == null) {
            throw new IllegalStateException("SendGameStateCommand no está configurado correctamente.");
        }

        Set<Cliente> observadores = comServer.obtenerObservadores(jugador);

        for (Cliente espectador : observadores) {
            socketServer.enviarMensaje(espectador, gameStateJson);
        }

        //System.out.println("Estado del juego enviado a los observadores del jugador: " + jugador.getId());
    }

    @Override
    public String getType() {
        return "sendGameState";
    }

    @Override
    public Map<String, Object> toMap() {
        return Map.of(
                "type", getType(),
                "jugadorId", jugador != null ? jugador.getId() : null,
                "gameStateJson", gameStateJson
        );
    }

    @Override
    public void configure(Map<String, Object> params) {
        this.gameStateJson = (String) params.get("gameStateJson"); // JSON completo
        this.jugador = (Cliente) params.get("emisor"); // Usar "emisor" como jugador
        this.comServer = (ComServer) params.get("comServer");
        this.socketServer = (SocketServer) params.get("socketServer");
    }
}

