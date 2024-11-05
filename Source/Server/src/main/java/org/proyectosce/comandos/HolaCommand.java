// HolaCommand.java
package org.proyectosce.comandos;

import org.proyectosce.comunicaciones.Cliente;

public class HolaCommand implements Command<Cliente> {
    @Override
    public void ejecutar(Cliente cliente) {
        System.out.println("Comando 'hola' ejecutado para el cliente " + cliente);
    }
}
