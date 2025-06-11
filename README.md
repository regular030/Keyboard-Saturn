# **Keyboard Saturn, A Simple Keyboard With An OLED Display**

![98a7035d-8480-496f-965c-6c204143d6b0](https://github.com/user-attachments/assets/317e6d03-7107-4c13-aaac-688303fa6b32)
![image](https://github.com/user-attachments/assets/40f2bb41-a31f-4db6-9303-55106f4096e9)
![image](https://github.com/user-attachments/assets/f23ca1d3-1136-4d6a-9a44-25fd33c32397)

# **Why Did I Make This**

After Hackpad, I started getting recommended lots of Scotto Keebs videos on YouTube, which made me much more interested in making my own keyboard. Now that I have more free time, I decided to make my 5-month-in-thought project a reality.

# **Design Choices**
- SN74HC165N
  - Instead of using any other GPIO expanders, I wanted to use the SN because it is an 8-bit parallel-load shift register, instead of using the I2C protocol to communicate with the MCU, which is around 8 times faster
- Raspberry Pi Pico
  - Cheap and Reliable
- OLED 32-128
  -  Just wanted to add a display where I can show whether or not NUM/CAPS/SCRL lock is on, + show Spotify music, + other stuff
- 100% Layout
  - I use the Numpad for anything numbers-related
- Minimalist chassis design
  - I just wanted to make something  that was simple and looked clean, kind of like cyber truck, but better 
 
# **Files**
<br>
chassis
<br>

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
<br>
kicad
<br>

- All the KiCad 8.0.5 files for the project 

# **BOM:**
![image](https://github.com/user-attachments/assets/f35cf10c-4651-452d-aa8c-597d1cdf5df1)

https://docs.google.com/spreadsheets/d/1oLEcZ3zztSK3hqa6-qvamrNF2S6u_cLABAHxks2xx3M/edit?usp=sharing

# **TODO:**
- Create art for displays
  - Something for Spotify
  - Something for caps lock, num lock, etc.
