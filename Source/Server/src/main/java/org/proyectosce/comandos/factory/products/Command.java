package org.proyectosce.comandos.factory.products;

import java.util.Map;

public interface Command {
    // Ejecuta el comando
    void ejecutar();

    // Devuelve el tipo de comando
    String getType();

    // Convierte el comando en un mapa (para serialización, por ejemplo, JSON)
    Map<String, Object> toMap();

    // Método para inyectar dependencias o configurar datos adicionales, si es necesario
    void configure(Map<String, Object> params);
}
