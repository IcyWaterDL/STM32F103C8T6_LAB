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
#include "ADC.h"
#include "stm32f10x_conf.h"

/* Private typedef ----------------------------------------------------*/

/* Private define -----------------------------------------------------*/
#define knob GPIO_Pin_0
/* Private macro ------------------------------------------------------*/

/* Private variables --------------------------------------------------*/
GPIO_InitTypeDef gpio_a0_init_struct;
GPIO_InitTypeDef gpio_a1_init_struct;
GPIO_InitTypeDef GPIO_Struct;

/* Private functions --------------------------------------------------*/
void setup(void);
void config_ADC(void);
void TIM2_PWM_config(void);
int Map(int x, int inMin, int inMax, int outMin, int outMax);

void config_ADC(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_Struct.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Struct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &GPIO_Struct);
    ADC1_begin();
}

void TIM2_PWM_config(void) {
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);   // Enable clock for TIM2
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  // Enable clock for GPIOA

  // Cấu hình chung:
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_TimeBaseStructure.TIM_Prescaler = 72;                     // Prescaler
  TIM_TimeBaseStructure.TIM_Period = 20000;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  // đếm lên
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_Cmd(TIM2, ENABLE);    // enable timer

//    GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);
  // Cấu hình kênh 1:
//  {
//    TIM_OCInitTypeDef  oc1_init_struct;
//    oc1_init_struct.TIM_Pulse = (5*100)/100;               // độ rộng xung
//    oc1_init_struct.TIM_OCMode = TIM_OCMode_PWM1;               // chế độ PWM 1
//    oc1_init_struct.TIM_OCPolarity = TIM_OCPolarity_High;       // tích cực thuận
//    oc1_init_struct.TIM_OutputState = TIM_OutputState_Enable;   // enable channel 1
//    TIM_OC1Init(TIM2, &oc1_init_struct);
//
//    gpio_a0_init_struct.GPIO_Pin = GPIO_Pin_0;
//    gpio_a0_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
//    gpio_a0_init_struct.GPIO_Mode = GPIO_Mode_AF_PP;
//    GPIO_Init(GPIOA, &gpio_a0_init_struct);
//  }


  // Cấu hình kênh 2:
  {
    TIM_OCInitTypeDef  oc2_init_struct;
//    oc2_init_struct.TIM_Pulse = (15 * 65535 / 100);               // độ rộng xung
    oc2_init_struct.TIM_OCMode = TIM_OCMode_PWM1;               // chế độ PWM 1
    oc2_init_struct.TIM_OCPolarity = TIM_OCPolarity_High;       // tích cực thuận
    oc2_init_struct.TIM_OutputState = TIM_OutputState_Enable;   // enable channel 2
    TIM_OC2Init(TIM2, &oc2_init_struct);

    gpio_a1_init_struct.GPIO_Pin = GPIO_Pin_1;
    gpio_a1_init_struct.GPIO_Speed = GPIO_Speed_50MHz;
    gpio_a1_init_struct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &gpio_a1_init_struct);
  }
}

int Map(int x, int inMin, int inMax, int outMin, int outMax) {
  return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

void setup(void) {
    config_ADC();
    TIM2_PWM_config();
}


int main(void) {
    Core_begin();

    setup();

    int compare = Map(0, 0, 180, 500, 2500);

    /* Infinite loop */
    while (1) {
        TIM_SetCompare2(TIM2, compare);

        int sum = 0;
        int knobVal = 0;
        for (int i = 0; i < 16; i++) {
            knobVal = ADC1_read12(ADC_Channel_8, ADC_SampleTime_239_5Cycles);
            sum += knobVal;
        }
        int avrg = sum / 16;
        compare = Map(avrg, 0, 4095, 500, 2500);
        delay_ms(10);
    }
}
