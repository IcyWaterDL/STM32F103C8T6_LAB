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
typedef uint8_t byte;

/* Private define -----------------------------------------------------*/
//#define A 0x42
//#define B 0x57
//#define C 0xe1
//#define D 0x41
//#define E 0xe0
//#define F 0xe2
//#define G 0x61
//#define H 0x4a
//#define I 0x5f
//#define L 0xe9
//#define N 0x43
//#define O 0x41
//#define P 0xc2
//#define S 0x64
//#define T 0x57
//#define U 0x49

#define SDI GPIO_Pin_5
#define SCK GPIO_Pin_6
#define RCK GPIO_Pin_7

/* Private macro ------------------------------------------------------*/

/* Private variables --------------------------------------------------*/
byte k;
GPIO_InitTypeDef GPIO_struct;

byte digitNum[10] = {0x41, 0x5f, 0xd0, 0x54, 0x4e, 0x64, 0x60, 0x57, 0x40, 0x44};

byte c[26] = {0x42, 0x40, 0xe1, 0x41, 0xe0, 0xe2, 0x61, 0x4a, 0x5f, 0xff, 0xff, 0xe9, 0xff,
            0x43, 0x41, 0xc2, 0xff, 0xff, 0x64, 0x57, 0x49, 0xff, 0xff, 0xff, 0x4e, 0xff};
/* Private functions --------------------------------------------------*/

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
    byte bNum[4] = {0xff, 0xff, 0xff, 0xff};
    byte i = 3;
    while (k--) {
        if ((str[k] >= 'A' && str[k] <= 'Z') || (str[k] >= 'a' && str[k] <= 'z')) {
            if (str[k] >= 'a' && str[k] <= 'z') str[k] -= 32;
            int a = str[k] - 65;
            bNum[i] = c[a];
        }
        else if (str[k] >= '0' && str[k] <= '9') {
            int a = str[k] - 48;
            bNum[i] = digitNum[a];
        }
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

/**********************************************************************/
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




