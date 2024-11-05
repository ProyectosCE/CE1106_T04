import socket
import json

# Función para iniciar el servidor de pruebas
def start_test_server():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind(('0.0.0.0', 12345))  # Puerto 12345
    server_socket.listen(5)

    print("Servidor de pruebas escuchando en el puerto 12345...")

    while True:
        client_socket, addr = server_socket.accept()
        print(f"Cliente conectado desde {addr}")

        # Recibir datos del cliente
        data = client_socket.recv(1024).decode('utf-8')
        print(f"Datos recibidos: {data}")

        # Procesar el mensaje como JSON
        try:
            message = json.loads(data)
            print(f"Mensaje procesado: {message}")

            # Responder al cliente con un JSON
            response = {
                "command": "response",
                "message": f"Hola cliente, el servidor recibió: {message['message']}"
            }

            # Convertir la respuesta en JSON y enviarla al cliente
            response_json = json.dumps(response)
            client_socket.sendall(response_json.encode('utf-8'))
            print(f"Respuesta enviada: {response_json}")

        except json.JSONDecodeError:
            print("Error al decodificar el JSON")

        # Cerrar la conexión con el cliente
        # client_socket.close()
        # print(f"Conexión cerrada con el cliente {addr}")

if __name__ == "__main__":
    start_test_server()
