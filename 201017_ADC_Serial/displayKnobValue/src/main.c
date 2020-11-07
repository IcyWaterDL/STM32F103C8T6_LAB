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

/* Private typedef ----------------------------------------------------*/
/* Private define -----------------------------------------------------*/
#define SDI GPIO_Pin_7
#define SCK GPIO_Pin_8
#define RCK GPIO_Pin_9
#define Knob GPIO_Pin_0
/* Private macro ------------------------------------------------------*/

/* Private variables --------------------------------------------------*/
GPIO_InitTypeDef GPIO_struct;
uint8_t digitNum[10] = {0x41, 0x5f, 0xd0, 0x54, 0x4e, 0x64, 0x60, 0x57, 0x40, 0x44};

/* Private functions --------------------------------------------------*/
void shiftOut(int data);
void setNum(int Num);

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
    GPIO_struct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &GPIO_struct);

    ADC1_begin();

    /* Infinite loop */
    while (1) {
        int sum = 0;
        for (int i = 0; i < 16; i++) {
            int knobVal = ADC1_read12(ADC_Channel_0, ADC_SampleTime_239_5Cycles);
            sum += knobVal;
        }
        int avrg = sum / 16;
        printf("%d\n", avrg);
        setNum(avrg);
        delay_ms(100);
    }
}

void shiftOut(int data) {
    for (int i = 0; i < 8; i++) {
        if (((data << i) & 0x80) == 0x80) {
            GPIO_SetBits(GPIOB, SDI);
        }
        else {
            GPIO_ResetBits(GPIOB, SDI);
        }
        delay_us(1);
        GPIO_SetBits(GPIOB, SCK);
        delay_us(1);
        GPIO_ResetBits(GPIOB, SCK);
    }
}

void setNum(int Num) {
    uint8_t bNum[4] = {0xff, 0xff, 0xff, 0x41};
    int index = 3;

    while (Num > 0) {
        bNum[index] = digitNum[Num % 10];
        Num /= 10;
        index--;
    }

    for (int i = 0; i < 4; i++) {
        shiftOut(bNum[i]);
    }
    delay_us(1);
    GPIO_SetBits(GPIOB, RCK);
    delay_us(1);
    GPIO_ResetBits(GPIOB, RCK);
}



