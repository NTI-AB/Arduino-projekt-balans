// Program som använder en LIS3DH-accelerometer för att styra två servon
// Servona stabiliseras genom att använda ett glidande medelvärde

// Bibliotek
#include <Adafruit_LIS3DH.h> // LIS3DH-bibliotek
#include <Adafruit_Sensor.h> // Sensorbibliotek
#include <Servo.h>           // Servobibliotek

// LIS3DH-objekt
Adafruit_LIS3DH lis = Adafruit_LIS3DH();

// Servovariabler
Servo servoX;
Servo servoY;
const int servoXPin = 8; // Pin för X-servo
const int servoYPin = 9; // Pin för Y-servo

// Servogränser
const int minServoAngle = 0;
const int maxServoAngle = 179;

// Array för att stabilisera X- och Y-axelns vinklar
const int smoothingLength = 10; // Antal värden för glidande medelvärde
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
  // Starta seriell kommunikation
  Serial.begin(9600);

  // Initiera LIS3DH-accelerometern
  if (!lis.begin(0x18)) {
    Serial.println("Kunde inte hitta LIS3DH. Kontrollera anslutningar.");
    while (1); // Stoppa programmet om sensorn inte hittas
  }
  lis.setRange(LIS3DH_RANGE_2_G); // Ställ in känslighet

  // Initiera servon
  servoX.attach(servoXPin);
  servoY.attach(servoYPin);
  servoX.write(90); // Starta i neutralläge
  servoY.write(90);
}

/*
  Funktion för att beräkna ett glidande medelvärde av en vinkel
  
  Parametrar:
    - newAngle: Den nya uppmätta vinkeln
    - angleHistory: Array som lagrar tidigare vinklar
    - sum: Referens till summan av lagrade vinklar
    - index: Referens till index i arrayen
    - isHistoryFull: Referens till flagga om historiken är full
  
  Returnerar:
    - Medelvärdet av de senaste värdena, eller 90 om historiken inte är full
*/
int calculateSmoothedAngle(int newAngle, int* angleHistory, int& sum, int& index, bool& isHistoryFull) {
  if (isHistoryFull) {
    sum -= angleHistory[index]; // Ta bort det äldsta värdet från summan
  }

  angleHistory[index] = newAngle; // Lägg till det nya värdet i historiken
  sum += newAngle; // Uppdatera summan

  index++; // Flytta index framåt
  if (index >= smoothingLength) {
    index = 0;
    isHistoryFull = true; // Efter första rundan är historiken full
  }

  return isHistoryFull ? sum / smoothingLength : 90; // Returnera medelvärde eller neutralläge
}

void loop() {
  sensors_event_t event;

  // Läs accelerometerdata
  if (!lis.getEvent(&event)) {
    Serial.println("Fel vid läsning av accelerometerdata!");
    delay(500);
    return;
  }

  // Beräkna vinklar baserat på accelerometerdata
  float angleX = atan2(event.acceleration.y, event.acceleration.z) * 180.0 / PI;
  float angleY = atan2(event.acceleration.x, event.acceleration.z) * 180.0 / PI;

  // Omvandla vinklar till servovänliga värden
  int servoXAngle = map(angleX, -90, 90, minServoAngle, maxServoAngle);
  int servoYAngle = map(angleY, -90, 90, minServoAngle, maxServoAngle);

  // Begränsa vinklarna inom servonas rörelseområde
  servoXAngle = constrain(servoXAngle, minServoAngle, maxServoAngle);
  servoYAngle = constrain(servoYAngle, minServoAngle, maxServoAngle);

  // Stabilisering genom glidande medelvärde
  int smoothedX = calculateSmoothedAngle(servoXAngle, xAngleHistory, xSum, xIndex, isHistoryFull);
  int smoothedY = calculateSmoothedAngle(servoYAngle, yAngleHistory, ySum, yIndex, isHistoryFull);

  // Uppdatera servon först när historiken är full
  if (isHistoryFull) {
    servoX.write(smoothedX);
    servoY.write(smoothedY);
  }

  // Debugging-utskrift
  Serial.print("Rå X: ");
  Serial.print(servoXAngle);
  Serial.print(" Stabiliserad X: ");
  Serial.print(smoothedX);
  Serial.print(" | Rå Y: ");
  Serial.print(servoYAngle);
  Serial.print(" Stabiliserad Y: ");
  Serial.println(smoothedY);

  delay(50); // Vänta innan nästa läsning
}
