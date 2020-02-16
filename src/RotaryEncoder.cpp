/*
 * RotaryEncoder.cpp
 */
#include <Arduino.h>
#include "RotaryEncoder.h"

RotaryEncoder* RotaryEncoder::_anchor = NULL;

// initialize RotaryEncoder object
RotaryEncoder::RotaryEncoder(uint8_t a, uint8_t b) 
{
	_anchor = this; // to workaround the calling attachInterrupt in a member function in a class
	_aPin = a;
	_bPin = b;

	// configure inputs with pullup for phase A and B outputs of rotary encoder
	pinMode(_aPin, INPUT_PULLUP);
	pinMode(_bPin, INPUT_PULLUP);

	// interrupt when value change
	// NOTE: callback function of 'proxy' is static member function!
	attachInterrupt(digitalPinToInterrupt(RotaryEncoder::_aPin), RotaryEncoder::proxy, CHANGE);
	attachInterrupt(digitalPinToInterrupt(RotaryEncoder::_bPin), RotaryEncoder::proxy, CHANGE);

	// initialize state variables
	set(0);					// intial position to 0
	_encoder.curState = 7;	// to idle state

	// set default limit -128 <= position <= 127
	setLimit(-128, 127); 
};

// proxy function for the callback function with ttachInterrupt call.
void RotaryEncoder::proxy()
{
	_anchor->readEnc();	// call actual callback function
}

// set current position of encoder;
void RotaryEncoder::set(int pos)
{
	_encoder.position = pos;
};	

// set limit of min/max
void RotaryEncoder::setLimit(int min, int max)
{
	_encMin = min;
	_encMax = max;
}

// get current position of encoder;
int RotaryEncoder::get()
{
	return _encoder.position;
};

// increment current position of encoder;
int RotaryEncoder::inc()
{
	if (_encoder.position < _encMax)
		_encoder.position++;
	return _encoder.position;
};

// decrement current position of encoder;
int RotaryEncoder::dec()
{
	if (_encoder.position > _encMin)
		_encoder.position--;
	return _encoder.position;
}

/*
 * readEnc: read rotary encoder status, update position according to its rotation.
 * You should call this function by interrupt on EncAIn and/or EncBIn with CHANGE mode.
 */
void RotaryEncoder::readEnc()
{
	// State transition table [currentState, a, b] -> [nextState]
	const uint8_t StateTransionTable[32] = {
		0, 1, 2, 7, // cs:0
		0, 1, 7, 3, // cs:1
		0, 7, 3, 3, // cs:2
		7, 1, 6, 3, // cs:3
		4, 5, 6, 7, // cs:4
		4, 5, 7, 3, // cs:5
		4, 7, 6, 3, // cs:6
		7, 7, 7, 3  // cs:7 
	};

	// Read encoder output of pin A and B
	uint8_t a = digitalRead(_aPin) & 1;
	uint8_t b = digitalRead(_bPin) & 1;

	// Calculate next state 
	uint8_t nextState = StateTransionTable[(_encoder.curState & 7) << 2 | a << 1 | b];
  
	if (nextState == 3) { // update position at last valid state transition
		if (_encoder.curState == 2) // CW
			inc();
   		if (_encoder.curState == 5) // CCW
       		dec();
  		}
  	_encoder.curState = nextState;
}

