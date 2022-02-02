# E-Kart-Project
E-Kart .ino code for Teensy 3.2 - 3.6 boards

Features:
- CAN bus manipulation (500 Kbit/s)
- ESC control (VESC)
- Car sound synthesizing with DAC hardware (ARB Waveform Generation)
- Power managment
- User interface
- Various datalogging features

Current Code:
- Audio synth [Teensy-Audio-System.ino] + [Samples.h] containing audio synth software
- CAN BUS example software [CAN-Transmitter] and [CAN-Reciever] .ino sketches

To Do:
- Random noise generator
- CAN bus access for VESC?

Done:
- SD card read function. This reads and parses a string of data on an SD card with required waveform points and only stores the desired waveforms in memory rather than storing all of them <Samples.h>
