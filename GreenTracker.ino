/**
 * Progetto: GreenTracker
 * Versione: 4.0 (Web App Edition)
 * Autore: SKYSIM PILOT
 * Descrizione: Sistema IoT umidità suolo a 16 canali con rilevamento Plug & Play, 
 *              display OLED, notifiche Pushover e Server Web integrato.
 */

#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>

// --- Credenziali WiFi ---
const char* ssid = "IL_TUO_WIFI";       
const char* password = "LA_TUA_PASSWORD";   

// --- Credenziali Pushover ---
const char* pushoverApiToken = "INSERISCI_QUI_IL_TOKEN_APP";
const char* pushoverUserKey = "INSERISCI_QUI_IL_USER_KEY";

// --- Inizializzazione Server Web sulla porta 80 ---
ESP8266WebServer server(80);

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
bool allarmeInviato[16]; 

const int valoreAriaSecca = 850; 
const int valoreAcqua = 350;

// --- Inizializzazione Display ---
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C display1(U8G2_R0, /* clock=*/ D1, /* data=*/ D2, /* reset=*/ U8X8_PIN_NONE);
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C display2(U8G2_R0, /* clock=*/ D5, /* data=*/ D6, /* reset=*/ U8X8_PIN_NONE);

// --- FUNZIONE: Ritardo Intelligente (Non blocca il server Web) ---
void attesaIntelligente(unsigned long ms) {
  unsigned long inizio = millis();
  while (millis() - inizio < ms) {
    server.handleClient(); // Ascolta le richieste web durante l'attesa
    delay(10);
  }
}

// --- FUNZIONE: Legge canale Multiplexer ---
int leggiCanaleMux(int canale) {
  digitalWrite(pin_S0, bitRead(canale, 0));
  digitalWrite(pin_S1, bitRead(canale, 1));
  digitalWrite(pin_S2, bitRead(canale, 2));
  digitalWrite(pin_S3, bitRead(canale, 3));
  delay(10); 
  return analogRead(pin_SIG);
}

// --- FUNZIONE: Genera la pagina HTML della Web App ---
void gestisciPaginaPrincipale() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'>"; // Risolve la codifica degli Emoji
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0, user-scalable=no'>";
  html += "<meta name='apple-mobile-web-app-capable' content='yes'>";
  html += "<title>GreenTracker Dashboard</title>";
  html += "<style>";
  html += "body { font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif; background-color: #121212; color: #ffffff; margin: 0; padding: 20px; text-align: center; }";
  html += "h1 { color: #4CAF50; font-size: 2em; margin-bottom: 20px; }";
  html += ".card { background-color: #1e1e1e; border-radius: 12px; padding: 20px; margin: 15px auto; max-width: 400px; box-shadow: 0 4px 10px rgba(0,0,0,0.3); text-align: left; border-left: 6px solid #4CAF50; display: flex; justify-content: space-between; align-items: center; }";
  html += ".card-title { font-size: 1.2em; font-weight: bold; color: #ddd; }";
  html += ".card-value { font-size: 1.6em; color: #4CAF50; font-weight: bold; }";
  html += ".alert { border-left-color: #F44336; } .alert .card-value { color: #F44336; }";
  html += ".footer { color: #777; font-size: 0.9em; margin-top: 40px; }";
  html += "button { background-color: #4CAF50; color: white; border: none; padding: 10px 20px; border-radius: 8px; font-size: 1em; cursor: pointer; margin-top: 20px; }";
  html += "</style></head><body>";
  
  html += "<h1>🌿 GreenTracker</h1>";
  
  bool almenoUnSensore = false;
  for (int i = 0; i < MAX_SENSORI; i++) {
    if (sensoreConnesso[i]) {
      almenoUnSensore = true;
      String alertClass = (umiditaPiante[i] < 20) ? " alert" : "";
      
      html += "<div class='card" + alertClass + "'>";
      html += "<span class='card-title'>Sensore " + String(i + 1) + " (Porta C" + String(i) + ")</span>";
      html += "<span class='card-value'>" + String(umiditaPiante[i]) + "%</span>";
      html += "</div>";
    }
  }
  
  if (!almenoUnSensore) {
    html += "<p style='color: #aaa;'>Nessun sensore rilevato. Controlla i collegamenti.</p>";
  }
  
  html += "<button onclick='location.reload()'>🔄 Aggiorna Dati</button>";
  html += "<div class='footer'>Rete WiFi: " + String(ssid) + "</div>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

// --- FUNZIONE: Invia notifica Pushover ---
void inviaNotificaPushover(String messaggio) {
  if (WiFi.status() != WL_CONNECTED) return;
  WiFiClientSecure client;
  client.setInsecure(); 
  HTTPClient http;
  http.begin(client, "https://api.pushover.net/1/messages.json");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String postData = "token=" + String(pushoverApiToken) + "&user=" + String(pushoverUserKey) + "&message=" + messaggio;
  http.POST(postData);
  http.end();
}

void setup() {
  Serial.begin(115200);

  pinMode(pin_S0, OUTPUT);
  pinMode(pin_S1, OUTPUT);
  pinMode(pin_S2, OUTPUT);
  pinMode(pin_S3, OUTPUT);
  pinMode(pin_SIG, INPUT);

  for(int i = 0; i < MAX_SENSORI; i++) {
    allarmeInviato[i] = false;
  }

  display1.begin();
  display2.begin();

  display2.clearBuffer();
  display2.setFont(u8g2_font_helvB12_tr);
  display2.drawStr(0, 13, "GreenTracker");
  display2.drawStr(0, 31, "Connessione...");
  display2.sendBuffer();

  WiFi.mode(WIFI_STA); 
  WiFi.disconnect();   
  delay(100);          
  WiFi.begin(ssid, password);
  
  Serial.print("\nConnessione WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  // --- Avvio del Server Web ---
  server.on("/", gestisciPaginaPrincipale); 
  server.begin();
  
  Serial.println("\n\n✅ WiFi Connesso!");
  Serial.print("🌐 INDIRIZZO IP DELLA WEB APP: ");
  Serial.println(WiFi.localIP()); 

  display2.clearBuffer();
  display2.drawStr(0, 13, "WiFi: Connesso!");
  display2.sendBuffer();
  
  display1.clearBuffer();
  display1.setFont(u8g2_font_helvB12_tr);
  display1.drawStr(0, 13, "Sistema OK");
  display1.drawStr(0, 31, "Avvio notifiche");
  display1.sendBuffer();

  inviaNotificaPushover("✅ GreenTracker WebApp Avviata! IP: " + WiFi.localIP().toString());

  attesaIntelligente(3000); 
}

void loop() {
  server.handleClient(); 

  // FASE 1: Lettura Sensori
  for (int i = 0; i < MAX_SENSORI; i++) {
    int valoreGrezzo = leggiCanaleMux(i);
    if (valoreGrezzo < 50) {
      sensoreConnesso[i] = false;
      allarmeInviato[i] = false; 
    } else {
      sensoreConnesso[i] = true;
      int perc = map(valoreGrezzo, valoreAriaSecca, valoreAcqua, 0, 100);
      umiditaPiante[i] = constrain(perc, 0, 100);

      if (umiditaPiante[i] < 20) {
        if (!allarmeInviato[i]) {
          String msg = "💧 ATTENZIONE: Sens. " + String(i + 1) + " ha sete! Umidità: " + String(umiditaPiante[i]) + "%";
          inviaNotificaPushover(msg);
          allarmeInviato[i] = true; 
        }
      } else if (umiditaPiante[i] > 25) {
        allarmeInviato[i] = false;
      }
    }
  }

  // FASE 2: Carosello Display (usando l'attesa intelligente)
  bool almenoUnSensore = false;

  for (int i = 0; i < MAX_SENSORI; i++) {
    if (!sensoreConnesso[i]) continue; 
    almenoUnSensore = true; 

    // Aggiorna Display 1
    display1.clearBuffer();
    display1.setFont(u8g2_font_helvB12_tr); 
    display1.drawStr(0, 13, ("Sens. " + String(i + 1)).c_str()); 
    display1.drawStr(0, 31, "Umid.:"); 
    display1.setFont(u8g2_font_helvB14_tr); 
    String percTesto = String(umiditaPiante[i]) + "%";
    display1.setCursor(128 - display1.getStrWidth(percTesto.c_str()), 31); 
    display1.print(percTesto);
    display1.sendBuffer(); 

    // Aggiorna Display 2 (Mostra anche l'IP)
    display2.clearBuffer();
    display2.setFont(u8g2_font_helvB10_tr); 
    display2.drawStr(0, 13, WiFi.localIP().toString().c_str());
    display2.setFont(u8g2_font_helvB12_tr);
    display2.drawStr(0, 31, "WiFi: OK"); 
    display2.sendBuffer(); 

    attesaIntelligente(3000); 
  }

  if (!almenoUnSensore) {
    display1.clearBuffer();
    display1.setFont(u8g2_font_helvB12_tr);
    display1.drawStr(0, 13, "Nessun sensore");
    display1.drawStr(0, 31, "collegato...");
    display1.sendBuffer();
    attesaIntelligente(2000);
  }
}