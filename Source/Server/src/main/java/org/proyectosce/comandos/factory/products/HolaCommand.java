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
 * Class: HolaCommand
 * Representa un comando simple que imprime un saludo.
 * El saludo puede ser personalizado mediante parámetros.
 *
 * Attributes:
 *     - saludo: String - El mensaje de saludo que será mostrado.
 *
 * Methods:
 *     - ejecutar(): Imprime el saludo en la consola.
 *     - getType(): Devuelve el tipo de comando ("hola").
 *     - toMap(): Convierte el comando en un mapa con detalles del saludo.
 *     - configure(Map<String, Object>): Configura el comando con los parámetros proporcionados.
 *
 * Example:
 *     Map<String, Object> params = Map.of("saludo", "Hola, usuario");
 *     Command command = new HolaCommand();
 *     command.configure(params);
 *     command.ejecutar(); // Imprime "Hola, usuario"
 *
 * Problems:
 *     - No se han reportado problemas conocidos en la implementación.
 *
 * References:
 */
public class HolaCommand implements Command {
    private String saludo;

    /* Constructor: HolaCommand
        Inicializa el comando con un saludo por defecto.
    */
    public HolaCommand() {
        this.saludo = "Hola, mundo"; // Valor por defecto
    }

    /* Function: ejecutar
        Imprime el saludo configurado en la consola.

        Example:
            Si el saludo es "Hola, mundo", imprime "Hola, mundo".
    */
    @Override
    public void ejecutar() {
        System.out.println(saludo);
    }

    /* Function: getType
        Devuelve el tipo de comando.

        Returns:
            - String: "hola"
    */
    @Override
    public String getType() {
        return "hola";
    }

    /* Function: toMap
        Convierte el comando en un mapa para su serialización o almacenamiento.

        Returns:
            - Map<String, Object>: Mapa con detalles del comando.
    */
    @Override
    public Map<String, Object> toMap() {
        return Map.of("type", getType(), "saludo", saludo);
    }

    /* Function: configure
        Configura el comando con los parámetros proporcionados.

        Params:
            - params: Map<String, Object> - Mapa con los parámetros necesarios para configurar el comando.
              Puede contener el parámetro "saludo" para modificar el mensaje mostrado.
    */
    @Override
    public void configure(Map<String, Object> params) {
        if (params.containsKey("saludo")) {
            this.saludo = (String) params.get("saludo");
        }
    }
}
