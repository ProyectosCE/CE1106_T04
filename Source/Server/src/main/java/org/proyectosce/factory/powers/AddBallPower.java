package org.proyectosce.factory.powers;
import org.proyectosce.game_objects.*;


public class AddBallPower implements Power {
    @Override
    public void apply(Brick brick) {
        brick.setAddBall(true);
    }

    @Override
    public void apply(Player player) {
        // No afecta al Player, no hace nada
    }
}
