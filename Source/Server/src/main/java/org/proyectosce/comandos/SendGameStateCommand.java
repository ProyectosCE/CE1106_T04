// SendGameStateCommand.java
package org.proyectosce.comandos;

import org.proyectosce.comunicaciones.ComServer;

public class SendGameStateCommand implements Command<Void> {
    private final String gameStateJson;

    public SendGameStateCommand(String gameStateJson) {
        this.gameStateJson = gameStateJson;
    }

    @Override
    public void ejecutar(Void contexto) {
        // Difunde el mensaje de estado del juego a todos los clientes conectados
        ComServer.getInstance().enviarMensajeATodos(gameStateJson);
    }
}

