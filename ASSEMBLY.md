# 🛠️ Guida all'Assemblaggio GreenTracker

## 🗺️ Layout Millefori (Ottimizzato per HW-178)

```text
    1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18
  +----------------------------------------------------+
1 | O [N] -  -  -  -  -  -  - [N] O  O  O  O  O  O  O  O |
2 | O [N]                       | O  O [D1] O  O [D2]  O | <- OLED 1 & 2
3 | O [N]                       | O  O [D1] O  O [D2]  O |    (4 pin cad.)
4 | O [N]     NODE MCU          | O  O [D1] O  O [D2]  O |
5 | O [N]     ESP8266           | O  O [D1] O  O [D2]  O |
6 | O [N]     (Antenna Su)      | O  O  O  O  O  O  O  O |
7 | O [N]                       | O  O  O  O  O  O  O  O |
8 | O [N]                       | O  O  O  O  O  O  O  O |
9 | O [N]                       | O  O  O  O  O  O  O  O |
10| O [N]                       | O  O  O  O  O  O  O  O |
11| O [N]                       | O  O  O  O  O  O  O  O |
12| O [N]                       | O  O  O  O  O  O  O  O |
13| O [N]                       | O  O  O  O  O  O  O  O |
14| O [N]                       | O  O  O  O  O  O  O  O |
15| O [N] -  -  -  -  -  -  - [N] O  O  O  O  O  O  O  O |
16| O  O  O  O  O  O  O  O  O  O  O  O  O  O  O  O  O  O |
17| O  O  O  O  O [m][m][m][m][m][m][m][m] O  O  O  O  O | <- MUX 8 Pin (Controllo)
18| O  O  O  O  O  |  |  |  |  |  |  |  |  O  O  O  O  O |
19| O  O  O  O  O  M  U  L  T  I  P  L  E  X  E  R  O  O |
20| O [M][M][M][M][M][M][M][M][M][M][M][M][M][M][M][M] O | <- MUX 16 Pin (Canali)
21| O  O  O  O  O  O  O  O  O  O  O  O  O  O  O  O  O  O |
22| O [V][V][V][V][V][V][V][V][V][V][V][V][V][V][V][V] O | <- RIGA VCC 3.3V
23| O [G][G][G][G][G][G][G][G][G][G][G][G][G][G][G][G] O | <- RIGA GND (Massa)
24| O [S][S][S][S][S][S][S][S][S][S][S][S][S][S][S][S] O | <- RIGA SEGNALI (Sensori)
  +----------------------------------------------------+


  ## 📋 Checklist Saldature

**FASE 1: Le Autostrade dell'Alimentazione (3.3V e GND)**

- [ ] **Binario GND (Massa):** Unire tutti i 16 fori della **Riga 23** con un filo continuo.
- [ ] Collegare il Binario GND al pin **GND** del NodeMCU.
- [ ] Collegare il Binario GND ai pin **GND** dei due connettori OLED.
- [ ] Collegare il Binario GND al pin **GND** del Multiplexer.
- [ ] Collegare il Binario GND al pin **EN** del Multiplexer.
- [ ] **Binario VCC (3.3V):** Unire tutti i 16 fori della **Riga 22** con un filo continuo.
- [ ] Collegare il Binario VCC al pin **3V3** del NodeMCU.
- [ ] Collegare il Binario VCC ai pin **VCC** dei due connettori OLED.
- [ ] Collegare il Binario VCC al pin **VCC** del Multiplexer.

**FASE 2: I Display (Piste I2C)**

- [ ] Connettore Display 1 **SCK** -> Pin **D1** (NodeMCU)
- [ ] Connettore Display 1 **SDA** -> Pin **D2** (NodeMCU)
- [ ] Connettore Display 2 **SCK** -> Pin **D5** (NodeMCU)
- [ ] Connettore Display 2 **SDA** -> Pin **D6** (NodeMCU)

**FASE 3: Controllo Multiplexer e Resistenza Plug & Play**

- [ ] Multiplexer **S0** -> Pin **D3** (NodeMCU)
- [ ] Multiplexer **S1** -> Pin **D4** (NodeMCU)
- [ ] Multiplexer **S2** -> Pin **D7** (NodeMCU)
- [ ] Multiplexer **S3** -> Pin **D8** (NodeMCU)
- [ ] Multiplexer **SIG** -> Pin **A0** (NodeMCU)
- [ ] **Resistenza Plug & Play:** Saldare DUE resistenze da 10k Ohm in serie (per ottenere 20k Ohm) a ponte tra il pin **A0** e il binario del **GND**.

**FASE 4: Segnali Sensori (Pettine Riga 24)**

- [ ] Pettine Pin 1 (Estrema Sinistra) -> Multiplexer **C15**
- [ ] Pettine Pin 2 -> Multiplexer **C14**
- [ ] Pettine Pin 3 -> Multiplexer **C13**
- [ ] Pettine Pin 4 -> Multiplexer **C12**
- [ ] Pettine Pin 5 -> Multiplexer **C11**
- [ ] Pettine Pin 6 -> Multiplexer **C10**
- [ ] Pettine Pin 7 -> Multiplexer **C9**
- [ ] Pettine Pin 8 -> Multiplexer **C8**
- [ ] Pettine Pin 9 -> Multiplexer **C7**
- [ ] Pettine Pin 10 -> Multiplexer **C6**
- [ ] Pettine Pin 11 -> Multiplexer **C5**
- [ ] Pettine Pin 12 -> Multiplexer **C4**
- [ ] Pettine Pin 13 -> Multiplexer **C3**
- [ ] Pettine Pin 14 -> Multiplexer **C2**
- [ ] Pettine Pin 15 -> Multiplexer **C1**
- [ ] Pettine Pin 16 (Estrema Destra) -> Multiplexer **C0**