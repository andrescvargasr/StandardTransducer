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

//extern volatile double pressure;

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

  display.display();
  delay(2000);

  // Clear the buffer.
  display.clearDisplay();

  
//  display.setTextSize(2);
//  display.setTextColor(WHITE);
//  display.print("0x"); display.println(0xDEADBEEF, HEX);
  

  while (TRUE)
  {
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("STrans:");
    //display.setTextColor(BLACK, WHITE); // 'inverted' text
    //display.println("3.141592");
//    display.setTextSize(2);
//    display.setTextColor(WHITE);
//    display.println("0x"); 
    //display.display();
    //delay(2000);
    //display.clearDisplay();
    //display.display();
    
    //dtostrf(temperature,5,2,buf);
    dtostrf(temperature,5,2,buf);
//    // Wait for other communication (THR_SERIAL, THR_PRESSURE, etc.)
//    //char cSREG = SREG;
//    chSemWait(&oneSlot);
//    //SREG = cSREG;
//    //sei();
//    //if (showSerial == 0) {
    //display.clearDisplay();
    //display.setCursor(0,15);
    display.print("Temperature: ");
    display.println(buf);
//      display.display();
//      chThdSleepSeconds(2);
//      // Clear previous data in display.
//      display.clearDisplay();
      display.display();
    chThdSleepSeconds(2);
    display.clearDisplay();
    display.display();
    //chThdSleepSeconds(2);
//    //}
    
    //chSemSignal(&oneSlot);
    chThdSleepSeconds(5);
  }
}

#endif // OLED
