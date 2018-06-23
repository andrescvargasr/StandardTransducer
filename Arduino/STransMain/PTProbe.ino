/************************************************************************/
/* Thread 2. Temperature PT100.                                         */
/************************************************************************/
/*
 * PT100.cpp
 *
 * Created: 21/06/2018 12:17:01 p. m.
 *  Author: camil
 */

/*
 * This code is the configuration of the ADC with
 * Free Running, ADC6 and ADC1 inputs in Differential
 * ADC, Prescaler 64 for 8 MHz clock and oversampling
 * 256 to change resolution 8-bits to 12-bits.
 *
 * The project is based on the ATMega32U4.
*/
#ifdef TEMP_SENSOR_R
#define THR_PT100


/************************************************************************/
/* CONSTANTS                                                            */
/************************************************************************/
#define VOPA_INIT DDRD |= (1<<PORTD6) // Set PORTD6 for output.

#define VOPA1     PORTD &= ~(1<<PORTD6) // Set On VOPA (P-MOSFET).
#define VOPA0     PORTD |= (1<<PORTD6)  // Set Off VOPA (P-MOSFET).

// ThPT100 sleeps while the ADC is active.

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <EEPROM.h>

//SEMAPHORE_DECL(adcSem, 0);

/* ADC6 & ADC1 are V+ & V- respectively input for differential ADC. 
 * Gain x200.
 */
//ADC1 x200 MUX5 1 | MUX[4..0] 11110
#define ADC_TEMP_MUXx200 0x1E
//ADC1 x40 MUX5 1 | MUX[4..0] 10110
#define ADC_TEMP_MUXx40 0x16
//ADC1 x10 MUX5 1 | MUX[4..0] 01110
#define ADC_TEMP_MUXx10 0x0E
// Oversampling 256
#define BUFFER_SIZE 257 // +1 because value 0 is not precise.

uint16_t adc_results[BUFFER_SIZE];
double adcR = 0;
volatile uint16_t adc_results_index = 0;
volatile uint8_t adc_conversion_done = 0;
//double temperature;

//EEPROM.write(PARAM_AT_MAG,40);
int a = setP(PARAM_AT_MAG,40);
int b = setP2(SHOW_MENU_HELP,1);
int showSerial;
//int a = setP(PARAM_AT_MAG, 10);
int paramATMag = eeprom_read_word(PARAM_AT_MAG);
//byte paramATMag = EEPROM.read(PARAM_AT_MAG);
//EEPROM.write(PARAM_AT_MAG,40);
//paramATMag = 
//EEPROM.put(PARAM_AT_MAG,40);
//EEPROM.get(PARAM_AT_MAG,paramATMag);

int setP(byte number, int value) {
  //The address of the parameter is given by : EE_START_PARAM+number*2
  eeprom_write_word((uint16_t*) 0 + number, value);
  return 1;
}

int setP2(int number, byte value) {
  //The address of the parameter is given by : EE_START_PARAM+number*2
  EEPROM.write(number, value);
  return 1;
}

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
  /* AVcc as reference, ADC6 & ADC0 as input */
  if (paramATMag == 200)
    ADMUX = (1 << REFS0) | (ADC_TEMP_MUXx200);
  else if (paramATMag == 40)
    ADMUX = (1 << REFS0) | (ADC_TEMP_MUXx40);
  else 
    ADMUX = (1 << REFS0) | (ADC_TEMP_MUXx10);

  /* Free running trigger source */
  ADCSRB = 0;
  
  /* MUX5 is set logic one for ADC6 & ADC0 */
  ADCSRB = (1<<MUX5);

  /* Enable ADC and conversion complete interrupt. Trigger first conversion */
  ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADATE);
  // Set prescalar to 64
  ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
  // Digital Input Disable for ADC6 & ADC 0
  DIDR0 &= ~((1<<ADC6D)&(1<<ADC0D));

  /* Trigger first conversion */
  ADCSRA |= (1 << ADSC);
}

//-----------------------------------------------------------------------------
// Declare a stack with 64 bytes beyond context switch and interrupt needs.
THD_WORKING_AREA(waPT100, 64);

// Declare thread function for thread 1.
THD_FUNCTION(ThPT100, arg) {
  (void)arg;
  //paramATMag = EEPROM.read(PARAM_AT_MAG);
  //showSerial = EEPROM.read(SHOW_MENU_HELP*2);
  paramATMag = eeprom_read_word(PARAM_AT_MAG);
  showSerial = eeprom_read_word((const uint16_t *) 0 + SHOW_MENU_HELP);
  //Serial.begin(9600);
  temperature = 1;

  while (TRUE) {
    chSemWait(&oneSlot);
    //showSerial = EEPROM.read(SHOW_MENU_HELP*2);
    paramATMag = eeprom_read_byte((uint8_t )PARAM_AT_MAG*2);
    //showSerial = eeprom_read_byte((uint8_t )SHOW_MENU_HELP*2);
    showSerial = eeprom_read_word((const uint16_t *)(SHOW_MENU_HELP*2));
    Serial.println("constant:");
    Serial.println(SHOW_MENU_HELP);
    Serial.println("show:");
    Serial.println(showSerial);
    
    /* Set VOPA pin as output and LOW for P-MOSFET in AVCC to AVCCOPA */
    VOPA_INIT;
    VOPA1;
    
    /* Enable interrupts */
    //sei();
  
    /* Do BUFFER_SIZE conversions using the free running mode */
    setup_adc_freerunning_mode();
  
    /* Wait for the conversion to complete */    
    chSemWait(&adcSem);
    
    /* Set off VOPA */
    VOPA0;

    //temperature++;

    adcR = 0;
    for(int i = 1; i < BUFFER_SIZE; i++) {
      adcR += adc_results[i];
    }

    adcR = adcR/(BUFFER_SIZE-1);
    //Serial.print("Pressure: ");
    float vin = adcR*(3310.0)/((float)paramATMag*512.0);

    if (showSerial == 1) {
      Serial.println("Mag: ");
      Serial.println(paramATMag);
      Serial.println("vin: ");
      Serial.println(vin);
    }

    float A = 3.908e-3;
    float B = -5.775e-7;
    float R0 = 100.0;
    float Is = 124e-6;
    temperature = (1000.0*R0*Is);
    temperature = 1.0-vin/temperature;
    temperature = A*A-4.0*B*temperature;
    temperature = sqrt(temperature);
    temperature = -A+temperature;
    temperature = temperature/(2.0*B);
    //temperature = (1-vin/(1000*R0*Is));
    //temperature = (-A+sqrt(A*A-4*B*(1-vin/(R0*Is))))/(2*B);
    if (showSerial == 1) {
      Serial.print("vin: ");
      Serial.println(vin);
      Serial.print(temperature,8);
      Serial.println(" °T");
    }
   
    /* Reset conversion done flag */
    adc_conversion_done = 0;
//
    chSemSignal(&oneSlot);
    chThdSleepSeconds(10);
    
  }
}
#endif // TEMP_SENSOR
