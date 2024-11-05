package org.proyectosce.factory.powers;
import org.proyectosce.game_objects.*;

public class HalfRacketPower implements Power {
    @Override
    public void apply(Brick brick) {
        brick.setAddHalfRacket(true);
    }

    @Override
    public void apply(Player player) {
        player.setHalfRacket(true);
    }
}
