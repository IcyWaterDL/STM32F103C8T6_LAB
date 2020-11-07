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

/* Private typedef ----------------------------------------------------*/

/* Private define -----------------------------------------------------*/
#define LED0 GPIO_Pin_0
#define LED1 GPIO_Pin_1
#define LED2 GPIO_Pin_2
#define LED3 GPIO_Pin_3
#define LED4 GPIO_Pin_6
#define LED5 GPIO_Pin_7
#define LED6 GPIO_Pin_0
#define LED7 GPIO_Pin_1

#define Prescaler     7200
#define Period         200
#define ClockDivision    0

/* Private macro ------------------------------------------------------*/

/* Private variables --------------------------------------------------*/

/* Private functions --------------------------------------------------*/
void setup(void);
void configureGPIO(void);
void configureTIM2PWM(void);
void configureTIM3PWM(void);
void setLed(int indexLed, int brightness);

void configureGPIO(void) {

    GPIO_InitTypeDef GPIO_Struct;
//    F103_GPIO_pinMode_output(GPIOA, , int mode_speed);

    //configure GPIOA
    GPIO_Struct.GPIO_Pin = LED0 | LED1 | LED2 | LED3 | LED4 | LED5;
    GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Struct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_Struct);

    //configure GPIOB
    GPIO_Struct.GPIO_Pin = LED6 | LED7;
    GPIO_Struct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Struct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_Struct);
}

void configureTIM2PWM(void) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    // enable timer clock

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);   // Enable clock for TIM2

    // configure timer

    TIM_TimeBaseStructure.TIM_Prescaler = Prescaler;                // Prescaler
    TIM_TimeBaseStructure.TIM_Period = Period;                      // Preiod
    TIM_TimeBaseStructure.TIM_ClockDivision = ClockDivision;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     // Count up mode
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

//    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
//    GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);

    // PWM1 Mode configuration: channel 1, 2, 3, 4

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               // Mode PWM1
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       // Specifies the output polarity: positive mode
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   // enable channel 1, 2, 3, 4

    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);
    TIM_OC4Init(TIM2, &TIM_OCInitStructure);

    // enable timer

    TIM_Cmd(TIM2, ENABLE);

}

void configureTIM3PWM(void){
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    // enable timer clock

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    // configure timer

    TIM_TimeBaseStructure.TIM_Prescaler = Prescaler;                // Prescaler
    TIM_TimeBaseStructure.TIM_Period = Period;
    TIM_TimeBaseStructure.TIM_ClockDivision = ClockDivision;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     // Count up mode
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    // PWM1 Mode configuration: channel 1, 2, 3, 4

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               // Mode PWM1
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       // Specifies the output polarity: positive mode
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   // enable channel 1, 2, 3, 4

    TIM_OC1Init(TIM3, &TIM_OCInitStructure);
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);

    // enable timer

    TIM_Cmd(TIM3, ENABLE);
}

void setup(void) {
    configureTIM2PWM();
    configureTIM3PWM();
    configureGPIO();
}

void setLed(int indexLed, int brightness) {
    if (indexLed == 0) {
        TIM_SetCompare1(TIM2, brightness);
    }
    else if (indexLed == 1) {
        TIM_SetCompare2(TIM2, brightness);
    }
    else if (indexLed == 2) {
        TIM_SetCompare3(TIM2, brightness);
    }
    else if (indexLed == 3) {
        TIM_SetCompare4(TIM2, brightness);
    }
    else if (indexLed == 4) {
        TIM_SetCompare1(TIM3, brightness);
    }
    else if (indexLed == 5) {
        TIM_SetCompare2(TIM3, brightness);
    }
    else if (indexLed == 6) {
        TIM_SetCompare3(TIM3, brightness);
    }
    else {
        TIM_SetCompare4(TIM3, brightness);
    }
}

int main(void) {
    Core_begin();

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);  // Enable clock for GPIOA
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  // Enable clock for GPIOB

    setup();

    /* Infinite loop */
    while (1) {
//        int i = 3, j = 4, brightness = 100;
//        while (i >= 0 && j <= 7) {
//            setLed(i, brightness);
//            setLed(j, brightness);
//            brightness -= 30;
//            i--; j++;
//            delay_ms(100);
//        }
//        for (int k = 0; k < 8; k++) {
//            setLed(k, 0);
//        }

        for (int i = 0; i < 8; i++) {
            int brightness = 5;
            for (int j = i; j < i + 4; j++) {
                setLed(j, brightness);
                brightness += 60;
            }
            if (i < 7) setLed(i-1, 0);
            else {
                setLed(i-1, 0);
                setLed(i, 0);
            }
            delay(70);
        }
        delay_ms(1000);
    }
}
