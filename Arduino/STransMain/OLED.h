/*
 * OLED.h
 *
 * Created: 13/06/2018 4:08:03 p. m.
 *  Author: camil
 */ 


#ifndef OLED_H_
#define OLED_H_

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

#define OLED1      PORTD &= ~(1<<PORTD5) // Set On OLED (P-MOSFET).
#define OLED0     PORTD |= (1<<PORTD5)  // Set Off OLED (P-MOSFET).

#endif /* OLED_H_ */
