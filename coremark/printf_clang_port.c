#include <stdio.h>
#include <string.h>
#include "stm32f4xx_hal.h"
#include "usart.h"

static int my_fputc(char ch, FILE *f) {
    HAL_UART_Transmit(&huart1, (uint8_t *) &ch, 1, 1000);
    return ch;
}

static FILE __stdio = FDEV_SETUP_STREAM(my_fputc, NULL, NULL, _FDEV_SETUP_WRITE);
FILE *const stdin = &__stdio;
__strong_reference(stdin, stdout);
__strong_reference(stdin, stderr);
