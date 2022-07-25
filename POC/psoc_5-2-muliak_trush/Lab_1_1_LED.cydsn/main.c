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

int main()
{
    CyGlobalIntEnable;
    
	for(;;)
    {
        Pin_LED_Write(0);
        CyDelay(300);
        Pin_LED_Write(1);
        
        Pin_LED_Blue_Write(0);
        CyDelay(300);
        Pin_LED_Blue_Write(1);
    }
}


