package org.proyectosce.comandos;

public class PowerCommand implements Command {
    private final String type;
    private int f; // posición x
    private int c; // posición y

    public PowerCommand(String type) {
        this.type = type;
    }

    @Override
    public String getType() {
        return type;
    }

    @Override
    public void setParams(Object params) {
        if (params instanceof int[]) {
            int[] coordinates = (int[]) params;
            this.f = coordinates[0];
            this.c = coordinates[1];
        }
    }

    public int getF() {
        return f;
    }

    public int getC() {
        return c;
    }
}
