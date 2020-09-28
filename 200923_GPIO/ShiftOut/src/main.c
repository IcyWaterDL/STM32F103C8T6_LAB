#include "Core.h"
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>

#define SCK GPIO_Pin_15
#define RCK GPIO_Pin_14
#define SDI GPIO_Pin_13

GPIO_InitTypeDef GPIO_struct;

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

void effect1(void) {
    for (int i = 0; i < 8; i++) {
        setLed(i, 1);
        delay_ms(200);
    }
    for (int i = 0; i < 8; i++) {
        setLed(i, 0);
    }
}

void effect2(void) {
    int i = 3, j = 4;
    while (i >= 0 && j < 8) {
        setLed(i, 1);
        setLed(j, 1);
        delay_ms(100);
        setLed(i, 0);
        setLed(j, 0);
        i--; j++;
    }

    i = 0, j = 7;
    while (i <= j) {
        setLed(i, 1);
        setLed(j, 1);
        delay_ms(100);
        setLed(i, 0);
        setLed(j, 0);
        i++; j--;
    }
}

int main(void) {
    Core_begin();

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_struct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_struct.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13;
    GPIO_struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_struct);

    while (1) {
        effect1();
        delay_ms(1000);
        effect2();
        delay_ms(1000);
    }
}
