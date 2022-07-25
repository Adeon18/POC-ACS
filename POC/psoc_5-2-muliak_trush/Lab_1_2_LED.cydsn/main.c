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
        Pin_LED_Write(~Pin_LED_Blue_Read());
        CyDelay(500);
        Pin_LED_Write(~Pin_LED_Read());
        Pin_LED_Blue_Write(~Pin_LED_Read());
        CyDelay(2000);
        Pin_LED_Blue_Write(~Pin_LED_Blue_Read());
    }
}

/* [] END OF FILE */
