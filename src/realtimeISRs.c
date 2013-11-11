/* FreeMS2 - the open source engine management system
 *
 * Copyright 2008, 2009 Fred Cooke
 *
 * This file is part of the FreeMS2 project.
 *
 * FreeMS2 software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FreeMS2 software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with any FreeMS2 software.  If not, see http://www.gnu.org/licenses/
 *
 * We ask that if you make any changes to this file you email them upstream to
 * us at admin(at)diyefi(dot)org or, even better, fork the code on github.com!
 *
 * Thank you for choosing FreeMS2 to run your engine!
 */


/**	@file realtimeISRs.c
 * @ingroup interruptHandlers
 *
 * @brief Real time interrupts
 *
 * This file contains real time interrupt handlers. Mainly it holds the RTI
 * handler itself, however the modulus down counter and ETC timer overflow
 * functions are here too.
 *
 * @author Fred Cooke
 */


#define REALTIMEISRS_C
#include "inc/FreeMS2.h"
#include "inc/interrupts.h"
#include "inc/commsISRs.h"


/** @brief Real Time Interrupt Handler
 *
 * Handles time keeping, including all internal clocks, and generic periodic
 * tasks that run quickly and must be done on time.
 *
 * @author Fred Cooke
 */
void RTIISR(){
	/* Clear the RTI flag */
	CRGFLG = 0x80;

	/* Record time stamp for code run time reporting */
	unsigned short startTimeRTI = TCNT;

	/* Increment the counter */
	Clocks.realTimeClockMain++;

	/* This function could be performed without the extra variables by rolling over the main ones at the largest multiples of the next ones, but I'm not sure thats better */

	// TODO add content to eighths of a milli RTC ?

	/* Every 8th RTI execution is one milli */
	if(Clocks.realTimeClockMain % 8 == 0){
		/* Increment the milli counter */
		Clocks.realTimeClockMillis++;

		/* Increment the milli roll over variable */
		Clocks.millisToTenths++;

		/* Perform all tasks that are once per millisecond here or preferably main */
		Clocks.timeoutADCreadingClock++;
		if(Clocks.timeoutADCreadingClock > fixedConfigs2.sensorSettings.readingTimeout){
			/* Set force read adc flag */
			coreStatusA |= FORCE_READING;
			Clocks.timeoutADCreadingClock = 0;
		}

		/* Every 100 millis is one tenth */
		if(Clocks.millisToTenths % 100 == 0){
			/* Increment the tenths counter */
			Clocks.realTimeClockTenths++;

			/* Increment the tenths roll over variable */
			Clocks.tenthsToSeconds++;

			/* Reset the millis roll over variable */
			Clocks.millisToTenths = 0;

			/* Perform all tasks that are once per tenth of a second here or preferably main */
			// decrement port H debounce variable till it's zero again.
			if(portHDebounce != 0){
				portHDebounce -= 1;
			}

			/* Every 10 tenths is one second */
			if(Clocks.tenthsToSeconds % 10 == 0){
				/* Increment the seconds counter */
				Clocks.realTimeClockSeconds++;

				// fuel pump flasher
				//PORTE ^= BIT4;

				//if(PORTM){
				//	PORTM = 0x00;
				//}else{
				//	PORTM = 0xFF;
				//}
				/* Increment the seconds roll over variable */
				Clocks.secondsToMinutes++;

				/* Reset the tenths roll over variable */
				Clocks.tenthsToSeconds = 0;
				/* Perform all tasks that are once per second here or preferably main */

				// temp throttling for log due to tuner performance issues (in the bedroom)
				ShouldSendLog = TRUE;
			
				/* Toggle/Flash Port M5 (Yellow LED on Sportster) as a "heartbeat" to let new users know it's alive */
				PORTM ^= BIT5; // 0b0010_0000 = 0x20 = Port M5

				/* Every 60 seconds is one minute, 65535 minutes is enough for us :-) */
				if(Clocks.secondsToMinutes % 60 == 0){
					/* Increment the minutes counter */
					Clocks.realTimeClockMinutes++;

					/* Potentially put an hours field in here and below, but that would be excessive */
					// TODO add hours RTC ?

					/* Reset the seconds roll over variable */
					Clocks.secondsToMinutes = 0;

					/* Perform all tasks that are once per minute here or preferably main */
					// TODO add content in minutes RTC ?

					/* Hours if statement here if we do hours which we probably won't */
				}
			}
		}
	}
	RuntimeVars.RTCRuntime = TCNT - startTimeRTI;
}


/** @brief ECT overflow handler
 *
 * When the ECT free running timer hits 65535 and rolls over, this is run. Its
 * job is to extend the timer to an effective 32 bits for longer measuring much
 * longer periods with the same resolution.
 *
 * @author Fred Cooke
 */
void TimerOverflow(){
	/* Increment the timer extension variable */
	timerExtensionClock++;

	PORTE ^= BIT4; // Toggle Fuel Pump (Sportster Green LED) on Port E4 each time this function is run.

	/* Clear the timer overflow interrupt flag */
	TFLGOF = 0x80;
}


/** @todo TODO This could be useful in future once sleeping is implemented.
// Generic periodic interrupt (This only works from wait mode...)
void VRegAPIISR(){
	// Clear the flag needs check because writing a 1 can set this one
	//if(VREGAPICL & 0x01){ // if the flag is set...
		VREGAPICL |= 0x01; // clear it...
	//} // and not otherwise!

	// DO stuff
}
*/
