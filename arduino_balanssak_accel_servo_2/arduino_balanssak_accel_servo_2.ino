#include <Adafruit_LIS3DH.h> // Inkludera biblioteket för LIS3DH
#include <Adafruit_Sensor.h> // Behövs för sensorer
#include <Servo.h> // Inkludera bibliotek för att använda servomotorer

// Skapa ett LIS3DH-objekt
Adafruit_LIS3DH lis = Adafruit_LIS3DH();

// Konstanter och variabler för servomotorer
Servo servoX; // Servomotor för X-axeln
Servo servoY; // Servomotor för Y-axeln
const int servoXPin = 8; // Pin för att styra X-axelns servomotor
const int servoYPin = 9; // Pin för att styra Y-axelns servomotor

// Justeringsparametrar
const int minServoAngle = 0;   // Minsta vinkel servomotorn kan röra sig till
const int maxServoAngle = 179; // Största vinkel servomotorn kan röra sig till

void setup() {
  // Initiera seriell kommunikation för debugging
  Serial.begin(9600);

  // Initiera LIS3DH
  if (!lis.begin(0x18)) { // Standard I2C-adress för LIS3DH
    Serial.println("Kunde inte hitta LIS3DH. Kontrollera anslutningar.");
    while (1);
  }

  Serial.println("LIS3DH initierad!");
  lis.setRange(LIS3DH_RANGE_2_G); // Ställ in mätområde (2G, 4G, 8G, 16G)

  // Initiera servomotorerna och sätt dem till mittläge
  servoX.attach(servoXPin);
  servoY.attach(servoYPin);
  servoX.write(90); // Mittläge för X-axeln
  servoY.write(90); // Mittläge för Y-axeln
}

void loop() {
  // Läs accelerometerdata
  sensors_event_t event;
  lis.getEvent(&event);

  // Beräkna vinklar baserat på accelerometerdata
  float angleX = atan2(event.acceleration.y, event.acceleration.z) * 180.0 / PI; // X-vinkel (i grader)
  float angleY = atan2(event.acceleration.x, event.acceleration.z) * 180.0 / PI; // Y-vinkel (i grader)

  // Omvandla vinklar till servovänliga värden (mellan min och max vinkel)
  int servoXAngle = map(angleX, -90, 90, minServoAngle, maxServoAngle);
  int servoYAngle = map(angleY, -90, 90, minServoAngle, maxServoAngle);

  // Begränsa vinklarna för säkerhet
  servoXAngle = constrain(servoXAngle, minServoAngle, maxServoAngle);
  servoYAngle = constrain(servoYAngle, minServoAngle, maxServoAngle);

  // Debugging: Skriv ut de beräknade vinklarna till seriell monitor
  Serial.print("X Vinkel: ");
  Serial.print(servoXAngle);
  Serial.print(" Y Vinkel: ");
  Serial.println(servoYAngle);

  // Ställ in servovinklar
  servoX.write(servoXAngle);
  servoY.write(servoYAngle);

  // Kort paus för stabilitet
  delay(100);
}
