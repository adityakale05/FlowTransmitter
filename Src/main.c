#include <stdint.h>
#include <stdbool.h>
#include "DAU.h"
#include "LDU.h"
#include "UART.h"

#define STOP_COUNT 15       // consecutive zero-flow passes before a measurement counts as finished

int main(){
    DAU_Init();
    LCD_Init(0x27);
    UART_Init();

    LCD_SetCursor(0, 0);
    print("Flow LPM :");
    LCD_SetCursor(1, 0);
    print("Flow in L:");

    FlowData_t flow = {0};

    bool measuring = false;
    uint32_t zero_count = 0;
    float start_total = 0.0f;
    float peak_flow = 0.0f;
    float peak_mass = 0.0f;

    while(1){
        DAU_Get_Flow_Metrics(&flow);

        LCD_SetCursor(0, 10);
        print_float(flow.flow_L_min);

        LCD_SetCursor(1, 10);
        print_float(flow.total_liters);

        if(flow.flow_L_min > 0){                        // flowing
            GPIOC_ODR &= ~(1U << 13);                   // LED ON (active low)
            zero_count = 0;

            if(!measuring){                             // a new measurement starts
                measuring = true;
                start_total = flow.total_liters;
                peak_flow = 0.0f;
                peak_mass = 0.0f;
            }
            if(flow.flow_L_min > peak_flow) peak_flow = flow.flow_L_min;
            if(flow.mass_flow_kg_min > peak_mass) peak_mass = flow.mass_flow_kg_min;
        }
        else{                                           // no flow
            GPIOC_ODR |= (1U << 13);                    // LED OFF

            if(measuring && ++zero_count >= STOP_COUNT){
                measuring = false;                      // measurement finished: send once

                UART_Print("Peak flow: ");
                UART_PrintFloat(peak_flow, 3);
                UART_Print(" L/mi\nVolume: ");
                UART_PrintFloat(flow.total_liters - start_total, 3);
                UART_Print(" L\nTotal: ");
                UART_PrintFloat(flow.total_liters, 3);
                UART_Print(" L\nPeak mass: ");
                UART_PrintFloat(peak_mass, 3);
                UART_Print(" kg/min\r\n\n\n\n");
            }
        }
    }
}
