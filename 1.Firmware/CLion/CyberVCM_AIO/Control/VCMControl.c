#include "VCMControl.h"
#include "tim.h"
#include "param.h"


Main_Data main_data; 
PWM_Control_Params pwm_control_params;

/** 
 * @brief vcm position init 
 * 
 *
 */
void UP_Move(uint8_t duty)
{
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, duty);
	HAL_Delay(50);
}





