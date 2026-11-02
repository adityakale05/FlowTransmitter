#ifndef STM_H
#define STM_H

#include <stdint.h>

// --- Base Addresses ---
#define RCC     0x40021000UL
#define AFIO    0x40010000UL
#define TIM3    0x40000400UL
#define NVIC    0xE000E100UL
#define TIM2    0x40000000UL
#define GPIOA   0x40010800UL
#define GPIOB   0x40010C00UL
#define GPIOC   0x40011000UL
#define USART2  0x40004400UL
#define I2C1    0x40005400UL

// --- Register Access Macros ---
#define RCC_APB1ENR     *(volatile uint32_t*)(RCC + 0x1C)
#define RCC_APB2ENR     *(volatile uint32_t*)(RCC + 0x18)

#define GPIOA_CRL       *(volatile uint32_t*)(GPIOA + 0x00)
#define GPIOA_ODR       *(volatile uint32_t*)(GPIOA + 0x0C)

#define GPIOB_CRL       *(volatile uint32_t*)(GPIOB + 0x00)
#define GPIOB_CRH       *(volatile uint32_t*)(GPIOB + 0x04)
#define GPIOB_ODR       *(volatile uint32_t*)(GPIOB + 0x0C)

#define GPIOC_CRH       *(volatile uint32_t*)(GPIOC + 0x04)
#define GPIOC_ODR       *(volatile uint32_t*)(GPIOC + 0x0C)

#define AFIO_MAPR       *(volatile uint32_t*)(AFIO + 0x04)

#define TIM2_CR1        *(volatile uint32_t*)(TIM2 + 0x00)
#define TIM2_DIER       *(volatile uint32_t*)(TIM2 + 0x0C)
#define TIM2_SR         *(volatile uint32_t*)(TIM2 + 0x10)
#define TIM2_CNT        *(volatile uint32_t*)(TIM2 + 0x24)
#define TIM2_PSC        *(volatile uint32_t*)(TIM2 + 0x28)
#define TIM2_ARR        *(volatile uint32_t*)(TIM2 + 0x2C)

#define TIM3_CR1        *(volatile uint32_t*)(TIM3 + 0x00)
#define TIM3_SMCR       *(volatile uint32_t*)(TIM3 + 0x08)
#define TIM3_CCMR1      *(volatile uint32_t*)(TIM3 + 0x18)
#define TIM3_CCMR2      *(volatile uint32_t*)(TIM3 + 0x1C)
#define TIM3_CCER       *(volatile uint32_t*)(TIM3 + 0x20)
#define TIM3_CNT        *(volatile uint32_t*)(TIM3 + 0x24)

#define USART2_SR       *(volatile uint32_t*)(USART2 + 0x00)
#define USART2_DR       *(volatile uint32_t*)(USART2 + 0x04)
#define USART2_BRR      *(volatile uint32_t*)(USART2 + 0x08)
#define USART2_CR1      *(volatile uint32_t*)(USART2 + 0x0C)
#define USART2_CR3 *(volatile uint32_t*)(USART2+0x14)

#define I2C1_CR1        *(volatile uint32_t*)(I2C1 + 0x00)
#define I2C1_CR2        *(volatile uint32_t*)(I2C1 + 0x04)
#define I2C1_DR         *(volatile uint32_t*)(I2C1 + 0x10)
#define I2C1_SR1        *(volatile uint32_t*)(I2C1 + 0x14)
#define I2C1_SR2        *(volatile uint32_t*)(I2C1 + 0x18)
#define I2C1_CCR        *(volatile uint32_t*)(I2C1 + 0x1C)
#define I2C1_TRISE      *(volatile uint32_t*)(I2C1 + 0x20)

#define NVIC_ISER0      *(volatile uint32_t*)(NVIC + 0x00)
#define NVIC_ISER1      *(volatile uint32_t*)(NVIC + 0x04)

#endif // STM_H
