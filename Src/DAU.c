#include "DAU.h"

#define PULSES_PER_L_MIN 7.5f       // Hz per L/min (YF-S201 style sensor)
#define FLUID_DENSITY    1.0f       // kg per L (water)

// Global Variables
volatile int frequency = 0;
static volatile float cumulative_L = 0.0f;

void DAU_Init(void) {
    RCC_APB1ENR |= (3U << 0);   // TIM2-3
    RCC_APB2ENR |= (1U << 0);   // AFIO
    RCC_APB2ENR |= (1U << 2);   // GPIOA
    RCC_APB2ENR |= (1U << 4);   // GPIOC

    GPIOA_CRL &= ~(0x0F << 28);
    GPIOA_CRL |=  (8U << 28);
    GPIOA_ODR |=  (1U << 7);
    AFIO_MAPR &= ~(3U << 10);
    GPIOC_CRH &= ~(0x0F << 20);
    GPIOC_CRH |=  (2U << 20);

    TIM2_PSC = 799;
    TIM2_ARR = 999;
    TIM2_CNT = 0;
    TIM2_DIER |= (1U << 0);

    __asm volatile ("cpsid i" ::: "memory");
    NVIC_ISER0 |= (1U << 28);
    __asm volatile ("cpsie i" ::: "memory");

    TIM3_CCMR1 &= ~(3U << 8);
    TIM3_CCMR1 |=  (1U << 8);
    TIM3_CCER  |=  (1U << 4);
    TIM3_CCER  |=  (1U << 5);
    TIM3_SMCR  &= ~(7U << 0);

    TIM3_SMCR  |=  (7U << 0);
    TIM3_SMCR  |=  (6U << 4);
    TIM3_CNT   = 0;
    TIM3_CR1  |= (1U << 0);
    TIM2_CR1  |= (1U << 0);
}

void DAU_Get_Flow_Metrics(FlowData_t *data) {
    data->flow_L_min = ((float)frequency * 10.0f) / PULSES_PER_L_MIN;
    data->total_liters = cumulative_L;
    data->mass_flow_kg_min = data->flow_L_min * FLUID_DENSITY;
}

// TIM2 fires strictly every 100ms
void TIM2_IRQHandler(void) {
    frequency = TIM3_CNT;
    TIM3_CNT = 0;

    // Volume for this 100ms window (1/600 of a minute)
    cumulative_L += (((float)frequency * 10.0f) / PULSES_PER_L_MIN) / 600.0f;

    TIM2_SR &= ~(1U << 0);
}
