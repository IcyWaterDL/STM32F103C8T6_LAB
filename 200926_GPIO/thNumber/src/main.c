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
#define SCK GPIO_Pin_13
#define SDI GPIO_Pin_14
#define RCK GPIO_Pin_15

/* Private macro ------------------------------------------------------*/

/* Private variables --------------------------------------------------*/
GPIO_InitTypeDef GPIO_struct;

/* Private functions --------------------------------------------------*/
void shiftOut(int data);
void setNum(float num);

int main(void) {
    Core_begin();

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_struct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_struct.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13;
    GPIO_struct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &GPIO_struct);
    /* Infinite loop */
    while (1) {
//        for (float i = 5.0; i < 6.0; i += 0.1) {
//            setNum(i);
//            delay_ms(500);
//        }
        setNum(10.23);
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

byte digitNum[10] = {0xbe, 0xA0, 0x2F, 0xAB, 0xB1, 0x9B, 0x9F, 0xA8, 0xBF, 0xBB};

void setNum(float num) {
    int bNum[4] = {0xff, 0xff, 0xff, 0x41};
    int y = 0;
    float value = num;
    while ((int)num != num) {
        k[y] = (int)num;
        num *= 10;
        y++;
    }

    int x = 3, val = abs(num);

    while (val > 0) {
        bNum[x] = ~digitNum[val % 10];
        val /= 10;
        x--;
    }

    if (abs(num) != num) {
        if (value < 1 && value > -1)
            bNum[--x] &= 0xfe;
        else bNum[x] &= 0xfe;
    }
    if ((int)value != value) {
        bNum[3-y] &= 0xbf;
        if (value < 1 && value > -1) {
            bNum[++x] &= 0x41;
        }
    }
    for (int i = 0; i < 4; i++) {
        shiftOut(bNum[i]);
    }
    delay_us(1);
    GPIO_SetBits(GPIOB, RCK);
    delay_us(1);
    GPIO_ResetBits(GPIOB, RCK);
}
