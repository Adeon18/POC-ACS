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
#include <project.h>
#include <stdio.h>
#include <string.h>

#define LED_ON  0
#define LED_OFF 1

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

int main()
{
    char ch;
    
    char data[6];
    char end_char = '\0';
    int num_data;
    int8 decimals_after_comma;
    char *point;

    CyGlobalIntEnable;
                    
    /* Start SCB UART TX+RX operation */    
    UART_Start(); 
    SPIM_Start();
    isr_100Hz_StartEx(isr_100Hz);
    
    CyDelay(1000);
    
	UART_UartPutString("LAB 3.2 \r\n");
    
    for(;;)
    {   
        decimals_after_comma = 0;
        // Clear the buffer
        memset(data, 0, sizeof data);
        ch = UART_UartGetChar();
        
        while(ch != '\r') {
            UART_UartPutChar(ch);
            strncat(data, &ch, 1);
            ch = UART_UartGetChar();
        }
        strncat(data, &end_char, 1);
        
        point = strchr(data, '.');
        // If there is a point, we assign its location, else the location is 0;
        if (point) {
            decimals_after_comma = (int)(point - data);
        } else {
            decimals_after_comma = 0;
        }
        
        if (ch==0) continue;
        // If the decimals after comma is not 0
        if (decimals_after_comma != 0) {
            memmove(&data[decimals_after_comma],&data[decimals_after_comma+1],strlen(data)-decimals_after_comma);
        }
        
        sscanf(data, "%d", &num_data);
        
        if (decimals_after_comma != 0) {
            out7seg((int16)num_data, strlen(data) - decimals_after_comma);
        } else {
            out7seg((int16)num_data, decimals_after_comma);
        }
        
        UART_UartPutString("\r\n");
    }
}


/* [] END OF FILE */
