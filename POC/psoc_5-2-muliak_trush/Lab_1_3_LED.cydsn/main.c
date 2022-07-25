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

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    PWM_Start();

    for(;;)
    {
        PWM_WriteCompare(9000);
        CyDelay(500);
        PWM_WriteCompare(100);
        CyDelay(500);
    }
}

/* [] END OF FILE */
