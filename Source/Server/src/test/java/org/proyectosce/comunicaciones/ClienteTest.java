package org.proyectosce.comunicaciones;

import org.junit.Test;

import java.nio.channels.SocketChannel;

import static org.junit.Assert.assertEquals;
import static org.junit.jupiter.api.Assertions.assertAll;

public class ClienteTest {

    @Test
    public void testCantidadUsuarios(){
        int uno=2;
        assertAll(() -> assertEquals(2,uno));
    }
}
