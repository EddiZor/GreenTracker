### 4. File di Log delle Versioni (`CHANGELOG.md`)

```markdown
# Changelog
Tutte le modifiche rilevanti a questo progetto saranno documentate in questo file.

## [4.0.0] - 2026-09-20

### Aggiunto
- **Server Web Nativo:** Implementata la libreria `ESP8266WebServer` per ospitare una pagina HTML/CSS direttamente sulla memoria del NodeMCU.
- **Web App Dashboard:** Creata un'interfaccia utente responsive per smartphone (stile Dark Mode) che mostra in tempo reale i valori di umidità e segnala in rosso i sensori in allarme.
- **Supporto IP su Display:** Il secondo schermo OLED ora alterna la visualizzazione dello stato del WiFi con l'indirizzo IP locale, rendendo immediato l'accesso alla Web App.

### Modificato
- **Codifica Caratteri:** Aggiunto il tag `<meta charset='UTF-8'>` nell'HTML generato per interpretare correttamente gli emoji nei browser mobile.
- **Gestione del Timing:** Sostituiti i classici `delay()` nel loop principale con la nuova funzione custom `attesaIntelligente()`, che esegue un polling continuo di `server.handleClient()` garantendo l'assenza di lag durante la navigazione sulla Web App.

## [3.0.0] - 2026-09-19

### Aggiunto
- **Rilevamento Plug & Play:** Implementata logica per ignorare automaticamente le porte vuote del multiplexer.
- **Supporto Pushover:** Inserita libreria `ESP8266HTTPClient` per inviare notifiche push (HTTPS) allo smartphone.
- **Logica Anti-Spam:** Sistema di booleani per evitare invii ripetuti di allarmi sotto la soglia critica del 20%.
- **Layout Hardware HW-178:** Creata una mappatura dedicata per la basetta millefori 24x18.
- **Checklist Saldature:** Aggiunto un tracker in Markdown nel file ASSEMBLY.md.

### Modificato
- **Routine di connessione WiFi:** Aggiunto blocco `WiFi.mode(WIFI_STA); WiFi.disconnect();` nel setup per forzare il reset della memoria radio ed evitare loop di connessione.