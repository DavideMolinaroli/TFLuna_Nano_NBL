### DIY 2D LIDAR

This repository contains the source code of a simple DIY 2D LIDAR. 

The main components are an Arduino Nano, a TFLuna ToF sensor, and a Nema 17 Stepper motor. 

The Arduino Nano receives data from the TFLuna via I2C with a frequency of 250Hz and forwards it via a serial port. 

To accommodate the frequency of the TFLuna, the stepper motor performs a step every 4.5ms, but acceptable practical results were observed even for 3ms per step.

An HTD belt transmits motion between the motor and the ToF sensor with a speed reduction ratio of 0.75. So, the LIDAR gathers 267 measurements each turn.

To make the device suitable for a mobile robot that works in a fast-paced environment, another ToF sensor should be considered. For example, the TFMini has a frequency up to 1KHz, thus letting the stepper motor make a step every 1ms.

### CAD
[The CAD project is available in Onshape](https://cad.onshape.com/documents/249c78569ae08aaf29020e79/w/a476b28fa653176d6b6f8158/e/9c39c3b85f20b129b75a8c5a?renderMode=0&uiState=66b2395569574c4dad6f0dd0&renderMode=0&leftPanel=false&uiState=68f42b6bf4284e82af42bd74)

### Code organization

The code has been developed using PlatformIo.

The `lib` directory contains an interface to communicate with the TFLuna using I2C.

The `src` directory contains a simple arduino program that steps the stepper motor, collects data from the TFLuna and forwards it to its serial port.

The file `visualization.pde` contains a simple script written in Processing that visualizes in real time the data coming from the Arduino Nano.
