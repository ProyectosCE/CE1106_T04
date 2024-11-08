import websocket

def on_error(ws, error):
    print("Error:", error)

def on_close(ws, close_status_code, close_msg):
    print("Conexión cerrada")

def on_open(ws):
    print("Conexión abierta")

def on_message(ws, message):
    if message in ["Derecha", "Izquierda", "Arriba", "Abajo", "Presionado"]:
        print(message)

def start_websocket(esp32_ip):
    websocket.enableTrace(False)
    ws = websocket.WebSocketApp(esp32_ip,
                                on_open=on_open,
                                on_message=on_message,
                                on_error=on_error,
                                on_close=on_close)
    ws.run_forever()
