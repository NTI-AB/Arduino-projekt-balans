#include <Adafruit_LIS3DH.h> // LIS3DH-bibliotek
#include <Adafruit_Sensor.h> // Sensorbibliotek
#include <Servo.h>           // Servobibliotek

// LIS3DH-objekt
Adafruit_LIS3DH lis = Adafruit_LIS3DH();

// Servovariabler
Servo servoX;
Servo servoY;
const int servoXPin = 8;
const int servoYPin = 9;

// Servogränser
const int minServoAngle = 0;
const int maxServoAngle = 179;

// Array för att stabilisera X- och Y-axelns vinklar
const int smoothingLength = 10;
int xAngleHistory[smoothingLength] = {0};
int yAngleHistory[smoothingLength] = {0};
int xIndex = 0;
int yIndex = 0;

// Variabler för att hålla summan av vinklarna (för att snabbare räkna medelvärdet)
int xSum = 0;
int ySum = 0;

// Variabel för att hålla reda på om historiken är full
bool isHistoryFull = false;

void setup() {
  // Seriell kommunikation
  Serial.begin(9600);

  // Initiera LIS3DH
  if (!lis.begin(0x18)) {
    Serial.println("Kunde inte hitta LIS3DH. Kontrollera anslutningar.");
    while (1);
  }
  lis.setRange(LIS3DH_RANGE_2_G);

  // Initiera servos
  servoX.attach(servoXPin);
  servoY.attach(servoYPin);
  servoX.write(90);
  servoY.write(90);
}

int calculateSmoothedAngle(int newAngle, int* angleHistory, int& sum, int& index, bool& isHistoryFull) {
  // Ta bort det äldsta värdet från summan om historiken är full
  if (isHistoryFull) {
    sum -= angleHistory[index];
  }

  // Lägg till det nya värdet till arrayen och summan
  angleHistory[index] = newAngle;
  sum += newAngle;

  // Flytta index framåt (cirkulärt)
  index++;
  if (index >= smoothingLength) {
    index = 0;
    isHistoryFull = true; // Efter första rundan är historiken full
  }

  // Beräkna medelvärdet
  if (isHistoryFull) {
    return sum / smoothingLength;
  } else {
    return 90; // Servon hålls i mittläge tills historiken är full
  }
}

void loop() {
  sensors_event_t event;

  // Läs accelerometerdata
  if (!lis.getEvent(&event)) {
    Serial.println("Fel vid läsning av accelerometerdata!");
    delay(500);
    return;
  }

  // Beräkna vinklar
  float angleX = atan2(event.acceleration.y, event.acceleration.z) * 180.0 / PI;
  float angleY = atan2(event.acceleration.x, event.acceleration.z) * 180.0 / PI;

  // Omvandla vinklar till servovänliga värden
  int servoXAngle = map(angleX, -90, 90, minServoAngle, maxServoAngle);
  int servoYAngle = map(angleY, -90, 90, minServoAngle, maxServoAngle);

  // Begränsa vinklarna
  servoXAngle = constrain(servoXAngle, minServoAngle, maxServoAngle);
  servoYAngle = constrain(servoYAngle, minServoAngle, maxServoAngle);

  // Stabilisering med medelvärdesuträkning
  int smoothedX = calculateSmoothedAngle(servoXAngle, xAngleHistory, xSum, xIndex, isHistoryFull);
  int smoothedY = calculateSmoothedAngle(servoYAngle, yAngleHistory, ySum, yIndex, isHistoryFull);

  // Ställ in servon endast om historiken är full
  if (isHistoryFull) {
    servoX.write(smoothedX);
    servoY.write(smoothedY);
  }

  // Debugging
  Serial.print("Rå X: ");
  Serial.print(servoXAngle);
  Serial.print(" Stabiliserad X: ");
  Serial.print(smoothedX);
  Serial.print(" | Rå Y: ");
  Serial.print(servoYAngle);
  Serial.print(" Stabiliserad Y: ");
  Serial.println(smoothedY);

  delay(100);
}
