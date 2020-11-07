//**********************************************************************/
//
//   Last committed:     $Revision: 00 $
//   Last changed by:    $Author: $
//   Last changed date:  $Date:  $
//   ID:                 $Id:  $
//
//**********************************************************************/

/* Includes -----------------------------------------------------------*/
#include "Core.h"
#include <stdio.h>

/* Private typedef ----------------------------------------------------*/

/* Private define -----------------------------------------------------*/
#define dataServo   GPIO_Pin_0

/* Private macro ------------------------------------------------------*/

/* Private variables --------------------------------------------------*/
GPIO_InitTypeDef        GPIO_Struct;
TIM_TimeBaseInitTypeDef TimerInit;

/* Private functions --------------------------------------------------*/
void config_gpio(void);
void config_TIMER2(void);
void config_PWM(void);
void setup(void);
void rotateServo(int position);
void Delay(int time);
int Map(int numOfInput, int numOfInputMin, int numOfInputMax,
                    int numOfOutputMin, int numOfOutputMax);


void config_gpio(void) {
    //config gpio port B
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_Struct.GPIO_Mode = GPIO_Mode_AF_PP | GPIO_Mode_Out_PP;
    GPIO_Struct.GPIO_Pin  = dataServo;
    GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_Struct);
}

void config_TIMER2(void) {
    //config timer2
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	TimerInit.TIM_CounterMode = TIM_CounterMode_Up;
	TimerInit.TIM_Period = 0x4E20;
	TimerInit.TIM_Prescaler = 72 - 1;
	TIM_TimeBaseInit(TIM2, &TimerInit);
	TIM_Cmd(TIM2, ENABLE);
}

void config_PWM(void) {
    RCC->APB1ENR |= 0x01;
    TIM2->ARR = 20000 - 1;
    TIM2->PSC = 72 - 1;
    TIM2->CCMR1 = 0x60;
    TIM2->CCER = 0x01;
    TIM2->CR1 = 0X01;
    TIM2->EGR = 0x01;
}

void setup(void) {
    config_gpio();
    config_TIMER2();
    config_PWM();
}

void Delay(int time) {
    while (time) {
        TIM_SetCounter(TIM2, 0);
        while (TIM_GetCounter(TIM2) < 1000);
        time--;
    }
}

int Map(int numOfInput, int numOfInputMin, int numOfInputMax, int numOfOutputMin, int numOfOutputMax) {
  return (numOfInput - numOfInputMin) * (numOfOutputMax - numOfOutputMin) / (numOfInputMax - numOfInputMin) + numOfOutputMin;
}

void rotateServo(int position) {
    int counter = Map(position, 0, 180, 500, 2500);
    TIM2->CCR1 = counter;
}


int main(void) {

    Core_begin();
    Serial.begin(BAUD_1M);
    Core_PrintInfo();

    setup();

    rotateServo(0);

    /* Infinite loop */
    while (1) {
        if (Serial.available() > 0) {
            char serialDataAsString[100] = "";
            int i = -1;
            while (Serial.available() > 0) {
                serialDataAsString[++i] = Serial.read();
            }
            if (strcmp(serialDataAsString, "quay") == 0) {
                for (int i = 0; i <= 180; i++) {
                    rotateServo(i);
                    delay_ms(15);
                }
                Delay(1000);
            }
        }
        delay_ms(100);
//        for (int i = 0; i <= 180; i++) {
//            rotateServo(i);
//            delay_ms(15);
//        }
//        Delay(1000);
//        for (int i = 180; i >= 0; i--) {
//            rotateServo(i);
//            delay_ms(10);
//            if (i == 90) Delay(1000);
//        }
//        delay_ms(1000);
    }
}

