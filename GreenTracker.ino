#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32 // Nota: i display da 0.91" sono solitamente 128x32
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int sensorPin = A0;

// Valori di calibrazione tipici per il sensore capacitivo
const int dryVal = 600;   
const int wetVal = 300;   

void setup() {
  Serial.begin(9600);

  // Inizializzazione OLED 128x32 (Indirizzo 0x3C)
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 fallito"));
    for(;;);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
}

void loop() {
  int sensorValue = analogRead(sensorPin);
  
  // Calcolo percentuale
  int moisturePercent = map(sensorValue, dryVal, wetVal, 0, 100);
  moisturePercent = constrain(moisturePercent, 0, 100);

  // Aggiornamento Display OLED 0.91" (layout compatto in altezza)
  display.clearDisplay();

  // Riga 1: Titolo
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(F("GREEN TRACKER"));

  // Riga 2: Valore percentuale grande
  display.setTextSize(2);
  display.setCursor(0, 12);
  display.print(moisturePercent);
  display.print(F("%"));

  // Stato testuale a destra
  display.setTextSize(1);
  display.setCursor(70, 16);
  if (moisturePercent < 30) {
    display.print(F("SECCO!"));
  } else if (moisturePercent >= 30 && moisturePercent < 65) {
    display.print(F("OTTIMO"));
  } else {
    display.print(F("UMIDO"));
  }

  display.display();
  delay(2000);
}