# Changelog

Tutte le modifiche di rilievo al progetto GreenTracker saranno documentate in questo file.

## [3.0] - 2026-09-16

### Aggiunto
* **Supporto Multi-Sensore:** Integrazione del modulo multiplexer CD74HC4067 a 16 canali.
* **Carosello UI:** Il Display 1 ora ruota ciclicamente le informazioni di ogni pianta collegata (ogni 3 secondi).
* **Case 3D:** Aggiunta cartella `Case_3D` con modello parametrico OpenSCAD e file STL pronto per la stampa in PLA.

### Modificato
* **UI Ottimizzata:** Ridotta l'altezza del font per la percentuale di umidità (da `helvB18` a `helvB14`) per evitare sovrapposizioni e migliorare l'allineamento estetico.

## [2.0] - 2026-09-13

### Aggiunto
* **Modulo WiFi:** Implementata la connessione alla rete domestica tramite libreria `ESP8266WiFi`.
* **Stato di Rete su UI:** Il Display 2 mostra le fasi ("Connessione...", "WiFi: OK", "WiFi: Perso"). Aggiunta schermata temporanea ("WiFi: Connesso!") per 3 secondi all'avvio.
* **Documentazione WiFi:** Istruzioni nel `README.md` per la configurazione della rete.

### Modificato
* **Sicurezza Firmware:** Variabili `ssid` e `password` valorizzate ad asterischi `***` come placeholder per il repository pubblico.

---

## [1.0] - 2026-09-13

### Aggiunto
* **Firmware base:** Prima release stabile per NodeMCU ESP8266.
* **Doppio Display OLED:** Utilizzo di bus I2C software via pin D5/D6 tramite libreria `U8g2` per bypassare l'indirizzo hardware fisso.
* **Interfaccia Utente:** Layout con allineamento a destra dinamico per la percentuale e font Sans-Serif (Helvetica Bold 12 e 18).
* **Sensore Capacitivo:** Lettura analogica sul pin `A0` con mappatura percentuale (0-100%).