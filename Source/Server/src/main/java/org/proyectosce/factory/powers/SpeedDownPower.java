package org.proyectosce.factory.powers;
import org.proyectosce.game_objects.*;

public class SpeedDownPower implements Power {
    @Override
    public void apply(Brick brick) {
        brick.setSpeedDown(true);
    }

    @Override
    public void apply(Player player) {
        // No afecta al Player, no hace nada
    }
}
