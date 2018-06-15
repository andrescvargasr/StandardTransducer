/*
 * OLED
 *
 * Created: 13/06/2018 4:26:58 p. m.
 *  Author: camil
 */ 
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET -1 // Reset pin is not necessary
Adafruit_SSD1306 display(OLED_RESET);

#define XPOS 0
#define YPOS 1

/************************************************************************/
/* Thread 1. OLED communication.                                        */
/************************************************************************/
// Declare a stack with 32 bytes beyond context switch and interrupt needs.
THD_WORKING_AREA(waOLED, 16);

// Declare thread function for thread 1.
THD_FUNCTION(ThrOLED, arg) {
  (void)arg;
  /* Se debe tener en cuenta que al existir las constantes las constantes para
  * cada uno de los componentes, se debe preguntar si existen a fin de activar
  * las secciones que sean necesarias
  */
  // PIND5 se encarga de habilitar la alimentaci�n del OLED a trav�s del
  // MOSFET-P.
  // PIND7 se encarga de habilitar la alimentaci�n de la comunicaci�n LoRa
  // RA-01, RN2483 y Bluetooth mediante el driver BNP75N en ANGND.

  //pinMode(PIN_OLED, OUTPUT);
  //digitalWrite(LED_BUILTIN_TX, LOW);
  //digitalWrite(PIN_OLED, LOW);
  OLED_INIT;
  OLED1;

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  // init done

  display.clearDisplay();

  // Turn on OLED display
  while (TRUE)
  {
    //chThdSleepMilliseconds(1000);
    // Aqu? debe insertarse la primera imagen que despliega el transductor
    // en este caso corresponde al valor del sensor, el nivel de voltaje y
    // la comunicaci?n sea LoRa o Bluetooth.

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
    chThdSleepMilliseconds(1);
  }
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
