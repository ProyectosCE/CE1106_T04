package org.proyectosce.comandos;

import org.proyectosce.comunicaciones.Cliente;

/*
 * Interface: Command
 *
 * Interfaz para la ejecución de comandos.
 */
public interface Command {

    // Método para obtener el tipo de comando
    String getType();

    // Método para establecer parámetros del comando (opcional)
    void setParams(Object params);
}

