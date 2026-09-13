# 🌱 GreenTracker

GreenTracker è un sistema di monitoraggio intelligente per l'umidità del terreno basato su NodeMCU ESP8266. Il progetto utilizza un doppio display OLED per fornire un feedback visivo immediato sullo stato della pianta e sul sistema, ed è dotato di connettività WiFi per predisporre il sensore all'invio dei dati in rete.

## 🚀 Novità della Versione 2.0
* **Connettività WiFi:** Il sistema si connette alla rete domestica.
* **Doppio Schermo Indipendente:** Gestione di due display OLED cloni in modo totalmente indipendente tramite bus I2C hardware e software.
* **UI Migliorata:** Interfaccia grafica moderna utilizzando font Sans-Serif (Helvetica).

## 🛠️ Componenti Hardware
* 1x Scheda NodeMCU ESP8266
* 2x Moduli Display OLED 0.91" (128x32) I2C
* 1x Sensore Capacitivo di Umidità del Terreno (Analogico)
* Breadboard e cavetti Jumper

## 🔌 Schema Elettrico

Tutti i componenti condividono l'alimentazione dai pin **3V3** e **GND** del NodeMCU.

| Componente | Pin NodeMCU | Funzione |
| :--- | :--- | :--- |
| **Display 1 (Sensore)** | `D1` / `D2` | SCK / SDA (Hardware I2C) |
| **Display 2 (Sistema)** | `D5` / `D6` | SCK / SDA (Software I2C) |
| **Sensore Capacitivo** | `A0` | Lettura Analogica |

## 💻 Installazione e Configurazione

1. Clona questo repository in locale usando il terminale:
   `git clone https://github.com/EddiZor/GreenTracker.git`

2. Assicurati di aver installato le librerie `U8g2` e `ESP8266WiFi` nell'IDE di Arduino (tramite Gestore Librerie).

3. **⚠️ CONFIGURAZIONE WIFI:** Prima di caricare lo sketch, apri il file `GreenTracker.ino` e sostituisci gli asterischi `***` con il nome (SSID) e la password esatti della tua rete domestica:
   `const char* ssid = "***";`
   `const char* password = "***";`

4. Carica il firmware sulla scheda, impostando il Monitor Seriale a `115200 baud` per leggere l'indirizzo IP assegnato durante l'avvio.

## 🎛️ Calibrazione
Per ottenere una lettura percentuale (0-100%) il più accurata possibile con il tuo terreno specifico, testa il sensore da completamente asciutto e poi immerso in un bicchiere d'acqua. 
Aggiorna di conseguenza i parametri `valoreAriaSecca` e `valoreAcqua` che trovi in cima al codice.