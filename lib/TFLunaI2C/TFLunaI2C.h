#include <Arduino.h>

#define TFLUNA_I2C_ADDR	0x10
#define DIST_L          0x00
#define DIST_H          0x01
#define AMP_L           0x02
#define AMP_H           0x03
#define TEMP_L          0x04
#define TEMP_H          0x05
#define TICK_L          0x06
#define TICK_H          0x07
#define FREQ_L          0x26
#define FREQ_H          0x27

class TFLunaI2C {
	public: 
		TFLunaI2C();
		void get_freq(uint16_t &freq);
		void set_freq(uint16_t &freq);
		bool get_data(uint16_t &dist);

	private:
		uint8_t rbyte(uint8_t num);
		void wbyte(uint8_t num, uint8_t data);
		uint8_t data_buf[6];
};