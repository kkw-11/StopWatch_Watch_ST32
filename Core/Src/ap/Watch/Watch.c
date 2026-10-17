/*
 * Watch.c
 *
 *  Created on: 2026. 10. 16.
 *      Author: kccistc
 */

#include "../../driver/button/button.h"
#include "../../driver/led/led.h"
#include "../../driver/fnd/fnd.h"

static void watch_run();
static void watch_run(void);
void watch_Task(void);

static void watch_update(void);

#define WATCH_DISP_MMSS   1

static uint8_t hour = 12;
static uint8_t min = 0;
static uint8_t sec = 0;
static volatile uint8_t half = 0;
static uint32_t prevTick = 0;
static uint8_t dispMode = 1;      // 0 = (HHMM), 1 = (MMSS)

static led_t led_hour_min;
static led_t led_min_sec;

static button_t btn_disp_mode;

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim2;

void watch_init(void) {
	dispMode = 0;

	led_init(&led_hour_min, GPIOB, GPIO_PIN_13);
	led_init(&led_min_sec, GPIOC, GPIO_PIN_4);

	button_init(&btn_disp_mode, GPIOC, GPIO_PIN_1);


}

/* TIM2 인터럽트에서 0.5초마다 호출 */
void watch_interrupt(void) {
	half ^= 1;

	if (half == 0) {
		if (++sec >= 60) {
			sec = 0;
			if (++min >= 60) {
				min = 0;
				if (++hour >= 24) {
					hour = 0;
				}
			}
		}
	}
}

void watch_Task(void) {

	watch_run();

}

static void watch_run(void) {
	led_t led_watch = { GPIOB, GPIO_PIN_15 };
	led_t led_stopWatch = { GPIOB, GPIO_PIN_14 };
	led_t led_stop = { GPIOB, GPIO_PIN_13 };
	led_t led_run = { GPIOC, GPIO_PIN_4 };

	uint16_t num = 0;


	switch (dispMode) {
	case 0:
		num = hour * 100 + min;

		led_on(&led_watch);
		led_off(&led_stopWatch);
		led_off(&led_stop);
		led_on(&led_run);

		if (button_getState(&btn_disp_mode) == ACT_PUSH) {
			dispMode = 1;
		}
		break;
	case 1:
		num = min * 100 + sec;

		led_on(&led_watch);
		led_off(&led_stopWatch);
		led_off(&led_stop);
		led_off(&led_run);

		if (button_getState(&btn_disp_mode) == ACT_PUSH) {
			dispMode = 0;
		}
		break;
	}

	fnd_set_num(num);

	if (half == 0) {
		fnd_set_dp(1);
	} else {
		fnd_set_dp(0);
	}
}

static void watch_update(void) {
	uint32_t now = HAL_GetTick();

	if (prevTick == 0)
		prevTick = now;

	while (now - prevTick >= 1000) {
		prevTick += 1000;

		if (++sec >= 60) {
			sec = 0;
			if (++min >= 60) {
				min = 0;
				if (++hour >= 24) {
					hour = 0;
				}
			}
		}
	}
}

