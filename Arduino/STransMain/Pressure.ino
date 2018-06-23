/************************************************************************/
/* Thread 1. Pressure.                                                  */
/************************************************************************/
#ifdef PRESSURE_SENSOR

#define THR_PRESSURE
/*
 * Pressure.cpp
 *
 * Created: 20/06/2018 1:17:01 p. m.
 *  Author: camil
 */ 

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
#include <avr/io.h>
#include <avr/interrupt.h>
//Beginning of Auto generated function prototypes by Atmel Studio
//ISR(ADC_vect );
//void setup_adc_freerunning_mode(void );
//THD_FUNCTION(Thread1,arg );
//End of Auto generated function prototypes by Atmel Studio

//SEMAPHORE_DECL(adcSem, 0);


/* ADC0 & ADC1 are V+ & V- respectively input for differential ADC. 
 * Gain x10.
 */
#define ADC_MUX_SETTING 0x09
// Oversampling 256
#define BUFFER_SIZE 257 // +1 because value 0 is not precise.

uint16_t adc_results[BUFFER_SIZE];
uint16_t adcR = 0;
volatile uint16_t adc_results_index = 0;
volatile uint8_t adc_conversion_done = 0;
volatile double pressure;

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
THD_WORKING_AREA(waPressure, 64);

// Declare thread function for thread 1.
THD_FUNCTION(ThPressure, arg) {
  (void)arg;

  //Serial.begin(9600);
//  while (!Serial) {
//    chThdSleepMilliseconds(1); // wait for serial port to connect. Needed for native USB port only
//  }
  
  while (TRUE) {
    chSemWait(&oneSlot);
    /* Enable interrupts */
    sei();
  
    /* Do BUFFER_SIZE conversions using the freerunning mode */
    setup_adc_freerunning_mode();
  
    /* Wait for the conversion to complete */    
    chSemWait(&adcSem);

    adcR = 0;
    for(int i = 1; i < BUFFER_SIZE; i++) {
      //Serial.println(adc_results[i]);
      adcR += adc_results[i];
      //Serial.println(adcR);
    }
    
    //Serial.println(adcR);
    adcR = adcR/(BUFFER_SIZE - 1);
    Serial.print("Pressure: ");
    double vin = (double)adcR*3300/(10*512);
    Serial.print(adcR);
    Serial.println(" kPa");
    Serial.print(vin, 1);
    Serial.println(" mV");
    pressure = (double)adcR/(5120*0.4);
    Serial.print(pressure);
    Serial.println(" kPa");
  
    /* Reset conversion done flag */
    adc_conversion_done = 0;

    char cSREG = SREG;
    chSemSignal(&oneSlot);
    SREG = cSREG;
    chThdSleepSeconds(10);
    
  }
}

#endif  // _PRESSURE_
