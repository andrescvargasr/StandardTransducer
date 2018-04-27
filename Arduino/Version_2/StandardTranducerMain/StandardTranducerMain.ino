/*
 * StandardTranducerMain.cpp
 *
 * Created: 21/04/2018 11:12:48 a. m.
 *  Author: camil
 */

#include "StandardTranducerMain.h"
#include <util/delay.h>
#include "Parameters.h"

/************************************************************************/
/* SETUP                                                                */
/************************************************************************/

void setup() {
  _delay_ms(1000);
  Serial.begin(9600);
  _delay_ms(1000);

  setupParameters();  // Download parameters saved in the EEPROM

  chFillStacks();
  // Start ChiNil RTOS
  chBegin();
}

void loop() {}

////Global Thread Locking
//bool lockTimeCriticalZone = false;
//void protectThread() {
  //while (lockTimeCriticalZone) {
    //chThdSleepMilliseconds(5);
  //}
  //lockTimeCriticalZone = true;
//}
//
//void unprotectThread() {
  //lockTimeCriticalZone = false;
//}
