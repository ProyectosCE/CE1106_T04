package org.proyectosce.comunicaciones;

import org.junit.jupiter.api.Test;
import org.proyectosce.comandos.*;

import static org.junit.jupiter.api.Assertions.*;

class JsonProcessorTest {

    @Test
    void crearMensajeSalida() {
        JsonProcessor jsonProcessor = JsonProcessor.getInstance();
        String mensaje = "Hola mundo";

        // Valor esperado
        String expected = "{\"respuesta\":\"Hola mundo\"}";

        // Valor actual
        String actual = jsonProcessor.crearMensajeSalida(mensaje);

        // Aserciones
        assertNotNull(actual, "El mensaje de salida no debe ser nulo");
        assertEquals(expected, actual, "El mensaje JSON no coincide con el esperado");

        // Formato de exportación
        System.out.println("expected: " + expected);
        System.out.println("actual: " + actual);
        System.out.println("pass: " + expected.equals(actual));
    }

    @Test
    void procesarComando() {
        JsonProcessor jsonProcessor = JsonProcessor.getInstance();

        // Caso de éxito
        String mensajeJson = "{\"comando\":\"hola\"}";
        Command expectedCommand = new HolaCommand();

        Command actualCommand = jsonProcessor.procesarComando(mensajeJson);

        assertNotNull(actualCommand, "El comando procesado no debe ser nulo");
        assertTrue(actualCommand instanceof HolaCommand, "El comando procesado debe ser de tipo HolaCommand");

        // Formato de exportación
        System.out.println("expected: " + expectedCommand.getClass().getSimpleName());
        System.out.println("actual: " + actualCommand.getClass().getSimpleName());
        System.out.println("pass: " + (actualCommand instanceof HolaCommand));

        // Caso de error
        String mensajeInvalido = "{\"comando\":\"desconocido\"}";
        Command actualCommandNull = jsonProcessor.procesarComando(mensajeInvalido);

        assertNull(actualCommandNull, "El comando procesado debe ser nulo para comandos desconocidos");

        // Formato de exportación
        System.out.println("expected: null");
        System.out.println("actual: " + (actualCommandNull == null ? "null" : actualCommandNull.getClass().getSimpleName()));
        System.out.println("pass: " + (actualCommandNull == null));

    }
}