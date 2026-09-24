#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>

// Definizione Pin Multiplexer
const int pin_S0 = D3;
const int pin_S1 = D4;
const int pin_S2 = D7;
const int pin_S3 = D8;
const int pin_SIG = A0;

// Configurazione Display (SCK, SDA)
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C display1(U8G2_R0, D1, D2, U8X8_PIN_NONE);
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C display2(U8G2_R0, D5, D6, U8X8_PIN_NONE);

// Funzione Scanner I2C
void scansionaBusI2C(int pinSDA, int pinSCL, String nomePorta) {
  Serial.print("🔍 Controllo cablaggi su ");
  Serial.print(nomePorta);
  Serial.println("...");

  Wire.begin(pinSDA, pinSCL);
  byte errore, indirizzo;
  int dispositiviTrovati = 0;

  for(indirizzo = 1; indirizzo < 127; indirizzo++ ) {
    Wire.beginTransmission(indirizzo);
    errore = Wire.endTransmission();
    
    if (errore == 0) {
      Serial.print("   ✅ TROVATO DISPLAY all'indirizzo: 0x");
      if (indirizzo < 16) Serial.print("0");
      Serial.println(indirizzo, HEX);
      dispositiviTrovati++;
    } 
  }
  
  if (dispositiviTrovati == 0) {
    Serial.println("   ❌ NESSUN DISPOSITIVO TROVATO. Controllare i cavi.");
  }
  Serial.println("-----------------------------------");
}

void setup() {
  Serial.begin(115200);
  delay(2000); // Attesa per aprire il monitor seriale
  
  Serial.println("\n\n🛠️ INIZIO TEST HARDWARE GREENTRACKER 🛠️\n");

  // TEST 1: Scansione elettrica delle piste I2C
  scansionaBusI2C(D2, D1, "PRESA OLED 1 (Pin D1/D2)"); // SDA=D2, SCK=D1
  scansionaBusI2C(D6, D5, "PRESA OLED 2 (Pin D5/D6)"); // SDA=D6, SCK=D5

  // TEST 2: Accensione Display
  Serial.println("Accensione Display 1...");
  display1.begin();
  display1.clearBuffer();
  display1.setFont(u8g2_font_helvB12_tr);
  display1.drawStr(5, 20, "OLED 1: TEST OK");
  display1.sendBuffer();

  Serial.println("Accensione Display 2...");
  display2.begin();
  display2.clearBuffer();
  display2.setFont(u8g2_font_helvB12_tr);
  display2.drawStr(5, 20, "OLED 2: TEST OK");
  display2.sendBuffer();

  // Configurazione Pin Multiplexer per il loop
  pinMode(pin_S0, OUTPUT);
  pinMode(pin_S1, OUTPUT);
  pinMode(pin_S2, OUTPUT);
  pinMode(pin_S3, OUTPUT);
  pinMode(pin_SIG, INPUT);
}

void loop() {
  Serial.println("\n📡 Lettura in tempo reale dei canali Multiplexer:");
  
  for (int i = 0; i < 16; i++) {
    digitalWrite(pin_S0, bitRead(i, 0));
    digitalWrite(pin_S1, bitRead(i, 1));
    digitalWrite(pin_S2, bitRead(i, 2));
    digitalWrite(pin_S3, bitRead(i, 3));
    delay(10);
    int valore = analogRead(pin_SIG);

    Serial.print("   C");
    if (i < 10) Serial.print("0");
    Serial.print(i);
    Serial.print(": \t[");
    Serial.print(valore);
    Serial.println("]");
  }
  
  delay(5000); // Ripete la lettura ogni 5 secondi
}