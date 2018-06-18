/*
 * Parameters.cpp
 *
 * Created: 15/06/2018 10:13:22 a. m.
 *  Author: camil
 */ 

/*********************************************
   This file is used to declare the parameters
   table used by the program.

   We use the EEPROM for saving the parameters
   changed by the user during the functionment
   of the Bioreactor.

   The parameter are loaded during the boot.

   All change to important parameters are saved
   to the EEPROM
 *********************************************/

#include <avr/eeprom.h>
#include "Arduino.h"
#include "ChNil.h"
//#include "BioMain.h"

#define MAX_PARAM 52   // If the MAX_PARAM change you need to change the pointer in the EEPROM

// Definition of all events to be logged
#define EVENT_STATUS_ENABLE             3
#define EVENT_STATUS_DISABLE            4

#define EVENT_ERROR_FAILED              6
#define EVENT_ERROR_RECOVER             7

#define EVENT_ERROR_NOT_FOUND_ENTRY_N  150

#define EVENT_SAVE_ALL_PARAMETER     255
//When parameters are set (and saved) an event is recorded (256-281 : A-Z + .... (if more parameters than 262 ...)
#define EVENT_PARAMETER_SET          256




#define EE_LORA_DEVADDR          150  // 8 bytes
#define EE_LORA_NWKSKEY          158  // 32 bytes
#define EE_LORA_APPSKEY          190  // 32 bytes
#define EE_QUALIFIER             222


#define EE_START_PARAM           0 // We save the parameter from byte 0 of EEPROM
#define EE_LAST_PARAM            (MAX_PARAM*2-1) // The last parameter is stored at byte 50-51

#define EEPROM_MIN_ADDR            0
#define EEPROM_MAX_ADDR          511

// value that should not be taken into account
// in case of error the parameter is set to this value
#define ERROR_VALUE  -32768

int parameters[MAX_PARAM];

void printParameter(Print* output, byte number) {
  output->print(number);
  output->print("-");
  if (number > 25) {
    output->print((char)(floor(number / 26) + 64));
    } else {
    output->print(" ");
  }
  output->print((char)(number - floor(number / 26) * 26 + 65));
  output->print(": ");
  output->println(parameters[number]);
}

/*
  This will take time, around 4 ms
  This will also use the EEPROM that is limited to 100000 writes
*/
void setAndSaveParameter(byte number, int value) {
  parameters[number] = value;
  //The address of the parameter is given by : EE_START_PARAM+number*2
  eeprom_write_word((uint16_t*) EE_START_PARAM + number, value);
}
