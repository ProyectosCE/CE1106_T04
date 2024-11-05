package org.proyectosce.comandos.factory.products;

import java.util.HashMap;
import java.util.Map;

public class PowerCommand implements Command {
    private final String type;
    private final int f; // posición x
    private final int c; // posición y

    public PowerCommand(int f, int c, String type) {
        this.f = f;
        this.c = c;
        this.type = type;
    }

    @Override
    public String getType() {
        return type;
    }

    public int getF() {
        return f;
    }

    public int getC() {
        return c;
    }

    public Map<String, Object> toMap() {
        Map<String, Object> jsonMessage = new HashMap<>();
        jsonMessage.put("command", type);

        Map<String, Integer> position = new HashMap<>();
        position.put("x", c);
        position.put("y", f);

        jsonMessage.put("position", position);
        return jsonMessage;
    }
}
