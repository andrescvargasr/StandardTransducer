/*
 * ChNilAdcSTrans.h
 *
 * Created: 5/06/2018 3:28:30 p. m.
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
 * @file    ChNilAdcSTrans.h
 * @brief   ChNilAdcSTrans read ADC Single/Differential Input on ATMEGA32U4
 *
 * @defgroup ChNilAdc Read AVR ADC
 * @details ChNilAdc functions.
 * @{
 */

#ifndef CHNILADCSTRANS_H_
#define CHNILADCSTRANS_H_
#define CH_NIL_ADC_IRQ        \
SEMAPHORE_DECL(adcSem, 0);    \
CH_IRQ_HANDLER(ADC_vect) {    \
  CH_IRQ_PROLOGUE();          \
  chSysLockFromISR();         \
  chSemSignalI(&adcSem);      \
  chSysUnlockFromISR();       \
  CH_IRQ_EPILOGUE();          \
}
extern semaphore_t adcSem;

#define SET(register, bit) register |= _BV(bit)
#define CLEAR(register, bit) register &= ~_BV(bit)

void chAnalogPrescalarSTrans(uint8_t ps);	// Configure ADPS[2..0] in ADCSRA
int chAnalogReadSTrans(uint8_t pinP, uint8_t pinN, uint8_t gain);	// Differencial Input
void chAnalogReferenceSTrans(uint8_t mode);
int chSetReadFreeRuningSTrans(uint8_t, uint8_t, uint8_t);
#endif /* CHNILADCSTRANS_H_ */
/** @} */
