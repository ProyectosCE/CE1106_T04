package org.proyectosce.comandos.factory.products;

import java.util.Map;

public interface Command {
    void ejecutar();

    // Método para obtener el tipo de comando
    String getType();

    // Método para convertir el comando en un mapa (ideal para serialización, por ejemplo, a JSON)
    Map<String, Object> toMap();
}
