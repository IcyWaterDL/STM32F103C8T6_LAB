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
/* Private define -----------------------------------------------------*/
/* Private macro ------------------------------------------------------*/
/* Private variables --------------------------------------------------*/
GPIO_InitTypeDef GPIO_struct;
ADC_InitTypeDef  ADC_struct;
/* Private functions --------------------------------------------------*/


int main(void) {
    Core_begin();

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    GPIOB
    /* Infinite loop */
    while (1) {

    }
}


