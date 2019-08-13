# Standard Transducer Versión 2.0
## Basado en el hardware: StandardTransducer-V0.2dQ

## EEPROM
El *Standard Transducer* (en adelante STrans) consta de un conjunto de **15** parámetros, los cuales tienen un tamaño de 2 bytes correspondientes a un valor tipo int. Igualmente, la memoria EEPROM del ATmega32u4 tiene una capacidad de almacenamiento de 1024 bytes, si tenemos en cuenta el tamaño de los datos que serán almacenados, entonces es posible almacenar:

1024 bytes / 2 bytes = 512 bloques

La cantidad de **512 bloques de datos de 2 bytes**, donde **12** pertenecen a los parámetros actuales y **3** pertenecen a las banderas de estados, habilitación y errores para un total de **15** variables almacenadas.

#### Parámetros
A continuación, se presenta la tabla de parámetros con los que se cuenta actualmente:

| ID | P | NAME                 | DESCRIPTION                              |
| -- | - | -------------------- | ---------------------------------------- |
| 0  | A | PARAM_PRESSURE       | Pressure of the system.                  |
| 1  | B | PARAM_ANALOG_TEMP    | Analog temperature of the system.        |
| 2  | C | PARAM_DIGITAL_TEMP_1 | Digital temperature of the sensor 1.     |
| 3  | D | PARAM_DIGITAL_TEMP_2 | Digital temperature of the sensor 2.     |
| 4  | E | PARAM_DIGITAL_TEMP_3 | Digital temperature of the sensor 3.     |
| 5  | F | PARAM_DIGITAL_TEMP_4 | Digital temperature of the sensor 4.     |
| 6  | G | PARAM_DIGITAL_HUMI_1 | Digital humidity of the sensor 1.        |
| 7  | H | PARAM_BUTTONS        | Button state of S+, S-, Z+ and Z-.       |
| 8  | I | PARAM_OLED           | Estado de la pantalla OLED.              |
| 9  | J | PARAM_WIRELESS       | Comunicación inalámbrica habilitada.     |
| 10 | K |                      |                                          |
| 11 | L | PARAM_P_MAG          | Magnitud de lectura de presión.          |
| 12 | M | PARAM_P_RANGE        | Rango de lectura de presión.             |
| 13 | N | PARAM_P_OFFSET       | _Offset_ del sensor de presión.          |
| 14 | O | PARAM_AT_MAG         | Magnitud de lectura de la temperatura.   |
| 15 | P | PARAM_AT_RANGE       | Rango de lectura de la temperatura.      |
| 16 | Q | PARAM_AT_OFFSET      | _Offset_ del sensor de temperatura.      |
| 17 | R |                      |                                          |
| 18 | S |                      |                                          |
| 19 | T |                      |                                          |
| 20 | U |                      |                                          |
| 21 | V |                      |                                          |
| 22 | W | PARAM_SERIAL_DISPLAY | Habilita/deshabilita resumen en serial.  |
| 23 | X | PARAM_ERROR          | Errores producidos en la ejecución.      |
| 24 | Y | PARAM_STATUS         | Estados del sistema.                     |
| 25 | Z | PARAM_ENABLED        | Características disponibles del sistema. |

Cada uno de estos parámetros se encargan de presentar el estado actual de cada una de las opciones disponibles en el sistema; además, las últimas opciones se encargan de presentar un compendio que incluye los errores producidos durante el funcionamiento del transductor (**PARAM_ERROR**), el estado de todas las variables sensibles para el usuario (**PARAM_STATUS**) y la posibilidad de identificar cuáles características se encuentran actualmente habilitadas (**PARAM_ENABLED**).

Existen algunos casos especiales de parámetros, **PARAM_BUTTONS** y **PARAM_WIRELESS**, los cuales representan el estado de varias opciones, de modo que **PARAM_BUTTONS** entrega los estados de los botones _Z+_, _Z-_, _S+_ y _S-_, y **PARAM_WIRELESS** entrega la disponibilidad de los sitemas de comunicación _Bluetooth_, _LoRa-Ra1_, _LoRa-RN2483_ y _LoRa-RN2903_; todo esto implica que si los botones están disponibles o algún tipo de comunicación, estos parámetros entregarán un 1 según su posición, siguiendo la tabla a continuación:

| PARAM_BUTTONS | Bit | Botón activado en PARAM_BUTTONS   |
| ------------- | --- | --------------------------------- |
| PARAM_S_P     | 0   | 0b00000001                        |
| PARAM_S_N     | 1   | 0b00000010                        |
| PARAM_Z_P     | 2   | 0b00000100                        |
| PARAM_S_N     | 3   | 0b00001000                        |

| PARAM_WIRELESS    | Bit | Comunicación disponible en PARAM_BUTTONS |
| ----------------- | --- | ---------------------------------------- |
| PARAM_BLUETOOTH   | 0   | 0b00000001                               |
| PARAM_LORA_RA1    | 1   | 0b00000010                               |
| PARAM_LORA_RN2483 | 2   | 0b00000100                               |
| PARAM_LORA_RN2903 | 3   | 0b00001000                               |

También se debe describir el valor que se entrega para los parámetros asociados a la temperatura y la presión, como son el caso de **PARAM_P_MAG**, **PARAM_P_RANGE**, **PARAM_P_OFFSET** para presión y **PARAM_AT_MAG**, **PARAM_AT_RANGE** y **PARAM_AT_OFFSET** los de la temperatura análoga.

| Parámetro        | Descripción                                  |
| ---------------- | -------------------------------------------- |
| PARAM_P_MAG      | Indica la magnitud de la lectura de presión. |
| 0                | Pascal (Pa).                                 |
| 1                | Kilo Pascal (kPa).                           |
| 2                | Libra de fuerza por pulgada cuadrada (psi).  |
| 3                | Milímetros de agua (mmH2O).                  |
| PARAM_P_RANGE    | Indica el rango de medición del sensor.      |
| 0                | 0 hasta 50 kPa (0 hasta 7.25 psi).           |
| 1                | 0 hasta 700 kPa (0 hasta 101.5 psi).         |
| PARAM_P_OFFSET   | Indica el valor _Offset_ para la presión.    |

| Parámetro        | Descripción                                      |
| ---------------- | ------------------------------------------------ |
| PARAM_AT_MAG     | Indica la magnitud de la lectura de temperatura. |
| 0                | Grados Celsius (°C).                             |
| 1                | Grados Farenheit (°F).                           |
| PARAM_AT_RANGE   | Indica el rango de medición del sensor.          |
| 0                | -212 hasta +850 °C (-331 hasta +1562 °F).        |
| 1                | -202 hasta +631 °C (-331 hasta +1168 °F).        |
| 2                | 0 hasta +100 °C (+32 hasta +212 °F).             |
| PARAM_AT_OFFSET  | Indica el valor _Offset_ para la temperatura.    |

#### Estado del transductor

Existen 3 variables que se encargan de manejar el estado del STrans, a saber:

- FLAG_STATUS: Presenta los estados actuales de los diferentes componentes del STrans.
- FLAG_ERROR: Esta variable controla los mensajes que aparecen en el sistema si ocurre algún error en los procesos.
- FLAG_ENABLED: Muestra cuáles funciones están activas y cuales no.

**FLAG_ENABLED** permite activar o desactivar determinadas funciones del _STrans_. Por ejemplo, es posible desactivar la lectura del sensor análogo de temperatura.

#### FLAG_STATUS
**FLAG_STATUS** presenta las funciones que están activas actualmente, dispone de **16 bits de datos**. Actualmente, cada uno de esos bits repreentan un estado de cada función programada en el _STrans_. ~~Se compone de diferentes *bits* que pueden ser habilitados/deshabilitados usando el método *start()* y *stop()*. Es posible comprobar el estado de una función usando *getStatus*~~.

| Bit | FLAG_STATUS            | Comment |
| --- | ---------------------- | ------- |
| 0   | FLAG_PRESSURE_CTRL     |  |
| 1   | FLAG_ANAG_TEMP_CTRL    |  |
| 2   | FLAG_DIG_TEMP_1_CTRL   |  |
| 3   | FLAG_DIG_TEMP_2_CTRL   |  |
| 4   | FLAG_DIG_TEMP_3_CTRL   |  |
| 5   | FLAG_DIG_TEMP_4_CTRL   |  |
| 6   | FLAG_DIG_HUMI_1_CTRL   |  |
| 7   | FLAG_BUTTONS_CTRL      |  |
| 8   | FLAG_OLED_CTRL         |  |
| 9   | FLAG_BLUETOOTH         |  |
| 10  | FLAG_LORA_RA_01        |  |
| 11  | FLAG_LORA_RN2483       |  |
| 12  | FLAG_LORA_RN2903       |  |
| 13  | FLAG_A0                |  |
| 14  | FLAG_BATTERY           |  |
| 15  | FLAG_REGULATOR         |  |

~~El parámetro que actualmente gobierna esta bandera es **Y**.~~

#### FLAG_ENABLED
**FLAG_ENABLED** permite activar/desactivar las funciones del _STrans_. Actualmente se pueden controlar las funciones de presión, temperatura tanto análoga como digital, humedad, lectura del nivel de batería, ~~alimentación sensores~~, comunicación *Bluetooth*, comunicación LoRa, pantalla OLED, botones.

| Bit | FLAG_ENABLED            | Comment |
| --- | ----------------------  | ------- |
| 0   | FLAG_EN_PRESSURE        |  |
| 1   | FLAG_EN_ANAG_TEMP       |  |
| 2   | FLAG_EN_DIG_TEMP_1      |  |
| 3   | FLAG_EN_DIG_TEMP_2      |  |
| 4   | FLAG_EN_DIG_TEMP_3      |  |
| 5   | FLAG_EN_DIG_TEMP_4      |  |
| 6   | FLAG_EN_DIG_HUMI_1_CTRL |  |
| 7   | FLAG_EN_BUTTONS         |  |
| 8   | FLAG_EN_OLED            |  |
| 9   | FLAG_EN_BLUETOOTH       |  |
| 10  | FLAG_EN_LORA_RA_01      |  |
| 11  | FLAG_EN_LORA_RN2483     |  |
| 12  | FLAG_EN_LORA_RN2903     |  |
| 13  | FLAG_EN_A0              |  |
| 14  | FLAG_EN_BATTERY         |  |
| 15  | FLAG_EN_REGULATOR       |  |

Debido a su composición en *bits*, cada uno puede ser habilitado/deshabilitado usando el método *start()* y *stop()*. Es posible comprobar el estado de una función usando *getStatus*.

Esta variable se puede modificar a través del parámetro **Z**. Se puede cambiar el estado de cualquier función modificando este valor. Por ejemplo, si se desea forzar el _STrans_ a detener la comunicación inalámbrica y los sensores digitales, se debe asegurar que los *bits* **FLAG_EN_DIGITAL_SENSORS** y **FLAG_EN_COMMUNICATION** estén desactivados. En otras palabras, se debe ingresar 2^8 (256) + 2^14 (16384) = 16640 al parámetro **Z**:

```bash
Z 16640 [Enter]
```

#### FLAG_ERROR
| Bit | FLAG_ERROR                | Comment                                    |
| --- | ------------------------- | ------------------------------------------ |
| 0   | FLAG_ERROR_RANGE_PRESSURE | La medición de presión está fuera de rango. |
| 1   | FLAG_ERROR_ANAG_TEMP      | La sonda análoga de temperatura está fuera de rango. |
| 2   | FLAG_ERROR_DIG_TEMP_1     | Falla en la sonda digital de temperatura 1 (No hay respuesta). |
| 3   | FLAG_ERROR_DIG_TEMP_2     | Falla en la sonda digital de temperatura 2 (no hay respuesta). |
| 4   | FLAG_ERROR_DIG_TEMP_3     | Falla en la sonda digital de temperatura 3 (no hay respuesta). |
| 5   | FLAG_ERROR_DIG_TEMP_4     | Falla en la sonda digital de temperatura 4 (no hay respuesta). |
| 6   | FLAG_ERROR_DIG_HUMI_1     | Falla en la medición de humedad (dato inválido).  |
| 7  | FLAG_ERROR_OLED           | Falla en la pantalla OLED (no hay respuesta). |
| 8  | FLAG_ERROR_COMMUNICATION  | Falla en la comunicación (no hay ningún módulo de comunicación conectado). |
| 9  | FLAG_ERROR_A0             | Falla en la lectura del sensor. |
