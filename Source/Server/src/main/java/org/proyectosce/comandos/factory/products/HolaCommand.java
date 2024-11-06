package org.proyectosce.comandos.factory.products;

import java.util.Map;

public class HolaCommand implements Command {
    private String saludo;

    public HolaCommand() {
        this.saludo = "Hola, mundo"; // Valor por defecto
    }

    @Override
    public void ejecutar() {
        System.out.println(saludo);
    }

    @Override
    public String getType() {
        return "hola";
    }

    @Override
    public Map<String, Object> toMap() {
        return Map.of("type", getType(), "saludo", saludo);
    }

    @Override
    public void configure(Map<String, Object> params) {
        if (params.containsKey("saludo")) {
            this.saludo = (String) params.get("saludo");
        }
    }
}
