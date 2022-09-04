#include "common_inc.h"
#include "retarget.h"

void Main()
{
    for(;;)
    {
        float temp = 0.618;
        HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
        printf("Hello Word\n");
        //printf("%f\n", temp);
        HAL_Delay(500);
    }
}



