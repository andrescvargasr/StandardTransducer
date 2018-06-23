/*
 * OLED
 *
 * Created: 13/06/2018 4:26:58 p. m.
 *  Author: camil
 */ 
#ifdef OLED
#define THR_OLED

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Adafruit_SSD1306 display(OLED_RESET);

extern volatile double pressure;

/************************************************************************/
/* Thread 1. OLED communication.                                        */
/************************************************************************/
// Declare a stack with 32 bytes beyond context switch and interrupt needs.
THD_WORKING_AREA(waOLED, 32);

// Declare thread function for thread 1.
THD_FUNCTION(ThOLED, arg) {
  (void)arg;

  float temp = 12.13;
  char buf[5] = "";
  
  //Serial.begin(9600);
  /* Se debe tener en cuenta que al existir las constantes las constantes para
  * cada uno de los componentes, se debe preguntar si existen a fin de activar
  * las secciones que sean necesarias
  */
  // PIND5 se encarga de habilitar la alimentaci?n del OLED a trav?s del
  // MOSFET-P.
//  OLED_INIT;  // PORTD5 set OUTPUT
//  OLED1;      // PORTD5 set LOW becuase is connect to MOSFET-P.
  
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  // initialize with the I2C addr 0x3C (for the 128x32)
  //display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  // init done

  display.clearDisplay();

  while (TRUE)
  { 
    dtostrf(temperature,5,2,buf);
    // Wait for other communication (THR_SERIAL, THR_PRESSURE, etc.)
    //char cSREG = SREG;
    chSemWait(&oneSlot);
    //SREG = cSREG;
    //sei();
    //if (showSerial == 0) {
      display.setCursor(0,15);
      display.print("Temperature: ");
      display.println(buf);
      display.display();
      chThdSleepSeconds(2);
      // Clear previous data in display.
      display.clearDisplay();
      display.display();
      chThdSleepSeconds(2);
    //}
    
    chSemSignal(&oneSlot);
    chThdSleepSeconds(5);
  }
}

void setOLED(void) {
  display.ssd1306_command(SSD1306_SETDISPLAYCLOCKDIV);            // 0xD5
  display.ssd1306_command(0x80);                                  // the suggested ratio 0x80
  display.ssd1306_command(SSD1306_SETMULTIPLEX);                  // 0xA8
  display.ssd1306_command(SSD1306_LCDHEIGHT - 1);

  display.ssd1306_command(SSD1306_SETDISPLAYOFFSET);              // 0xD3
  display.ssd1306_command(0x0);                                   // no offset
  display.ssd1306_command(SSD1306_SETSTARTLINE | 0x0);            // line #0
  display.ssd1306_command(SSD1306_CHARGEPUMP);                    // 0x8D
  
  display.ssd1306_command(0x14);
  display.ssd1306_command(SSD1306_MEMORYMODE);                    // 0x20
  display.ssd1306_command(0x00);                                  // 0x0 act like ks0108
  display.ssd1306_command(SSD1306_SEGREMAP | 0x1);
  display.ssd1306_command(SSD1306_COMSCANDEC);

  #if defined SSD1306_128_32
  display.ssd1306_command(SSD1306_SETCOMPINS);                    // 0xDA
  display.ssd1306_command(0x02);
  display.ssd1306_command(SSD1306_SETCONTRAST);                   // 0x81
  display.ssd1306_command(0x8F);

  #endif

  display.ssd1306_command(SSD1306_SETPRECHARGE);                  // 0xd9
  //else
  display.ssd1306_command(0xF1);
  display.ssd1306_command(SSD1306_SETVCOMDETECT);                 // 0xDB
  display.ssd1306_command(0x40);
  display.ssd1306_command(SSD1306_DISPLAYALLON_RESUME);           // 0xA4
  display.ssd1306_command(SSD1306_NORMALDISPLAY);                 // 0xA6

  display.ssd1306_command(SSD1306_DEACTIVATE_SCROLL);

  display.ssd1306_command(SSD1306_DISPLAYON);//--turn on oled panel
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

#endif // OLED
