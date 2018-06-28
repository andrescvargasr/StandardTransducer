/************************************************************************/
/* Thread 1. Pressure.                                                  */
/************************************************************************/

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

#ifdef PRESSURE_SENSOR
#define THR_PRESSURE

// Test chAnalogRead to verify execution of idle loop. 
// chAnalogRead sleeps while the ADC is active.

// ThPressure sleeps while the ADC is active.

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

// Semaphore declare adcSem is in StransMain.

// ADC1 & ADC0 are V+ & V- respectively input for differential ADC. 
// Gain x200 MUX5 0 | MUX[4..0] 01011
#define ADC_P_MUXx200 0x0B
// Gain x40 MUX5 1 | MUX[4..0] 00110
#define ADC_P_MUXx40 0x06
// Gain x10 MUX5 0 | MUX[4..0] 01001
#define ADC_P_MUXx10 0x09
// Oversampling 256
#define BUFFER_SIZE 257 // +1 because value 0 is not precise.

// adc_results is use in Pressure and PT100 both.
//uint16_t adc_results[BUFFER_SIZE];
int counter = 0;
volatile uint16_t adcR = 0;
volatile uint16_t adc_results_index = 0;
volatile uint8_t adc_conversion_done = 0;

//float pressure is define in StransMain
//volatile double pressureADC;

int showSerial;
//int paramATMag = eeprom_read_word(PARAM_P_MAG);
int paramATMag = 40;

ISR(ADC_vect)
{
  CH_IRQ_PROLOGUE();          
  chSysLockFromISR();
  if (counter > 0)
    adcR += ADC;

  counter++;
  if (counter >= BUFFER_SIZE) {
    counter = 0;
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
  /* AVcc as reference, ADC6 & ADC0 as input */
  if (paramATMag == 200)
    ADMUX = (1 << REFS0) | (ADC_P_MUXx200);
  else if (paramATMag == 40)
    ADMUX = (1 << REFS0) | (ADC_P_MUXx40);
  else
    ADMUX = (1 << REFS0) | (ADC_P_MUXx10);

  /* Free running trigger source */
  ADCSRB = 0;

  /* MUX5 is set logic one for ADC6 & ADC0 */
  if (paramATMag == 40)
    ADCSRB = (1<<MUX5);

  /* Enable ADC and conversion complete interrupt. Trigger first conversion */
  ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADATE);
  /* Set prescaler to 64. */
  ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);

  /* Digital Input Disable for ADC1 & ADC 0. */
  //DIDR0 &= ~((1<<ADC1D)&(1<<ADC0D));

  /* Trigger first conversion */
  ADCSRA |= (1 << ADSC);
}

//-----------------------------------------------------------------------------
// Declare a stack with 64 bytes beyond context switch and interrupt needs.
THD_WORKING_AREA(waPressure, 64);

// Declare thread function for thread 1.
THD_FUNCTION(ThPressure, arg) {
  (void)arg;
  
  paramATMag = eeprom_read_word(PARAM_P_MAG);
  showSerial = eeprom_read_word((const uint16_t *) 0 + SHOW_MENU_HELP);
  //Serial.begin(9600);
  pressure = 1;

  /* Test */
//  paramATMag = 40;
//  showSerial =  1;

  
  
  while (TRUE) {
    chSemWait(&oneSlot);
    
    /* Read Gain. */
    paramATMag = eeprom_read_byte((uint8_t )PARAM_P_MAG*2);
    showSerial = eeprom_read_byte((uint8_t )SHOW_MENU_HELP*2);
    //showSerial = eeprom_read_word((const uint16_t *)(SHOW_MENU_HELP*2));

    /* Test */
    //showSerial = 1;
    
    Serial.println("constant:");
    Serial.println(SHOW_MENU_HELP);
    Serial.println("show:");
    Serial.println(showSerial);
  
    /* Do BUFFER_SIZE conversions using the free running mode */
    setup_adc_freerunning_mode();
  
    /* Wait for the conversion to complete */    
    chSemWait(&adcSem);

    uint16_t pressureValue = adcR;
    adcR = 0;
    pressureValue /= (BUFFER_SIZE - 1);
    float vin = pressureValue*(3310.0);
    float d = (float)paramATMag*512.0;
    vin = vin / d;
    d = (512.0*paramATMag*0.4);
    pressure = (float)pressureValue / d;

    if (showSerial == 1) {
      Serial.println("Mag: ");
      Serial.println(paramATMag);
      Serial.print("Vin: ");
      Serial.print(vin);    
      Serial.println(" mV");
      Serial.print("Pressure: ");
     
      Serial.print(pressure,2);
      Serial.println(" kPa");
    }
  
    /* Reset conversion done flag */
    adc_conversion_done = 0;

    //char cSREG = SREG;
    chSemSignal(&oneSlot);
    //SREG = cSREG;
    chThdSleepSeconds(10);
    
  }
}

#endif  // _PRESSURE_SENSOR_
