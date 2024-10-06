package org.proyectosce;

public class Main {

    public static void main(String[] args) {
        Server server = Server.getInstance(); // Obtener la instancia única
        server.startServer();
    }

}
