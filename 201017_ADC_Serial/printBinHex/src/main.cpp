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
#include <string.h>
#include "Core.h"
#include "Serial.h"
/* Private typedef ----------------------------------------------------*/
/* Private define -----------------------------------------------------*/
/* Private macro ------------------------------------------------------*/
/* Private variables --------------------------------------------------*/
/* Private functions --------------------------------------------------*/


int main(void) {
    /// Setup:
    Core_begin();
    delay(100);
    Serial.begin(BAUD_1M);
    Core_PrintInfo();
    /* Infinite loop */
    while (1) {
        if (Serial.available() > 0) {
            char s[100] = "";
            int i = -1;
            while (Serial.available() > 0) {
                s[++i] = Serial.read();
                delay_ms(1);
            }
            Serial.print(s);
            for (int j = 0; j < i; j++) {
                int x;
                if (s[j] >= 'A' && s[j] <= 'F') {
                    x = (int)s[j] - 55;
                }
                else {
                    x = (int)s[j] - 48;
                }
                int bin[4] = {}, idx = 4;
                while (x > 0 && idx >= 0) {
                    bin[--idx] = x % 2;
                    x /= 2;
                }
                for (int k = 0; k < 4; k++)
                    Serial.print(bin[k]);
            }
            Serial.println();

        }
    }
}
