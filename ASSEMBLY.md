# 🛠️ Guida all'Assemblaggio GreenTracker

## 🗺️ Layout Millefori (Griglia 18x24 - Scala Reale 1:1)

Questo schema riproduce fedelmente la matrice di 18 colonne x 24 righe della scheda. Ogni simbolo rappresenta un singolo foro.
* `O` = Foro vuoto
* `N` = Pin del NodeMCU
* `[H]` = Connettori femmina (Header) per gli OLED
* `M` = Pin del Multiplexer (HW-178)
* `[S]`, `[V]`, `[G]` = Pettini inferiori Sensore, VCC, GND

```text
    1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18
  +----------------------------------------------------+
 1| O  N  N  N  N  N  N  N  N  N  N  N  N  N  N  N  O  O | <- NodeMCU (Top)
 2| O  |                                         |  O  O |
 3| O  |                                         |  O  O |
 4| O  |           N O D E   M C U               |  O  O |
 5| O  |          (USB a sinistra)               |  O  O |
 6| O  |                                         |  O  O |
 7| O  |                                         |  O  O |
 8| O  |                                         |  O  O |
 9| O  |                                         |  O  O |
10| O  N  N  N  N  N  N  N  N  N  N  N  N  N  N  N  O  O | <- NodeMCU (Bot)
11| O  O  O  O  O  O  O  O  O  O  O  O  O  O  O  O  O  O |
12| O  O  O  O  O [H][H][H][H] O [H][H][H][H] O  O  O  O | <- Prese OLED
13| O  O  O  O  O  O  O  O  O  O  O  O  O  O  O  O  O  O |
14| O  O  O  O  O  O  O  O  O  O  O  O  O  O  O  O  O  O |
15| O  O  O  O  O  M  M  M  M  M  M  M  M  O  O  O  O  O | <- MUX (Controllo)
16| O  O  O  O  O  |                    |  O  O  O  O  O |
17| O  O  O  O  O  |       HW-178       |  O  O  O  O  O |
18| O  O  O  O  O  |                    |  O  O  O  O  O |
19| O  O  O  O  O  |                    |  O  O  O  O  O |
20| O  M  M  M  M  M  M  M  M  M  M  M  M  M  M  M  M  O | <- MUX (Canali)
21| O  O  O  O  O  O  O  O  O  O  O  O  O  O  O  O  O  O |
22| O [S][S][S][S][S][S][S][S][S][S][S][S][S][S][S][S] O | <- 1. Pettine Segnali
23| O [V][V][V][V][V][V][V][V][V][V][V][V][V][V][V][V] O | <- 2. Pettine VCC 3.3V
24| O [G][G][G][G][G][G][G][G][G][G][G][G][G][G][G][G] O | <- 3. Pettine GND
  +----------------------------------------------------+
```

### 🪧 Legenda dei Pin (Da Sinistra a Destra)
* **NodeMCU Top (Riga 1):** A0, RSV, RSV, SD3, SD2, SD1, CMD, SD0, CLK, GND, 3V3, EN, RST, GND, Vin
* **NodeMCU Bot (Riga 10):** D0, D1, D2, D3, D4, 3V3, GND, D5, D6, D7, D8, RX, TX, GND, 3V3
* **MUX Controllo (Riga 15):** SIG, S3, S2, S1, S0, EN, VCC, GND
* **MUX Canali (Riga 20):** C15, C14, C13, C12, C11, C10, C9, C8, C7, C6, C5, C4, C3, C2, C1, C0

---

## 📋 Checklist Cablaggi e Saldature

**FASE 1: Blocco di Alimentazione (Pettini Inferiori)**
*Prepara le tre righe di pettini femmina sul fondo della scheda.*

- [ ] **Autostrada GND:** Unisci tutti i pin della riga inferiore (Riga 24) per creare il binario di Massa.
- [ ] Collega il binario GND a un pin **GND** del NodeMCU.
- [ ] Collega il binario GND al pin **GND** del Multiplexer (Riga 15, Colonna 13).
- [ ] Collega il binario GND al pin **EN** del Multiplexer (Riga 15, Colonna 11).
- [ ] Collega il binario GND ai due pin **GND** dei connettori OLED (es. ai secondi pin delle prese).
- [ ] **Autostrada VCC:** Unisci tutti i pin della riga centrale (Riga 23) per creare il binario 3.3V.
- [ ] Collega il binario VCC al pin **3V3** del NodeMCU.
- [ ] Collega il binario VCC al pin **VCC** del Multiplexer (Riga 15, Colonna 12).
- [ ] Collega il binario VCC ai due pin **VCC** dei connettori OLED.

**FASE 2: Cablaggi Display I2C (Prese Centrali sulla Riga 12)**
*Essendo i connettori posizionati perfettamente sotto i pin I2C del NodeMCU, le saldature saranno cortissime.*

- [ ] Connettore OLED 1 (SCL) -> Pin **D1** (NodeMCU, Colonna 3)
- [ ] Connettore OLED 1 (SDA) -> Pin **D2** (NodeMCU, Colonna 4)
- [ ] Connettore OLED 2 (SCL) -> Pin **D5** (NodeMCU, Colonna 9)
- [ ] Connettore OLED 2 (SDA) -> Pin **D6** (NodeMCU, Colonna 10)

**FASE 3: Logica Multiplexer e Plug & Play**
*Collega la fila da 8 pin del Multiplexer (Riga 15) ai rispettivi pin del NodeMCU.*

- [ ] Multiplexer **S0** (Col. 10) -> Pin **D3** (NodeMCU, Col. 5)
- [ ] Multiplexer **S1** (Col. 9) -> Pin **D4** (NodeMCU, Col. 6)
- [ ] Multiplexer **S2** (Col. 8) -> Pin **D7** (NodeMCU, Col. 11)
- [ ] Multiplexer **S3** (Col. 7) -> Pin **D8** (NodeMCU, Col. 12)
- [ ] Multiplexer **SIG** (Col. 6) -> Pin **A0** (NodeMCU, Col. 2 superiore)
- [ ] **Resistenza Plug & Play:** Salda DUE resistenze da 10k Ohm in serie (totale 20k Ohm) tra la pista di Segnale (A0 / SIG) e un punto connesso al **GND**.

**FASE 4: Segnali Sensori a Cascata (Righe 20-22)**
*Crea dei ponti verticali diritti di un paio di millimetri saltando la riga vuota (21), unendo i pin del Multiplexer (Riga 20) al pettine segnali sottostante (Riga 22).*

- [ ] Multiplexer **C15** -> Pettine Segnali Pin 1 (Estrema Sinistra, Colonna 2)
- [ ] Multiplexer da **C14** a **C1** -> Collega verticalmente ciascun pin al rispettivo foro sottostante (Colonne 3-16).
- [ ] Multiplexer **C0** -> Pettine Segnali Pin 16 (Estrema Destra, Colonna 17)