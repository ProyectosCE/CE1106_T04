package org.proyectosce.comandos;

import org.proyectosce.comunicaciones.Cliente;
import org.proyectosce.comunicaciones.ComServer;
import org.proyectosce.comunicaciones.SocketServer;

import java.util.Set;

public class SendGameStateCommand implements Command {
    private final String gameStateJson;
    private final Cliente jugador;

    public SendGameStateCommand(String gameStateJson, Cliente jugador) {
        this.gameStateJson = gameStateJson;
        this.jugador = jugador;
    }

    @Override
    public void ejecutar() {
        ComServer comServer = ComServer.getInstance();
        SocketServer socketServer = SocketServer.getInstance();

        Set<Cliente> observadores = comServer.obtenerObservadores(jugador);

        for (Cliente espectador : observadores) {
            socketServer.enviarMensaje(espectador, gameStateJson);

        }
        System.out.println("Update enviado");
    }
}
