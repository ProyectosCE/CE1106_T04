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

import org.proyectosce.comunicaciones.Cliente;
import org.proyectosce.comunicaciones.ComServer;
import java.util.Map;

/*
 * Class: GameSpectatorCommand
 * Implementa el comando para registrar a un espectador que desea observar a un jugador específico.
 * El espectador podrá recibir información sobre el estado del jugador observado.
 *
 * Attributes:
 *     - jugadorAObservar: Cliente - El jugador que será observado.
 *     - espectador: Cliente - El cliente que desea observar al jugador.
 *     - comServer: ComServer - Instancia del servidor de comunicación que gestiona la conexión.
 *
 * Methods:
 *     - ejecutar(): Ejecuta el comando registrando al espectador para observar al jugador.
 *     - getType(): Devuelve el tipo de comando ("GameSpectator").
 *     - toMap(): Convierte el comando en un mapa con detalles del espectador y el jugador observado.
 *     - configure(Map<String, Object>): Configura el comando con los parámetros proporcionados.
 *
 * Example:
 *     Map<String, Object> params = Map.of("jugadorAObservar", jugador, "espectador", espectador, "comServer", comServer);
 *     Command command = new GameSpectatorCommand();
 *     command.configure(params);
 *     command.ejecutar();
 *
 * Problems:
 *     - Si el comando no está configurado correctamente, lanzará una excepción.
 *
 * References:
 */
public class GameSpectatorCommand implements Command {
    private Cliente jugadorAObservar;
    private Cliente espectador;
    private ComServer comServer;

    /* Constructor: GameSpectatorCommand
        Constructor vacío para permitir configuración dinámica del comando.
    */
    public GameSpectatorCommand() {}

    /* Function: ejecutar
        Ejecuta el comando de registrar a un espectador para observar a un jugador específico.
        El espectador será capaz de recibir actualizaciones sobre el estado del jugador observado.

        Throws:
            - IllegalStateException: Si el comando no ha sido configurado correctamente.
    */
    @Override
    public void ejecutar() {
        if (jugadorAObservar == null || espectador == null || comServer == null) {
            throw new IllegalStateException("GameSpectatorCommand no está configurado correctamente.");
        }

        comServer.registrarObservador(jugadorAObservar, espectador);
        System.out.println("Espectador registrado para observar al jugador: " + jugadorAObservar.getId());
    }

    /* Function: getType
        Devuelve el tipo de comando.

        Returns:
            - String: "GameSpectator"
    */
    @Override
    public String getType() {
        return "GameSpectator";
    }

    /* Function: toMap
        Convierte el comando en un mapa para su serialización o almacenamiento.

        Returns:
            - Map<String, Object>: Mapa con detalles del comando.
    */
    @Override
    public Map<String, Object> toMap() {
        return Map.of(
                "type", getType(),
                "jugadorId", jugadorAObservar != null ? jugadorAObservar.getId() : null,
                "espectadorId", espectador != null ? espectador.getId() : null
        );
    }

    /* Function: configure
        Configura el comando con los parámetros proporcionados.

        Params:
            - params: Map<String, Object> - Mapa con los parámetros necesarios para configurar el comando.
    */
    @Override
    public void configure(Map<String, Object> params) {
        this.jugadorAObservar = (Cliente) params.get("jugadorAObservar");
        this.espectador = (Cliente) params.get("espectador");
        this.comServer = (ComServer) params.get("comServer");
    }
}
