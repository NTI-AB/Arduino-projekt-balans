# Balanserande Plattform med Arduino

## Beskrivning
Detta projekt syftar till att skapa en balanserande plattform med hjälp av en Arduino, en accelerometer (LIS3DH) och två servon. Plattformen justerar sin lutning automatiskt baserat på sensoravläsningar för att hålla sig stabil.

[![Demonstration](https://imgur.com/a/JFdZtVj)](https://youtube.com/shorts/fYFBIF4i_Ew?feature=share)


## Funktioner
- Använder en accelerometer för att mäta plattformens lutning i X- och Y-led.
- Justerar två servon för att balansera plattformen.
- Implementerar medelvärdesutjämning för att minska brus och förbättra stabilitet.
- Möjlighet att utöka med LCD-skärm eller trådlös kommunikation.

## Komponenter
- **Arduino Uno (eller kompatibel mikrokontroller)**
- **Adafruit LIS3DH accelerometer**
- **2x SG90 servon**
- **Strömkälla (5V)**
- **Kablage och kopplingsdäck**
- **Plattform och monteringsmaterial**

## Installation och Användning
### 1. Hårdvaruinstallation
1. Anslut LIS3DH-accelerometern till Arduino enligt databladets specifikationer.
2. Koppla servona till digitala pinnar 8 och 9 på Arduino.
3. Förse systemet med tillräcklig strömförsörjning.
4. Montera plattformen på servomekanismen.

### 2. Programinstallation
1. Klona detta repository:
   ```sh
   git clone https://github.com/ditt-användarnamn/balanserande-plattform.git
   ```
2. Öppna `balanserande_plattform.ino` i Arduino IDE.
3. Installera nödvändiga bibliotek via Library Manager:
   - `Adafruit_LIS3DH`
   - `Adafruit_Sensor`
   - `Servo`
4. Kompilera och ladda upp koden till din Arduino.
5. Öppna seriell monitor för att övervaka sensorvärden och servovinklar.

## Kodöversikt
```cpp
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
#include <Servo.h>

// Skapa objekt för accelerometer och servon
Adafruit_LIS3DH lis = Adafruit_LIS3DH();
Servo servoX, servoY;

void setup() {
  Serial.begin(9600);
  lis.begin(0x18);
  servoX.attach(8);
  servoY.attach(9);
}

void loop() {
  sensors_event_t event;
  lis.getEvent(&event);
  // Beräkna och justera servovinklar baserat på lutning
}
```

## Utökningar
- **PID-reglering** för bättre kontroll.
- **OLED/LCD-skärm** för realtidsvisning av data.
- **Trådlös kommunikation** via Bluetooth eller Wi-Fi.

## Licens
Asså de här ett skolprojekt jag ha ingen aning

