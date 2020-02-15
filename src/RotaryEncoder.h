/*
 * RotaryEncoder.h
 * Tested on Teensy-LC with the rotary encoder ( http://akizukidenshi.com/catalog/g/gP-05771/ )
 */
#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H
#include <Arduino.h>

class RotaryEncoder {

public:
	// constructor: take pin# for signal A and B
	RotaryEncoder(uint8_t a, uint8_t b);

	// set current position of encoder;
	void set(int pos);

	// set limit of min/max
	void setLimit(int min, int max);

	// get current position of encoder, return current position;
	int get();

	// increment current position of encoder, return updated position;
	int inc();

	// decrement current position of encoder, return updated position;
	int dec();

private:
	static RotaryEncoder* _anchor;
	volatile uint8_t	_aPin;	// pin# A
	volatile uint8_t 	_bPin;	// pin# B
	volatile int 		_encMin;	// lower limit of encoder position, default is -128
	volatile int 		_encMax;	// upper limit of encoder position, default is 127

	volatile struct {
		int32_t position;   // encoder position
		uint8_t curState;   // {dir, a, b}: cw when dir==0, ccw when dir==1
	} _encoder;

	/*
	 * readEnc: read rotary encoder status, update position according to its rotation.
	 * You should call this function by interrupt on EncAIn and/or EncBIn with CHANGE mode.
	 */
	void readEnc();

	/* 
     * Used a proxy function to allow calling callback function of readEnc() for attachInterrupt() call
	 * Note: declar proxy() member function as 'static' or otherwise you will get compile error 
	 */
	static void proxy();
};
#endif // ROTARY_ENCODER_H
