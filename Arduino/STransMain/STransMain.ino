#include "STransMain.h"

/************************************************************************/
/* This code run OLED display on ChiNil RTOS.                           */
/************************************************************************/

// #define F_CPU 16000000UL  //Leonardo Bootloader
#define F_CPU 8000000UL //Lilypad USB Bootloader

#include "ChNil.h"
//#include "Parameters.h"
#include "OLED.h"


volatile uint16_t count = 0;

// Declare and initialize a semaphore for limiting access to one region.
// Mutexes aren't available for ChiNil RTOS, is available for ChiRT.
// Este semaforo vigila el acceso entre el monitor Serial y la pantalla
// OLED, de modo que no genere conflictos entre ellas.
SEMAPHORE_DECL(oneSlot, 1);

/************************************************************************/
/* SETUP                                                                */
/************************************************************************/
void setup() {
    //chFillStacks();
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
