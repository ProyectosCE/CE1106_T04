cmake_minimum_required(VERSION 3.29)
project(Client)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)

#FIND PACKEGES HERE:
find_package(cJSON)
find_package(raylib)
find_package(log.c)
find_package(inih)

#Executables here:
add_executable(Client "main_con_gui_(depreciate).c"
        game.c
        game.h
        spectator.c
        spectator.h
        comunicaciones/comServer.c
        comunicaciones/comServer.h
        comunicaciones/socketServer.c
        comunicaciones/socketServer.h
        comunicaciones/jsonProcessor.c
        comunicaciones/jsonProcessor.h
        main.c
        configuracion/configuracion.c
        configuracion/configuracion.h
        logs/saveLog.c
        logs/saveLog.h
)


# Copiar el archivo settings.ini al directorio de salida
configure_file(settings.ini ${CMAKE_CURRENT_BINARY_DIR}/settings.ini COPYONLY)

# Crear la carpeta logs en tiempo de compilación
add_custom_command(
        TARGET Client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:Client>/logs
)

#Targets here:
target_link_libraries(Client cjson::cjson raylib)
target_link_libraries(Client raylib)
target_link_libraries(Client log.c::log.c)
target_link_libraries(Client inih::inih)

# Establecer la carpeta de salida de los ejecutables en "logs"
set_target_properties(Client PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/logs")