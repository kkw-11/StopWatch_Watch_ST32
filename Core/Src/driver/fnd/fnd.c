/*
 * fnd.c
 *
 *  Created on: 2026. 10. 14.
 *      Author: kccistc
 */

#include "fnd.h"
#include "../button/button.h"
#include "../led/led.h"
#include "../../ap/Watch/watch.h"
#include "../../ap/StopWatch/StopWatch.h"

#define FND_DIGIT_1 0
#define FND_DIGIT_2 1
#define FND_DIGIT_3 2
#define FND_DIGIT_4 3


typedef struct seg_port {
	GPIO_TypeDef *GPIOx;
	uint16_t GPIO_Pin;
} seg_port_t;

typedef enum {
	WATCH,
	STOPWATCH
} fnd_mode_state_t;


static led_t stopWatch_watch;
static button_t btn_stopwatch_watch;

static fnd_mode_state_t fnd_mode_state;

static void fnd_all_off(void);
static void fnd_sel_digit(int digit);

static uint16_t num = 0;

static void fnd_disp_number(int number, int dp);

static uint8_t fndDp = 0;

void fnd_set_dp(uint8_t on){
	fndDp = on;
}


void fnd_set_num(uint16_t n){
	num = n;
}

uint16_t fnd_get_num(void){
	return num;
}


static void fnd_setState(fnd_mode_state_t s){
	fnd_mode_state = s;
}

static void fnd_all_off(void) {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); // D1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET); // D2
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET); // D3
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET); // D4
}

static void fnd_sel_digit(int digit) {

	switch (digit) {
	case 0:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET); // D1
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET); // D2
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET); // D3
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET); // D4
		break;
	case 1:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); // D1
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET); // D2
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET); // D3
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET); // D4
		break;
	case 2:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); // D1
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET); // D2
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET); // D3
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_SET); // D4
		break;
	case 3:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET); // D1
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET); // D2
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET); // D3
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, GPIO_PIN_RESET); // D4
		break;
	}
}

static void fnd_disp_number(int number, int dp) {
	seg_port_t seg_font[8] = { { GPIOA, GPIO_PIN_6 }, { GPIOA, GPIO_PIN_9 }, {
			GPIOB, GPIO_PIN_5 }, { GPIOB, GPIO_PIN_10 }, { GPIOA,
	GPIO_PIN_8 }, { GPIOA, GPIO_PIN_7 }, { GPIOB,
	GPIO_PIN_3 }, { GPIOB, GPIO_PIN_4 } };

	int seg_font_data[10][8] = { { 1, 1, 1, 1, 1, 1, 0, 0 }, //0
			{ 0, 1, 1, 0, 0, 0, 0, 0 }, //1
			{ 1, 1, 0, 1, 1, 0, 1, 0 }, //2
			{ 1, 1, 1, 1, 0, 0, 1, 0 }, //3
			{ 0, 1, 1, 0, 0, 1, 1, 0 }, //4
			{ 1, 0, 1, 1, 0, 1, 1, 0 }, //5
			{ 1, 0, 1, 1, 1, 1, 1, 0 }, //6
			{ 1, 1, 1, 0, 0, 0, 0, 0 }, //7
			{ 1, 1, 1, 1, 1, 1, 1, 0 }, //8
			{ 1, 1, 1, 1, 0, 1, 1, 0 }, //9
			};

	for (int i = 0; i < 8; i++) {
		HAL_GPIO_WritePin(
				seg_font[i].GPIOx,
				seg_font[i].GPIO_Pin,
				seg_font_data[number][i]);
	}

	//DP(seg_font[7])
	HAL_GPIO_WritePin(seg_font[7].GPIOx, seg_font[7].GPIO_Pin,
	                  dp ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void fnd_show_number() {

	static int state = 0;
	//int data = number;
	int data = fnd_get_num();

	fnd_all_off();

	switch (state) {
	case 0:
		fnd_disp_number(data / 1000 % 10, 0);
		fnd_sel_digit(FND_DIGIT_1);
		break;
	case 1:
		fnd_disp_number(data / 100 % 10, fndDp);
		fnd_sel_digit(FND_DIGIT_2);
		break;
	case 2:
		fnd_disp_number(data / 10 % 10, 0);
		fnd_sel_digit(FND_DIGIT_3);
		break;
	case 3:
		fnd_disp_number(data % 10, 0);
		fnd_sel_digit(FND_DIGIT_4);
		break;
	}

	//HAL_Delay(1); //interrupt안에서 delay 금지
	state = (state + 1) % 4;

}

void fnd_mode_init(void){

	led_init(&stopWatch_watch, GPIOB, GPIO_PIN_15);

	button_init(&btn_stopwatch_watch, GPIOC, GPIO_PIN_0);

}


void fnd_change_mode(){
	static int fnd_state = STOPWATCH;

	switch(fnd_state){
		case WATCH:
			if(button_getState(&btn_stopwatch_watch) == ACT_PUSH){
				fnd_state = STOPWATCH;
			}
			watch_Task();
			break;
		case STOPWATCH:

			if(button_getState(&btn_stopwatch_watch) == ACT_PUSH){
				fnd_state = WATCH;
			}
			fnd_set_dp(0);
			stopWatch_task();
			break;
		default:
			break;
	}
}

