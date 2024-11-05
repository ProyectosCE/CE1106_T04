package org.proyectosce.comandos;

import org.proyectosce.comunicaciones.Cliente;

/*
 * Interface: Command
 *
 * Interfaz para la ejecución de comandos.
 */
public interface Command {
    void ejecutar(Cliente cliente);
}

