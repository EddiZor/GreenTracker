# GreenTracker 🌿

**GreenTracker** è un monitor compatto di umidità del suolo per piante d'appartamento e talee, basato su microcontrollore **NodeMCU (ESP8266)**, sensore capacitivo e display **OLED 0.91" I2C**.

Il dispositivo effettua letture analogiche periodiche, converte il valore tramite calibrazione personalizzata in percentuale (0–100%) e visualizza lo stato idrico del substrato in tempo reale.

---

## 🛠️ Componenti Hardware

- **Microcontrollore:** NodeMCU 1.0 (ESP-12E Module)
- **Sensore:** Sensore di umidità capacitivo del suolo (Capacitive Soil Moisture Sensor v1.2 / v2.0)
- **Display:** OLED 0.91" I2C (128x32 px, driver SSD1306)
- **Alimentazione:** 5V via Micro-USB (regolatore interno a 3.3V)

---

## 📌 Schema di Collegamento (Pinout)

Tutte le periferiche sono alimentate a **3.3V** per garantire compatibilità logica con i pin dell'ESP8266.

### Display OLED 0.91" (I2C)
| Pin OLED | Pin NodeMCU | Descrizione |
|---|---|---|
| **VCC** | `3V3` | Alimentazione 3.3V |
| **GND** | `GND` | Massa comune |
| **SDA** | `D2` | I2C Data (GPIO 4) |
| **SCL** | `D1` | I2C Clock (GPIO 5) |

### Sensore di Umidità Capacitivo
| Pin Sensore | Pin NodeMCU | Descrizione |
|---|---|---|
| **VCC** | `3V3` | Alimentazione 3.3V |
| **GND** | `GND` | Massa comune |
| **AOUT** | `A0` | Uscita analogica (ADC0) |

---

## 📚 Librerie Richieste

Per compilare lo sketch sono necessarie le seguenti librerie (installabili tramite Library Manager dell'IDE Arduino):

- **Adafruit SSD1306** (di Adafruit)
- **Adafruit GFX Library** (di Adafruit)
- **Wire** (inclusa nel core ESP8266)

---

## ⚙️ Calibrazione della Sonda

Il sensore capacitivo restituisce valori analogici inversamente proporzionali all'umidità (valore più alto all'asciutto, più basso in acqua).

Nello sketch sono presenti due costanti di calibrazione:

```cpp
const int dryVal = 600;   // Valore analogico rilevato con sonda all'aria aperta
const int wetVal = 300;   // Valore analogico rilevato con sonda immersa in acqua
```

Per ricalibrare la sonda:
1. Leggere i valori grezzi (`sensorValue`) tramite monitor seriale a 9600 baud.
2. Rilevare il valore con la sonda completamente asciutta (`dryVal`).
3. Rilevare il valore immergendo la lama della sonda fino al livello massimo consentito (`wetVal`).
4. Aggiornare i valori nel file principale e ricaricare il firmware.

---

## 📊 Soglie di Stato

| Umidità (%) | Indicazione Display | Condizione Substrato |
|---|---|---|
| **< 30%** | `SECCO!` | Terreno asciutto, richiede annaffiatura |
| **30% – 65%** | `OTTIMO` | Umidità ideale per la maggior parte delle piante |
| **> 65%** | `UMIDO` | Terreno molto umido / saturo |

---

## 📄 Licenza

Distribuito sotto licenza MIT. Libero per uso personale e modifiche.