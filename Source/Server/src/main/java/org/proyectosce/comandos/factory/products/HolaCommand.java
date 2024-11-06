// HolaCommand.java
package org.proyectosce.comandos.factory.products;

import java.util.Map;

public class HolaCommand implements Command {
    @Override
    public void ejecutar() {
        System.out.println("Comando 'hola' ejecutado para el cliente ");
    }

    @Override
    public String getType() {
        return "";
    }

    @Override
    public Map<String, Object> toMap() {
        return Map.of();
    }
}
