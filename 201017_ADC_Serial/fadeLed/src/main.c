//**********************************************************************/
//
//   Last committed:     $Revision: 00 $
//   Last changed by:    $Author: $
//   Last changed date:  $Date:  $
//   ID:                 $Id:  $
//
//**********************************************************************/

/* Includes -----------------------------------------------------------*/
#include <stdio.h>
#include "Core.h"
#include "ADC.h"
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>

/* Private typedef ----------------------------------------------------*/
/* Private define -----------------------------------------------------*/
#define SDI GPIO_Pin_10
#define RCK GPIO_Pin_11
#define SCK GPIO_Pin_12
#define Knob GPIO_Pin_0
/* Private macro ------------------------------------------------------*/

/* Private variables --------------------------------------------------*/
GPIO_InitTypeDef GPIO_struct;

/* Private functions --------------------------------------------------*/

void shiftOut(int data) {
    for (int i = 0; i < 8; i++) {
        if ((data & (1 << i)) == (1 << i)) {
            GPIO_SetBits(GPIOB, SDI);
        }
        else {
            GPIO_ResetBits(GPIOB, SDI);
        }
        GPIO_SetBits(GPIOB, SCK);
        GPIO_ResetBits(GPIOB, SCK);
    }
}

int output = 0x00;

void setLed(int index, bool state) {
    output = output & ~(1 << (7 - index)) | (state << (7 - index));
    shiftOut(output);
    GPIO_SetBits(GPIOB, RCK);
    GPIO_ResetBits(GPIOB, RCK);
}

int main(void) {
    Core_begin();
    USART1_begin_A9A10(BAUD_1M);
    Core_PrintInfo();

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_struct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_struct.GPIO_Pin = SCK | RCK | SDI;
    GPIO_struct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &GPIO_struct);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_struct.GPIO_Mode = GPIO_Mode_AIN;
//    GPIO_struct.GPIO_Pin = Knob;
    GPIO_struct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &GPIO_struct);

    ADC1_begin();

    /* Infinite loop */
    while (1) {
        int knobVal = ADC1_read12(ADC_Channel_0, ADC_SampleTime_239_5Cycles);
        int nLed = knobVal * 8 / 4090;
        printf("%d\t%d\n", knobVal, nLed);
        for (int i = nLed; i < 8; i++) {
            setLed(i, false);
        }
        for (int i = 0; i < nLed; i++) {
            setLed(i, true);
        }
        delay_ms(20);
    }
}

