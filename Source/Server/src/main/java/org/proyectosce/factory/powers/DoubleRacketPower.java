package org.proyectosce.factory.powers;
import org.proyectosce.game_objects.*;

public class DoubleRacketPower implements Power {
    @Override
    public void apply(Brick brick) {
        brick.setAddDoubleRacket(true);
    }

    @Override
    public void apply(Player player) {
        player.setDoubleRacket(true);
    }
}
