package org.proyectosce.comandos;

import org.proyectosce.comunicaciones.Cliente;

/*
 * Class: HolaCommand
 *
 * Clase para el comando "hola", que se ejecuta cuando el cliente se conecta.
 */
public class HolaCommand implements Command {
    @Override
    public void ejecutar(Cliente cliente) {
        System.out.println("Comando 'hola' ejecutado para el cliente " + cliente);
    }
}
