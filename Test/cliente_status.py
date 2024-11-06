import socket
import json
import time

def jugador_cliente():
    # Configuración del servidor
    host = '192.168.100.6'  # Cambia esto si el servidor está en otra máquina
    port = 12345

    # Crear socket y conectar al servidor
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.connect((host, port))

        # Enviar mensaje de tipoCliente para registrarse como "player"
        tipo_cliente = json.dumps({
            "command": "tipoCliente",
            "tipoCliente": "player"
        })
        sock.sendall(tipo_cliente.encode('utf-8'))
        time.sleep(1)

        # Enviar estado del juego continuamente
        while True:
            estado_juego = json.dumps({
                "command": "sendGameState",
                "player": {
                    "positionX": 400,
                    "positionY": 393,
                    "sizeX": 80,
                    "sizeY": 10,
                    "life": 3,
                    "score": 10
                },
                "balls": [
                    {"active": True, "positionX": 410, "positionY": 370},
                    {"active": False, "positionX": 400, "positionY": 363}
                ]
            })

            # Enviar estado de juego al servidor
            sock.sendall(estado_juego.encode('utf-8'))
            print("Estado del juego enviado al servidor.")

            # Pausa antes de enviar el próximo estado
            time.sleep(30)

# Ejecutar la función del cliente jugador
if __name__ == "__main__":
    jugador_cliente()
