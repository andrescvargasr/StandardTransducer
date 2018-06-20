// Test chAnalogRead to verify execution of idle loop. 
// chAnalogRead sleeps while the ADC is active.

#define F_CPU 8000000UL

#include "ChNil.h"
#include "ChNilAdcStrans.h"

#include <avr/io.h>
#include <avr/interrupt.h>

// Use tiny unbuffered ChNilSerial library.
// Macro to redefine Serial as ChNilSerial.
// Remove definition to use standard Arduino Serial.
//ChNilSerialClass ChNilSerial;
//#define Serial ChNilSerial

// count of calls to loop(), the idle thread.
volatile uint16_t idleCount;
//-----------------------------------------------------------------------------
// Declare ADC semaphore and IRQ.
CH_NIL_ADC_IRQ
//-----------------------------------------------------------------------------
// Declare a stack with 64 bytes beyond context switch and interrupt needs.
THD_WORKING_AREA(waThread1, 64);

// Declare thread function for thread 1.
THD_FUNCTION(Thread1, arg) {
  (void)arg;
  Serial.println("         usec val count");
  while (TRUE) {
    idleCount = 0;
    uint32_t usec = micros();
    int val = analogRead(0);
    int over = 0;
    int adc0 = 0;
    usec = micros() - usec; 
    Serial.print("Arduino:  ");
    Serial.print(usec);
    Serial.print(' ');
    Serial.print(val);
    Serial.print(' ');
    Serial.println(idleCount);
 
    idleCount = 0;    
    usec = micros();
    chAnalogReferenceSTrans(1);
    chAnalogPrescalarSTrans(6);
    val = chAnalogReadSTrans(4, 5, 200);
    usec = micros() - usec;
    Serial.print("ChNilAdc: ");
    Serial.print(usec);
    Serial.print(' ');
    Serial.print(val);
    Serial.print(' ');
    Serial.println(idleCount);

//    val = ADC;
//    usec = micros() - usec;
//    Serial.print("ChNilAdc: ");
//    Serial.print(usec);
//    Serial.print(' ');
//    Serial.print(val);
  
    over = chSetReadFreeRuningSTrans(4, 5, 200);
    Serial.print("ChNilAdcFreeRunning1: ");
    Serial.print(over);
    Serial.print(' ');    
    Serial.print(adc0);
    Serial.print(' ');
    Serial.println();

    over = 0;
//    int clo = 14/125000;
    for(int i=0; i<128; i++) {
      SET(ADCSRA, ADSC);
      //SET(ADCSRA, ADIF);
      chSemWait(&adcSem);
      over += ADC;
      
    }
    over /= 128;
    Serial.print("ChNilAdcFreeRunning2: ");
    Serial.print(over);
    Serial.print(' ');    
    Serial.print(adc0);
    Serial.print(' ');


    Serial.println();
    chThdSleep(1000);    
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
  // Disable interrupts to insure increment is atomic.
  chSysLock();
  idleCount++;
  chSysUnlock();
}
