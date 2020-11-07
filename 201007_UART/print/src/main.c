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
#include <stdio.h>

/* Private typedef ----------------------------------------------------*/
/* Private define -----------------------------------------------------*/
/* Private macro ------------------------------------------------------*/
/* Private variables --------------------------------------------------*/
GPIO_InitTypeDef GPIO_struct;
/* Private functions --------------------------------------------------*/

int main(void) {

    Core_begin();
    USART1_begin_A9A10(BAUD_1M);
    Core_PrintInfo();

    /* Infinite loop */
    while (1) {
        printf("%d\t", 2500);
        delayMicroseconds(2500);
        printf("%d", 17500);
        delayMicroseconds(17500);
    }

}


