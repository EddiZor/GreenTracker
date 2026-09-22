/**
 * Progetto: GreenTracker
 * Versione: 5.0 (Live Dashboard & LDR Ready)
 * Autore: SKYSIM PILOT
 */

#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include <time.h> // Nuova libreria per l'orario

// --- Credenziali WiFi ---
const char* ssid = "IL_TUO_WIFI";       
const char* password = "LA_TUA_PASSWORD";   

// --- Credenziali Pushover ---
const char* pushoverApiToken = "INSERISCI_QUI_IL_TOKEN_APP";
const char* pushoverUserKey = "INSERISCI_QUI_IL_USER_KEY";

ESP8266WebServer server(80);

const int pin_S0 = D3;
const int pin_S1 = D4;
const int pin_S2 = D7;
const int pin_S3 = D8;
const int pin_SIG = A0;

const int MAX_SENSORI = 16; 
int valoriSensori[16]; // Può essere umidità o luce
bool sensoreConnesso[16]; 
bool allarmeInviato[16]; 

const int valoreAriaSecca = 850; 
const int valoreAcqua = 350;

U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C display1(U8G2_R0, D1, D2, U8X8_PIN_NONE);
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C display2(U8G2_R0, D5, D6, U8X8_PIN_NONE);

void attesaIntelligente(unsigned long ms) {
  unsigned long inizio = millis();
  while (millis() - inizio < ms) {
    server.handleClient(); 
    delay(10);
  }
}

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
  // Lettura Orario Corrente
  time_t now = time(nullptr);
  struct tm* timeinfo = localtime(&now);
  String orarioAggiornamento = "Sincronizzazione in corso...";
  if (timeinfo->tm_year > 120) { // Se l'orario è stato scaricato correttamente (anno > 2020)
    char timeString[10];
    sprintf(timeString, "%02d:%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    orarioAggiornamento = String(timeString);
  }

  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0, user-scalable=no'>";
  html += "<meta name='apple-mobile-web-app-capable' content='yes'>";
  html += "<meta http-equiv='refresh' content='30'>"; // AUTO-REFRESH ogni 30 secondi
  html += "<title>GreenTracker Live</title>";
  html += "<style>";
  html += "body { font-family: 'Segoe UI', sans-serif; background-color: #121212; color: #fff; margin: 0; padding: 15px; text-align: center; }";
  html += "h1 { color: #4CAF50; font-size: 1.8em; margin-bottom: 10px; }";
  html += ".card { background-color: #1e1e1e; border-radius: 10px; padding: 15px; margin: 15px auto; max-width: 400px; box-shadow: 0 4px 8px rgba(0,0,0,0.4); text-align: left; }";
  html += ".card-header { display: flex; justify-content: space-between; align-items: center; margin-bottom: 8px; }";
  html += ".card-title { font-size: 1.1em; font-weight: bold; color: #eee; }";
  html += ".card-value { font-size: 1.3em; font-weight: bold; }";
  
  // Stili per la barra di riempimento
  html += ".progress-bg { background-color: #333; border-radius: 5px; height: 10px; width: 100%; overflow: hidden; }";
  html += ".progress-bar { height: 100%; border-radius: 5px; transition: width 0.5s; }";
  html += ".bg-green { background-color: #4CAF50; color: #4CAF50; }";
  html += ".bg-yellow { background-color: #FFC107; color: #FFC107; }";
  html += ".bg-red { background-color: #F44336; color: #F44336; }";
  
  // Stile speciale per la card della Luce
  html += ".card-luce { border-left: 4px solid #FFC107; }";
  html += ".card-pianta { border-left: 4px solid #4CAF50; }";
  
  html += ".footer { color: #888; font-size: 0.85em; margin-top: 30px; line-height: 1.5; }";
  html += "</style></head><body>";
  
  html += "<h1>🌿 GreenTracker</h1>";
  
  bool almenoUnSensore = false;
  
  // CICLO SENSORI PIANTE (Porte C0 - C14)
  for (int i = 0; i < 15; i++) {
    if (sensoreConnesso[i]) {
      almenoUnSensore = true;
      int umidita = valoriSensori[i];
      
      String coloreClasse = "bg-green";
      if (umidita <= 20) coloreClasse = "bg-red";
      else if (umidita <= 40) coloreClasse = "bg-yellow";
      
      html += "<div class='card card-pianta'>";
      html += "<div class='card-header'>";
      html += "<span class='card-title'>🌱 Sensore " + String(i + 1) + " (C" + String(i) + ")</span>";
      html += "<span class='card-value " + coloreClasse + "'>" + String(umidita) + "%</span>";
      html += "</div>";
      html += "<div class='progress-bg'><div class='progress-bar " + coloreClasse + "' style='width: " + String(umidita) + "%;'></div></div>";
      html += "</div>";
    }
  }

  // SEZIONE SENSORE LUCE (Esclusiva per la Porta C15)
  if (sensoreConnesso[15]) {
      almenoUnSensore = true;
      int luce = valoriSensori[15];
      html += "<div class='card card-luce'>";
      html += "<div class='card-header'>";
      html += "<span class='card-title'>☀️ Esposizione Luce</span>";
      html += "<span class='card-value bg-yellow'>" + String(luce) + "%</span>";
      html += "</div>";
      html += "<div class='progress-bg'><div class='progress-bar bg-yellow' style='width: " + String(luce) + "%;'></div></div>";
      html += "</div>";
  }
  
  if (!almenoUnSensore) {
    html += "<p style='color: #aaa; margin-top:30px;'>Nessun sensore rilevato.</p>";
  }
  
  html += "<div class='footer'>Ultimo aggiornamento: <b>" + orarioAggiornamento + "</b><br>Rete WiFi: " + String(ssid) + "</div>";
  html += "</body></html>";
  
  server.send(200, "text/html", html);
}

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
  
  while (WiFi.status() != WL_CONNECTED) { delay(1000); }
  
  // Sincronizzazione Orario (Fuso orario Roma/Italia)
  configTime("CET-1CEST,M3.5.0,M10.5.0/3", "pool.ntp.org");
  
  server.on("/", gestisciPaginaPrincipale); 
  server.begin();
  
  display2.clearBuffer();
  display2.drawStr(0, 13, "WiFi: Connesso!");
  display2.sendBuffer();
  
  display1.clearBuffer();
  display1.setFont(u8g2_font_helvB12_tr);
  display1.drawStr(0, 13, "Sistema OK");
  display1.sendBuffer();

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
      
      if (i == 15) { 
        // Logica Sensore Luce (Mappatura temporanea 0-1023)
        // La calibreremo precisamente quando collegherai l'LDR
        int perc = map(valoreGrezzo, 0, 1023, 0, 100);
        valoriSensori[i] = constrain(perc, 0, 100);
      } else {
        // Logica Sensori Umidità
        int perc = map(valoreGrezzo, valoreAriaSecca, valoreAcqua, 0, 100);
        valoriSensori[i] = constrain(perc, 0, 100);

        if (valoriSensori[i] < 20) {
          if (!allarmeInviato[i]) {
            String msg = "💧 Sensore " + String(i + 1) + " ha sete! Umidità: " + String(valoriSensori[i]) + "%";
            inviaNotificaPushover(msg);
            allarmeInviato[i] = true; 
          }
        } else if (valoriSensori[i] > 25) {
          allarmeInviato[i] = false;
        }
      }
    }
  }

  // FASE 2: Carosello Display
  bool almenoUnSensore = false;

  for (int i = 0; i < MAX_SENSORI; i++) {
    if (!sensoreConnesso[i]) continue; 
    almenoUnSensore = true; 

    display1.clearBuffer();
    display1.setFont(u8g2_font_helvB12_tr); 
    
    if (i == 15) {
      display1.drawStr(0, 13, "Sens. Luce"); 
      display1.drawStr(0, 31, "Esposiz.:");
    } else {
      display1.drawStr(0, 13, ("Sens. " + String(i + 1)).c_str()); 
      display1.drawStr(0, 31, "Umid.:"); 
    }
     
    display1.setFont(u8g2_font_helvB14_tr); 
    String percTesto = String(valoriSensori[i]) + "%";
    display1.setCursor(128 - display1.getStrWidth(percTesto.c_str()), 31); 
    display1.print(percTesto);
    display1.sendBuffer(); 

    display2.clearBuffer();
    display2.setFont(u8g2_font_helvB10_tr); 
    display2.drawStr(0, 13, WiFi.localIP().toString().c_str());
    
    time_t now = time(nullptr);
    struct tm* timeinfo = localtime(&now);
    if (timeinfo->tm_year > 120) {
      char timeStr[10];
      sprintf(timeStr, "%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min);
      display2.setFont(u8g2_font_helvB12_tr);
      display2.drawStr(0, 31, timeStr); 
    } else {
      display2.drawStr(0, 31, "WiFi: OK"); 
    }
    display2.sendBuffer(); 

    attesaIntelligente(3000); 
  }

  if (!almenoUnSensore) {
    display1.clearBuffer();
    display1.setFont(u8g2_font_helvB12_tr);
    display1.drawStr(0, 13, "Nessun sensore");
    display1.sendBuffer();
    attesaIntelligente(2000);
  }
}