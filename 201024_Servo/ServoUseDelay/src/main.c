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
#define dataServo GPIO_Pin_0
/* Private macro ------------------------------------------------------*/

/* Private variables --------------------------------------------------*/
GPIO_InitTypeDef GPIO_Struct;

/* Private functions --------------------------------------------------*/
void angle(int pos, int delayTime);
int Map(int x, int inMin, int inMax, int outMin, int outMax);

int Map(int x, int inMin, int inMax, int outMin, int outMax) {
  return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

void angle(int pos, int delay_msTime) {
    int nLoop = delay_msTime / 20;
    int duration = Map(pos, 0, 180, 500, 2500);
    for (int i = 0; i < nLoop; i++) {
        GPIO_SetBits(GPIOB, dataServo);
        delayMicroseconds(duration);
        GPIO_ResetBits(GPIOB, dataServo);
        delayMicroseconds(20000 - duration);
    }
}

int main(void) {

    Core_begin();

    USART1_begin_A9A10(BAUD_1M);
    Core_PrintInfo();

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_Struct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Struct.GPIO_Pin = dataServo;
    GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_Struct);

    /* Infinite loop */
    while (1) {
        for (int i = 0; i <= 180; i += 45) {
            printf("angle = %d\n", i);
            angle(i, 20);
            delay(500);
        }
        for (int i = 180; i >= 0; i--) {
            printf("angle = %d\n", i);
            angle(i, 20);
        }
    }
}

