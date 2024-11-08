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

/* Class: SettingsReader
Clase encargada de leer configuraciones desde un archivo INI para configurar el sistema.
Attributes:
    - instance: SettingsReader - instancia única de la clase (patrón Singleton).
    - ini: Ini - objeto que representa el archivo de configuración.
Constructor:
    - SettingsReader: Constructor privado que carga el archivo `settings.ini`.
Methods:
    - getInstance: Obtiene la instancia única de la clase.
    - getSocketAddress: Devuelve la dirección del socket desde el archivo de configuración.
    - getSocketPort: Devuelve el puerto del socket desde el archivo de configuración.
Example:
    SettingsReader reader = SettingsReader.getInstance();
    String address = reader.getSocketAddress();
    int port = reader.getSocketPort();
Problems:

References:

*/

package org.proyectosce;

import org.ini4j.Ini;

import java.io.File;
import java.io.IOException;

public class SettingsReader {
    private static SettingsReader instance;
    private final Ini ini;

    /* Function: SettingsReader
    Constructor privado que inicializa el objeto `Ini` con el contenido del archivo `settings.ini`.
    Params:
        - No aplica (constructor sin parámetros).
    Returns:
        - No aplica (es un constructor).
    Restriction:
        - El archivo `settings.ini` debe existir en el directorio raíz del proyecto.
    Example:
    No aplica.
    Problems:

    References:

    */
    private SettingsReader() {
        try {
            ini = new Ini(new File("settings.ini"));
        } catch (IOException e) {
            throw new RuntimeException("Error al leer el archivo settings.ini", e);
        }
    }

    /* Function: getInstance
    Obtiene la instancia única de la clase (patrón Singleton).
    Params:
        - No aplica.
    Returns:
        - SettingsReader - la instancia única de la clase.
    Restriction:
        - Debe llamarse siempre para obtener la instancia de la clase.
    Example:
        SettingsReader reader = SettingsReader.getInstance();
    Problems:

    References:

    */
    public static synchronized SettingsReader getInstance() {
        if (instance == null) {
            instance = new SettingsReader();
        }
        return instance;
    }

    /* Function: getSocketAddress
    Devuelve la dirección del socket desde el archivo de configuración.
    Params:
        - No aplica.
    Returns:
        - String - dirección del socket.
    Restriction:
        - El archivo `settings.ini` debe contener la sección y clave necesarias.
    Example:
        String address = SettingsReader.getInstance().getSocketAddress();
    Problems:

    References:

    */
    public String getSocketAddress() {
        return ini.get("socket", "address", String.class);
    }

    /* Function: getSocketPort
    Devuelve el puerto del socket desde el archivo de configuración.
    Params:
        - No aplica.
    Returns:
        - int - número del puerto del socket.
    Restriction:
        - El archivo `settings.ini` debe contener la sección y clave necesarias.
    Example:
        int port = SettingsReader.getInstance().getSocketPort();
    Problems:

    References:

    */
    public int getSocketPort() {
        return ini.get("socket", "port", int.class);
    }
}
