/*
 * StandardTranducerMain.h
 *
 * Created: 21/04/2018 11:13:55 a. m.
 *  Author: camil
 */ 


#ifndef STANDARDTRANDUCERMAIN_H_
#define STANDARDTRANDUCERMAIN_H_

/************************************************************************/
/* LIBRARIES                                                            */
/************************************************************************/

#include <ChNil.h>    // RTOS MultiThread   (ChNil_h)
#include <SPI.h>    //Flash SPI       (_SPI_H_INCLUDED)
#include <avr/wdt.h>  //Watchdog        (_AVR_WDT_H_)

#define SHOW_MENU_HELP 1 // if we don't show the help we spare a lot of memory
/************************************************************************/
/* CARACTERISTICS                                                       */
/************************************************************************/
#define PRESSURE_SENSOR   // MPX53D pressure sensor
#define TEMP_SENSOR     // PT100 temperature sensor
#define ONE_WIRE_SENSORS  // DS18B20 temperature sensor
#define LCD         // 128x32 OLED display with I2C communication
#define BUTTONS       // Resistive buttons
#define BLUETOOTH     // BLE
#define LORA
#define LORA_RA1      // LoRa Ra-01 communication module
#define LORA_RN2483     // RN2483 communication module

#ifdef PRESSURE_SENSOR || TEMP_SENSOR
  #define ANALOG_SENSORS  // Analog sensors is present
#endif

#ifdef BLUETOOTH || LORA
  #define WIRELESS_COMM // Wireless communication is available
#endif

#define BATTERY_SENSOR    // Check the level of battery
//#define CONTROL_REGULATOR // Enable 3V3 regulator for external modules

/************************************************************************/
/* PIN CONFIGURATION                                                    */
/************************************************************************/
#ifdef PRESSURE_SENSOR
  #define PRESSURE_H    40  // Differential ADC: positive (ADC1)
  #define PRESSURE_L    41  // Differential ADC: negative (ADC0)
#endif

#ifdef BATTERY_SENSOR
  #define VBAT      39  // Voltage level of battery
#endif

#ifdef CONTROL_REGULATOR
  #define REG3V3      38  // Control pin for 3.3 V regulator (NOT USED)
#endif

#ifdef TEMP_SENSOR
  #define VANALOG     37  // Single-ended ADC: (ADC6)
#endif

//#define A0          36  // (NOT USED)

#ifdef WIRELESS_COMM
  #define W_COMM      1 // Wireless communication: LoRa/Bluetooth
  #define GND_COMM    27  // Enable GND for wireless communication
#endif

#ifdef BUTTONS
  #define Z_P       31  // Z+ button
  #define S_P       30  // S+ button
  #define Z_N       29  // Z- button
  #define S_N       28  // S- button
#endif

#ifdef ONE_WIRE_SENSORS
  #define ONE_WIRE    32  // One-wire communication
  #define EN_DVSENSORS  12  // Enable voltage source for One-wire sensors
#endif

#ifdef ANALOG_SENSORS
  #define VOPA      26  // Enable voltage source for OPA
#endif

#ifdef LCD
  #define VLCD      22  // Enable LCD voltage source
#endif

#ifdef LORA
  #define LORA_COMM   25  // LoRa communication chip: Ra-01/RN2483
#endif

/************************************************************************/
/* PARAMETER DEFINITION                                                 */
/************************************************************************/
#ifdef PRESSURE_SENSOR
  #define PARAM_PRESSURE    0 // A - Pressure.
#endif
#ifdef TEMP_SENSOR
  #define PARAM_ANALOG_TEMP 1 // B - Analog Temperature
#endif
#ifdef ONE_WIRE_SENSORS
  #define PARAM_DIGITAL_TEMP_1  2 // C - Digital Temperature:
                                  // Digital sensor have a 64-bit ID and
                                  // 8-bit family ID, this identifies the
                                  // device type and functionality.
  #define PARAM_DIGITAL_TEMP_2  3
  #define PARAM_DIGITAL_TEMP_3  4
  #define PARAM_DIGITAL_TEMP_4  5
  
  #define PARAM_DIGITAL_HUMI_1  6 // D - Digital Humidity
                                  // If use DHT22 (AM2302), humidity is
                                  // available to read.
#endif

#ifdef BUTTONS            // E - Buttons states
  #define PARAM_S_P     0
  #define PARAM_S_N     1
  #define PARAM_Z_P     2
  #define PARAM_Z_N     3
  #define MASK_BUTTONS    0b00001111  // Where are the bit
#endif



/************************************************************************/
/* FLAG DEFINITION                                                      */
/************************************************************************/
#define FLAG_STATUS             8   // Currently active services
#define FLAG_ENABLED            9   // Enabled service (set by user)
#define FLAG_ERROR              24  // Error in executed orders.

// PARAM_ERROR codes
//#define FLAG_TEMP_PCB_PROBE_ERROR      0   // pcb probe failed (one wire not answering)
//#define FLAG_TEMP_LIQ_PROBE_ERROR      1   // liquid probe failed (one wire not answering)
//#define FLAG_TEMP_PCB_RANGE_ERROR      2   // temperature of pcb is outside range
//#define FLAG_TEMP_LIQ_RANGE_ERROR      3   // temperature of liquid is outside range
//#define FLAG_TEMP_TARGET_RANGE_ERROR   4   // target temperature is outside range
//#define MASK_TEMP_ERROR                0b00011111  // where are the bit for temperature error

// #define SHOW_MENU_HELP 1 // if we don't show the help we spare a lot of memory


/******************************
  SERIAL, LOGGER AND DEBUGGERS
*******************************/

//#define EVENT_LOGGING   // This parameter is use when external Flash memory is available.

//#define THR_LINEAR_LOGS    1

//#define DEBUG_LOGS         1
//#define DEBUG_WEIGHT       1
//#define DEBUG_LCD          1
//#define DEBUG_ONEWIRE      1
//#define DEBUG_PID          1

#ifdef THR_LINEAR_LOGS
  #define FLASH_SELECT A3 //Flash SS_SPI
  #define LOG_INTERVAL 10  //Interval in (s) between logs logger
#endif



void writeLog(uint16_t event_number, int parameter_value);
boolean clearParameterBit(byte number, byte bitToClear);
void setupMemory();
void recoverLastEntryN();
uint8_t loadLastEntryToParameters();
uint16_t findSectorOfN( );
uint8_t toHex(Print* output, byte value);
uint8_t toHex(Print* output, int value);
uint8_t toHex(Print* output, long value);



#endif /* STANDARDTRANDUCERMAIN_H_ */
