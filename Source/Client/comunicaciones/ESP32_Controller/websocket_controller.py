import websocket
import threading
import time
from pynput.keyboard import Controller, Key

# Controlador de teclado
keyboard = Controller()

# Función para emular la pulsación de teclas
def emulate_key_press(key):
    keyboard.press(key)
    time.sleep(0.1)  # Mantiene la tecla presionada un breve instante
    keyboard.release(key)

# Funciones de manejo de eventos WebSocket
def on_error(ws, error):
    print("Error:", error)

def on_close(ws, close_status_code, close_msg):
    print("Conexión cerrada")

def on_open(ws):
    print("Conexión abierta")

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


# Función para iniciar el WebSocket en un hilo
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