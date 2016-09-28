//*************************************************************************************
/** \file VNH5019MotorDriver.h
 *    This file contains a a motor driver. This driver allows for motor power to be 
 *	  specified, for the braking function to be enabled, and for the motor current to be read.
 *	  Use with Teensy 3.0 or later!
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

#ifndef VNH5019_MOTOR_DRIVER_H
#define VNH5019_MOTOR_DRIVER_H

#include <Arduino.h>

class VNH5019MotorDriver {
	public:
		// Constructor
		VNH5019MotorDriver(uint8_t INA, 
						   uint8_t INB, 
						   uint8_t ENA_DIAGA, 
						   uint8_t ENB_DIAGB,
						   uint8_t CS,
						   uint8_t PWM);
						   
		// Public Methods
		void init();	// Initialize pins, set PWM frequency to 20kHz
		void setMtrSpeed(int16_t speed); // Speed value serves as PWM dutycycle
		void setMtrBrake(int16_t brake); // Brake value of 0 is also freewheeling, 1023 is full brake
		uint16_t getCurrentMilliAmps();
		bool getFaultA();
		bool getFaultB();
		
	
	private:
		// Pin assignments
		uint8_t _INA;
		uint8_t _INB;
		uint8_t _ENA_DIAGA;
		uint8_t _ENB_DIAGB;
		uint8_t _CS;
		uint8_t _PWM;
		
};

#endif