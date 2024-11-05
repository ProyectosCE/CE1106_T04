package org.proyectosce;

import org.proyectosce.comunicaciones.Cliente;

/* Interface: Command
    Esta interfaz define un comando que se puede ejecutar. La interfaz proporciona un metodo execute que debe ser implementado por las clases concretas que representan comandos específicos.

   Methods:
     - execute: Este metodo ejecuta el comando.
*/
public interface Command {
    void execute();

    void ejecutar(Cliente cliente);
}

/* Class: PrintCommand
    Clase que representa un comando concreto que imprime un mensaje. Implementa la interfaz Command y proporciona una implementación para el metodo execute.

    Fields:
      - message: El mensaje a imprimir.

    Methods:
      - PrintCommand: El constructor de la clase PrintCommand.
      - execute: Este metodo imprime el mensaje cuando se ejecuta el comando.
*/
abstract class PrintCommand implements Command {
    private String message;

    public PrintCommand(String message) {
        this.message = message;
    }

    @Override
    public void execute() {
        System.out.println("Comando Print ejecutado: " + message);
    }
}