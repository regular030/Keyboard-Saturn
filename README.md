**Keyboard Saturn, A Simple Keyboard With An OLED Display**

![image](https://github.com/user-attachments/assets/6df31256-2c50-42c2-93f2-c2fba238719a)
![image](https://github.com/user-attachments/assets/40f2bb41-a31f-4db6-9303-55106f4096e9)
![image](https://github.com/user-attachments/assets/f23ca1d3-1136-4d6a-9a44-25fd33c32397)

**Files**
<br>
chassis
- .Step files for the chassis + PCB
  - KeyboardL.step
      - Bottom left side of keyboard
  - KeyboardM.step
      - Bottom middle side of keyboard
  - KeyboardR.step
      - Bottom right side of keyboard
  - KeyboardTop.step
      - Top side of the keyboard
      - Use M2 screws to screw it into the bottom parts (screw holes in top and bottom in the middle, left and right sections) 
  - insert-keyboard v1.step
      - The inserts for the keyboard to connect KeyboardM, KeyboardL and KeyboardR, please use super glue to make sure they are in, then super glue the chassis together
kicad
<br>

- All the KiCad 8.0.5 files for the project 

|Part Name                          |QTY                                                                                               |Price Per|Link (please look at BOM)|Notes                            |
|-----------------------------------|--------------------------------------------------------------------------------------------------|---------|-------------------------|---------------------------------|
|Raspberry Pi Pico (U1)             |1                                                                                                 |US $2.08 ||                                 |
|MCP23017_SO (U2)                   |2                                                                                                 |US $1.36 ||buying one more Incase I screw up|
|OLED 128x32 (J1)                   |1                                                                                                 |US $0.99 ||                                 |
|Resistor 2kohms (R1-R4) pack of 100|1                                                                                                 |US $1.67 ||lowest # I can get               |
|Diode IN4148 (D1-D104)  pack of 100|2                                                                                                 |US $1.96 ||                                 |
|SW_Push (SW1-SW104)                |110                                                                                               |US $18.13||                                 |
|Keyboard Keycaps pack of 126       |1                                                                                                 |US $7.55 ||                                 |
|Solder Wire Desoldering            |1                                                                                                 |US $2.36 ||                                 |
|PCB from JLCPCB                    |5                                                                                                 |US $31.00|    |with tax + shipping              |
|3D printed Case for JLCPCB         |1                                                                                                 |US $14.78|    |Top only                         |
|M Screws pack of 250               |1                                                                                                 |US $2.66 ||Cheapest I could find            |
|                                   |                                                                                                  |         |    |                                 |
|Total:                             |US $46.32 (Aliexpess) + $45.78 (PCB + top case) = USD $92.1 = $93 (prices flucates on aliexpress) |         |    |                                 |

**TODO:**
- Create art for displays
  - Something for Spotify
  - Something for caps lock, num lock, etc.
 - Create firmware
