package org.proyectosce;

import org.ini4j.Ini;

import java.io.File;
import java.io.IOException;

public class SettingsReader {
    private static SettingsReader instance;
    private final Ini ini;

    private SettingsReader() {
        try {
            ini = new Ini(new File("settings.ini"));
        } catch (IOException e) {
            throw new RuntimeException("Error al leer el archivo settings.ini", e);
        }
    }

    public static synchronized SettingsReader getInstance() {
        if (instance == null) {
            instance = new SettingsReader();
        }
        return instance;
    }

    public String getSocketAddress() {
        return ini.get("socket", "address", String.class);
    }

    public int getSocketPort() {
        return ini.get("socket", "port", int.class);
    }
}
