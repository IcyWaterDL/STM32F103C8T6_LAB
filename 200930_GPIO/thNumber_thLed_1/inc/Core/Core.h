#ifndef __CORE_H__
#define __CORE_H__


/**********************************************************************
// Select Options
**********************************************************************/
//#define  CORETICK_SOURCE_DWT              /// not available on F0XX
//#define  CORETICK_SOURCE_SYSTICK_IRQ
  #define  CORETICK_SOURCE_SYSTICK

//#define  WAIT_MODE_CLASSIC                /// using classic method
  #define  WAIT_MODE_EVENTS                 /// using __WFE() and DMA

#ifdef WAIT_MODE_EVENTS
  //#define  USART1_TX_MODE_DMA_CHANNEL2    /// not available on F10X_MD
    #define  USART1_TX_MODE_DMA_CHANNEL4

  //#define  USART1_RX_MODE_DISABLE
  //#define  USART1_RX_MODE_DMA_CHANNEL3    /// not available on F10X_MD
  #define  USART1_RX_MODE_DMA_CHANNEL5

  //#define  CORE_DMA_BACKUP
#endif

#ifdef WAIT_MODE_CLASSIC
    #define  USART1_TX_MODE_CLASSIC

    #define  USART1_RX_MODE_DISABLE
  //#define  USART1_RX_MODE_IRQ
#endif

#define  USART1_RX_BUFF_SIZE  32


/**********************************************************************
// Clock and Flash latency settings
**********************************************************************/
#ifdef STM32F10X_MD
  #define  Core_ClkFreq_MHz    (72UL)
  #define  Core_Flash_Latency  FLASH_Latency_1
#endif

#ifdef STM32F0XX
  #define  Core_ClkFreq_MHz    (48UL)
  #define  Core_Flash_Latency  FLASH_Latency_0
#endif



/**********************************************************************/
#include "basic.h"
C_DECLS_BEGIN



/**********************************************************************
// CoreTick
**********************************************************************/

uint32_t CoreTick32();
uint64_t CoreTick64();

uint64_t micros64();
uint64_t millis64();

uint32_t micros();
uint32_t millis();

#if defined (CORETICK_SOURCE_SYSTICK_IRQ) || \
    defined (CORETICK_SOURCE_SYSTICK)
  uint32_t SysTick24();

  #ifdef CORETICK_SOURCE_SYSTICK_IRQ
    void SysTick_Handler();
  #endif
#endif



/**********************************************************************
// Delay
**********************************************************************/

void delay_us(uint32_t us);
void delay_ms(uint32_t ms);

#define  delayMicroseconds(us)  delay_us(us)
#define  delay(ms)              delay_ms(ms)

void delay_until_us(uint32_t target_us);
void delay_until_ms(uint32_t target_ms);



/**********************************************************************
// Timeout
**********************************************************************/
#ifdef WAIT_MODE_EVENTS
  void timeout_start_us(uint32_t us);
  void timeout_start_ms(uint32_t ms);
  void timeout_end();
  bool timeout_is_busy();
#endif


/**********************************************************************
// Core
**********************************************************************/
void Core_begin();

void Core_PrintInfo();

uint16_t Core_GetUid16();

#ifdef STM32F10X_MD
  #define  FLASH_SIZE   (*((uint16_t*)0x1FFFF7E0))
  #define  FLASH_UID_0  (*((uint32_t*)0x1FFFF7E8))
  #define  FLASH_UID_1  (*((uint32_t*)0x1FFFF7EC))
  #define  FLASH_UID_2  (*((uint32_t*)0x1FFFF7F0))
#endif


#ifdef STM32F0XX
  #define  FLASH_SIZE   (*((uint16_t*)0x1FFFF7CC))
  #define  FLASH_UID_0  (*((uint32_t*)0x1FFFF7AC))
  #define  FLASH_UID_1  (*((uint32_t*)0x1FFFF7B0))
  #define  FLASH_UID_2  (*((uint32_t*)0x1FFFF7B4))
#endif


void Flash_SetLatency(uint32_t WaitState);


/**********************************************************************
// Reset Flags
**********************************************************************/
#define RESET_FLAG_POWER                  RCC_CSR_PORRSTF
#define RESET_FLAG_LOW_POWER              RCC_CSR_LPWRRSTF
#define RESET_FLAG_WATCHDOG_WINDOWN       RCC_CSR_WWDGRSTF
#define RESET_FLAG_WATCHDOG_INDEPENDENT   RCC_CSR_IWDGRSTF
#define RESET_FLAG_SOFTWARE               RCC_CSR_SFTRSTF
#define RESET_FLAG_NRST_PIN               RCC_CSR_PINRSTF

void ResetFlags_Begin();
void ResetFlags_Clear();
bool ResetFlags_Check(uint32_t flag);


/**********************************************************************
// USART1
**********************************************************************/
//#define  USART1_TxPin   9     /// A9
//#define  USART1_RxPin  10     /// A10

#define  USART1_TxPin   2     /// A2
#define  USART1_RxPin   3     /// A3

#define  BAUD_3M     3000000
#define  BAUD_2M     2000000
#define  BAUD_1M5    1500000
#define  BAUD_1M     1000000
#define  BAUD_115200  115200
#define  BAUD_9600      9600

void USART1_begin_A9A10(uint32_t baud);
void USART1_write(int data);

int _write(int file, char *ptr, int len);

#if defined(USART1_RX_MODE_DMA_CHANNEL3) || \
    defined(USART1_RX_MODE_DMA_CHANNEL5) || \
    defined(USART1_RX_MODE_IRQ)

  int  USART1_available();
  int  USART1_read();
  int  USART1_peek();
  void USART1_flushRx();

  #ifdef USART1_RX_MODE_IRQ
    void USART1_IRQHandler();
  #endif
#endif



/**********************************************************************
// GPIO
**********************************************************************/
#ifdef STM32F10X_MD
  void     F103_GPIO_pinMode_output(GPIO_TypeDef* GPIOx, int pin, int mode_speed);
  void     F103_GPIO_pinMode_input (GPIO_TypeDef* GPIOx, int pin, int mode);

  uint32_t F103_GPIO_get_rcc_clock_mask(void *periph);
  void     F103_GPIO_external_interrupt_enable(GPIO_TypeDef* GPIOx, int pin, int trigger);
#endif

#ifdef STM32F0XX
  void     F030_GPIO_pinMode_output_pupd(GPIO_TypeDef* GPIOx, int pin, int mode, int type, int speed, int pupd);
  void     F030_GPIO_pinMode_output(GPIO_TypeDef* GPIOx, int pin, int mode, int type, int speed);
  void     F030_GPIO_pinMode_input (GPIO_TypeDef* GPIOx, int pin, int mode, int pupd);
  void     F030_GPIO_pinAF_select(GPIO_TypeDef* GPIOx, int pin, int selection);

  uint32_t F030_GPIO_get_rcc_clock_mask(void *periph);
  void     F030_GPIO_external_interrupt_enable(GPIO_TypeDef* GPIOx, int pin, int trigger);
#endif


/**********************************************************************/
C_DECLS_END
#endif // __CORE_H__
