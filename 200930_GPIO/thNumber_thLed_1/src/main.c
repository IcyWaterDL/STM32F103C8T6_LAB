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
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>

/* Private typedef ----------------------------------------------------*/
typedef uint8_t byte;

/* Private define -----------------------------------------------------*/
#define SDI GPIO_Pin_13
#define SCK GPIO_Pin_14
#define RCK GPIO_Pin_15
#define b1  GPIO_Pin_4
#define b2  GPIO_Pin_5
#define b3  GPIO_Pin_6
#define b4  GPIO_Pin_15

/* Private macro ------------------------------------------------------*/

/* Private variables --------------------------------------------------*/
GPIO_InitTypeDef GPIO_struct;
byte digitNum[10] = {0x41, 0x5f, 0xd0, 0x54, 0x4e, 0x64, 0x60, 0x57, 0x40, 0x44};
int dir = -1;
int nCount = 0;
int indexLed = 0;

/* Private functions --------------------------------------------------*/
void shiftOut(int data);
void setNum(int nCount);
void checkButton(void);

int main(void) {

    Core_begin();

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_struct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_struct.GPIO_Pin = SCK | RCK | SDI;
    GPIO_struct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &GPIO_struct);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_struct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_struct.GPIO_Pin = b1 | b2 | b3 | b4;
    GPIO_struct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &GPIO_struct);

    /* Infinite loop */
    while (1) {
        checkButton();
        if (dir == 1) {
            while (1) {
                if (nCount > 9999) nCount = 0;
                else nCount++;
                setNum(nCount);
                delay_ms(100);
                checkButton();
                if (dir != 1) break;
            }
        }
        else if (dir == 0) {
            while (1) {
                if (nCount < 0) nCount = 9999;
                else nCount--;
                setNum(nCount);
                delay_ms(100);
                checkButton();
                if (dir != 0) break;
            }
        }
    }
}

void checkButton(void) {
    if (GPIO_ReadInputDataBit(GPIOA, b1) == 0) {
        dir = 1;
        return;
    }
    if (GPIO_ReadInputDataBit(GPIOA, b3) == 0) {
        dir = 0;
        return;
    }
    if (GPIO_ReadInputDataBit(GPIOA, b2) == 0) {
        delay(50);
        if (GPIO_ReadInputDataBit(GPIOA, b2) == 0) {
            while (GPIO_ReadInputDataBit(GPIOA, b2) == 0);
            if (indexLed > 7) indexLed = 0;
            else indexLed++;
        }
    }
    else if (GPIO_ReadInputDataBit(GPIOA, b4) == 0) {
        delay(50);
        if (GPIO_ReadInputDataBit(GPIOA, b4) == 0) {
            while (GPIO_ReadInputDataBit(GPIOA, b4) == 0);
            if (indexLed < 0) indexLed = 7;
            else indexLed--;
        }
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

void setNum(int nCount) {
    byte bNum[5] = {0xff, 0xff, 0xff, 0xff, 0x41};
    int x = 4, val = nCount;
    while (val > 0) {
        bNum[x] = digitNum[val % 10];
        val /= 10;
        x--;
    }
    bNum[0] = (1 << indexLed);
    for (int i = 0; i < 5; i++) {
        shiftOut(bNum[i]);
    }
    delay_us(1);
    GPIO_SetBits(GPIOB, RCK);
    delay_us(1);
    GPIO_ResetBits(GPIOB, RCK);
}
