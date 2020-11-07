#include <stdio.h>
#include <string.h>
#include "Core.h"
#include "Serial.h"

/**********************************************************************/
#define SDI GPIO_Pin_5
#define SCK GPIO_Pin_6
#define RCK GPIO_Pin_7

int k;
GPIO_InitTypeDef GPIO_struct;
int digitNum[10] = {0x41, 0x5f, 0xd0, 0x54, 0x4e, 0x64, 0x60, 0x57, 0x40, 0x44};

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

void setNum(char str[4]) {
    int bNum[4] = {0xff, 0xff, 0xff, 0xff};
    int i = 3;
    while (k--) {
        int a = str[k] - 48;
        bNum[i] = digitNum[a];
        i--;
    }
    for (int i = 0; i < 4; i++) {
        shiftOut(bNum[i]);
    }
    delay_us(1);
    GPIO_SetBits(GPIOB, RCK);
    delay_us(1);
    GPIO_ResetBits(GPIOB, RCK);
}

int main(void)
{
  /// Setup:
    Core_begin();
    delay(100);
    Serial.begin(BAUD_1M);
    Core_PrintInfo();

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_struct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_struct.GPIO_Pin = SCK | RCK | SDI;
    GPIO_struct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &GPIO_struct);

    while (1) {
        if (Serial.available() > 0) {
            char s[4] = "";
            k = -1;
            while (Serial.available() > 0) {
                s[++k] = Serial.read();
                delay_ms(1);
            }
            for (int i = 0; i <= k; i++) {
                Serial.print(s[i]);
            }
            Serial.println();
            setNum(s);
        }
        delay(500);
    }
}
