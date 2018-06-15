// #define F_CPU 16000000UL  //Leonardo Bootloader
#define F_CPU 8000000UL //Lilypad USB Bootloader

#include "ChNil.h"

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
