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
/* Private macro ------------------------------------------------------*/
/* Private variables --------------------------------------------------*/
/* Private functions --------------------------------------------------*/
void GPIO_configurature(void);
void ADC_configurature(void);

int main(void) {
    Core_begin();
    delay_ms(100);
    USART1_begin_A9A10(BAUD_1M);
    Core_PrintInfo();

    GPIO_configurature();
    ADC_configurature();

    /* Infinite loop */
    while (1) {
        int ain = ADC_GetConversionValue(ADC1);
        printf("ain = %d\n", ain);
        delay_ms(50);
    }
}

void GPIO_configurature(void) {

    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // Enable clock GPIO port A
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;            // Select alternate function mode for ADC
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;         // Speed
    // Initilaze GPIOA
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void ADC_configurature(void) {

    ADC_InitTypeDef ADC_InitStructure;

    /* Enable ADC1 and GPIOC clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    /* ADC1 configuration ------------------------------------------------------*/
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              //
    ADC_InitStructure.ADC_NbrOfChannel = 1;                             // The number of ADC channels that will be converted
    ADC_Init(ADC1, &ADC_InitStructure);                                 // Initilaze ADC

    // Configure ADC_IN6
    ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 1, ADC_SampleTime_239Cycles5);
    /* Enable ADC1 DMA */
//    ADC_DMACmd(ADC1, ENABLE);

    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);

    /* Enable ADC1 reset calibaration register */
    ADC_ResetCalibration(ADC1);
    /* Check the end of ADC1 reset calibration register */
    while(ADC_GetResetCalibrationStatus(ADC1));

    /* Start ADC1 calibaration */
    ADC_StartCalibration(ADC1);

    /* Check the end of ADC1 calibration */
    while(ADC_GetCalibrationStatus(ADC1));

    /* Start ADC1 Software Conversion */
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
