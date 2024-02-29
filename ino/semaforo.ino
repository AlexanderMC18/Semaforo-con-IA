// Definición de pines para la primera vía
const int pinRojo1 = 11;
const int pinAmarillo1 = 12;
const int pinVerde1 = 13;

// Definición de pines para la segunda vía
const int pinRojo2 = 2;
const int pinAmarillo2 = 3;
const int pinVerde2 = 4;

// Definición de pines para la tercera vía
const int pinRojo3 = 5;
const int pinAmarillo3 = 6;
const int pinVerde3 = 7;

// Definición de pines para la cuarta vía
const int pinRojo4 = 8;
const int pinAmarillo4 = 9;
const int pinVerde4 = 10;

void setup() {
  // Configuración de los pines como salidas
  pinMode(pinRojo1, OUTPUT);
  pinMode(pinAmarillo1, OUTPUT);
  pinMode(pinVerde1, OUTPUT);

  pinMode(pinRojo2, OUTPUT);
  pinMode(pinAmarillo2, OUTPUT);
  pinMode(pinVerde2, OUTPUT);

  pinMode(pinRojo3, OUTPUT);
  pinMode(pinAmarillo3, OUTPUT);
  pinMode(pinVerde3, OUTPUT);

  pinMode(pinRojo4, OUTPUT);
  pinMode(pinAmarillo4, OUTPUT);
  pinMode(pinVerde4, OUTPUT);

  Serial.begin(115200);
  Serial.println("Conexión Serial Establecida");
}

void loop() {
  for (int i = 2; i < 14; i = i + 1) { // Garantizo que todo esté apagado
    digitalWrite(i, LOW);
  }

  if (Serial.available() > 0) {
    char command = Serial.read();
    Serial.println("Comando recibido: " + String(command)); // Línea de depuración

    if (command == '1') {
      // Estado rojo
      digitalWrite(pinVerde1, HIGH); // enciendo el primer verde
      digitalWrite(pinRojo2, HIGH);  // Enciendo los demás rojos
      digitalWrite(pinRojo3, HIGH);
      digitalWrite(pinRojo4, HIGH);
      delay(2000); // espero

      digitalWrite(pinAmarillo1, HIGH); // enciendo el amarillo
      delay(500); // espero
      digitalWrite(pinVerde1, LOW); // apagar el verde y el amarillo
      digitalWrite(pinAmarillo1, LOW);
      digitalWrite(pinRojo1, HIGH);

    } else if (command == '2') {
      digitalWrite(pinVerde2, HIGH); // segundo semáforo verde
      digitalWrite(pinRojo2, LOW);  // apago el rojo 2
      delay(2000); // espero
      digitalWrite(pinAmarillo2, HIGH); // encender el 2do amarillo
      delay(500); // espero
      digitalWrite(pinVerde2, LOW); // apago ambos
      digitalWrite(pinAmarillo2, LOW);
      digitalWrite(pinRojo2, HIGH); // enciendo el rojo

    } else if (command == '3') {
      // Estado verde
      digitalWrite(pinVerde3, HIGH); // tercer semáforo verde
      digitalWrite(pinRojo3, LOW);    // apago el rojo 3
      delay(2000); // espero
      digitalWrite(pinAmarillo3, HIGH); // encender el 3erdo amarillo
      delay(500); // espero
      digitalWrite(pinAmarillo3, LOW); // apago ambos
      digitalWrite(pinVerde3, LOW);
      digitalWrite(pinRojo3, HIGH);

    } else if (command == '4') {
      digitalWrite(pinVerde4, HIGH); // cuarto semáforo verde
      digitalWrite(pinRojo4, LOW);
      delay(2000); // espero
      digitalWrite(pinAmarillo4, HIGH); // encender el 4to amarillo
      delay(500); // espero
      digitalWrite(pinVerde4, LOW); // apago ambos
      digitalWrite(pinAmarillo4, LOW);
      digitalWrite(pinRojo4, HIGH);
    } else {
      command = 0; // Restablecer a 0 solo cuando el comando no es '1', '2', '3' ni '4'
    }
  }
    digitalWrite(pinVerde1, HIGH); // enciendo el primer verde
    digitalWrite(pinRojo2, HIGH);  // Enciendo los demás rojos
    digitalWrite(pinRojo3, HIGH);
    digitalWrite(pinRojo4, HIGH);
    delay(2000); // espero

    digitalWrite(pinAmarillo1, HIGH); // enciendo el amarillo
    delay(500); // espero
    digitalWrite(pinVerde1, LOW); // apagar el verde y el amarillo
    digitalWrite(pinAmarillo1, LOW);
    digitalWrite(pinRojo1, HIGH);

    digitalWrite(pinVerde2, HIGH); // segundo semáforo verde
    digitalWrite(pinRojo2, LOW);  // apago el rojo 2
    delay(2000); // espero
    digitalWrite(pinAmarillo2, HIGH); // encender el 2do amarillo
    delay(500); // espero
    digitalWrite(pinVerde2, LOW); // apago ambos
    digitalWrite(pinAmarillo2, LOW);
    digitalWrite(pinRojo2, HIGH); // enciendo el rojo

    digitalWrite(pinVerde3, HIGH); // tercer semáforo verde
    digitalWrite(pinRojo3, LOW); // apago el rojo 3
    delay(2000); // espero
    digitalWrite(pinAmarillo3, HIGH); // encender el 3erdo amarillo
    delay(500); // espero
    digitalWrite(pinAmarillo3, LOW); // apago ambos
    digitalWrite(pinVerde3, LOW);
    digitalWrite(pinRojo3, HIGH);

    digitalWrite(pinVerde4, HIGH); // cuarto semáforo verde
    digitalWrite(pinRojo4, LOW);
    delay(2000); // espero
    digitalWrite(pinAmarillo4, HIGH); // encender el 4to amarillo
    delay(500); // espero
    digitalWrite(pinVerde4, LOW); // apago ambos
    digitalWrite(pinAmarillo4, LOW);
    digitalWrite(pinRojo4, HIGH);
}
