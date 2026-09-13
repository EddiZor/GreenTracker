#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <ESP8266WiFi.h>

// --- Credenziali WiFi ---
const char* ssid = "***";       // INSERISCI IL NOME DELLA TUA RETE PRIMA DI CARICARE
const char* password = "***";   // INSERISCI LA PASSWORD PRIMA DI CARICARE

// --- Configurazione Pin e Sensore ---
const int sensorPin = A0; 
const int valoreAriaSecca = 850; 
const int valoreAcqua = 350;

// --- Inizializzazione Display (0.91" 128x32 OLED) ---
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C display1(U8G2_R0, /* clock=*/ D1, /* data=*/ D2, /* reset=*/ U8X8_PIN_NONE);
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C display2(U8G2_R0, /* clock=*/ D5, /* data=*/ D6, /* reset=*/ U8X8_PIN_NONE);

void setup() {
  Serial.begin(115200);

  display1.begin();
  display2.begin();

  // --- Connessione WiFi ---
  display2.clearBuffer();
  display2.setFont(u8g2_font_helvB12_tr);
  display2.drawStr(0, 13, "GreenTracker");
  display2.drawStr(0, 31, "Connessione...");
  display2.sendBuffer();

  WiFi.begin(ssid, password);
  
  Serial.print("\nConnessione al WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi connesso!");
  Serial.print("Indirizzo IP: ");
  Serial.println(WiFi.localIP());

  // --- Schermata di conferma per 3 secondi ---
  display2.clearBuffer();
  display2.setFont(u8g2_font_helvB12_tr);
  display2.drawStr(0, 13, "GreenTracker");
  display2.drawStr(0, 31, "WiFi: Connesso!");
  display2.sendBuffer();
  
  display1.clearBuffer();
  display1.setFont(u8g2_font_helvB12_tr);
  display1.drawStr(0, 13, "Rete OK");
  display1.drawStr(0, 31, "Avvio sensore...");
  display1.sendBuffer();

  delay(3000); // Pausa di 3 secondi
}

void loop() {
  // Lettura sensore capacitivo
  int sensorValue = analogRead(sensorPin);
  
  // Calcolo percentuale
  int umiditaPerc = map(sensorValue, valoreAriaSecca, valoreAcqua, 0, 100);
  umiditaPerc = constrain(umiditaPerc, 0, 100);

  // --- Aggiornamento Display 1 (Dati) ---
  display1.clearBuffer();
  display1.setFont(u8g2_font_helvB12_tr); 
  display1.drawStr(0, 13, "Terr."); 
  display1.drawStr(0, 31, "Umid.:"); 
  
  display1.setFont(u8g2_font_helvB18_tr);
  String percTesto = String(umiditaPerc) + "%";
  int larghezzaTesto = display1.getStrWidth(percTesto.c_str());
  int posX = 128 - larghezzaTesto; 
  
  display1.setCursor(posX, 32); 
  display1.print(percTesto);
  display1.sendBuffer(); 

  // --- Aggiornamento Display 2 (Stato) ---
  display2.clearBuffer();
  display2.setFont(u8g2_font_helvB12_tr);
  display2.drawStr(0, 13, "GreenTracker");
  
  if(WiFi.status() == WL_CONNECTED){
     display2.drawStr(0, 31, "WiFi: OK"); 
  } else {
     display2.drawStr(0, 31, "WiFi: Perso"); 
  }
  display2.sendBuffer(); 

  Serial.print("Valore Grezzo: ");
  Serial.print(sensorValue);
  Serial.print(" | Umidità: ");
  Serial.print(umiditaPerc);
  Serial.println("%");

  delay(2000); 
}