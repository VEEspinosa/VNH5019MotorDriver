//*************************************************************************************
/** \file VNH5019MotorDriver.cpp
 *    This file contains a a motor driver. This driver allows for motor power to be 
 *	  specified, for the braking function to be enabled, and for the motor current to be read.
 *
 *  Revisions:
 *    27-09-2016 VEE Original file.
 *
 *  License:
 *    This file is copyright 2016 by V. Espinosa and released under the Lesser GNU 
 *    Public License, version 2. It intended for educational use only, but its use
 *    is not limited thereto. */
/*    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 *    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
 *    IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 *    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 *    LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUEN-
 *    TIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS 
 *    OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 *    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
 *    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. */
//*************************************************************************************

#include "VNH5019MotorDriver.h"

// Constructor
VNH5019MotorDriver::VNH5019MotorDriver(uint8_t INA, 
									   uint8_t INB, 
									   uint8_t ENA_DIAGA, 
									   uint8_t ENB_DIAGB,
									   uint8_t CS,
									   uint8_t PWM) {
   
   _INA = INA;
   _INB = INB;
   _ENA_DIAGA = ENA_DIAGA;
   _ENB_DIAGB = ENB_DIAGB;
   _CS = CS;
   _PWM = PWM;
}

void VNH5019MotorDriver::init() {
	// Define pin modes 
	pinMode(_INA,OUTPUT);
	pinMode(_INB,OUTPUT);
	pinMode(_ENA_DIAGA,INPUT);
	pinMode(_ENB_DIAGB,INPUT);
	pinMode(_CS,INPUT);
	pinMode(_PWM,OUTPUT);
	
	// Set PWM frenquency to 20kHz using analogWriteFrequency
	analogWriteFrequency(_PWM, 20000);
	
	// Set PWM dutycycle resolution to 10 bits, which is compatible with 72MHz Teensy
	analogWriteResolution(10); // Max value is 1023
}

void VNH5019MotorDriver::setMtrSpeed(int16_t speed) {
	// Default direction is CW, so reset CCW flag
	bool CCW = false;
	
	// Read the speed value, and if negative, then set CCW flag and set speed to positive
	if (speed < 0) {
		speed = -speed;
		CCW = true;
	}
	
	// Make sure that the value for speed does not exceed the max dutycycle value of 1023
	if (speed > 1023) {
		speed = 1023;
	}
	
	// Send PWM signal to VNH5019 using 10-bit dutycycle
	analogWrite(_PWM,speed);
	
	// With speed value adjusted, set the INA/INB accordingly
	if (speed == 0) {
		digitalWrite(_INA,LOW);	// Both LOW means both half bridges are off --> freewheeling
		digitalWrite(_INB,LOW);
	}
	else if (CCW) {
		digitalWrite(_INA,LOW);	// Configuration for CCW rotation
		digitalWrite(_INB,HIGH);
	}
	else {
		digitalWrite(_INA,HIGH); // Config for CW rotation
		digitalWrite(_INB,LOW);
	}
}

void VNH5019MotorDriver::setMtrBrake(int16_t brake) {
	// Braking doesn't care about direction, so we only care about magnitude of brake.
	if (brake < 0) {
		brake = -brake;
	}
	
	// Saturate brake value to max value for 10-bits
	if (brake > 1023) {
		brake = 1023;
	}
	
	// INA/INB must be set LOW to brake outputs to ground.
	digitalWrite(_INA,LOW);
	digitalWrite(_INB,LOW);
	
	// Send brake signal, where brake dutycycle of 0 is the same as freewheeling, and 
	// a value of 1023 is full brake. When the PWM line is held high (1023 value), then
	// the internal low side MOSFETs of both half-bridges are on, shorting them to ground.
	analogWrite(_PWM,brake);
		
}

uint16_t VNH5019MotorDriver::getCurrentMilliAmps() {
	// Read 10bit ADC pin connected to CS, and scale to mA.
	// 3.3V / 1024 ADC LSB / 140 mA per LSB = 23 mA per LSB
	return analogRead(_CS) * 23;
}

bool VNH5019MotorDriver::getFaultA() {
	return !digitalRead(_ENA_DIAGA); // Invert logic of diagnostic A pin so that a fault returns true value
}

bool VNH5019MotorDriver::getFaultB() {
	return !digitalRead(_ENB_DIAGB); // Invert logic of diagnostic B pin so that a fault returns true value
}


