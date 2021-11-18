# Heron
The Heron is a synthesiser control board developed by Chris Deakin & Daniel Ross in 2021 as a final year project at RMIT University. It is designed to control the [Axoloti](http://www.axoloti.com/) synthesiser. The Heron is built around an ESP-32, controlling an SPI touch screen, eight encoders, two sliders, a two-axis joystick, and a large button matrix. 

The key features of the Heron are: 
- Assignable control elements, whereby the encoders, sliders, and joystick can control user-assigned elements on the connected synthesiser.
- Virtual banking of these control elements, and saving/loading of user presets, to multiply the number of controllable variables beyond the number of physical controls.
- A touch UI on a 320x240 3.5" screen, made with the LVGL library.
- A 16-track sequencer, with two rows of physical keys controlling up to eight virtually banked tracks.
- A two octave keyboard, for directly playing notes.

Heron code was written in C/C++, using the ESP32 port of FreeRTOS within the ESP-IDF framework. Development of the project is documented in the final report included in this directory.

Final Heron Board:
![Final Board](/Heron.jpg)

Final PCB Layout:
![Final Design](/Layout.png)
