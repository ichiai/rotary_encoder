/*
 * Simple example of the RotaryEncoder class
 * 2020.02.15: ichiai, Initial release.
 * 
 * This example monitor the rotary encoder then echo its position when it is changed.
 * Light Green LED when CW rotation, Red when CCW rotation.
 * 
 * Tested on Teensy-LC with the rotary encoder ( http://akizukidenshi.com/catalog/g/gP-05771/ )
 * No other configuration has tested yet.
 */
#include "RotaryEncoder.h"

/* 
 * Pin configurations for the Teensy-LC. Change these pins for your hardware.
 * CAUTION: RotaryEncoder class is 'interrupt driven' so EncAIn and EncBIn must be used as interrupt pin.
 *  
 * NOTE: Here are interrupt request pin availbale for Teensy-LC
 * 2-12,14,15,20-23: Could not find official reference yet ...
 */
const int EncAIn = 3;    // Encoder output A
const int EncBIn = 4;    // Encoder output B
const int GrLedOut = 14; // Green LED on encoder
const int RdLedOut = 15; // Red LED on encoder

/*
 * RotaryEncoder object initialization
 * It configure two input pins for the encoder with the interrupt handler enabled.
 */
RotaryEncoder encoder(EncAIn, EncBIn);

void setup() {
  // Configure LED drivers
  pinMode(GrLedOut, OUTPUT);
  pinMode(RdLedOut, OUTPUT);
  Serial.begin(38400);
  Serial.println("RotaryEncoder simple test example.");
}

int pos=0;

// Monitor the rotary encoder, report its position with LED indication
void loop() {
  if (pos != encoder.get()) {
    if (pos < encoder.get()) {			  // Lit Green when CW rotation
        digitalWrite(GrLedOut, HIGH);
        digitalWrite(RdLedOut, LOW);
    } else if (pos > encoder.get()) {	// lit Red when CCW rotation
        digitalWrite(GrLedOut, LOW);
        digitalWrite(RdLedOut, HIGH);
    }
    Serial.println(encoder.get());
    pos = encoder.get();
  }
  delay(10);
}
