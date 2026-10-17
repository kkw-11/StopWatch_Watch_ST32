/*
 * Watch.c
 *
 *  Created on: 2026. 10. 16.
 *      Author: kccistc
 */

#include "../../driver/button/button.h"
#include "../../driver/led/led.h"
#include "../../driver/fnd/fnd.h"


static void Watch_run();
static void Watch_run(void);
void Watch_Task(void);

static void Watch_update(void);

#define WATCH_DISP_MMSS   1

static uint8_t  hour = 12;
static uint8_t  min  = 0;
static uint8_t  sec  = 0;
static uint32_t prevTick = 0;
static uint8_t dispMode = 1;      // 0 = (HHMM), 1 = (MMSS)

void Watch_Task(void){
	led_t led_watch = { GPIOB, GPIO_PIN_15 };
	led_t led_stopWatch = { GPIOB, GPIO_PIN_14 };
	led_t led_stop = { GPIOB, GPIO_PIN_13 };
	led_t led_run = { GPIOC, GPIO_PIN_4 };

	Watch_run();
	led_on(&led_watch);
	led_off(&led_stopWatch);
	led_off(&led_stop);
	led_off(&led_run);

}


static void Watch_run(void){
	uint16_t num;

	Watch_update();

	if(dispMode == 1){
		num = min * 100 + sec;
	}
	else{
		num = hour * 100 + min;
	}

	fnd_set_num(num);

	if(HAL_GetTick() - prevTick < 500){
			fnd_set_dp(1);
		}
	else{
		fnd_set_dp(0);
	}
}

static void Watch_update(void){
	uint32_t now = HAL_GetTick();

	if(prevTick == 0) prevTick = now;

	while(now - prevTick >= 1000){
		prevTick += 1000;

		if(++sec >= 60){
			sec = 0;
			if(++min >= 60){
				min = 0;
				if(++hour >= 24){
					hour = 0;
				}
			}
		}
	}
}


