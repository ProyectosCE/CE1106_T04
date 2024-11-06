import socket
import json

def espectador_cliente():
    # Configuración del servidor
    host = '192.168.100.6'  # Cambia esto si el servidor está en otra máquina
    port = 12345

    # Crear socket y conectar al servidor
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as sock:
        sock.connect((host, port))

        # Enviar mensaje de tipoCliente para registrarse como "spectador"
        tipo_cliente = json.dumps({
            "command": "tipoCliente",
            "tipoCliente": "spectador"
        })
        sock.sendall(tipo_cliente.encode('utf-8'))
        
        # Esperar respuesta con lista de jugadores
        data = sock.recv(1024).decode('utf-8')
        jugadores_disponibles = json.loads(data)
        print("Jugadores disponibles para observar:", jugadores_disponibles["data"])

         # Elegir el jugador a observar
        jugador_id = input("Ingresa el ID del jugador que deseas observar: ")
        observar_comando = json.dumps({
            "command": "GameSpectator",
            "jugadorId": jugador_id
        })
        sock.sendall(observar_comando.encode('utf-8'))


        # Esperar y mostrar el estado del jugador observado en un bucle
        while True:
            estado_juego = sock.recv(1024).decode('utf-8')
            if estado_juego:
                try:
                    estado = json.loads(estado_juego)
                    print("Estado del jugador observado:", json.dumps(estado, indent=4))
                except json.JSONDecodeError:
                    print("Error al decodificar el estado del juego recibido.")
            else:
                print("El servidor ha cerrado la conexión.")
                break

# Ejecutar la función del cliente espectador
if __name__ == "__main__":
    espectador_cliente()
