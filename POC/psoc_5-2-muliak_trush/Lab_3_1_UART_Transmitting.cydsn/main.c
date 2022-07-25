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

char TEXT_BUF[20];


int main()
{
    uint8 i = 0;
    
    CyGlobalIntEnable;

    /* Start SCB UART TX+RX operation */    
    UART_Start(); 
    
    CyDelay(100);
    
	/* Transmit String through UART TX Line */
	UART_UartPutString("CY8CKIT-042 USB-UART");
    
    for(;;)
    {
        i++;
        sprintf(TEXT_BUF,"%d\r\n",i);
        UART_UartPutString(TEXT_BUF);
        CyDelay(1000);        
    }
}


/* [] END OF FILE */
