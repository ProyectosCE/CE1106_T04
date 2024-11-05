import socket
import json

# Datos del servidor
HOST = '127.0.0.1'  # Dirección IP del servidor
PORT = 12345        # Puerto en el que está escuchando el servidor

# Mensaje de conexión inicial en formato JSON
mensaje_inicial = {
    "comando": "hola",
    "msg": "Cliente Python conectado"
}

# Convertir el mensaje a JSON
mensaje_json = json.dumps(mensaje_inicial)

try:
    # Crear un socket TCP/IP
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        # Conectar con el servidor
        s.connect((HOST, PORT))
        print(f"Conectado al servidor en {HOST}:{PORT}")

        # Enviar el mensaje de conexión inicial
        s.sendall(mensaje_json.encode('utf-8'))
        print(f"Mensaje enviado: {mensaje_json}")

        # Recibir respuesta del servidor
        respuesta = s.recv(1024).decode('utf-8')
        print(f"Respuesta del servidor: {respuesta}")

except ConnectionRefusedError:
    print("Error: No se pudo conectar al servidor.")
except Exception as e:
    print(f"Error durante la comunicación: {e}")
