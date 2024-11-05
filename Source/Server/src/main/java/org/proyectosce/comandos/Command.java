// Command.java
package org.proyectosce.comandos;

public interface Command<T> {
    void ejecutar(T contexto); // contexto es genérico y flexible para cualquier tipo
}