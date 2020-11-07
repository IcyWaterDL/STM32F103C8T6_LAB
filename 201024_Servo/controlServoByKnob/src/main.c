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
#include "ADC.h"
#include <stdio.h>

/* Private typedef ----------------------------------------------------*/

/* Private define -----------------------------------------------------*/
#define dataServo        GPIO_Pin_0
#define Knob             GPIO_Pin_7
#define cycleMicroseconds 20000

/* Private macro ------------------------------------------------------*/

/* Private variables --------------------------------------------------*/
GPIO_InitTypeDef GPIO_Struct;


/* Private functions --------------------------------------------------*/
void angle(int position, int timeDelayMillis);
void speed(int sp);
void config_gpio(void);
void config_ADC(void);
void setup(void);
int Map(int x, int inMin, int inMax, int outMin, int outMax);

void config_gpio(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_Struct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Struct.GPIO_Pin = dataServo;
    GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_Struct);
}

void config_ADC(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_Struct.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Struct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &GPIO_Struct);
    ADC1_begin();
}

void setup(void) {
    config_gpio();
    config_ADC();
}

int Map(int x, int inMin, int inMax, int outMin, int outMax) {
  return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

void angle(int position, int timeDelayMillis) {
    int numberOfLoopFor = timeDelayMillis / 20;
    int duration = Map(position, 0, 180, 500, 2500);
    for (int i = 0; i < numberOfLoopFor; i++) {
        GPIO_SetBits(GPIOB, dataServo);
        delayMicroseconds(duration);
        GPIO_ResetBits(GPIOB, dataServo);
        delayMicroseconds(cycleMicroseconds - duration);
    }
}

int main(void) {
    Core_begin();
    USART1_begin_A9A10(BAUD_1M);
    Core_PrintInfo();

    setup();
    int pos = 0;

    /* Infinite loop */
    while (1) {

        angle(pos, 20);
        int sum = 0;
        int knobVal = 0;
        for (int i = 0; i < 16; i++) {
            knobVal = ADC1_read12(ADC_Channel_7, ADC_SampleTime_239_5Cycles);
            sum += knobVal;
        }
        int avrg = sum / 16;
        pos = Map(avrg, 0, 4095, 0, 180);
        printf("avrg = %d\t pos = %d\n", avrg, pos);
        delay_ms(10);
    }
}
