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
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
/* Private typedef ----------------------------------------------------*/
/* Private define -----------------------------------------------------*/
#define b1 GPIO_Pin_4
/* Private macro ------------------------------------------------------*/
/* Private variables --------------------------------------------------*/
GPIO_InitTypeDef GPIO_struct;
/* Private functions --------------------------------------------------*/

#define ARR_SIZE 500

uint8_t arr[ARR_SIZE];
int main(void) {

    Core_begin();

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_struct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_struct.GPIO_Pin = GPIO_Pin_2;
    GPIO_struct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &GPIO_struct);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_struct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_struct.GPIO_Pin = b1;
    GPIO_struct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &GPIO_struct);

    bool ledState = false;
    bool buttonState = false;
    bool lastButtonState = false; // trang thai button truoc khi kiem tra
    int lastDebounceTime = 0;
    int delayDebounce = 50; // thoi gian giam nhieu

    USART1_begin_A9A10(BAUD_1M);
    Core_PrintInfo();

    /* Infinite loop */
    while (1) {
        bool reading = GPIO_ReadInputDataBit(GPIOA, b1); // doc trang thai button
        //printf("%d\n", reading);
//        if (reading == 0) {
//            for (int i = 0; i < ARR_SIZE; i++) {
//                arr[i] = GPIO_ReadInputDataBit(GPIOA, b1);
//            }
//
//            //printf("#############\n");
//
//            for (int i = 0; i < ARR_SIZE; i++) {
//                printf("%d\n", arr[i]);
//            }
//        }

//
        if (reading != lastButtonState) { // neu trang thai button thay doi thi luu thoi gian trang thai button truoc
            lastDebounceTime = millis();
        }
        if ((millis() - lastDebounceTime) > delayDebounce) { // neu thoi gian giu trang thai hien tai lon hon thoi gian delayBounce
            if (reading != buttonState) { // neu trang thai button doc duoc da thay doi
                buttonState = reading;
                if (buttonState == false) { // neu trang thai button = false thi thay doi trang thai led
                    ledState = !ledState;
                }
            }
        }
        if (ledState) {
            GPIO_SetBits(GPIOB, GPIO_Pin_2);
        }
        else {
            GPIO_ResetBits(GPIOB, GPIO_Pin_2);
        }
        lastButtonState = reading; // gan gia tri button trc khi kiem tra tiep
    }

}


