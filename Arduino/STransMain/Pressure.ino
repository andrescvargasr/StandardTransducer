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

#include <avr/interrupt.h>
#include <avr/eeprom.h>

/* Use Sleep modes like ADC Reduction Noise Mode */
#include <avr/sleep.h>

// Semaphore declare adcSem in StransMain.

/* Pressure sensor is MPX53D
 * Pressure range: 0 - 50 kPa.
 * Full scale span: typ. 60 mV.
 * Offset: typ. 20 mV.
 * Sensitivity (Vcc 3.0 Vdc): 1.2 mV/kPa.
 */


// ADC1 & ADC0 are V+ & V- respectively input for differential ADC. 
// Gain x200 MUX5 0 | MUX[4..0] 01011
//#define ADC_P_MUXx200 0x0B
// Gain x40 MUX5 1 | MUX[4..0] 00110
#define ADC_P_MUXx40 0x06
// Gain x10 MUX5 0 | MUX[4..0] 01001
//#define ADC_P_MUXx10 0x09
// Oversampling 256
#define BUFFER_SIZE 257 // +1 because value 0 is not precise.

// adc_results is use in Pressure and PT100 both.
//uint16_t adc_results[BUFFER_SIZE];

/* Counter for number of ADC read. */
int counter = 0;

/* BUFFER_SIZE is more that 127, create two parameters to save ADC lecture */
volatile uint16_t adcResult1 = 0;
volatile uint16_t adcResult2 = 0;

/* Test last ADC result */
volatile uint16_t lastADC = 0;

/* Test Pressure Offset */
float pressureOffset = 0.0;

int showSerial;
//int paramATMag = eeprom_read_word(PARAM_P_MAG);
int paramATMag = 40;

/* Set ADC Reduction Noise Mode sleep_cpu */
volatile bool adcEnd = false;

ISR(ADC_vect)
{
  CH_IRQ_PROLOGUE();          
  chSysLockFromISR();
  if (counter > 0) {
    /* Test */
    lastADC = ADC;
    if (counter < 128) {
      adcResult1 += lastADC;
    }
    else {
      adcResult2 += lastADC;
    }
  }

  counter++;
  if (counter >= BUFFER_SIZE) {
    counter = 0;
    adcEnd = true;
    /* Disable ADC and clear interrupt flag. we are done */
    //ADCSRA = 0;
    /* ADC Reduction Noise Mode */
    sleep_disable();
    ADCSRA = (1 << ADIF);
    chSemSignalI(&adcSem);
  }
  chSysUnlockFromISR();
  CH_IRQ_EPILOGUE();
}

static void setup_adc_freerunning_mode(void)
{
  /* AVcc as reference, ADC1 & ADC0 as input */
  //if (paramATMag == 200)
    //ADMUX = (1 << REFS0) | (ADC_P_MUXx200);
  //else if (paramATMag == 40)
    //ADMUX = (1 << REFS0) | (ADC_P_MUXx40);
    ADMUX = 0 << REFS1 | 1 << REFS0 | ADC_P_MUXx40;
  //else
    //ADMUX = (1 << REFS0) | (ADC_P_MUXx10);

  /* Free running trigger source */
  ADCSRB = 0;

  /* MUX5 is set logic one for ADC6 & ADC0 */
  //if (paramATMag == 40)
    ADCSRB = (1<<MUX5);

  /* Enable ADC and conversion complete interrupt. Trigger first conversion */
  //ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADATE);
  ///* Set prescaler to 64 for 8 MHz clock source. */
  //ADCSRA |= (1<<ADPS2)|(1<<ADPS1);
  //ADCSRA &= ~(1<<ADPS0);
  
  ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADATE) | (1 << ADPS2) | (1 << ADPS1) | (0 << ADPS0);

  /* Digital Input Disable for ADC1 & ADC0. */
  DIDR0 |= (1<<ADC1D) | (1<<ADC0D);
  
  /* If we use the ADC Reduction Noise Mode, It's not necessary start conversion. */
  /* Trigger first conversion */
  //ADCSRA |= (1 << ADSC);
}

//-----------------------------------------------------------------------------
// Declare a stack with 64 bytes beyond context switch and interrupt needs.
THD_WORKING_AREA(waPressure, 32);

// Declare thread function for thread 1.
THD_FUNCTION(ThPressure, arg) {
  (void)arg;
  
  /* Read PARAM_P_MAG and SHOW_MENU_HELP values. */
  //paramATMag = eeprom_read_word(PARAM_P_MAG*2);
  //showSerial = eeprom_read_word(HOW_MENU_HELP*2);
  // Only is necessary read first byte.
  //paramATMag = eeprom_read_byte((uint8_t )PARAM_P_MAG*2);
  //showSerial = eeprom_read_byte((uint8_t )SHOW_MENU_HELP*2);

  /* Test */
//  paramATMag = 40;
//  showSerial =  1;
  
  while (TRUE) {
    chSemWait(&oneSlot);
    
    /* Read Gain and show Serial. */
    //paramATMag = eeprom_read_byte((uint8_t )PARAM_P_MAG*2);
    showSerial = eeprom_read_byte((uint8_t )PARAM_SERIAL_DISPLAY*2);
    //showSerial = eeprom_read_word((const uint16_t *)(SHOW_MENU_HELP*2));

    /* Test */
    //showSerial = 1;
    
    if (showSerial == 1) {
      Serial.println("constant:");
      Serial.println(PARAM_SERIAL_DISPLAY);
      Serial.println("show:");
      Serial.println(showSerial);
    }
    /* end Test */
    
    /* ADC Reduction Noise Mode */
    set_sleep_mode(SLEEP_MODE_ADC);
    
    /* Do BUFFER_SIZE conversions using the free running mode */
    setup_adc_freerunning_mode();
    
    /* ADC Reduction Noise Mode */
    sleep_enable();
    /* Enable interrupts */
    sei();
    
    while(!adcEnd) {
      sleep_cpu();
    }
    adcEnd = false;
  
    /* Wait for the conversion to complete */    
    chSemWait(&adcSem);
    
    /* Test */
    if (showSerial == 1) {
      Serial.print("lastADC: ");
      Serial.println(lastADC, HEX);
      Serial.println(lastADC);
    }
    /* end Test */
    
    adcResult1 = (adcResult1>>7); // Division for 128.
    adcResult2 = (adcResult2>>7); // Division for 128.
    
    if (showSerial == 1) {
      Serial.print("1 prom: ");
      Serial.println(adcResult1, HEX);
      Serial.println(adcResult1);
      Serial.print("2 prom: ");
      Serial.println(adcResult2, HEX);
      Serial.println(adcResult2);
      adcResult1 = (adcResult1 + adcResult2);
      adcResult1 >>= 1;  // Division for 2.
      Serial.print("Prom: ");
      Serial.println(adcResult1, HEX);
      Serial.println(adcResult1);
    }
    
    float pressureValue = adcResult1;
    float vin = pressureValue*(3310);
    float d = paramATMag*512;
    vin = vin / d;
    d *= 0.4;
    pressure = (pressureValue * 1000) / d;
    
    /* Define PressureOffset */
    if (vin < 21.0) {
      pressureOffset = pressure;
      //eeprom_write_word((uint16_t*) EE_START_PARAM + FLAG_ENABLED, flagEnabled);
      //eeprom_write_float ((float*)PARAM_P_OFFSET, pressureOffset);
      //eeprom_write_word((uint16_t*)PARAM_P_OFFSET, pressureOffset);
      setAndSaveParameter(PARAM_P_OFFSET, pressureOffset);
    }
    
    /* Delete PressureOffset */
    pressure -= pressureOffset;

    if (showSerial == 1) {
      Serial.println("Mag: ");
      Serial.println(paramATMag);
      Serial.print("Vin: ");
      Serial.print(vin);    
      Serial.println(" mV");
      Serial.print("Pressure: ");
      Serial.print(pressure,2);
      Serial.println(" kPa");
      Serial.print("Pressure Offset: ");
      Serial.print(getParameter(PARAM_P_OFFSET));
      Serial.println(" kPa");
    }
    
    adcResult1 = 0;
    adcResult2 = 0;

    //char cSREG = SREG;
    chSemSignal(&oneSlot);
    //SREG = cSREG;
    chThdSleepSeconds(10);
    
  }
}

#endif  // _PRESSURE_SENSOR_
