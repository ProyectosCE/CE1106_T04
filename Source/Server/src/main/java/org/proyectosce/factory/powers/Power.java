package org.proyectosce.factory.powers;
import org.proyectosce.game_objects.Brick;
import org.proyectosce.game_objects.Player;

public interface Power {
    void apply(Brick brick);
    void apply(Player player);
}
