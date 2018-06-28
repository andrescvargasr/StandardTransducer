/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */

/*
 * This code is the configuration of the ADC with
 * Free Running, ADC1 and ADC0 inputs in Differential
 * ADC, Prescaler 64 for 8 MHz clock and oversampling
 * 256 to change resolution 8-bits to 12-bits.
 *
 * The project is based on the ATMega32U4.
*/

// Test chAnalogRead to verify execution of idle loop. 
// chAnalogRead sleeps while the ADC is active.

#define F_CPU 8000000UL

#include "ChNil.h"

#include <avr/io.h>
#include <avr/interrupt.h>
//Beginning of Auto generated function prototypes by Atmel Studio
ISR(ADC_vect );
void setup_adc_freerunning_mode(void );
THD_FUNCTION(Thread1,arg );
//End of Auto generated function prototypes by Atmel Studio

SEMAPHORE_DECL(adcSem, 0);


/* ADC1 & ADC0 are V+ & V- respectively input for differential ADC. */
#define ADC_MUX_SETTING 0x09
// Oversampling 256
#define BUFFER_SIZE 257 // +1 because value 0 is not precise.

uint16_t adc_results[BUFFER_SIZE];
uint16_t adcR = 0;
volatile uint16_t adc_results_index = 0;
volatile uint8_t adc_conversion_done = 0;

ISR(ADC_vect)
{
  CH_IRQ_PROLOGUE();          
  chSysLockFromISR();         
  adc_results[adc_results_index++] = ADC;

  if (adc_results_index >= BUFFER_SIZE) {
    adc_results_index = 0;
    adc_conversion_done = 1;
    /* Disable ADC and clear interrupt flag. we are done */
    ADCSRA = 0;
    ADCSRA = (1 << ADIF);
    chSemSignalI(&adcSem);
  }
  chSysUnlockFromISR();
  CH_IRQ_EPILOGUE();
}

static void setup_adc_freerunning_mode(void)
{
  /* AVcc as reference, ADC1 & ADC0 as input */
  ADMUX = (1 << REFS0) | (ADC_MUX_SETTING);

  /* Free running trigger source */
  ADCSRB = 0;

  /* Enable ADC and conversion complete interrupt. Trigger first conversion */
  ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADATE);
  // Set prescalar to 64
  ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);

  /* Trigger first conversion */
  ADCSRA |= (1 << ADSC);
}

//-----------------------------------------------------------------------------
// Declare a stack with 64 bytes beyond context switch and interrupt needs.
THD_WORKING_AREA(waThread1, 64);

// Declare thread function for thread 1.
THD_FUNCTION(Thread1, arg) {
  (void)arg;

  while (TRUE) {
    /* Enable interrupts */
    sei();
  
    /* Do BUFFER_SIZE conversions using the freerunning mode */
    setup_adc_freerunning_mode();
  
      /* Wait for the conversion to complete */
    //while (!adc_conversion_done);
    
    chSemWait(&adcSem);

    adcR = 0;
    for(int i = 1; i < BUFFER_SIZE; i++) {
      Serial.println(adc_results[i]);
      adcR += adc_results[i];
      Serial.println(adcR);
    }

    Serial.println(adcR);
    adcR = adcR/(BUFFER_SIZE - 1);
    Serial.println(adcR);

    Serial.print("Vin: ");
    float vin = adcR*(3310.0)/((float)10*512.0);
    Serial.print(vin);    
    Serial.println(" mV");
    float pressure = (float)adcR/(5120.0*0.4);
    Serial.print(pressure,8);
    Serial.println(" kPa");

    delay(1000);
  
//    for(int i = 0; i < BUFFER_SIZE; i++) {
//      Serial.println(adc_results[i]);
//    }
  
    /* Reset conversion done flag */
    adc_conversion_done = 0;
    
  }
}
//------------------------------------------------------------------------------
/*
 * Threads static table, one entry per thread.  A thread's priority is
 * determined by its position in the table with highest priority first.
 *
 * These threads start with a null argument.  A thread's name is also
 * null to save RAM since the name is currently not used.
 */
THD_TABLE_BEGIN
  THD_TABLE_ENTRY(waThread1, NULL, Thread1, NULL)
THD_TABLE_END
//------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  chBegin();
}
//------------------------------------------------------------------------------
// Loop is the idle thread.  The idle thread must not invoke any
// kernel primitive able to change its state to not runnable.
void loop() {
}

