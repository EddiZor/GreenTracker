#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <ESP8266WiFi.h>

// --- Credenziali WiFi ---
const char* ssid = "***";       
const char* password = "***";   

// --- Configurazione Pin Multiplexer (CD74HC4067) ---
const int pin_S0 = D3;
const int pin_S1 = D4;
const int pin_S2 = D7;
const int pin_S3 = D8;
const int pin_SIG = A0;

// --- Configurazione Sensori e Piante ---
const int NUM_SENSORI = 3;  // Imposta quanti sensori sono fisicamente collegati

// Nomi delle piante (C0 = Ficus, C1 = Spathiphyllum, C2 = Limone, ecc.)
String nomiPiante[] = {"Ficus", "Spathiphyllum", "Limone", "Pianta 4", "Pianta 5"}; 
int umiditaPiante[16]; // Array dove il sistema salverà le letture

// Calibrazione base (valida per tutti i canali)
const int valoreAriaSecca = 850; 
const int valoreAcqua = 350;

// --- Inizializzazione Display (0.91" 128x32 OLED) ---
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C display1(U8G2_R0, /* clock=*/ D1, /* data=*/ D2, /* reset=*/ U8X8_PIN_NONE);
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C display2(U8G2_R0, /* clock=*/ D5, /* data=*/ D6, /* reset=*/ U8X8_PIN_NONE);


// --- FUNZIONE: Legge uno specifico canale del Multiplexer ---
int leggiCanaleMux(int canale) {
  digitalWrite(pin_S0, bitRead(canale, 0));
  digitalWrite(pin_S1, bitRead(canale, 1));
  digitalWrite(pin_S2, bitRead(canale, 2));
  digitalWrite(pin_S3, bitRead(canale, 3));
  
  delay(10); // Pausa per far stabilizzare il segnale
  return analogRead(pin_SIG);
}


void setup() {
  Serial.begin(115200);

  // Imposta i pin del Multiplexer
  pinMode(pin_S0, OUTPUT);
  pinMode(pin_S1, OUTPUT);
  pinMode(pin_S2, OUTPUT);
  pinMode(pin_S3, OUTPUT);
  pinMode(pin_SIG, INPUT);

  display1.begin();
  display2.begin();

  // --- Avvio e connessione WiFi ---
  display2.clearBuffer();
  display2.setFont(u8g2_font_helvB12_tr);
  display2.drawStr(0, 13, "GreenTracker");
  display2.drawStr(0, 31, "Connessione...");
  display2.sendBuffer();

  WiFi.begin(ssid, password);
  
  Serial.print("\nConnessione WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  display2.clearBuffer();
  display2.drawStr(0, 13, "GreenTracker");
  display2.drawStr(0, 31, "WiFi: Connesso!");
  display2.sendBuffer();
  
  display1.clearBuffer();
  display1.setFont(u8g2_font_helvB12_tr);
  display1.drawStr(0, 13, "Multiplexer OK");
  display1.drawStr(0, 31, "Lettura sensori.");
  display1.sendBuffer();

  delay(3000); 
}


void loop() {
  // FASE 1: Lettura di tutti i sensori connessi
  Serial.println("--- Nuova Lettura ---");
  for (int i = 0; i < NUM_SENSORI; i++) {
    int valoreGrezzo = leggiCanaleMux(i);
    int perc = map(valoreGrezzo, valoreAriaSecca, valoreAcqua, 0, 100);
    umiditaPiante[i] = constrain(perc, 0, 100);
    
    Serial.print(nomiPiante[i]);
    Serial.print(" (C");
    Serial.print(i);
    Serial.print("): ");
    Serial.print(umiditaPiante[i]);
    Serial.println("%");
  }

  // FASE 2: Carosello sui Display
  for (int i = 0; i < NUM_SENSORI; i++) {
    
    // -- Aggiornamento Display 1 (Dati Pianta) --
    display1.clearBuffer();
    display1.setFont(u8g2_font_helvB12_tr); 
    display1.drawStr(0, 13, nomiPiante[i].c_str()); 
    display1.drawStr(0, 31, "Umid.:"); 
    
    display1.setFont(u8g2_font_helvB14_tr); // Font ridotto per allineamento perfetto
    String percTesto = String(umiditaPiante[i]) + "%";
    int larghezzaTesto = display1.getStrWidth(percTesto.c_str());
    int posX = 128 - larghezzaTesto; 
    
    display1.setCursor(posX, 31); 
    display1.print(percTesto);
    display1.sendBuffer(); 

    // -- Aggiornamento Display 2 (Stato Generale) --
    display2.clearBuffer();
    display2.setFont(u8g2_font_helvB12_tr);
    display2.drawStr(0, 13, "GreenTracker");
    if(WiFi.status() == WL_CONNECTED){
       display2.drawStr(0, 31, "WiFi: OK"); 
    } else {
       display2.drawStr(0, 31, "WiFi: Perso"); 
    }
    display2.sendBuffer(); 

    delay(3000); // Mostra ogni pianta per 3 secondi
  }
}