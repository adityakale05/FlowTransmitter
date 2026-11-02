#include <stdint.h>
#include <stdbool.h>
#include "STM.h"
#include "LDU.h"


#define LCD_EN        0x04 // P2
#define LCD_RS        0x01 // P0
#define LCD_BACKLIGHT 0x08 // P3

static uint8_t SLAVE;

static void delay(volatile uint32_t count);
static void I2C1_Init(void);
static bool I2C1_Wait(uint32_t mask);
static bool I2C1_Send(uint8_t data);
static void LCD_Send(uint8_t value, bool rs);

// Public functions
void LCD_Init(uint8_t slave){
    SLAVE = slave;
    delay(500000);                                  // let the backpack power up
    I2C1_Init();
    delay(400000);

    for(int i = 0; i < 3; i++){
        uint8_t d = 0x30 | LCD_BACKLIGHT;
        I2C1_Send(d | LCD_EN);
        delay(1000);
        I2C1_Send(d & ~LCD_EN);
        delay(50000);
    }
    uint8_t d = 0x20 | LCD_BACKLIGHT;               // 4-bit mode
    I2C1_Send(d | LCD_EN);
    delay(1000);
    I2C1_Send(d & ~LCD_EN);
    delay(10000);

    LCD_Send(0x28, false);                          // 4-bit, 2 lines
    LCD_Send(0x0C, false);                          // display on
    LCD_Send(0x01, false);                          // clear
    delay(50000);
    LCD_Send(0x06, false);                          // entry mode
}

void LCD_SetCursor(uint8_t row, uint8_t col){
    LCD_Send(0x80 | ((row ? 0x40 : 0x00) + col), false);
}

void print(char *str){
    while(*str){
        LCD_Send(*str++, true);
    }
}

void print_float(float val){
    if(val < 0) val = 0;
    if(val > 99.999f) val = 99.999f;					// Clamping

    uint32_t scaled = (uint32_t)(val * 1000 + 0.5f);
    uint32_t whole = scaled / 1000;
    uint32_t frac = scaled % 1000;

    LCD_Send(whole >= 10 ? '0' + whole / 10 : ' ', true);
    LCD_Send('0' + whole % 10, true);
    LCD_Send('.', true);
    LCD_Send('0' + frac / 100, true);
    LCD_Send('0' + (frac / 10) % 10, true);
    LCD_Send('0' + frac % 10, true);
}

// Internal functions
static void delay(volatile uint32_t count){
    while (count--) __asm("nop");
}

static void I2C1_Init(){
    RCC_APB1ENR |= (1U << 21);
    RCC_APB2ENR |= (1U << 0);
    RCC_APB2ENR |= (1U << 3);
    delay(100);

    GPIOB_CRL &= ~(0x0FU << 24);
    GPIOB_CRL &= ~(0x0FU << 28);
    GPIOB_CRL |= (0x0FU << 24);
    GPIOB_CRL |= (0x0FU << 28);

    AFIO_MAPR &= ~(1U << 1);

    I2C1_CR1 |= (1U << 15);             // software reset
    I2C1_CR1 &= ~(1U << 15);

    I2C1_CR1 &= ~(1U << 0);
    I2C1_CR2 &= ~(0x3FU << 0);
    I2C1_CR2 |= (8U << 0);              // PCLK1 = 8 MHz

    I2C1_CCR = (40U << 0);              // 100 kHz
    I2C1_TRISE = (9U << 0);
    I2C1_CR1 |= (1U << 0);
}

static bool I2C1_Wait(uint32_t mask){
    uint32_t timeout = 100000;
    while(!(I2C1_SR1 & mask)){
        if(I2C1_SR1 & (1U << 10)) return false;     // no ACK
        if(--timeout == 0) return false;
    }
    return true;
}

static bool I2C1_Send(uint8_t data){
    bool ok;

    I2C1_CR1 |= (1U << 8);                          // START
    ok = I2C1_Wait(1U << 0);                        // SB

    if(ok){
        I2C1_DR = (SLAVE << 1);
        ok = I2C1_Wait(1U << 1);                    // ADDR
    }

    if(ok){
        volatile uint32_t temp = I2C1_SR1;
        temp = I2C1_SR2;
        (void)temp;
        ok = I2C1_Wait(1U << 7);                    // TXE
    }

    if(ok){
        I2C1_DR = data;
        ok = I2C1_Wait(1U << 2);                    // BTF
    }

    if(!ok) I2C1_SR1 &= ~(1U << 10);                // clear AF

    I2C1_CR1 |= (1U << 9);                          // STOP
    uint32_t timeout = 100000;
    while((I2C1_CR1 & (1U << 9)) && --timeout);

    return ok;
}

static void LCD_Send(uint8_t value, bool rs){
    uint8_t rs_bit = rs ? LCD_RS : 0;
    uint8_t high = (value & 0xF0) | rs_bit | LCD_BACKLIGHT;
    uint8_t low  = ((value << 4) & 0xF0) | rs_bit | LCD_BACKLIGHT;

    I2C1_Send(high | LCD_EN);
    delay(100);
    I2C1_Send(high & ~LCD_EN);
    delay(100);

    I2C1_Send(low | LCD_EN);
    delay(100);
    I2C1_Send(low & ~LCD_EN);
    delay(100);
}
