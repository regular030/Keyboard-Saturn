---
title: "Keyboard Saturn"
author: "Kunshpreet"
description: "Simple Keyboard with OLED screen"
---

**Total time spent thus far: 34h**

# January: Research 

- Started looking at how to make my keyboard (looking at yt videos, etc) 
- Created a plan
  - Make the keyboard a 100% layout
  - Add an OLED screen to display extra info 
- Started thinking about what components to use (what type of switches ex. red, brown, blue, etc.)
- Started looking at the pinout of the Pi Pico (my microcontroller)

**Total time spent: 5h**

# May 17-20: PCB making 

- started creating the PCB
- Found out that the PI Pico doesn't have enough pins for my matrix for the keys
  - Thought about using Japanese matrices, but instead I opted to use the MCP23017_SO since I want to learn how to solder SMD components 
- After creating the schematic, I posted it on Slack to get some feedback
(Add an image of the schematic.) 
- Then I started creating the PCB itself
- Routing was hard, so for the first time, I used Vias
(Add an image of the PCB.) 
**Total time spent: 15h**

# May 20-22: Making the chassis 

- started creating the chassis
- Since I'm using somewhat custom spacing, it took a long time to make the top part of my keyboard
(add img of failed top part)
(add img of good top part) 
- Once done, I started making the bottom part
- After that, I started seeing how I was going to print out the chassis
  - JLC charges 100$ for the chassis
- So instead, I made the chassis printable by splitting it into 3 parts and joining them via these standoffs that I made
(add pic of standoffs + pic of chassis parts) 
**Total time spent: 8h**

# May 22-23: Creating BOM

- BOM done, please check read me
**Total time spent: 4h**

# May 23: Updating Readme and other things

- Readme updated
**Total time spent: 2h**

# May 23 - xx: Firmware 

- Started making firmware
  - took too long setting up QMK (like 5 hours in one day) 
- Found out the MCP23017_SO is way too slow since it's using the I2C connection protocol
  - Instead im using the SN74HC165N, which uses the spi protocal which is 100x faster
  - replaced it in the PCB
- made the keyboard layout for my keyboard in QMK
- TODO: make custom matrix in order to read from the SN74HC165N
**Total time spent: 7h**
