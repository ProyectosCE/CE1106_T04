/*
================================== LICENCIA =================
=================================
MIT License
Copyright (c) 2024  José Bernardo Barquero Bonilla,
                    Jose Eduardo Campos Salazar,
                    Jimmy Feng Feng,
                    Alexander Montero Vargas
Consulta el archivo LICENSE para más detalles.
=============================================================
Cambios y Configuraciones del Proyecto3=================================
*/
package org.proyectosce.comandos.factory.products;

import java.util.Map;

/*
 * Interface: Command
 * Define los métodos que deben implementar todos los comandos. Este diseño se utiliza para facilitar la ejecución y configuración de comandos,
 * así como la serialización de los mismos para su envío o almacenamiento.
 *
 * Methods:
 *     - ejecutar(): Ejecuta el comando.
 *     - getType(): Devuelve el tipo de comando como un String.
 *     - toMap(): Convierte el comando en un mapa (por ejemplo, para serialización).
 *     - configure(Map<String, Object>): Inyecta dependencias o configura datos adicionales en el comando.
 *
 * Example:
 *     Command command = new SendGameStateCommand();
 *     command.configure(params);
 *     command.ejecutar();
 *     String type = command.getType();
 *
 * Problems:
 *
 * References:
 */
public interface Command {

    /* Function: ejecutar
        Ejecuta el comando.
    */
    void ejecutar();

    /* Function: getType
        Devuelve el tipo de comando.

        Returns:
            - String - El tipo de comando.
    */
    String getType();

    /* Function: toMap
        Convierte el comando en un mapa de claves y valores, útil para la serialización (como JSON).

        Returns:
            - Map<String, Object> - El comando representado como un mapa.
    */
    Map<String, Object> toMap();

    /* Function: configure
        Configura el comando, inyectando dependencias o datos adicionales.

        Params:
            - params: Map<String, Object> - Los parámetros para configurar el comando.
    */
    void configure(Map<String, Object> params);
}
