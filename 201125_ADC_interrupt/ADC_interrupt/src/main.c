//**********************************************************************/
//
//   Last committed:     $Revision: 00 $
//   Last changed by:    $Author: $
//   Last changed date:  $Date:  $
//   ID:                 $Id:  $
//
//**********************************************************************/

/* Includes -----------------------------------------------------------*/
#include <stdio.h>
#include "Core.h"

/* Private typedef ----------------------------------------------------*/

/* Private define -----------------------------------------------------*/
#define LED_PIN     GPIO_Pin_2
#define Knob_pin    GPIO_Pin_4
#define Prescaler          720
#define Period           50000

/* Private macro ------------------------------------------------------*/
/* Private variables --------------------------------------------------*/
//int ADC_Result;

/* Private functions --------------------------------------------------*/
void GPIO_configurature(void);
void TIM2_configurature(void);
void TIM2_NVIC_configurature(void);
void ADC_configurature(void);
void ADC_NVIC_configurature(void);

int main(void) {

    Core_begin();
    USART1_begin_A9A10(BAUD_1M);

    GPIO_configurature();
    TIM2_configurature();
//    TIM2_NVIC_configurature();
    ADC_configurature();
    ADC_NVIC_configurature();

    Core_PrintInfo();

    /* Infinite loop */
    while (1) {

    }
}

void GPIO_configurature(void) {

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // Enable clock for GPIOA

	GPIO_InitTypeDef GPIOA_InitStructure;
	GPIOA_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOA_InitStructure.GPIO_Pin = Knob_pin;
	GPIOA_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIOA_InitStructure);

	/* RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  // Enable clock for GPIOB

	GPIO_InitTypeDef GPIOB_InitStructure;
	GPIOB_InitStructure.GPIO_Pin = LED_PIN;
	GPIOB_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIOB_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIOB_InitStructure);
	*/
}

void TIM2_configurature(void) {

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    // enable timer clock
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);   // Enable clock for TIM2
    // configure base timer
    TIM_TimeBaseStructure.TIM_Prescaler = Prescaler;                // Prescaler
    TIM_TimeBaseStructure.TIM_Period = Period;                      // Preiod
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     // Count up mode
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);                 // initialization

    TIM_Cmd(TIM2, ENABLE); // enable Timer
}

void TIM2_NVIC_configurature(void) {
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);   // cho phép ngắt tràn
}

void ADC_configurature(void) {

    // Enable ADC1 and GPIOC clock
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    /* ADC2 configuration ------------------------------------------------------*/
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              //
    ADC_InitStructure.ADC_NbrOfChannel = 1;                             // The number of ADC channels that will be converted
    ADC_Init(ADC1, &ADC_InitStructure);                                 // Initilaze ADC

    // Configure ADC_IN4
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 1, ADC_SampleTime_239Cycles5);

    /*
    ADC_DMACmd(ADC1, ENABLE);   // Enable ADC1 DMA
    */
    // Enable ADC1
    ADC_Cmd(ADC1, ENABLE);
    // Enable ADC1 reset calibaration register
    ADC_ResetCalibration(ADC1);
    // Check the end of ADC1 reset calibration register
    while(ADC_GetResetCalibrationStatus(ADC1));
    /* Start ADC1 calibaration */
    ADC_StartCalibration(ADC1);
    // Check the end of ADC1 calibration
    while(ADC_GetCalibrationStatus(ADC1));
    //Start ADC1 Software Conversion
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void ADC_NVIC_configurature(void) {

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);   // cho phép ngắt tràn
//    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}

void TIM2_IRQHandler(void) {
//  GPIO_WriteBit(GPIOB, LED_PIN, !GPIO_ReadOutputDataBit(GPIOB, LED_PIN));  // toggle led
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET) {
        ADC_SoftwareStartConvCmd(ADC1, ENABLE);
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // clear update interrupt flag
    }
}

void ADC1_2_IRQHandler() {
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
    int ADC_Result = ADC_GetConversionValue(ADC1);
    printf("value = %d\n", ADC_Result);
}
