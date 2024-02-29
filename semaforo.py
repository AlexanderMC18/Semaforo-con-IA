import cv2
import torch
import numpy as np
import matplotlib.path as mplPath
import serial
import time

# Coordenadas originales para ZONE_1, ZONE_2, ZONE_3, y ZONE_4
ZONE_1 = np.array([
    [1500, 393],
    [750, 600],  # Ajusté la coordenada para hacer la "V" más grande y larga
    [1000, 1000],  # Ajusté la coordenada para hacer la "V" más grande y larga
    [1500, 700],
])

ZONE_2 = np.array([
    [817, 800],  # Ajusté la coordenada para hacer la "V" más grande y larga
    [80, 1000],  # Ajusté la coordenada para hacer la "V" más grande y larga
    [80, 150],
    [500, 254],
])

ZONE_3 = np.array([
    [1500, 393],
    [750, 600],  # Ajusté la coordenada para hacer la "V" más grande y larga
    [1000, 1000],  # Ajusté la coordenada para hacer la "V" más grande y larga
    [1500, 700],
])

ZONE_4 = np.array([
    [817, 800],  # Ajusté la coordenada para hacer la "V" más grande y larga
    [80, 1000],  # Ajusté la coordenada para hacer la "V" más grande y larga
    [80, 150],
    [500, 254],
])

def get_center(bbox):
    center = ((bbox[0] + bbox[2]) // 2, (bbox[1] + bbox[3]) // 2)
    return center

def load_model():
    model = torch.hub.load("ultralytics/yolov5", model="yolov5n", pretrained=True)
    return model

def get_bboxes(preds):
    df = preds.pandas().xyxy[0]
    df = df[df["confidence"] >= 0.5]
    df = df[df["name"] == "car"]
    return df[["xmin", "ymin", "xmax", "ymax"]].values.astype(int)

def get_zone(xc, yc):
    zones = [ZONE_1, ZONE_2, ZONE_3, ZONE_4]
    for i, zone in enumerate(zones, start=1):
        if mplPath.Path(zone).contains_point((xc, yc)):
            return i
    return 0  # No pertenece a ninguna zona

def enviar_comando(ser, zona):
    command = '1' if zona == 1 else '2' if zona == 2 else '3' if zona == 3 else '4'
    print(f"Enviando comando a Arduino: {command}")
    ser.write(command.encode())
    time.sleep(0.1)  # Esperar para asegurar la recepción del comando


def detector(cap1, cap2):
    # Configurar la comunicación serial
    SERIAL_PORT = '/dev/cu.usbmodem101'  # Ajusta el puerto serial según tu configuración
    ser = serial.Serial(SERIAL_PORT, 9600, timeout=1)

    model = load_model()

    while cap1.isOpened() or cap2.isOpened():
        status1, frame1 = cap1.read()
        status2, frame2 = cap2.read()

        if not status1 and not status2:
            break

        if status1:
            preds1 = model(frame1)
            bboxes1 = get_bboxes(preds1)
            detections1 = [0, 0]

            for box in bboxes1:
                xc, yc = get_center(box)
                zone = get_zone(xc, yc)

                if zone == 1 or zone == 2:
                    detections1[zone-1] += 1

                cv2.circle(img=frame1, center=(xc, yc), radius=5, color=(0, 255, 0), thickness=-1)
                cv2.rectangle(img=frame1, pt1=(box[0], box[1]), pt2=(box[2], box[3]), color=(255, 0, 0), thickness=1)

            for i, count in enumerate(detections1, start=1):
                cv2.putText(img=frame1, text=f"Cam 1 - Zone {i}: {count}", org=(100, 100 + 50*i), fontFace=cv2.FONT_HERSHEY_PLAIN, fontScale=2, color=(0, 0, 0), thickness=2)

            # Enviar comando solo si se detectan carros
            if any(detections1):
                max_zone = np.argmax(detections1) + 1
                enviar_comando(ser, max_zone)

            for zone in [ZONE_1, ZONE_2]:
                cv2.polylines(img=frame1, pts=[zone], isClosed=True, color=(0, 0, 255), thickness=4)

            cv2.imshow("Cam 1", frame1)

        if status2:
            preds2 = model(frame2)
            bboxes2 = get_bboxes(preds2)
            detections2 = [0, 0]

            for box in bboxes2:
                xc, yc = get_center(box)
                zone = get_zone(xc, yc)

                if zone == 3 or zone == 4:
                    detections2[zone-3] += 1

                cv2.circle(img=frame2, center=(xc, yc), radius=5, color=(0, 255, 0), thickness=-1)
                cv2.rectangle(img=frame2, pt1=(box[0], box[1]), pt2=(box[2], box[3]), color=(255, 0, 0), thickness=1)

            for i, count in enumerate(detections2, start=3):
                cv2.putText(img=frame2, text=f"Cam 2 - Zone {i}: {count}", org=(100, 100 + 50*i), fontFace=cv2.FONT_HERSHEY_PLAIN, fontScale=2, color=(0, 0, 0), thickness=2)

            # Enviar comando solo si se detectan carros
            if any(detections2):
                max_zone = np.argmax(detections2) + 3
                enviar_comando(ser, max_zone)

            for zone in [ZONE_3, ZONE_4]:
                cv2.polylines(img=frame2, pts=[zone], isClosed=True, color=(0, 0, 255), thickness=4)

            cv2.imshow("Cam 2", frame2)

        if cv2.waitKey(10) & 0xFF == 27:
            break

    cap1.release()
    cap2.release()

if __name__ == '__main__':
    cap1 = cv2.VideoCapture(0)  # Cambia el argumento a 0 para usar la cámara predeterminada
    cap2 = cv2.VideoCapture(1)  # Cambia el argumento a 1 o el número de la segunda cámara disponible

    detector(cap1, cap2)

    # Cerrar la comunicación serial al finalizar
    ser.close()
