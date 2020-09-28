#include "Core.h"
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>

#define LedPin GPIO_Pin_10

GPIO_InitTypeDef GPIO_Struct;

int main(void) {
    Core_begin();
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_Struct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Struct.GPIO_Pin = LedPin;
    GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_Struct);

    while (1) {
        GPIO_SetBits(GPIOB, LedPin);
        delay_ms(500);
        GPIO_ResetBits(GPIOB, LedPin);
        delay_ms(500);
    }
}
