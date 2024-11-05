package org.proyectosce.factory;
import org.proyectosce.factory.powers.*;

public class PowerFactory {
    public static Power createPower(String powerType) {
        return switch (powerType) {
            case "add_life" -> new AddLifePower();
            case "add_ball" -> new AddBallPower();
            case "double_racket_size" -> new DoubleRacketPower();
            case "half_racket_size" -> new HalfRacketPower();
            case "add_speed" -> new SpeedUpPower();
            case "reduce_speed" -> new SpeedDownPower();
            default -> throw new IllegalArgumentException("Tipo de poder desconocido: " + powerType);
        };
    }
}
