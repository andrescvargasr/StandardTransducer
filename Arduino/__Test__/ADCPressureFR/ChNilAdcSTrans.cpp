/*
 * ChNilAdcSTrans.cpp
 *
 * Created: 5/06/2018 3:39:05 p. m.
 *  Author: camil
 */ 

/* Arduino ChiNilRTOS Library
 * Copyright (C) 2018 by Andres Vargas
 *
 * Arduino NilRTOS Library
 * Copyright (C) 2013 by William Greiman
 *
 * This file is part of the Arduino NilRTOS Library
 *
 * This Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with the Arduino NilRTOS Library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */
/**
 * @file    ChNilAdc.cpp
 * @brief   ADC read source
 *
 * @defgroup ChNilAdc Read AVR ADC
 * @details ChNilAdc functions.
 * @{
 */
#include "ChNil.h"
#include "ChNilAdcSTrans.h"
#include "Arduino.h"
static uint8_t nil_analog_reference = DEFAULT;


//------------------------------------------------------------------------------
/**
 * Set the ADC prescalar factor.
 * @param[in] ps Prescalar bits.
 *  - ADC_PS_128: Arduino default (125 kHz on a 16 MHz CPU)
 *  - ADC_PS_64: (250 kHz on a 16 MHz CPU)
 *  - ADC_PS_32: (500 kHz on a 16 MHz CPU)
 */
void chAnalogPrescalarSTrans(uint8_t ps) {
  ADCSRA &= ~ADC_PS_BITS;
  ADCSRA |= ps;
}
//------------------------------------------------------------------------------
/**
 * Configures the reference voltage used for analog input
 * (i.e. the value used as the top of the input range). The options are:
 * @param[in] mode the ADC reference mode.
    - DEFAULT: the default analog reference of 5 volts (on 5V Arduino boards)
      or 3.3 volts (on 3.3V Arduino boards)
    - INTERNAL: an built-in reference, equal to 1.1 volts on the ATmega168 or
      ATmega328 and 2.56 volts on the ATmega8 (not available on the Arduino Mega)
    - INTERNAL1V1: a built-in 1.1V reference (Arduino Mega only)
    - INTERNAL2V56: a built-in 2.56V reference (Arduino Mega only)
    - EXTERNAL: the voltage applied to the AREF pin (0 to 5V only) is used
      as the reference.
*/
void chAnalogReferenceSTrans(uint8_t mode)
{
	// can't actually set the register here because the default setting
	// will connect AVCC and the AREF pin, which would cause a short if
	// there's something connected to AREF.
	nil_analog_reference = mode;
}

/**
 * Read the value from the specified analog pin.
 * @note nilAnalogRead() sleeps, if possible, while the ADC conversion
 *                       is in progress.
 * @param[in] pin the number of the analog input pin to read from (0 to 5 on
 *            most boards, 0 to 7 on the Mini and Nano, 0 to 15 on the Mega)
 *
 * @return pin ADC conversion value (0 to 1023).
 *
 */

int chAnalogReadSTrans(uint8_t pinP, uint8_t pinN, uint8_t gain) {
	#if defined(__AVR_ATmega32U4__)
	pinP = analogPinToChannel(pinP);
	pinN = analogPinToChannel(pinN);
	
	if(pinP == 1 && pinN == 0) {
		if (gain == 10 || gain == 200)
		{
			ADCSRB &= ~(1 << MUX5);
		}
		if (gain == 40)
		{
			ADCSRB |= (1 << MUX5);
		}
	}
	#endif

	// set the analog reference (high two bits of ADMUX) and select the
	// channel (low 4 bits).  this also sets ADLAR (left-adjust result)
	// to 0 (the default).
	#if defined(ADMUX)
	if (gain == 10)
	{
		ADMUX = (nil_analog_reference << 6) | (0x09);
	}
	if (gain == 40)
	{
		ADMUX = (nil_analog_reference << 6) | (0x06);
	}
	if (gain == 200)
	{
		ADMUX = (nil_analog_reference << 6) | (0x0B);
	}
	#endif

	if (!chIsIdleThread()) {
		// Not idle thread so use interrupt and sleep.
		ADCSRA |= (1 << ADIE) | (1 << ADSC);
		chSemWait(&adcSem);
    ADCSRA |= (1 << ADSC);
    chSemWait(&adcSem);
		ADCSRA &= ~(1 << ADIE);
		} else {
		ADCSRA |= (1 << ADSC);
		// ADSC is cleared when the conversion finishes
		while (ADCSRA & (1 << ADSC));
   ADCSRA |= (1 << ADSC);
   // ADSC is cleared when the conversion finishes
    while (ADCSRA & (1 << ADSC));
	}
	// this will access ADCL first.
	return ADC;
}

int chSetReadFreeRuningSTrans(uint8_t pinP, uint8_t pinN, uint8_t gain) {
  // Use AVCC as the reference for the ADC.
  SET(ADMUX, REFS0);
  CLEAR(ADMUX, REFS1);

  // Set the clock divider to 64x.
  // TODO: Experiment with reducing the divider when logic is in.
  ADCSRA &= ~ADC_PS_BITS;
  CLEAR(ADCSRA, ADPS0);
  SET(ADCSRA, ADPS1);
  SET(ADCSRA, ADPS2);

  // Specify that we are only interested in ADC1 and ADC0. x200
  SET(ADMUX, MUX0);
  SET(ADMUX, MUX1);
  CLEAR(ADMUX, MUX2);
  SET(ADMUX, MUX3);
  CLEAR(ADMUX, MUX4);

  CLEAR(ADCSRB,MUX5);

  

  // Set to free-running mode; the ADC will run continuously.
  CLEAR(ADCSRB, ADTS0);
  CLEAR(ADCSRB, ADTS1);
  CLEAR(ADCSRB, ADTS2);
  CLEAR(ADCSRB, ADTS3);

  if (!chIsIdleThread()) {
    //* Free running trigger source */
    ADCSRB = 0;
  
    /* Enable ADC and conversion complete interrupt. Trigger first conversion */
    ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADATE);
  
    /* Trigger first conversion */
    ADCSRA |= (1 << ADSC);
    chSemWait(&adcSem);
    SET(ADCSRA, ADSC);
    chSemWait(&adcSem);
    //CLEAR(ADCSRA, ADEN);
    } else {
    ADCSRA |= (1 << ADSC);
    // ADSC is cleared when the conversion finishes
    while (ADCSRA & (1 << ADSC));
    ADCSRA |= (1 << ADSC);
    // ADSC is cleared when the conversion finishes
    while (ADCSRA & (1 << ADSC));
  }
  return ADC;

  // Enable interrupts now everything is configured.
  // -------------------------------------------------------------------------
  //sei();
}
