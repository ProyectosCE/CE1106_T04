'''
================================== LICENCIA ==================================================
MIT License
Copyright (c) 2024  José Bernardo Barquero Bonilla,
                    Jose Eduardo Campos Salazar,
                    Jimmy Feng Feng,
                    Alexander Montero Vargas
                    Consulta el archivo LICENSE para más detalles.
==============================================================================================
'''
import websocket
import threading
import time
from pynput.keyboard import Controller, Key

# Controlador de teclado
keyboard = Controller()

"""
Function: emulate_key_press
Emula la pulsación de una tecla específica utilizando el controlador de teclado.

Params:
- key: str - La tecla que se desea emular.

Returns:
- None - No retorna ningún valor.

Restriction:
La función utiliza una breve pausa (`time.sleep(0.1)`) para simular un tiempo real de pulsación de la tecla.

Example:
emulate_key_press('a')  # Emula la pulsación de la tecla 'A'.
"""
def emulate_key_press(key):
    keyboard.press(key)
    time.sleep(0.1)  # Mantiene la tecla presionada un breve instante
    keyboard.release(key)

"""
Function: on_error
Maneja los eventos de error durante la comunicación del WebSocket.

Params:
- ws: websocket.WebSocketApp - La instancia del WebSocket.
- error: Exception - El error ocurrido.

Returns:
- None - No retorna ningún valor.

Example:
# Este manejador se configura automáticamente cuando se inicializa el WebSocket.
"""
def on_error(ws, error):
    print("Error:", error)

"""
Function: on_close
Maneja los eventos de cierre del WebSocket, mostrando información relevante.

Params:
- ws: websocket.WebSocketApp - La instancia del WebSocket.
- close_status_code: int - Código de estado del cierre.
- close_msg: str - Mensaje asociado al cierre.

Returns:
- None - No retorna ningún valor.

Example:
# Este manejador se configura automáticamente cuando se inicializa el WebSocket.
"""
def on_close(ws, close_status_code, close_msg):
    print("Conexión cerrada")

"""
Function: on_open
Se ejecuta al abrir la conexión del WebSocket, confirmando la conexión.

Params:
- ws: websocket.WebSocketApp - La instancia del WebSocket.

Returns:
- None - No retorna ningún valor.

Example:
# Este manejador se configura automáticamente cuando se inicializa el WebSocket.
"""
def on_open(ws):
    print("Conexión abierta")

"""
Function: on_message
Maneja los mensajes recibidos por el WebSocket y ejecuta acciones específicas basadas en el contenido del mensaje.

Params:
- ws: websocket.WebSocketApp - La instancia del WebSocket.
- message: str - Mensaje recibido desde el servidor.

Returns:
- None - No retorna ningún valor.

Example:
on_message(ws, "Izquierda")  # Llama a `emulate_key_press('a')`.
"""
def on_message(ws, message):
    if message == "Izquierda":
        print("")
        emulate_key_press('a')  # Emula la tecla A
    elif message == "Derecha":
        print("")
        emulate_key_press('d')  # Emula la tecla D
    elif message == "Presionado":
        print("")
        emulate_key_press('w')  # Emula la tecla W

"""
Function: start_websocket
Inicia un cliente WebSocket que se ejecuta en un hilo separado y se conecta a la IP especificada.

Params:
- esp32_ip: str - La dirección IP del ESP32 en formato WebSocket (e.g., "ws://192.168.15.125:81/").

Returns:
- None - No retorna ningún valor.

Restriction:
Se utiliza un hilo separado para mantener la conexión WebSocket viva mientras se ejecuta el programa principal.

Problems:
- Errores de conexión o cierre inesperado.
- Reconexión automática tras un fallo en la conexión.

Example:
start_websocket("ws://192.168.15.125:81/")  # Conecta al ESP32 en la dirección especificada.
"""
def start_websocket(esp32_ip):
    def run():
        websocket.enableTrace(False)  # Desactiva rastreo para mayor rendimiento
        ws = websocket.WebSocketApp(
            esp32_ip,
            on_open=on_open,
            on_message=on_message,
            on_error=on_error,
            on_close=on_close
        )
        while True:
            try:
                ws.run_forever(ping_interval=30, ping_timeout=5)  # Mantén la conexión viva
            except Exception as e:
                print("Reconectando tras error:", e)
                time.sleep(5)  # Espera 5 segundos antes de reconectar

    # Correr el WebSocket en un hilo separado
    thread = threading.Thread(target=run)
    thread.daemon = True  # Terminar el hilo al cerrar el programa
    thread.start()

# IP del ESP32
esp32_ip = "ws://192.168.15.125:81/"
start_websocket(esp32_ip)

# Mantén el programa principal corriendo
while True:
    try:
        time.sleep(1)  # Mantén el programa vivo mientras el WebSocket trabaja
    except KeyboardInterrupt:
        print("Cerrando cliente...")
        break