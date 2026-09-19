/**
 * Progetto: GreenTracker
 * Versione: 3.0
 * Autore: SKYSIM PILOT
 * Descrizione: Sistema IoT di monitoraggio umidità del suolo a 16 canali con rilevamento 
 *              Plug & Play, display OLED I2C multipli e notifiche push via Pushover.
 */

#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>

// --- Credenziali WiFi ---
const char* ssid = "IL_TUO_WIFI";       
const char* password = "LA_TUA_PASSWORD";   

// --- Credenziali Pushover ---
const char* pushoverApiToken = "INSERISCI_QUI_IL_TOKEN_APP";
const char* pushoverUserKey = "INSERISCI_QUI_IL_USER_KEY";

// --- Configurazione Pin Multiplexer (CD74HC4067 / HW-178) ---
const int pin_S0 = D3;
const int pin_S1 = D4;
const int pin_S2 = D7;
const int pin_S3 = D8;
const int pin_SIG = A0;

// --- Configurazione Sensori ---
const int MAX_SENSORI = 16; 
int umiditaPiante[16]; 
bool sensoreConnesso[16]; 
bool allarmeInviato[16]; // Memoria per logica anti-spam

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
  
  delay(10); 
  return analogRead(pin_SIG);
}

// --- FUNZIONE: Invia notifica Pushover ---
void inviaNotificaPushover(String messaggio) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Errore: WiFi non connesso. Impossibile inviare Pushover.");
    return;
  }

  WiFiClientSecure client;
  client.setInsecure(); // Ignora la validazione del certificato SSL per risparmiare memoria RAM

  HTTPClient http;
  http.begin(client, "https://api.pushover.net/1/messages.json");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  String postData = "token=" + String(pushoverApiToken) +
                    "&user=" + String(pushoverUserKey) +
                    "&message=" + messaggio;

  Serial.println("Invio notifica Pushover...");
  int httpCode = http.POST(postData);
  
  if (httpCode > 0) {
    Serial.printf("Pushover inviato con successo! Codice: %d\n", httpCode);
  } else {
    Serial.printf("Errore invio Pushover: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();
}

void setup() {
  Serial.begin(115200);

  pinMode(pin_S0, OUTPUT);
  pinMode(pin_S1, OUTPUT);
  pinMode(pin_S2, OUTPUT);
  pinMode(pin_S3, OUTPUT);
  pinMode(pin_SIG, INPUT);

  // Azzera la memoria degli allarmi all'avvio
  for(int i = 0; i < MAX_SENSORI; i++) {
    allarmeInviato[i] = false;
  }

  display1.begin();
  display2.begin();

  // --- Avvio e connessione WiFi ---
  display2.clearBuffer();
  display2.setFont(u8g2_font_helvB12_tr);
  display2.drawStr(0, 13, "GreenTracker");
  display2.drawStr(0, 31, "Connessione...");
  display2.sendBuffer();

  // --- PULIZIA E FORZATURA MODULO WIFI ---
  WiFi.mode(WIFI_STA); 
  WiFi.disconnect();   
  delay(100);          

  WiFi.begin(ssid, password);
  
  Serial.print("\nConnessione WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
    Serial.print(WiFi.status()); 
  }
  
  display2.clearBuffer();
  display2.drawStr(0, 13, "GreenTracker");
  display2.drawStr(0, 31, "WiFi: Connesso!");
  display2.sendBuffer();
  
  display1.clearBuffer();
  display1.setFont(u8g2_font_helvB12_tr);
  display1.drawStr(0, 13, "Sistema OK");
  display1.drawStr(0, 31, "Avvio notifiche");
  display1.sendBuffer();

  // --- NOTIFICA DI AVVIO ---
  inviaNotificaPushover("✅ GreenTracker v3.0 Avviato e Connesso alla rete!");

  delay(3000); 
}

void loop() {
  // FASE 1: Lettura di tutti i 16 canali e logica allarmi
  Serial.println("--- Scansione Sensori ---");
  for (int i = 0; i < MAX_SENSORI; i++) {
    int valoreGrezzo = leggiCanaleMux(i);
    
    if (valoreGrezzo < 50) {
      sensoreConnesso[i] = false;
      allarmeInviato[i] = false; // Se stacco il sensore, resetto l'allarme
    } else {
      sensoreConnesso[i] = true;
      int perc = map(valoreGrezzo, valoreAriaSecca, valoreAcqua, 0, 100);
      umiditaPiante[i] = constrain(perc, 0, 100);
      
      Serial.print("Sens. ");
      Serial.print(i + 1);
      Serial.print(" (Porta C");
      Serial.print(i);
      Serial.print("): Raw=");
      Serial.print(valoreGrezzo);
      Serial.print(" -> ");
      Serial.print(umiditaPiante[i]);
      Serial.println("%");

      // --- LOGICA ALLARME PUSHOVER ---
      if (umiditaPiante[i] < 20) {
        if (!allarmeInviato[i]) {
          String msg = "💧 ATTENZIONE: Il Sens. " + String(i + 1) + " (Porta C" + String(i) + ") ha sete! Umidità al " + String(umiditaPiante[i]) + "%";
          inviaNotificaPushover(msg);
          allarmeInviato[i] = true; 
        }
      } else if (umiditaPiante[i] > 25) {
        // Riarmo allarme quando annaffiata
        allarmeInviato[i] = false;
      }
    }
  }

  // FASE 2: Carosello sui Display (salta le porte vuote)
  bool almenoUnSensore = false;

  for (int i = 0; i < MAX_SENSORI; i++) {
    if (!sensoreConnesso[i]) continue; 
    
    almenoUnSensore = true; 

    // -- Aggiornamento Display 1 (Dati Pianta) --
    display1.clearBuffer();
    display1.setFont(u8g2_font_helvB12_tr); 
    
    String nomeDaMostrare = "Sens. " + String(i + 1);
    
    display1.drawStr(0, 13, nomeDaMostrare.c_str()); 
    display1.drawStr(0, 31, "Umid.:"); 
    
    display1.setFont(u8g2_font_helvB14_tr); 
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

    delay(3000); // Mostra ogni sensore connesso per 3 secondi
  }

  if (!almenoUnSensore) {
    display1.clearBuffer();
    display1.setFont(u8g2_font_helvB12_tr);
    display1.drawStr(0, 13, "Nessun sensore");
    display1.drawStr(0, 31, "collegato...");
    display1.sendBuffer();
    delay(2000);
  }
}