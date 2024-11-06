// HolaCommand.java
package org.proyectosce.comandos;

import org.proyectosce.comunicaciones.Cliente;

public class HolaCommand implements Command {
    @Override
    public void ejecutar() {
        System.out.println("Comando 'hola' ejecutado para el cliente ");
    }
}
