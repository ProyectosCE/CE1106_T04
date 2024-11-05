package org.proyectosce.comandos.factory.products;

/*
 * Interface: Command
 *
 * Interfaz para la ejecución de comandos.
 */
import java.util.Map;

public interface Command {
    // Método para obtener el tipo de comando
    String getType();

    // Método para convertir el comando en un mapa (ideal para serialización, por ejemplo, a JSON)
    Map<String, Object> toMap();
}
