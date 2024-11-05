package org.proyectosce.factory.powers;
import org.proyectosce.game_objects.*;

public class AddLifePower implements Power {
    @Override
    public void apply(Brick brick) {
        brick.setAddLife(true);
    }

    @Override
    public void apply(Player player) {
        // No afecta al Player, no hace nada
    }
}
