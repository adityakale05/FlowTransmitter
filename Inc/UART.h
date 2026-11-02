#ifndef UART_H
#define UART_H

#include <stdint.h>

void UART_Init(void);
void UART_Send(uint8_t data);
void UART_Print(const char *str);
void UART_PrintInt(int32_t num);
void UART_PrintFloat(float val, uint8_t decimals);

#endif
