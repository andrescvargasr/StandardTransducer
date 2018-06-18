/************************************************************************/
/* Thread 2. Print Serial.                                              */
/************************************************************************/
#ifdef SERIAL

#define THR_SERIAL

#define MAX_PARAM 26

#define SERIAL_BUFFER_LENGTH 4
#define SERIAL_MAX_PARAM_VALUE_LENGTH  4
char serialBuffer[SERIAL_BUFFER_LENGTH];
byte serialBufferPosition = 0;

// Declare a stack with 100 bytes beyond context switch and interrupt needs.
THD_WORKING_AREA(waSerial, 64);

// Declare thread function for thread 2.
THD_FUNCTION(ThrSerial, arg) {
  (void)arg;

  Serial.begin(9600);
  while (!Serial) {
    chThdSleepMilliseconds(1); // wait for serial port to connect. Needed for native USB port only
  }

  // Wake time for thread sleep.
  //systime_t wakeTime = chVTGetSystemTimeX();



  // Add ticks for one second.  The MS2ST macro converts ms to system ticks.
  //  wakeTime += MS2ST(1000);

  while (TRUE)
  {
    // Preguntar si no se está usando la pantalla OLED
    chSemWait(&oneSlot);
    while (Serial.available()) {
      // get the new byte:
      char inChar = (char)Serial.read();
      
      // Detectar si finaliza la introducción de datos en el Serial.
      if (inChar == 13 || inChar == 10) { // CR & LF
        // this is a carriage return;
        if (serialBufferPosition > 0) {
          printResult(serialBuffer, &Serial);
        }
        serialBufferPosition = 0;
        serialBuffer[0] = '\0';
      }
      // Almacenar valor en la cadena y finalizar con \0.
      else {
        if (serialBufferPosition < SERIAL_BUFFER_LENGTH) {
          serialBuffer[serialBufferPosition] = inChar;
          serialBufferPosition++;
          if (serialBufferPosition < SERIAL_BUFFER_LENGTH) {
            serialBuffer[serialBufferPosition] = '\0';
          }
        }
      }
    }
    // Liberar el semáforo para la pantalla OLED
    chSemSignal(&oneSlot);
    chThdSleepMilliseconds(1);
  }
}

/* SerialEvent occurs whenever a new data comes in the
  hardware serial RX.  This routine is run between each
  time loop() runs, so using delay inside loop can delay
  response.  Multiple bytes of data may be available.

  This method will mainly set/read the parameters:
  Uppercase + number + CR ((-) and 1 to 5 digit) store a parameter (0 to 25 depending the letter)
  example: A100, A-1
  -> Many parameters may be set at once
  example: C10,20,30,40,50
  Uppercase + CR read the parameter
  example: A
  -> Many parameters may be read at once
  example: A,B,C,D
  s : read all the parameters
  h : help
  l : show the log file
*/

void printResult(char* data, Print* output) {
  boolean theEnd = false;
  byte paramCurrent = 0; // Which parameter are we defining
  char paramValue[SERIAL_MAX_PARAM_VALUE_LENGTH];
  paramValue[0] = '\0';
  byte paramValuePosition = 0;
  byte i = 0;
  boolean inValue = false;

  while (!theEnd) {
    // Almacenar dato
    byte inChar = data[i];
    i++;
    // Preguntar si es el límite del buffer.
    if (i == SERIAL_BUFFER_LENGTH) theEnd = true;

    // Preguntar si finaliza la cadena
    if (inChar == '\0') theEnd = true;

    // Verificar si es un número, incluyendo su signo.
    else if ((inChar > 47 && inChar < 58) || inChar == '-' || inValue) { // a number (could be negative)
      if (paramValuePosition < SERIAL_MAX_PARAM_VALUE_LENGTH) {
        paramValue[paramValuePosition] = inChar;
        paramValuePosition++;
        if (paramValuePosition < SERIAL_MAX_PARAM_VALUE_LENGTH) {
          paramValue[paramValuePosition] = '\0';
        }
      }
    }
    // Verificar si es una letra mayúscula.
    else if (inChar > 64 && inChar < 92) { // an UPPERCASE character so we define the field
      // we extend however the code to allow 2 letters fields !!!
      if (paramCurrent > 0) {
        paramCurrent *= 26;
      }
      paramCurrent += inChar - 64;
      if (paramCurrent > MAX_PARAM) {
        paramCurrent = 0;
      }
    }
    // Identificar si se agrega un comando más o finaliza
    if (inChar == ',' || theEnd) { // Store value and increment
      // Preguntar si ha ingresado una letra mayúscula
      if (paramCurrent > 0) {
        // Verificar si entró un nuevo dato para almacenar.
        if (paramValuePosition > 0) {
          setAndSaveParameter(paramCurrent - 1, atoi(paramValue));
          output->println(parameters[paramCurrent - 1]);
        }
        else output->println(parameters[paramCurrent - 1]);
        
        if (paramCurrent <= MAX_PARAM) {
          paramCurrent++;
          paramValuePosition = 0;
          paramValue[0] = '\0';
        }
      }
    }
    // Letra minúscula y (más de una letra)
    if (data[0] > 96 && data[0] < 123 && (i > 1 || data[1] < 97 || data[1] > 122)) { // we may have one or 2 lowercases
      inValue = true;
    }
  }

  // we will process the commands, it means it starts with lowercase
  switch (data[0]) {

    /* ========================================
       GENERAL AND REQUIRED PARAMETERS
      ========================================*/
    case 'c':
      if (paramValuePosition > 0);
      //printCompactParameters(output, atoi(paramValue));
      else
        //printCompactParameters(output);
        break;
    case 'h':
      printHelp(output);
      break;
    //Changed for (p)arameters
    //case 's':
      //printParameters(output);
      //break;
    case 'p':
      // Imprimir todos los parámetros según la función en Parameters
      //printParameters(output);
    break;
    case 'u':
      //processUtilitiesCommand(data[1], paramValue, output);
      break;
    default:
      processSpecificCommand(data, paramValue, output);
  }
  output->println("");
}

void printHelp(Print* output) {
  //#ifdef SHOW_MENU_HELP
  output->println(F("h - (h)elp"));
  output->println(F("p - (p)arameters"));
  output->println(F("u - (u)tilities"));

  printSpecificHelp(output);
  //#endif
}

void printSpecificHelp(Print* output) {
    //output->println(F("(i)2c"));
    //#ifdef THR_LINEAR_LOGS
    //output->println(F("(l)og"));
    //#endif
    output-> println(F("o - (o)led"));
    //output->println(F("(o)ne-wire"));
    output-> println(F("w - one-(w)ire"));
    //output->println(F("(p)aram"));

    //output->println(F("s(t)atus"));
    //#ifdef FOOD_CTRL
    //output->println(F("(w)eight"));
    //#endif
}


void noThread(Print* output) {
  output->println(F("No Thread"));
}


/* Fucntions to convert a number to hexadeciaml */

const char hex[] = "0123456789ABCDEF";

uint8_t toHex(Print* output, byte value) {
  output->print(hex[value >> 4 & 15]);
  output->print(hex[value >> 0 & 15]);
  return value;
}

uint8_t toHex(Print* output, int value) {
  byte checkDigit = toHex(output, (byte)(value >> 8 & 255));
  checkDigit ^= toHex(output, (byte)(value >> 0 & 255));
  return checkDigit;
}

uint8_t toHex(Print* output, long value) {
  byte checkDigit = toHex(output, (int)(value >> 16 & 65535));
  checkDigit ^= toHex(output, (int)(value >> 0 & 65535));
  return checkDigit;
}

void processSpecificCommand(char* data, char* paramValue, Print* output) {
  switch (data[0]) {
#ifdef THR_LORA
    case 'a':
      //processLoraCommand(data[1], paramValue, output);
      break;
#endif
    case 'i':
#if defined(GAS_CTRL) || defined(PH_CTRL)
      //wireInfo(output);
#else  //not elsif !!
      noThread(output);
#endif
      break;
#ifdef THR_LINEAR_LOGS
    case 'l':
      //processLoggerCommand(data[1], paramValue, output);
      break;
#else  //not elsif !!
      noThread(output);
#endif
    case 'o':
#if defined(TEMP_LIQ) || defined(TEMP_PCB)
      //oneWireInfo(output);
#else
      noThread(output);
#endif
      break;
    case 'p':
      printGeneralParameters(output);
      break;

    case 't':
      output->print(F("Status: "));
      //output->println(getParameter(PARAM_STATUS));
      for (byte i = 0; i < 10; i++) {
        output->print(i);
        output->print(": ");
        //output->println(getParameterBit(PARAM_STATUS, i));
      }
      output->print(F("Enabled: "));
      //output->println(getParameter(PARAM_ENABLED));
      for (byte i = 0; i < 3; i++) {
        output->print(i);
        output->print(": ");
        //output->println(getParameterBit(PARAM_ENABLED, i));
      }
      output->print(F("Error: "));
      //output->println(getParameter(PARAM_ERROR));
      for (byte i = 0; i < 6; i++) {
        output->print(i);
        output->print(": ");
        //output->println(getParameterBit(PARAM_ERROR, i));
      }
      break;
#ifdef FOOD_CTRL
    case 'w':
      //processWeightCommand(data[1], paramValue, output);
      break;
#endif
  }
}

void printGeneralParameters(Print* output) {
  output->print(F("EPOCH:"));
  //output->println(now());
  output->print(F("millis:"));
  output->println(millis());
#ifdef I2C_RELAY_FOOD
  output->print(F("I2C relay food:"));
  //output->println(I2C_RELAY_FOOD);
#endif
#ifdef FLUX
  output->print(F("I2C Flux:"));
  //output->println(I2C_FLUX);
#endif
#ifdef THR_LINEAR_LOGS
  output->print(F("Next log index:"));
  //output->println(nextEntryID);
#endif
#ifdef THR_LINEAR_LOGS
  output->print(F("FlashID:"));
  //sst.printFlashID(output);
#endif
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
#ifdef THR_SERIAL
THD_TABLE_ENTRY(waSerial, NULL, ThrSerial, NULL)
#endif
#ifdef THR_OLED
THD_TABLE_ENTRY(waOLED, NULL, ThrOLED, NULL)
#endif
THD_TABLE_END

#endif
