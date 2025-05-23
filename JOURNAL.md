---
title: "Keyboard Saturn"
author: "Kunshpreet"
description: "Simple Keyboard with OLED screen"
---

**Total time spent thus far: 48h**

# January: Research 

- Started looking at how to make my keyboard (looking at YouTube videos, etc) 
- Created a plan
  - Make the keyboard a 100% layout
  - Add an OLED screen to display extra info 
- Started thinking about what components to use (what type of switches,e.g., red, brown, blue, etc.)
- Started looking at the pinout of the Pi Pico (my microcontroller)

**Total time spent: 5h**

# May 17-20: PCB making 

- started creating the PCB
- Found out that the PI Pico doesn't have enough pins for my matrix for the keys
  - Thought about using Japanese matrices, but instead I opted to use the MCP23017_SO since I want to learn how to solder SMD components 
- After creating the schematic, I posted it on Slack to get some feedback
![image](https://github.com/user-attachments/assets/0d5e29d1-3064-4566-9d20-5dbb702a06ae)
- Then I started creating the PCB itself
- Routing was hard, so for the first time, I used Vias
![image](https://github.com/user-attachments/assets/44051060-9529-4fb8-b644-cb3b95a02d30)
**Total time spent: 15h**

# May 20-22: Making the chassis 

- started creating the chassis
- Since I'm using somewhat custom spacing, it took a long time to make the top part of my keyboard
![image](https://github.com/user-attachments/assets/4cf3d5bb-b595-48eb-bbbd-26a3c24997a1)
![image](https://github.com/user-attachments/assets/e06182c2-dea3-4b4e-9543-56e6c949e688)
- Once done, I started making the bottom part
- After that, I started seeing how I was going to print out the chassis
  - JLC charges 100$ for the chassis
- So instead, I made the chassis printable by splitting it into 3 parts and joining them via these standoffs that I made
![image](https://github.com/user-attachments/assets/f0eda411-4876-494b-92f9-f6d8a35cadc5)
**Total time spent: 8h**

# May 22-23: Creating BOM

- BOM done, please check the read me
**Total time spent: 4h**

# May 23: Updating Readme and other things

- Readme updated
**Total time spent: 2h**

# May 23 - 24: Firmware 

- Started making firmware
  - took too long setting up QMK (like 5 hours in one day) 
- Found out the MCP23017_SO is way too slow since it's using the I2C connection protocol
  - Instead im using the SN74HC165N, which uses the spi protocal which is 100x faster
  - replaced it in the PCB
- made the keyboard layout for my keyboard in QMK
May 24th:
- Took almost 5 hours to make the keyboard work, not even including the custom matrix
    - Not good with command-based apps, so I need to work on this in the future
- made a custom matrix to read from the SN74HC165N
    - TOOK A LONG TIME JUST TO LEARN HOW TO DO IT
- made the OLED work
- Here are some code issues that I had today:
  - setPinOutput(MATRIX_COL_PINS[c]) failed because the macro received 21 arguments instead of 1, likely because MATRIX_COL_PINS[c] expands improperly.
  - writePinHigh(MATRIX_COL_PINS[c]) and writePinLow(MATRIX_COL_PINS[col]) also failed due to the same macro argument issue.
  - Macros like gpio_set_pin_output_push_pull, gpio_write_pin_high, and gpio_write_pin_low were not recognized as functions; they're macros and not meant to handle complex input.
  - matrix_scan_change is used without declaration, leading to an implicit function declaration error.
  - All warnings were treated as errors (-Werror flag), causing the build to fail on these issues.
**Total time spent: 15h**

# May xx - xx: Firmware Pt2:
- TODO: Create the UI for the OLED

