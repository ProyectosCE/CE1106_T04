cmake_minimum_required(VERSION 3.27)
project(Client)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)


#FIND PACKEGES HERE:
find_package(cJSON)
find_package(raylib)
find_package(log.c)
find_package(inih)
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

#Executables here:
add_executable(Client
        game/spectator.c
        game/spectator.h
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
        game/game_server.c
        game/game_server.h
        game/game_logic.c
        game/game_logic.h
        game/game_screen.c
        game/game_screen.h
        game/Objects/player.c
        game/Objects/player.h
        game/Objects/ball.c
        game/Objects/ball.h
        game/Objects/brick.c
        game/Objects/brick.h
        gui/camera.c
        gui/camera.h
        gui/main_menu.c
        gui/main_menu.h
        game_status.c
        game_status.h
        game/collision_handler.c
        game/collision_handler.h
        game/powerHandler.c
        game/powerHandler.h
        gui/nameInput.c
        gui/nameInput.h
        gui/screenHandler.c
        comunicaciones/ESP32_Controller/websocket_client.c
        comunicaciones/ESP32_Controller/websocket_client.h

)


# Copiar el archivo settings.ini al directorio de salida
file(COPY ${CMAKE_SOURCE_DIR}/settings.ini DESTINATION ${CMAKE_BINARY_DIR})

set(PYTHON_SCRIPT_PATH ${CMAKE_SOURCE_DIR}/comunicaciones/ESP32_Controller)
add_compile_definitions(PYTHON_SCRIPT_PATH="${PYTHON_SCRIPT_PATH}")



# Crear la carpeta logs en tiempo de compilación
add_custom_command(
        TARGET Client POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:Client>/logs
)

#Targets here:


target_include_directories(Client PRIVATE ${Python3_INCLUDE_DIRS})
target_link_libraries(Client
        PRIVATE
        cjson::cjson
        raylib
        log.c::log.c
        inih::inih
        ${Python3_LIBRARIES}
)

# INSTALLATION RULES:
# Instalar el ejecutable en el directorio bin
install(TARGETS Client DESTINATION bin)

# Instalar archivos de configuración en el directorio etc
install(FILES ${CMAKE_SOURCE_DIR}/settings.ini DESTINATION etc/client)

# Crear el directorio logs en el runtime
install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/logs\")")
