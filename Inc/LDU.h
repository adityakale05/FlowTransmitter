#ifndef LDU_H
#define LDU_H

#include <stdint.h>

void LCD_Init(uint8_t slave);
void LCD_SetCursor(uint8_t row, uint8_t col);
void print(char *str);
void print_float(float val);

#endif
