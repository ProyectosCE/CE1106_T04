import cv2
import mediapipe as mp
import pyautogui

# Inicializar mediapipe para la detección de manos
mp_drawing = mp.solutions.drawing_utils
mp_hands = mp.solutions.hands

# Inicializar la cámara
cap = cv2.VideoCapture(0)

# Variables para monitorear el estado de cada tecla
teclas_estado = {'a': False, 'd': False, 'w': False}

# Definir si un dedo está abajo (en una posición inferior)
def dedo_abajo(y_punta, y_base):
    return y_punta > y_base

with mp_hands.Hands(min_detection_confidence=0.7, min_tracking_confidence=0.7) as hands:
    while cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            break

        # Voltear el frame y obtener dimensiones
        frame = cv2.flip(frame, 1)
        height, width, _ = frame.shape

        # Convertir a RGB y procesar con mediapipe
        frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
        result = hands.process(frame_rgb)

        if result.multi_hand_landmarks:
            for hand_landmarks in result.multi_hand_landmarks:
                # Obtener la punta y la base del dedo índice
                y_indice = int(hand_landmarks.landmark[mp_hands.HandLandmark.INDEX_FINGER_TIP].y * height)
                y_indice_base = int(hand_landmarks.landmark[mp_hands.HandLandmark.INDEX_FINGER_PIP].y * height)

                # Obtener la punta y la base del dedo medio
                y_medio = int(hand_landmarks.landmark[mp_hands.HandLandmark.MIDDLE_FINGER_TIP].y * height)
                y_medio_base = int(hand_landmarks.landmark[mp_hands.HandLandmark.MIDDLE_FINGER_PIP].y * height)

                # Obtener la punta y la base del dedo meñique
                y_menique = int(hand_landmarks.landmark[mp_hands.HandLandmark.PINKY_TIP].y * height)
                y_menique_base = int(hand_landmarks.landmark[mp_hands.HandLandmark.PINKY_PIP].y * height)

                # Dibujar la ubicación de la punta de los dedos
                cv2.circle(frame, (int(hand_landmarks.landmark[mp_hands.HandLandmark.INDEX_FINGER_TIP].x * width), y_indice), 10, (0, 255, 0), -1)
                cv2.circle(frame, (int(hand_landmarks.landmark[mp_hands.HandLandmark.MIDDLE_FINGER_TIP].x * width), y_medio), 10, (255, 0, 0), -1)
                cv2.circle(frame, (int(hand_landmarks.landmark[mp_hands.HandLandmark.PINKY_TIP].x * width), y_menique), 10, (0, 0, 255), -1)

                # Verificar si el índice está abajo (tecla 'A')
                if dedo_abajo(y_indice, y_indice_base):
                    if not teclas_estado['a']:
                        pyautogui.keyDown('a')
                        teclas_estado['a'] = True
                else:
                    if teclas_estado['a']:
                        pyautogui.keyUp('a')
                        teclas_estado['a'] = False

                # Verificar si el medio está abajo (tecla 'D')
                if dedo_abajo(y_medio, y_medio_base):
                    if not teclas_estado['d']:
                        pyautogui.keyDown('d')
                        teclas_estado['d'] = True
                else:
                    if teclas_estado['d']:
                        pyautogui.keyUp('d')
                        teclas_estado['d'] = False

                # Verificar si el meñique está abajo (tecla 'W')
                if dedo_abajo(y_menique, y_menique_base):
                    if not teclas_estado['w']:
                        pyautogui.keyDown('w')
                        teclas_estado['w'] = True
                else:
                    if teclas_estado['w']:
                        pyautogui.keyUp('w')
                        teclas_estado['w'] = False

                # Dibujar las manos
                mp_drawing.draw_landmarks(frame, hand_landmarks, mp_hands.HAND_CONNECTIONS)

        # Mostrar el video en vivo
        cv2.imshow("Control con Mano", frame)

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

# Liberar todos los recursos y cerrar ventanas
cap.release()
cv2.destroyAllWindows()
