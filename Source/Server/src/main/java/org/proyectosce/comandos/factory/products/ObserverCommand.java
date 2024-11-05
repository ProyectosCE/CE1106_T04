package org.proyectosce.comandos.factory.products;

import java.util.HashMap;
import java.util.Map;

public class ObserverCommand implements Command {
    private final String type = "observer_command";

    @Override
    public String getType() {
        return type;
    }

    @Override
    public Map<String, Object> toMap() {
        Map<String, Object> jsonMessage = new HashMap<>();
        jsonMessage.put("command", type);
        return jsonMessage;
    }
}