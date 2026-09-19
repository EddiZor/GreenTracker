### 4. File di Log delle Versioni (`CHANGELOG.md`)

```markdown
# Changelog
Tutte le modifiche rilevanti a questo progetto saranno documentate in questo file.

## [3.0.0] - 2026-09-19

### Aggiunto
- **Rilevamento Plug & Play:** Implementata logica per ignorare automaticamente le porte vuote del multiplexer.
- **Supporto Pushover:** Inserita libreria `ESP8266HTTPClient` per inviare notifiche push (HTTPS) allo smartphone.
- **Logica Anti-Spam:** Sistema di booleani per evitare invii ripetuti di allarmi sotto la soglia critica del 20%.
- **Layout Hardware HW-178:** Creata una mappatura dedicata per la basetta millefori 24x18 ottimizzata per la geometria dei pin del modello HW-178.
- **Checklist Saldature:** Aggiunto un tracker in Markdown nel file ASSEMBLY.md per seguire le fasi di costruzione fisica.

### Modificato
- **Routine di connessione WiFi:** Aggiunto blocco `WiFi.mode(WIFI_STA); WiFi.disconnect();` nel setup per forzare il reset della memoria radio ed evitare loop di connessione (Permission/Auth errors).
- **Interfaccia OLED:** Il carosello ora salta attivamente i cicli di rendering per i sensori non fisicamente connessi.