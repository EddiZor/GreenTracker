# Changelog

Tutte le modifiche di rilievo al progetto GreenTracker saranno documentate in questo file.

## [1.0] - 2026-09-13

### Aggiunto
* **Firmware base:** Prima release stabile del sistema di monitoraggio per scheda NodeMCU ESP8266.
* **Supporto Doppio Display OLED:** Integrazione simultanea di due schermi OLED I2C da 0.91" (128x32). Utilizzo di bus I2C software via pin D1/D2 (Display 1) e D5/D6 (Display 2) tramite la libreria `U8g2` per bypassare i limiti dell'indirizzo hardware fisso.
* **Interfaccia Utente (UI) ottimizzata:** 
  * *Display 1 (Sensore):* Layout con etichette accorciate ("Terr." e "Umid.:") e valore percentuale ingrandito, con calcolo automatico per l'allineamento a destra.
  * *Display 2 (Sistema):* Layout informativo con nome progetto e stato del sistema ("Stato: OK").
* **Nuova Tipografia:** Transizione ai font Sans-Serif (Helvetica Bold a 12 e 18 pixel) per un'interfaccia più moderna, pulita e leggibile.
* **Logica Sensore Capacitivo:** Lettura analogica sul pin `A0` con mappatura percentuale (0-100%) e funzione di blocco (constrain) per evitare valori fuori scala.
* **Sistema di Calibrazione:** Parametri `valoreAriaSecca` e `valoreAcqua` isolati e facilmente accessibili in testa al codice per una taratura rapida del sensore sul campo.