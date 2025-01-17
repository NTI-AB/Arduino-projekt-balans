// Arduino-kod för att balansera en sak med hjälp av en accelerometer och servomotor.

#include <Servo.h> // Inkluderar bibliotek för att använda servomotorer

// Konstanter och variabler för accelerometer
const int xPin = A0; // Analog pin för att läsa X-axeldata från accelerometer
const int yPin = A1; // Analog pin för att läsa Y-axeldata från accelerometer
const int zPin = A2; // Analog pin för att läsa Z-axeldata från accelerometer

// Konstanter och variabler för servomotorer
Servo servoX; // Servomotor för X-axeln
Servo servoY; // Servomotor för Y-axeln
const int servoXPin = 9; // Pin för att styra X-axelns servomotor
const int servoYPin = 10; // Pin för att styra Y-axelns servomotor

// Justeringsparametrar
const int minServoAngle = 0;   // Minsta vinkel servomotorn kan röra sig till
const int maxServoAngle = 180; // Största vinkel servomotorn kan röra sig till

void setup() {
  // Initiera seriell kommunikation för debugging
  Serial.begin(9600);

  // Initiera servomotorerna och sätt dem till mittläge
  servoX.attach(servoXPin);
  servoY.attach(servoYPin);
  servoX.write(90); // Mittläge för X-axeln
  servoY.write(90); // Mittläge för Y-axeln
}

void loop() {
  // Läs accelerometerdata från respektive axel
  int xVal = analogRead(xPin);
  int yVal = analogRead(yPin);
  int zVal = analogRead(zPin);

  // Debugging: Skriv ut rådata till seriell monitor
  Serial.print("X: ");
  Serial.print(xVal);
  Serial.print(" Y: ");
  Serial.print(yVal);
  Serial.print(" Z: ");
  Serial.println(zVal);

  // Beräkna servovinklar baserat på accelerometerdata
  int servoXAngle = map(xVal, 0, 1023, minServoAngle, maxServoAngle);
  int servoYAngle = map(yVal, 0, 1023, minServoAngle, maxServoAngle);

  // Begränsa vinklarna för säkerhet
  servoXAngle = constrain(servoXAngle, minServoAngle, maxServoAngle);
  servoYAngle = constrain(servoYAngle, minServoAngle, maxServoAngle);

  // Ställ in servovinklar
  servoX.write(servoXAngle);
  servoY.write(servoYAngle);

  // Kort paus för stabilitet
  delay(100);
}
