#include "TFLunaI2C.h"
#include <Wire.h>

// Constructor
TFLunaI2C::TFLunaI2C(){}

// Reads one byte from the TFLuna by sending a command number
uint8_t TFLunaI2C::rbyte(uint8_t num) {
    // Send given command to the TFLuna
    Wire.beginTransmission(TFLUNA_I2C_ADDR);
    Wire.write(num);
    Wire.endTransmission();

    // true will send a stop message after the request, releasing the bus. 
    // false will continually send a restart after the request, keeping the connection active.
    Wire.requestFrom((int)TFLUNA_I2C_ADDR, 1, true);

    // Return the read byte
    return (uint8_t)Wire.read();
}

void TFLunaI2C::wbyte(uint8_t num, uint8_t data) {
    Wire.beginTransmission(TFLUNA_I2C_ADDR);
    Wire.write(num);
    Wire.write(data);
    Wire.endTransmission();
}

// Reads the operating frequency (in Hz) of the TFLuna. 100Hz by default
void TFLunaI2C::get_freq(uint16_t &freq) {
    uint8_t * p_freq = (uint8_t *)&freq;
    p_freq[0] = rbyte(FREQ_L);
    p_freq[1] = rbyte(FREQ_H);
}

void TFLunaI2C::set_freq(uint16_t &freq) {
    uint8_t * p_freq = (uint8_t *)&freq;
    wbyte(FREQ_L, p_freq[0]);
    wbyte(FREQ_H, p_freq[1]);
}

// Reads distance data from the TFLuna, along with signal amplitude, chip temperature and timestamp.
bool TFLunaI2C::get_data(uint16_t &dist) {
    // Read the necessary bytes and store them inside data_buf
    for(uint8_t i = DIST_L; i<= DIST_H; i++) {
        data_buf[i] = rbyte(i);
    }

    // left shift == multiply by 255
    dist = (data_buf[0] + (data_buf[1]<<8));
    //amp = data_buf[2] + (data_buf[3]<<8);

    /*
    // From the docs: in I2C, the temperature value is provided in 0.01 Celsius.
    // So, dividing such value by 100 gives the temperature in Celsius.
    temp = (data_buf[4] + (data_buf[5]<<8));
    temp = temp/100;
    */

    // Check if data is reliable
    //if(amp <= (uint16_t)100 || amp >= (uint16_t)65535 || dist >= (uint16_t)900) {
    //if(dist>=900) {
    //    return false; 
    //}

    return true;
}