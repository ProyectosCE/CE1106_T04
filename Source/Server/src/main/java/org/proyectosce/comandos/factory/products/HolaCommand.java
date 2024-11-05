package org.proyectosce.comandos.factory.products;

import java.util.Map;

/*
 * Class: HolaCommand
 *
 * Clase para el comando "hola", que se ejecuta cuando el cliente se conecta.
 */
public class HolaCommand implements Command {

    @Override
    public String getType() {
        return "";
    }

    @Override
    public Map<String, Object> toMap() {
        return Map.of();
    }
}
