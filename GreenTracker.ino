#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

// --- Configurazione Pin ---
const int sensorPin = A0; // Pin analogico per il sensore capacitivo

// Valori di calibrazione del sensore (da testare e aggiustare in base al tuo terreno)
// Inserisci il sensore in aria secca per il primo valore, e in un bicchiere d'acqua per il secondo
const int valoreAriaSecca = 850; 
const int valoreAcqua = 350;

// --- Inizializzazione Display (0.91" 128x32 OLED) ---
// Display 1: Software I2C sui pin D1 (Clock) e D2 (Data)
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C display1(U8G2_R0, /* clock=*/ D1, /* data=*/ D2, /* reset=*/ U8X8_PIN_NONE);

// Display 2: Software I2C sui pin D5 (Clock) e D6 (Data)
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C display2(U8G2_R0, /* clock=*/ D5, /* data=*/ D6, /* reset=*/ U8X8_PIN_NONE);

void setup() {
  Serial.begin(115200);

  // Inizializzazione dei due display
  display1.begin();
  display2.begin();
}

void loop() {
  // 1. Lettura del sensore capacitivo
  int sensorValue = analogRead(sensorPin);
  
  // 2. Calcolo della percentuale di umidità
  int umiditaPerc = map(sensorValue, valoreAriaSecca, valoreAcqua, 0, 100);
  umiditaPerc = constrain(umiditaPerc, 0, 100);

  // --- 3. Aggiornamento Display 1 (Lettura Sensore) ---
  display1.clearBuffer();
  
  // Etichette a sinistra (font Helvetica Sans Serif medio)
  display1.setFont(u8g2_font_helvB12_tr); 
  display1.drawStr(0, 13, "Terr."); 
  display1.drawStr(0, 31, "Umid.:"); 
  
  // Valore percentuale ingrandito e spostato a destra
  display1.setFont(u8g2_font_helvB18_tr); // Font Helvetica Sans Serif grande (18 pixel)
  
  // Creiamo la stringa con il numero e il simbolo %
  String percTesto = String(umiditaPerc) + "%";
  
  // Calcoliamo la larghezza del testo in pixel per allinearlo a destra
  int larghezzaTesto = display1.getStrWidth(percTesto.c_str());
  int posX = 128 - larghezzaTesto; // 128 è la larghezza massima del display
  
  // Stampiamo il valore a destra. Y = 32 per appoggiarlo sul fondo
  display1.setCursor(posX, 32); 
  display1.print(percTesto);
  
  display1.sendBuffer(); 

  // --- 4. Aggiornamento Display 2 (Stato) ---
  display2.clearBuffer();
  
  display2.setFont(u8g2_font_helvB12_tr);
  display2.drawStr(0, 13, "GreenTracker");
  display2.drawStr(0, 31, "Stato: OK"); 
  display2.sendBuffer(); 

  // Stampa anche sul Monitor Seriale per comodità di debug
  Serial.print("Valore Grezzo: ");
  Serial.print(sensorValue);
  Serial.print(" | Umidità: ");
  Serial.print(umiditaPerc);
  Serial.println("%");

  delay(2000); // Pausa di 2 secondi prima della prossima lettura
}