/*
 * ap_main.c
 *
 *  Created on: 2026. 10. 16.
 *      Author: kccistc
 */


#include "ap_main.h"
#include "StopWatch/StopWatch.h"
#include "../driver/fnd/fnd.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* Prevent unused argument(s) compilation warning */

  if(htim->Instance == TIM3){
	  fnd_show_number();
  }

  if(htim->Instance == TIM2){
	  fnd_show_number();
  }

  /* NOTE : This function should not be modified, when the callback is needed,
            the HAL_TIM_PeriodElapsedCallback could be implemented in the user file
   */
}

int ap_main(void)
{

	StopWatch_init();
	Fnd_Mode_init();

	while(1){

		fnd_change_mode();

	}
	return 0;
}
