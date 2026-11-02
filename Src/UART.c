#include <stdint.h>
#include <stdbool.h>
#include "STM.h"
#include "UART.h"

static void PrintUInt(uint32_t n);

// Public functions
void UART_Init(){
    RCC_APB1ENR |= (1U << 17);          // USART2 clock
    RCC_APB2ENR |= (1U << 2);           // GPIOA clock

    GPIOA_CRL &= ~(0x0FU << 8);
    GPIOA_CRL |= (0x0BU << 8);          // PA2 TX: AF push-pull, 50 MHz

    USART2_BRR = 833;                   // 8 MHz / 9600
    USART2_CR1 |= (1U << 3);            // TE (transmit only)
    USART2_CR1 |= (1U << 13);           // UE
}

void UART_Send(uint8_t data){
    while(!(USART2_SR & (1U << 7)));    // wait TXE
    USART2_DR = data;
}

void UART_Print(const char *str){
    while(*str){
        UART_Send(*str++);
    }
}

void UART_PrintInt(int32_t num){
    if(num < 0){
        UART_Send('-');
        PrintUInt(-(uint32_t)num);
    }
    else{
        PrintUInt(num);
    }
}

void UART_PrintFloat(float val, uint8_t decimals){
    if(val != val){                     // NaN
        UART_Print("NaN");
        return;
    }

    bool neg = (val < 0);
    if(neg) val = -val;

    if(decimals > 6) decimals = 6;
    uint32_t scale = 1;
    for(uint8_t i = 0; i < decimals; i++){
        scale *= 10;
    }

    float f = val * scale + 0.5f;
    if(f >= 4000000000.0f){             // too big (or infinity)
        UART_Print("OVF");
        return;
    }

    uint32_t scaled = (uint32_t)f;
    if(neg && scaled != 0) UART_Send('-');     // no "-0.000"

    PrintUInt(scaled / scale);

    if(decimals > 0){
        uint32_t frac = scaled % scale;
        UART_Send('.');
        for(uint32_t d = scale / 10; d > 0; d /= 10){
            UART_Send('0' + (frac / d) % 10);
        }
    }
}

// Internal functions
static void PrintUInt(uint32_t n){
    char tmp[10];
    uint8_t i = 0;

    if(n == 0) tmp[i++] = '0';
    while(n > 0){
        tmp[i++] = '0' + (n % 10);
        n /= 10;
    }
    while(i > 0){
        UART_Send(tmp[--i]);
    }
}
