# 🌿 GreenTracker v4.0

GreenTracker è un sistema IoT basato su ESP8266 (NodeMCU) progettato per monitorare l'umidità del suolo di un massimo di 16 piante contemporaneamente. 
La versione 4.0 trasforma il dispositivo in un server web autonomo, offrendo una dashboard in tempo reale accessibile da qualsiasi smartphone, oltre al rilevamento intelligente dei sensori e agli avvisi push.

## ✨ Novità della Versione 4.0
* **Web App Integrata:** Il NodeMCU genera nativamente una pagina web in HTML/CSS (Responsive & Dark Mode). Inserendo l'indirizzo IP del dispositivo nel browser dello smartphone, si ottiene una dashboard completa con i dati di tutti i sensori connessi.
* **Ritardo Intelligente (Smart Delay):** Creata una funzione dedicata (`attesaIntelligente`) per mantenere in ascolto il server web in background durante il carosello dei display OLED, azzerando i tempi di blocco.
* **Sistema Plug & Play:** Rileva automaticamente quali sensori sono fisicamente collegati ignorando le porte vuote e aggiornando dinamicamente i display e l'interfaccia web.
* **Notifiche Pushover:** Invia un alert push allo smartphone quando l'umidità di una specifica pianta scende sotto il 20% (con logica Anti-Spam integrata).

## 🛠️ Hardware Necessario
* 1x ESP8266 NodeMCU (ESP-12E)
* 1x Multiplexer Analogico 16 canali CD74HC4067 (Modello **HW-178**)
* 2x Display OLED 0.91" 128x32 (I2C)
* Fino a 16 Sensori di umidità del suolo capacitivi
* 2x Resistenze da 10 kOhm (da collegare in serie per ottenere 20 kOhm come pull-down)
* 1x Basetta Millefori 24x18

## 📖 Istruzioni di Assemblaggio
Per i dettagli su come saldare i componenti sulla millefori (ottimizzata per il modello HW-178) e la checklist dei collegamenti, consulta il file [ASSEMBLY.md](ASSEMBLY.md).

# 🌿 GreenTracker v3.0

GreenTracker è un sistema IoT basato su ESP8266 (NodeMCU) progettato per monitorare l'umidità del suolo di un massimo di 16 piante contemporaneamente. 
La versione 3.0 introduce il rilevamento intelligente dei sensori, avvisi push su smartphone e un layout hardware ottimizzato.

## ✨ Novità della Versione 3.0
* **Sistema Plug & Play:** Il sistema rileva automaticamente quali sensori sono fisicamente collegati (grazie a una resistenza di pull-down da 20kOhm) e ignora le porte vuote, aggiornando dinamicamente il carosello sui display OLED.
* **Notifiche Pushover:** Integrazione con l'API HTTPS di Pushover. Invia un alert sullo smartphone quando l'umidità di una specifica pianta scende sotto il 20%.
* **Logica Anti-Spam:** Il sistema memorizza lo stato degli allarmi inviati. Non invierà nuove notifiche per la stessa pianta finché l'umidità non tornerà sopra il 25%.
* **Self-Healing WiFi:** Inserita una routine di formattazione temporanea della memoria radio all'avvio per evitare conflitti di credenziali.

## 🛠️ Hardware Necessario
* 1x ESP8266 NodeMCU (ESP-12E)
* 1x Multiplexer Analogico 16 canali CD74HC4067 (Modello **HW-178**)
* 2x Display OLED 0.91" 128x32 (I2C)
* Fino a 16 Sensori di umidità del suolo capacitivi
* 1x Resistenza da 20 kOhm
* 1x Basetta Millefori 24x18

## 📖 Istruzioni di Assemblaggio
Per i dettagli su come saldare i componenti sulla millefori e la checklist dei collegamenti, consulta il file [ASSEMBLY.md](ASSEMBLY.md).