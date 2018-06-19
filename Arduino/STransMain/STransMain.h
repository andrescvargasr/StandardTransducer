/*
 * STransMain.h
 *
 * Created: 15/06/2018 10:42:13 a. m.
 *  Author: camil
 */ 


#ifndef STRANSMAIN_H_
#define STRANSMAIN_H_

/************************************************************************/
/* LIBRARIES                                                            */
/************************************************************************/

#include <ChNil.h>    // RTOS MultiThread   (ChNil_h)
#include <avr/pgmspace.h>
//#include <SPI.h>    //Flash SPI       (_SPI_H_INCLUDED)
//#include <avr/wdt.h>  //Watchdog        (_AVR_WDT_H_)

#define SHOW_MENU_HELP 1 // if we don't show the help we spare a lot of memory
/************************************************************************/
/* CARACTERISTICS                                                       */
/************************************************************************/
#define SERIAL
//#define PRESSURE_SENSOR   // MPX53D pressure sensor
//#define TEMP_SENSOR        // PT100 temperature sensor
//#define ONE_WIRE_SENSORS  // DS18B20 temperature sensor
//#define OLED              // 128x32 OLED display with I2C communication
//#define BUTTONS           // Resistive buttons
//#define BLUETOOTH         // BLE
//#define LORA
//#define LORA_RA1          // LoRa Ra-01 communication module
//#define LORA_RN2483       // RN2483 communication module
//#define LORA_RN2903       //RN2903 communication module

#ifdef PRESSURE_SENSOR || TEMP_SENSOR
  #define ANALOG_SENSORS  // Analog sensors is present
#endif

#ifdef BLUETOOTH || LORA
  #define WIRELESS_COMM   // Wireless communication is available
#endif

//#define ADC_A0                
#define BATTERY_SENSOR    // Check the level of battery
//#define CONTROL_REGULATOR // Enable 3V3 regulator for external modules

/************************************************************************/
/* PIN CONFIGURATION (ATMEGA32U4 NOT ARDUINO)                           */
/************************************************************************/
#define PIN_RA1_2483    1
#define PIN_NSS         8 
#define PIN_SCK         9
#define PIN_MOSI        10
#define PIN_MISO        11
#define PIN_DVSENSORS   12
#define PIN_SCL         18
#define PIN_SDA         19
#define PIN_RX          20
#define PIN_TX          21
#define PIN_ENOLED      22
#define PIN_LORA_BLE    25
#define PIN_VOPA        26
#define PIN_D7          27
#define PIN_D6          28
#define PIN_D8          29
#define PIN_D9          30
#define PIN_D10         31
#define PIN_ONE_WIRE    32
#define PIN_AN_A0       36
#define PIN_AN_VANALOG  37
#define PIN_ENREG       38
#define PIN_AN_D3V3     39
#define PIN_AN_VOUT_N   40
#define PIN_AN_VOUT_P   41


#ifdef PRESSURE_SENSOR
  static const uint8_t PRESSURE_L = PIN_AN_VOUT_N;  // Differential ADC: negative (ADC0)
  static const uint8_t PRESSURE_H = PIN_AN_VOUT_P;  // Differential ADC: positive (ADC1)
#endif

#ifdef BATTERY_SENSOR
  static const uint8_t VBATTERY   = PIN_AN_D3V3;  // Voltage level of battery
#endif

#ifdef CONTROL_REGULATOR
  #define REG3V3      PIN_ENREG     // Control pin for 3.3 V regulator (NOT USED)
#endif

#ifdef ADC_A0
  static const uint8_t AN_A0      = PIN_AN_A0;
#endif

#ifdef TEMP_SENSOR
  static const uint8_t TEMP_PT100 = PIN_AN_VANALOG;  // Single-ended ADC: (ADC6)
#endif

#ifdef WIRELESS_COMM
  #define W_COMM      PIN_LORA_BLE  // Wireless communication: LoRa/Bluetooth
  #define ANGND       PIN_D7        // Enable GND for wireless communication
#endif

#ifdef LORA
#define LORA_COMM   PIN_RA1_2483  // LoRa module: Ra-01/RN2483/RN2903
#endif

#ifdef BUTTONS
  #define Z_P       PIN_D10   // Z+ button
  #define S_P       PIN_D9    // S+ button
  #define Z_N       PIN_D8    // Z- button
  #define S_N       PIN_D6    // S- button
#endif

#ifdef ONE_WIRE_SENSORS
  #define ONE_WIRE      PIN_ONE_WIRE  // One-wire communication
  #define EN_DVSENSORS  PIN_DVSENSORS  // Enable voltage source for One-wire sensors
#endif

#ifdef ANALOG_SENSORS
  #define VOPA      PIN_VOPA  // Enable voltage source for OPA
#endif

#ifdef OLED
  #define ENOLED      PIN_ENOLED  // Enable LCD voltage source
#endif

/************************************************************************/
/* PARAMETERS DEFINITION                                                 */
/************************************************************************/
#ifdef PRESSURE_SENSOR
  #define PARAM_PRESSURE    0 // A - Pressure.
#endif
#ifdef TEMP_SENSOR
  #define PARAM_ANALOG_TEMP 1 // B - Analog Temperature
#endif
#ifdef ONE_WIRE_SENSORS
  #define PARAM_DIGITAL_TEMP_1  2 // C - Digital Temperature 1:
                                  // Digital sensor have a 64-bit ID and
                                  // 8-bit family ID, this identifies the
                                  // device type and functionality.
  #define PARAM_DIGITAL_TEMP_2  3 // D - Digital Temperature 2
  #define PARAM_DIGITAL_TEMP_3  4 // E - Digital Temperature 3
  #define PARAM_DIGITAL_TEMP_4  5 // F - Digital Temperature 4
  
  #define PARAM_DIGITAL_HUMI_1  6 // G - Digital Humidity
                                  // If use DHT22 (AM2302), humidity is
                                  // available to read.
#endif

#ifdef BUTTONS            
  #define PARAM_BUTTONS 7         // H - Buttons states   
  #define PARAM_S_P     0
  #define PARAM_S_N     1
  #define PARAM_Z_P     2
  #define PARAM_Z_N     3
  #define MASK_BUTTONS  0b00001111  // Where are the bit
#endif

#ifdef OLED
  #define PARAM_OLED      8       // I - OLED state.
#endif

#ifdef WIRELESS_COMM
  #define PARAM_WIRELESS    9     // J - Wireless communication.  
  #define PARAM_BLUETOOTH   0
  #define PARAM_LORA_RA1    1
  #define PARAM_LORA_RN2483 2
  #define PARAM_LORA_RN2903 3
  #define MASK_WIRELESS     0b00001111
#endif

#define PARAM_ERROR       23      // X - Errors in system.
#define PARAM_STATUS      24      // Y - System state.
#define PARAM_ENABLED     25      // Z - Characteristics enabled/disabled.

/************************************************************************/
/* FLAG DEFINITION                                                      */
/************************************************************************/

/* FLAG_STATUS */
#if PARAM_STATUS
  #define FLAG_STATUS             PARAM_STATUS   // Currently active services
  
  #ifdef PRESSURE_SENSOR
    #define FLAG_PRESSURE_CTRL    0
  #endif
  
  #ifdef TEMP_SENSOR
    #define FLAG_ANAG_TEMP_CTRL   1 
  #endif
  
  #ifdef ONE_WIRE_SENSORS
    #define FLAG_DIG_TEMP_1_CTRL  2
    #define FLAG_DIG_TEMP_2_CTRL  3
    #define FLAG_DIG_TEMP_3_CTRL  4
    #define FLAG_DIG_TEMP_4_CTRL  5
    #define FLAG_DIG_HUMI_1_CTRL  6
  #endif
  
  #ifdef BUTTONS
    #define FLAG_BUTTONS          7
  #endif
  
  #ifdef OLED
    #define FLAG_OLED             8
  #endif
  
  #ifdef WIRELESS_COMM
    #define FLAG_BLUETOOTH        9
    #define FLAG_LORA_RA1         10
    #define FLAG_LORA_RN2483      11
    #define FLAG_LORA_RN2903      12
  #endif
  
  #ifdef ADC_A0
    #define FLAG_A0               13
  #endif
  
  #ifdef BATTERY_SENSOR
    #define FLAG_BATTERY          14
  #endif
  
  #ifdef CONTROL_REGULATOR
    #define FLAG_REGULATOR        15
  #endif
  
  #define MASK_STATUSH    0b11111111
  #define MASK_STATUSL    0b11111111
#endif

/* FLAG ENABLED */
#ifdef PARAM_ENABLED
  #define FLAG_ENABLED                PARAM_ENABLED   // Enabled service (set by user)
  
  #ifdef PRESSURE_SENSOR
    #define FLAG_EN_PRESSURE_CTRL     0
  #endif
  
  #ifdef TEMP_SENSOR
    #define FLAG_EN_ANAG_TEMP_CTRL    1
  #endif
  
  #ifdef ONE_WIRE_SENSORS
    #define FLAG_EN_DIG_TEMP_1_CTRL   2
    #define FLAG_EN_DIG_TEMP_2_CTRL   3
    #define FLAG_EN_DIG_TEMP_3_CTRL   4
    #define FLAG_EN_DIG_TEMP_4_CTRL   5
    #define FLAG_EN_DIG_HUMI_1_CTRL   6
  #endif
  
  #ifdef BUTTONS
    #define FLAG_EN_BUTTONS           7
  #endif
  
  #ifdef OLED
    #define FLAG_EN_OLED              8
  #endif
  
  #ifdef WIRELESS_COMM
    #define FLAG_EN_BLUETOOTH         9
    #define FLAG_EN_LORA_RA1          10
    #define FLAG_EN_LORA_RN2483       11
    #define FLAG_EN_LORA_RN2903       12
  #endif
  
  #ifdef ADC_A0
    #define FLAG_EN_A0                  13
  #endif
  
  #ifdef BATTERY_SENSOR
    #define FLAG_EN_BATTERY             14
  #endif
  
  #ifdef CONTROL_REGULATOR
    #define FLAG_EN_REGULATOR           15
  #endif
  
  #define MASK_ENABLEDH   0b11111111
  #define MASK_ENABLEDL   0b11111111
#endif

/* FLAG ERROR */
#ifdef PARAM_ERROR
  #define FLAG_ERROR                  PARAM_ERROR  // Error in executed orders.
  
  #ifdef PRESSURE_SENSOR
  #define FLAG_ERROR_RANGE_PRESSURE   0
  #endif
  
  #ifdef TEMP_SENSOR
  #define FLAG_ERROR_ANAG_TEMP        1
  #endif
  
  #ifdef ONE_WIRE_SENSORS
  #define FLAG_ERROR_DIG_TEMP_1       2
  #define FLAG_ERROR_DIG_TEMP_2       3
  #define FLAG_ERROR_DIG_TEMP_3       4
  #define FLAG_ERROR_DIG_TEMP_4       5
  #define FLAG_ERROR_DIG_HUMI_1       6
  #endif
  
  /* NOT IMPLEMENTED
  #ifdef BUTTONS
    #define FLAG_ERROR_BUTTONS            7
  #endif
  */
  
  #ifdef OLED
    #define FLAG_ERROR_OLED           7
  #endif
  
  #ifdef WIRELESS_COMM
    #define FLAG_ERROR_COMMUNICATION  8
  #endif
  
  #ifdef ADC_A0
  #define FLAG_ERROR_A0               9
  #endif
  
  #define MASK_ERRORH   0b00000011
  #define MASK_ERRORL   0b11111111
#endif

/************************************************************************/
/* FUNCTIONS                                                            */
/************************************************************************/

/* THR_SERIAL */
void pinPortMode(uint8_t, uint8_t);
void pinPortWrite(uint8_t, uint8_t);

void processSpecificCommand(char*, char*, Print*);
void printGeneralParameters(Print*);

void printResult(char*, Print*);

// Help menu
void printHelpMenu(Print*);
void printSpecificHelp(Print*);

// Flags menu
void processFlagsCommand(char, char*, Print*);
void printFlagsHelp(Print*);
void processFlagsCommand(char, char*, Print*);


/* PARAMETERS */
void setAndSaveParameter(byte, int);
void printParameters(Print*);
void printParameter(Print*, byte);
int getParameter(byte);
boolean getParameterBit(byte, byte);

#define pinPortToBitMask(P) ( pgm_read_byte( pin_port_to_bit_mask_PGM + (P) ) )
#define pinToPort(P) ( pgm_read_byte( pin_to_port_PGM + (P) ) )

#define PA 1
#define PB 2
#define PC 3
#define PD 4
#define PE 5
#define PF 6

const uint8_t PROGMEM pin_port_to_bit_mask_PGM[] = {
  NOT_A_PIN,  // 0
  _BV(6),     // 1  - PE6
  NOT_A_PIN,  // 2  - UVcc
  NOT_A_PIN,  // 3  - D-
  NOT_A_PIN,  // 4  - D+
  NOT_A_PIN,  // 5  - UGnd
  NOT_A_PIN,  // 6  - UCap
  NOT_A_PIN,  // 7  - VBus
  _BV(0),     // 8  - PB0 - SS
  _BV(1),     // 9  - PB1 - SCLK
  _BV(2),     // 10 - PB2 - MOSI
  _BV(3),     // 11 - PB3 - MISO
  
  _BV(7),     // 12 - PB7
  NOT_A_PIN,  // 13 - /RESET
  NOT_A_PIN,  // 14 - VCC
  NOT_A_PIN,  // 15 - GND
  NOT_A_PIN,  // 16 - XTAL2
  NOT_A_PIN,  // 17 - XTAL1
  _BV(0),     // 18 - PD0
  _BV(1),     // 19 - PD1
  _BV(2),     // 20 - PD2
  _BV(3),     // 21 - PD3
  _BV(5),     // 22 - PD5
  
  NOT_A_PIN,  // 23 - GND
  NOT_A_PIN,  // 24 - AVCC
  _BV(4),     // 25 - PD4
  _BV(6),     // 26 - PD6
  _BV(7),     // 27 - PD7
  _BV(4),     // 28 - PB4
  _BV(5),     // 29 - PB5
  _BV(6),     // 30 - PB6
  _BV(6),     // 31 - PC6
  _BV(7),     // 32 - PC7
  _BV(2),     // 33 - PE2
  
  NOT_A_PIN,  // 34 - VCC
  NOT_A_PIN,  // 35 - GND
  _BV(7),     // 36 - PF7 - A0
  _BV(6),     // 37 - PF6 - VANALOG
  _BV(5),     // 38 - PF5
  _BV(4),     // 39 - PF4 - D+3V3
  _BV(1),     // 40 - PF1 - VOUT-
  _BV(0),     // 41 - PF0 - VOUT+
  NOT_A_PIN,  // 42 - AREF
  NOT_A_PIN,  // 43 - GND
  NOT_A_PIN,  // 44 - AVCC
};

const uint8_t PROGMEM pin_to_port_PGM[] = {
  NOT_A_PIN,  // 0
  PE,         // 1 - PE6
  NOT_A_PIN,  // 2 - UVcc
  NOT_A_PIN,  // 3 - D-
  NOT_A_PIN,  // 4 - D+
  NOT_A_PIN,  // 5 - UGnd
  NOT_A_PIN,  // 6 - UCap
  NOT_A_PIN,  // 7 - VBus
  PB,         // 8 - PB0
  PB,         // 9 - PB1
  PB,         // 10 - PB2
  PB,         // 11 - PB3
  
  PB,         // 12 - PB7
  NOT_A_PIN, // 13 - /RESET
  NOT_A_PIN,  // 14 - VCC
  NOT_A_PIN, // 15 - GND
  NOT_A_PIN, // 16 - XTAL2
  NOT_A_PIN, // 17 - XTAL1
  PD,         // 18 - PD0
  PD,         // 19 - PD1
  PD,         // 20 - PD2
  PD,         // 21 - PD3
  PD,         // 22 - PD5
  
  NOT_A_PIN,  // 23 - GND
  NOT_A_PIN,  // 24 - AVCC
  PD,         // 25 - PD4
  PD,         // 26 - PD6
  PD,         // 27 - PD7
  PB,         // 28 - PB4
  PB,         // 29 - PB5
  PB,         // 30 - PB6
  PC,         // 31 - PC6
  PC,         // 32 - PC7
  PE,         // 33 - PE2
  
  NOT_A_PIN,  // 34 - VCC
  NOT_A_PIN, // 35 - GND
  PF,         // 36 - PF7
  PF,         // 37 - PF6
  PF,         // 38 - PF5
  PF,         // 39 - PF4
  PF,         // 40 - PF1
  PF,         // 41 - PF0
  NOT_A_PIN,  // 42 - AREF
  NOT_A_PIN,  // 43 - GND
  NOT_A_PIN,  // 44 - AVCC
};




void writeLog(uint16_t event_number, int parameter_value);
boolean clearParameterBit(byte number, byte bitToClear);
void setupMemory();
void recoverLastEntryN();
uint8_t loadLastEntryToParameters();
uint16_t findSectorOfN( );
uint8_t toHex(Print* output, byte value);
uint8_t toHex(Print* output, int value);
uint8_t toHex(Print* output, long value);



#endif /* STRANSMAIN_H_ */
