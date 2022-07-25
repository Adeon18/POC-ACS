/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h"

void out7seg(int16);

uint16 indyk[4]         = {0x10FF,0x20FF,0x40FF,0x80FF};
const uint8 KOD7[10]    = {0x03,0x9F,0x25,0x0D,0x99,0x49,0x41,0x1F,0x01,0x09};

CY_ISR(isr_100Hz)
{
    static uint8 N_indyk = 0u;
    
    SPIM_WriteTxData(indyk[N_indyk]);
    
    N_indyk++;
    N_indyk &= 3;    
}

void out7seg(int16 data)
{
    uint8 flag;
    if (data >= 0) {
        flag = 0;
    } else {
        flag = 1;
    }
    
    uint8 i;
    if (flag) {
        data = ~data + 1;
    }
    
    for (i = 0; i < 4; i++)
    {
        indyk[i] &= 0xFF00;
        // виводимо цифри числа попорядку
        indyk[i] |= (uint16)KOD7[data % 10];
        data /= 10;
        if (data == 0) break;
    }
    
    uint8 minus_pos = i + 1;
    
    for (i++; i < 4; i++)
    {
        indyk[i] |= 0x00FF;
    }
    if (flag) {
        indyk[minus_pos] &= 0xFF00;
        indyk[minus_pos] |= (uint16)0xFD;
    }
    
}

int main(void)
{
    int16 counter = 10;
    
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    SPIM_Start();
    isr_100Hz_StartEx(isr_100Hz);

    for(;;)
    {
        counter--;
        out7seg(counter);
        
        CyDelay(100);
        
        if (Pin_B1_Read() == 0)
        {
            CyDelay(200);
        }
        if (Pin_B2_Read() == 0) {
            counter = 0;
        }
    }
}

/* [] END OF FILE */