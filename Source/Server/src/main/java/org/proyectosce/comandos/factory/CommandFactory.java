package org.proyectosce.comandos.factory;

import org.proyectosce.comandos.factory.products.*;

public class CommandFactory {

    public Command crearComando(String tipoComando, String... parametros) {
        switch (tipoComando) {
            case "brick_pwr":
                if (parametros.length == 3) {
                    try {
                        int f = Integer.parseInt(parametros[0].trim());
                        int c = Integer.parseInt(parametros[1].trim());
                        String power = parametros[2].trim();
                        return new PowerCommand(f, c, power);
                    } catch (NumberFormatException e) {
                        throw new IllegalArgumentException("Las coordenadas f y c deben ser enteros.");
                    }
                }
                throw new IllegalArgumentException("Formato de comando incorrecto para brick_pwr.");

            case "new_connection":
                return new NewConnectionCommand();

            case "player_command":
                return new PlayerCommand();

            case "observer_command":
                return new ObserverCommand();

            case "game_state_command":
                return new GameStateCommand();

            default:
                throw new IllegalArgumentException("Comando desconocido: " + tipoComando);
        }
    }
}
