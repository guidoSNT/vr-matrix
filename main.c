#include <avr/io.h>
#include <stdint.h>
#define F_CPU 16000000UL
#include "I2C_Master.h"
#include "MPU6050_res_define.h"
#include "MPU6969.h"
#include "string.h"
#include "uart.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

#define MPU_ADD 0xD0
#define WEB 0
#define STEAMVR 1
#define CALIBRACION 2

int main() {
  char string[100];
  uint8_t return_value = 0;
  uint8_t state = 0;

  USART_init();

  while (1) {

    switch (state) {
    case WEB:
      strcat(string, "WEB: ");
      // get value from the mpu and concatenate with the string
      USART_putstring(string);
      break;
    case STEAMVR:
      // Comunicacion por el hid que todavia no vimos bien
      break;
    case CALIBRACION:
      USART_putstring("Calibration called. It may take a few seconds...\r\n");
      // calibration function called
      USART_putstring("Calibration finished, going back to web interface\r\n");
      state = 0;
      break;
    default:
      USART_putstring("An error ocurred with the returned value\r\n");
    }

    // Fijarse que tenga un timeout o algo por el estilo
    // La funcion del arduino esta muy buena por que es asincronica de lo que
    // reciba
    return_value = USART_receive();
    switch (return_value) {
    case 0:
      USART_putstring("Changed to WEB display\r\n");
      state = 0;
      break;
    case 1:
      USART_putstring("SteamVR selected for transmition\r\n"
                      "transmition with website will be shutdown\r\n");
      state = 1;
      break;
    case 2:
      USART_putstring("Changed to calibration\r\n");
      state = 2;
      break;
    }
    // delay
    _delay_ms(1000);
  }

  return 0;
}
