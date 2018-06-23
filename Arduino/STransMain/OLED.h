/*
 * OLED.h
 *
 * Created: 13/06/2018 4:08:03 p. m.
 *  Author: camil
 */ 


#ifndef OLED_H_
#define OLED_H_

/************************************************************************/
/* CONSTANTS                                                            */
/************************************************************************/
#define OLED_INIT DDRD |= (1<<PORTD5) // Set PORTD5 for output.

#define OLED1      PORTD &= ~(1<<PORTD5) // Set On OLED (P-MOSFET).
#define OLED0     PORTD |= (1<<PORTD5)  // Set Off OLED (P-MOSFET).

#define OLED_RESET -1 // Reset pin is not necessary

#define XPOS 0
#define YPOS 1

void setOLED(void);
void testdrawchar(void);

#endif /* OLED_H_ */
