#ifndef DAU_H
#define DAU_H

#include "STM.h"

typedef struct {
    float frequency;          // 1. Input frequency / pulse count
    float flow_L_min;         // 2. Q: Flow rate in Liters/minute (freq / 7.5)
    float total_liters;       // 5. FL: Cumulative total volume in Liters
    float mass_flow_kg_min;   // 7. Mass flow rate in kg/min (water density)
} FlowData_t;

extern volatile int frequency;
void DAU_Init(void);
void DAU_Get_Flow_Metrics(FlowData_t *data);
#endif // DAU_H
