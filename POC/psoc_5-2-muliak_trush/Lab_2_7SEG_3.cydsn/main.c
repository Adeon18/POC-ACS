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

void out7seg(int16, uint8);

uint16 indyk[4]         = {0x10FF,0x20FF,0x40FF,0x80FF};
const uint8 KOD7[10]    = {0x03,0x9F,0x25,0x0D,0x99,0x49,0x41,0x1F,0x01,0x09};

CY_ISR(isr_100Hz)
{
    static uint8 N_indyk = 0u;
    
    SPIM_WriteTxData(indyk[N_indyk]);
    
    N_indyk++;
    N_indyk &= 3;    
}

void out7seg(int16 data, uint8 comma_pos)
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
        if (comma_pos < 3 && comma_pos > 0) {
            indyk[comma_pos] ^= (-0 ^ indyk[comma_pos]) & (1UL << 0);
        }
    } else {
        if (comma_pos < 4 && comma_pos > 0) {
            indyk[comma_pos] ^= (-0 ^ indyk[comma_pos]) & (1UL << 0);
        }
    }
    
}


int main(void)
{
    uint16 counter = 100;
    uint8 decimals_after_comma = 2;
    /*float divider = 10;
    float data = 0;*/
    
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    SPIM_Start();
    isr_100Hz_StartEx(isr_100Hz);
    

    for(;;)
    {
        counter++;
        /*data = counter;
        decimals_after_comma = 0;
        while(data != ((int)data))
        {
            decimals_after_comma++;
            data = data * divider;
        }*/
        out7seg(counter, decimals_after_comma);
        
        CyDelay(300);
        
        if (Pin_B1_Read() == 0)
        {
            CyDelay(700);
        }
        if (Pin_B2_Read() == 0) {
            counter = 100;
        }
    }
}

/* [] END OF FILE */
