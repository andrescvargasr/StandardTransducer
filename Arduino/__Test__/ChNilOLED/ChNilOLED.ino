/*
 * ChNilOLED.cpp
 *
 * Created: 7/06/2018 5:58:09 p. m.
 *  Author: camil
 */ 

/************************************************************************/
/* This code run OLED display on ChiNil RTOS.                           */
/************************************************************************/

#define F_CPU 16000000UL

//#define LED_BUILTIN 13
//#define LED_BUILTIN_RX 17
//#define LED_BUILTIN_TX 30
//#define LED_BUILTIN 34
//#define LED_BUILTIN_RX 33
//#define LED_BUILTIN_TX 32

//#ifdef LED_BUILTIN_TX
    //#undef LED_BUILTIN_TX
    //#define LED_BUILTIN_TX 32
    #define LED_BUILTIN_TX_1 30
//#endif
//#ifdef LED_BUILTIN_RX
    //#undef LED_BUILTIN_RX
    //#define LED_BUILTIN_RX 33
    //#define LED_BUILTIN_RX_1 17
//#endif
//#ifdef LED_BUILTIN
    //#undef LED_BUILTIN
    //#define LED_BUILTIN 34
    //#define LED_BUILTIN_1 13
//#endif

//#define TX_RX_LED_INIT  DDRD |= (1<<5), DDRB |= (1<<0)
//#define TXLED0      PORTD |= (1<<5)
//#define TXLED1      PORTD &= ~(1<<5)
//#define RXLED0      PORTB |= (1<<0)
//#define RXLED1      PORTB &= ~(1<<0)


    #undef TXLED0
    #undef TXLED1
    #define TXLED0 0
    #define TXLED1 1
    #undef RXLED0
    #undef RXLED1
    #define RXLED0 0
    #define RXLED1 1
    
#include "ChNil.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET -1
Adafruit_SSD1306 display(OLED_RESET);

#define XPOS 0
#define YPOS 1

// Declare and initialize a semaphore for limiting access to a region.
SEMAPHORE_DECL(oneSlot, 1);

/************************************************************************/
/* Thread 1. OLED communication.                                        */
/************************************************************************/
// Declare a stack with 32 bytes beyond context switch and interrupt needs.
THD_WORKING_AREA(waThread1, 32);

// Declare thread function for thread 1.
THD_FUNCTION(Thread1, arg) {
  (void)arg;
    
    /* Aquí no se implementan las funciones pinMode() ni digitalWrite debido a que
     * en este caso el pin al cual se hace referencia apunta directamente a RXLED,
     * lo cual hace que no sea posible implementar estas funciones, puesto que no
     * están correctamente mapeados en las librerías.
     */

    /* Se debe tener en cuenta que al existir las constantes las constantes para
     * cada uno de los componentes, se debe preguntar si existen a fin de activar
     * las secciones que sean necesarias
     */

    // PIND5 se encarga de habilitar la alimentación del OLED a través del
    // MOSFET-P.
    // PIND7 se encarga de habilitar la alimentación de la comunicación LoRa
    // RA-01, RN2483
    // Y Bluetooth mediante el driver BNP75N en ANGND.

    pinMode(LED_BUILTIN_TX_1, OUTPUT);
    digitalWrite(LED_BUILTIN_TX_1, LOW);
  
  // Turn on OLED display
  while (TRUE)
  {
        // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
        display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
        // init done
        
        // Aquí debe insertarse la primera imagen que despliega el transductor
        // en este caso corresponde al valor del sensor, el nivel de voltaje y
        // la comunicación sea LoRa o Bluetooth.

        // Show image buffer on the display hardware.
        // Since the buffer is intialized with an Adafruit splashscreen
        // internally, this will display the splashscreen.
        chSemWait(&oneSlot);
            display.display();
            chThdSleepMilliseconds(2000);
        chSemSignal(&oneSlot);

        // Clear the buffer.
        display.clearDisplay();
        
        // draw the first ~12 characters in the font
        testdrawchar();
        display.display();
        delay(2000);
        display.clearDisplay();

        // draw scrolling text
        chSemWait(&oneSlot);
            testscrolltext();
            delay(2000);
        chSemSignal(&oneSlot);
        display.clearDisplay();

        // text display tests
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(0,0);
        display.println("Hello, world!");
        display.setTextColor(BLACK, WHITE); // 'inverted' text
        display.println(3.141592);
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.print("0x"); display.println(0xDEADBEEF, HEX);
        display.display();
        delay(2000);
        display.clearDisplay();
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
    
    static int count = 0;
    
    while (TRUE)
    {
        chSemWait(&oneSlot);
            Serial.println(count++);
        chSemSignal(&oneSlot);
        chThdSleepMilliseconds(2000);
    } 
}

/************************************************************************/
/* Threads static table, one entry per thread.  A thread's priority is  */
/* determined by its position in the table with highest priority first. */
/*                                                                      */
/* These threads start with a null argument.  A thread's name may also  */
/* be null to save RAM since the name is currently not used.            */
/************************************************************************/
THD_TABLE_BEGIN
    THD_TABLE_ENTRY(waThread1, "thread1", Thread1, NULL)
    THD_TABLE_ENTRY(waThread2, "thread2", Thread2, NULL)
THD_TABLE_END

/************************************************************************/
/* SETUP                                                                */
/************************************************************************/
void setup() {
    Serial.begin(9600);
    
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
    delay(1);
}

void testscrolltext(void) {
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(10,0);
    display.clearDisplay();
    display.println("scroll");
    display.display();
    delay(1);

    display.startscrollright(0x00, 0x0F);
    delay(2000);
    display.stopscroll();
    delay(1000);
    display.startscrollleft(0x00, 0x0F);
    delay(2000);
    display.stopscroll();
    delay(1000);
    display.startscrolldiagright(0x00, 0x07);
    delay(2000);
    display.startscrolldiagleft(0x00, 0x07);
    delay(2000);
    display.stopscroll();
}
