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
#include "Serial.h"
#include <stdio.h>
#include <string.h>

/* Private typedef ----------------------------------------------------*/

/* Private define -----------------------------------------------------*/
#define dataServo   GPIO_Pin_6

/* Private macro ------------------------------------------------------*/

/* Private variables --------------------------------------------------*/
GPIO_InitTypeDef        GPIO_Struct;
TIM_TimeBaseInitTypeDef TimerInit;

/* Private functions --------------------------------------------------*/
void config_gpio(void);
//void config_TIMER2(void);
void config_PWM(void);
void setup(void);
void rotateServo(int position);
void Delay(int time);
int Map(int numOfInput, int numOfInputMin, int numOfInputMax,
                    int numOfOutputMin, int numOfOutputMax);


void config_gpio(void) {
    //config gpio port B
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_Struct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Struct.GPIO_Pin  = dataServo;
    GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_Struct);
}

//void config_TIMER2(void) {
//    //config timer2
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
//	TimerInit.TIM_CounterMode = TIM_CounterMode_Up;
//	TimerInit.TIM_Period = 0x4E20;
//	TimerInit.TIM_Prescaler = 72 - 1;
//	TIM_TimeBaseInit(TIM2, &TimerInit);
//	TIM_Cmd(TIM2, ENABLE);
//}

void config_PWM(void) {
    RCC->APB1ENR |= 0x02; // enable clock timer 3
    TIM3->ARR = 20000 - 1; // gia tri dem
    TIM3->PSC = 72 - 1; // bo chia prescales
    TIM3->CCMR1 = 0x60; // pwm mode 1
    TIM3->CCER = 0x01; // capture/compare register
    TIM3->CR1 = 0X01; //
    TIM3->EGR = 0x01;
}

void setup(void) {
    config_gpio();
//    config_TIMER2();
    config_PWM();
}

//void Delay(int time) {
//    while (time) {
//        TIM_SetCounter(TIM2, 0);
//        while (TIM_GetCounter(TIM2) < 1000);
//        time--;
//    }
//}

int Map(int numOfInput, int numOfInputMin, int numOfInputMax, int numOfOutputMin, int numOfOutputMax) {
  return (numOfInput - numOfInputMin) * (numOfOutputMax - numOfOutputMin) / (numOfInputMax - numOfInputMin) + numOfOutputMin;
}

void rotateServo(int position) {
    int counter = Map(position, 0, 180, 500, 2500);
    TIM3->CCR1 = counter;
}


int main(void) {

    Core_begin();
    delay(100);
    Serial.begin(BAUD_1M);
    Core_PrintInfo();

    setup();

    rotateServo(180);

    /* Infinite loop */
    while (1) {
        if (Serial.available() > 0) {
            char serialDataAsString[5] = "";
            int k = -1;
            while (Serial.available() > 0 && k < 4) {
                serialDataAsString[++k] = Serial.read();
                delay(1);
            }
            Serial.print(serialDataAsString);
            char str[] = "quay";
            if (serialDataAsString[0] == 'q' && serialDataAsString[1] == 'u' &&
                serialDataAsString[2] == 'a' && serialDataAsString[3] == 'y') {
                for (int i = 0; i <= 180; i++) {
                    rotateServo(i);
                    delay_ms(15);
                }
            }
        }
        delay_ms(100);
        for (int i = 0; i <= 180; i++) {
            rotateServo(i);
            delay_ms(15);
        }
        delay(1000);
        for (int i = 180; i >= 0; i--) {
            rotateServo(i);
            delay_ms(10);
            if (i == 90) delay(1000);
        }
        delay_ms(1000);
    }
}

