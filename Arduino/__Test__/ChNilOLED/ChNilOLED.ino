/*
* ChNilOLED.cpp
*
* Created: 7/06/2018 5:58:09 p. m.
*  Author: camil
*/

/************************************************************************/
/* This code run OLED display on ChiNil RTOS.                           */
/************************************************************************/

// #define F_CPU 16000000UL  //Leonardo Bootloader
#define F_CPU 8000000UL //Lilypad USB Bootloader

#ifdef TX_RX_LED_INIT
#undef TX_RX_LED_INIT
#define TX_RX_LED_INIT  DDRF |= (1<<5), DDRF |= (1<<7)  // Pins aren't used
#endif

#ifdef TXLED0
#undef TXLED0
#define TXLED0      -1  // TXLED0 isn't used
#endif

#ifdef TXLED1
#undef TXLED1
#define TXLED1      -1  // TXLED1 isn't used
#endif

#ifdef RXLED0
#undef RXLED0
#define RXLED0      -1  // RXLED0 isn't used
#endif

#ifdef RXLED1
#undef RXLED1
#define RXLED1      -1  // RXLED1 isn't used
#endif

#define PIN_OLED  30  // PORT_MAP for digitalWrite() enable OLED PORTD5.
#define OLED_INIT DDRD |= (1<<PORTD5) // Set PORTD5 for output.

#define OLED1			PORTD &= ~(1<<PORTD5) // Set On OLED (P-MOSFET).
#define OLED0		  PORTD |= (1<<PORTD5)  // Set Off OLED (P-MOSFET).

#include "ChNil.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET -1 // Reset pin is not necessary
Adafruit_SSD1306 display(OLED_RESET);

#define XPOS 0
#define YPOS 1

// Declare and initialize a semaphore for limiting access to one region.
// Mutexes aren't available for ChiNil RTOS, is available for ChiRT.
SEMAPHORE_DECL(oneSlot, 1);

/************************************************************************/
/* Thread 1. OLED communication.                                        */
/************************************************************************/
// Declare a stack with 32 bytes beyond context switch and interrupt needs.
THD_WORKING_AREA(waThread1, 118);

// Declare thread function for thread 1.
THD_FUNCTION(Thread1, arg) {
  (void)arg;
  /* Se debe tener en cuenta que al existir las constantes las constantes para
  * cada uno de los componentes, se debe preguntar si existen a fin de activar
  * las secciones que sean necesarias
  */

  display.clearDisplay();

  // Turn on OLED display
  while (TRUE)
  {
    //chThdSleepMilliseconds(1000);
    // Aquí debe insertarse la primera imagen que despliega el transductor
    // en este caso corresponde al valor del sensor, el nivel de voltaje y
    // la comunicación sea LoRa o Bluetooth.

    // Show image buffer on the display hardware.
    // Since the buffer is intialized with an Adafruit splashscreen
    // internally, this will display the splashscreen.
    chSemWait(&oneSlot);
    {
      delay(100);
      OLED1;
      display.clearDisplay();
      //chSemSignal(&oneSlot);

      // draw the first ~12 characters in the font
      testdrawchar();
      chThdSleepMilliseconds(2000);
      display.clearDisplay();
    }

    chSemSignal(&oneSlot);
  }
}

/************************************************************************/
/* Thread 2. Print Serial.                                              */
/************************************************************************/
// Declare a stack with 100 bytes beyond context switch and interrupt needs.
THD_WORKING_AREA(waThread2, 100);

// Declare thread function for thread 2.
THD_FUNCTION(Thread2, arg) {
  (void)arg;

  // Wake time for thread sleep.
  //systime_t wakeTime = chVTGetSystemTimeX();

  

  // Add ticks for one second.  The MS2ST macro converts ms to system ticks.
  //  wakeTime += MS2ST(1000);

  while (TRUE)
  {

    // Sleep until next second.
//    chThdSleepUntil(wakeTime);

    chSemWait(&oneSlot);
    {
      // Print loopCount for previous second.
      Serial.print(F("Count: "));
      Serial.println(", ");

      // Print unused stack for thread 1, thread 2, and idle thread.
      //chPrintUnusedStack(&Serial);
    }
    chSemSignal(&oneSlot);
  }
}

/************************************************************************/
/* Threads static table, one entry per thread.  A thread's priority is  */
/* determined by its position in the table with highest priority first. */
/*                                                                      */
/* These threads start with a null argument.  A thread's name may also  */
/* be null to save RAM since the name is currently not used.            */
/************************************************************************/
// THD_TABLE_ENTRY(waThread1, "thread1", Thread1, NULL) Name is not necessary
THD_TABLE_BEGIN
THD_TABLE_ENTRY(waThread1, NULL, Thread1, NULL)
THD_TABLE_ENTRY(waThread2, NULL, Thread2, NULL)
THD_TABLE_END

/************************************************************************/
/* SETUP                                                                */
/************************************************************************/
void setup() {
  Serial.begin(9600);

  // PIND5 se encarga de habilitar la alimentación del OLED a través del
  // MOSFET-P.
  // PIND7 se encarga de habilitar la alimentación de la comunicación LoRa
  // RA-01, RN2483 y Bluetooth mediante el driver BNP75N en ANGND.

  //pinMode(PIN_OLED, OUTPUT);
  //digitalWrite(LED_BUILTIN_TX, LOW);
  //digitalWrite(PIN_OLED, LOW);
  OLED_INIT;
  OLED1;

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done

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

void testdrawchar(void) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  for (uint8_t i=0; i < 168; i++) {
    if (i == '\n') continue;
    display.write(i);
    if ((i > 0) && (i % 21 == 0))
    display.println();
  }
  display.display();
  chThdSleepMilliseconds(1);
}

void testscrolltext(void) {
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(10,0);
  display.clearDisplay();
  display.println("scroll");
  display.display();
  chThdSleepMilliseconds(1);

  display.startscrollright(0x00, 0x0F);
  chThdSleepMilliseconds(2000);
  display.stopscroll();
  chThdSleepMilliseconds(2000);
  display.startscrollleft(0x00, 0x0F);
  chThdSleepMilliseconds(2000);
  display.stopscroll();
  chThdSleepMilliseconds(2000);
  display.startscrolldiagright(0x00, 0x07);
  chThdSleepMilliseconds(2000);
  display.startscrolldiagleft(0x00, 0x07);
  chThdSleepMilliseconds(2000);
  display.stopscroll();
}
