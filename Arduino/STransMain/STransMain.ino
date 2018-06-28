#include "STransMain.h"

/************************************************************************/
/* This code run OLED display on ChiNil RTOS.                           */
/************************************************************************/

// #define F_CPU 16000000UL  //Leonardo Bootloader
#define F_CPU 8000000UL //Lilypad USB Bootloader

#include "ChNil.h"
//#include "Parameters.h"
#include "OLED.h"
//#include "PT100.h"

/* OLED display */
#ifdef OLED
  #include <Wire.h>
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>

  Adafruit_SSD1306 display(OLED_RESET);
#endif  // _OLED_

// Declare and initialize a semaphore for limiting access to one region.
// Mutexes aren't available for ChiNil RTOS, is available for ChiRT.
// Este semaforo vigila el acceso entre el monitor Serial y la pantalla
// OLED, de modo que no genere conflictos entre ellas.
SEMAPHORE_DECL(oneSlot, 1);

// Este semaforo permite controlar el acceso a la variable Pressure
// tanto desde THR_OLED como de THR_PRESSURE.
SEMAPHORE_DECL(adcPSem,0);

// Este semaforo controla la espera hasta que termina la lectura ADC.
SEMAPHORE_DECL(adcSem, 0);

volatile float temperature = 25.00;
volatile float pressure = 0.00;
/************************************************************************/
/* SETUP                                                                */
/************************************************************************/
void setup() {
  Serial.begin(9600);

  /* OLED display */
  #ifdef OLED
    OLED_INIT;  // PORTD5 set OUTPUT
    OLED1;      // PORTD5 set LOW because is connect to MOSFET-P.
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.display();
    //display.display();

//    display.setTextSize(2);
//    display.setTextColor(WHITE);
//    display.setCursor(0,0);
//    display.println("Pressure: 99.99 kPa");
//    display.setTextColor(BLACK, WHITE); // 'inverted' text
//    display.println("3.141592");
//    display.setTextSize(2);
//    display.setTextColor(WHITE);
//    display.println("0x"); 
//    display.display();
//    delay(2000);
//    display.clearDisplay();
//    display.display();
  #endif  // _OLED_
  
  chFillStacks();
  // Start ChiNil RTOS.
  chBegin();
}

/************************************************************************/
/* LOOP                                                                 */
/************************************************************************/
// Loop is the idle thread.  The idle thread must not invoke any
// kernel primitive able to change its state to not runnable.
void loop() {
}

/************************************************************************/
/* FUNCTIONS                                                            */
/************************************************************************/
void pinPortMode(uint8_t pin, uint8_t mode)
{
  uint8_t bit = pinPortToBitMask(pin);
  uint8_t port = pinToPort(pin);
  volatile uint8_t *reg, *out;

  if (port == NOT_A_PIN) return;

  // JWS: can I let the optimizer do this?
  reg = portModeRegister(port);
  out = portOutputRegister(port);

  if (mode == INPUT) {
    uint8_t oldSREG = SREG;
    cli();
    *reg &= ~bit;
    *out &= ~bit;
    SREG = oldSREG;
    } else if (mode == INPUT_PULLUP) {
    uint8_t oldSREG = SREG;
    cli();
    *reg &= ~bit;
    *out |= bit;
    SREG = oldSREG;
    } else {
    uint8_t oldSREG = SREG;
    cli();
    *reg |= bit;
    SREG = oldSREG;
  }
}

void pinPortWrite(uint8_t pin, uint8_t val)
{
  uint8_t bit = pinPortToBitMask(pin);
  uint8_t port = pinToPort(pin);
  volatile uint8_t *out;

  if (port == NOT_A_PIN) return;

  out = portOutputRegister(port);

  uint8_t oldSREG = SREG;
  cli();

  if (val == LOW) {
    *out &= ~bit;
    } else {
    *out |= bit;
  }

  SREG = oldSREG;
}
