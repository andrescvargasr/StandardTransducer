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
   of the STrans.

   The parameter are loaded during the boot.

   All change to important parameters are saved
   to the EEPROM
 *********************************************/

#include <avr/eeprom.h>
#include "Arduino.h"
#include "ChNil.h"
#include "STransMain.h"

#define MAX_PARAM 26   // If the MAX_PARAM change you need to change the pointer in the EEPROM

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

/*
  This will take time, around 4 ms
  This will also use the EEPROM that is limited to 100000 writes
*/
void setAndSaveParameter(byte number, int value) {
  parameters[number] = value;
  //The address of the parameter is given by : EE_START_PARAM+number*2
  eeprom_write_word((uint16_t*) EE_START_PARAM + number, value);
}

void printParameters(Print* output) {
  for (byte i = 0; i < MAX_PARAM; i++) {
    printParameter(output, i);
  }
}

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

int getParameter(byte number) {
  return parameters[number];
}

boolean getParameterBit(byte number, byte bitToRead) {
  // return (bitRead(parameters[number], bitToRead) == 1) ? true : false;
  return (parameters[number] >> bitToRead ) & 1;
}

#define ERROR_VALUE  -32768



void resetParameters() {
  //int flagEnabled = eeprom_read_word((uint16_t*) EE_START_PARAM + FLAG_ENABLED);
  
  // Flag Enabled tiene 2 bytes de banderas
  int flagEnabled = 0x0000;
  // Se configuran tanto las banderas como los parámetros en su valor por defecto
  #ifdef ANALOG_SENSORS
    // La presión es la variable que se mide por defecto
    flagEnabled |= (1 << FLAG_EN_PRESSURE_CTRL);
    setAndSaveParameter(PARAM_PRESSURE, 0);
    
    flagEnabled |= (0 << FLAG_EN_ANAG_TEMP_CTRL);
    setAndSaveParameter(PARAM_ANALOG_TEMP, ERROR_VALUE);
  #endif

  #ifdef ONE_WIRE_SENSORS
    // No hay ningún sensor digital one-wire habilitado por defecto
    flagEnabled |= (0 << FLAG_DIG_TEMP_1_CTRL);
    flagEnabled |= (0 << FLAG_DIG_TEMP_2_CTRL);
    flagEnabled |= (0 << FLAG_DIG_TEMP_3_CTRL);
    flagEnabled |= (0 << FLAG_DIG_TEMP_4_CTRL);
    flagEnabled |= (0 << FLAG_DIG_HUMI_1_CTRL);
    
    setAndSaveParameter(PARAM_DIGITAL_TEMP_1, ERROR_VALUE);
    setAndSaveParameter(PARAM_DIGITAL_TEMP_2, ERROR_VALUE);
    setAndSaveParameter(PARAM_DIGITAL_TEMP_3, ERROR_VALUE);
    setAndSaveParameter(PARAM_DIGITAL_TEMP_4, ERROR_VALUE);
    setAndSaveParameter(PARAM_DIGITAL_HUMI_1, ERROR_VALUE);
  #endif

  #ifdef OLED
    // La pantalla OLED está disponible por defecto
    flagEnabled |= (1 << FLAG_OLED);
    
    setAndSaveParameter(PARAM_OLED, 1);
  #endif
  
  #ifdef BUTTONS
    // Todos los botones están habilitados por defecto
    flagEnabled |= (1 << FLAG_BUTTONS);
    
    setAndSaveParameter(PARAM_BUTTONS, MASK_BUTTONS);
  #endif

  #ifdef WIRELESS_COMM
    // La comunicación LoRa por el módulo Ra-1 está habilitada por defecto
    flagEnabled |= (0 << FLAG_BLUETOOTH)|(1 << FLAG_LORA_RA1)|(0 << FLAG_LORA_RN2483)|(0 << FLAG_LORA_RN2903);
    
    setAndSaveParameter(PARAM_WIRELESS, MASK_WIRELESS & (1 << PARAM_LORA_RA1);  // LoRa Ra-1 Enabled by default
  #endif
  
  // Almacenar el nuevo dato de la bandera de habilitación
  eeprom_write_word((uint16_t*) EE_START_PARAM + FLAG_ENABLED, flagEnabled);
}


