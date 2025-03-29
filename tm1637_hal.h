#ifndef TM1637_HAL_H
#define TM1637_HAL_H

#ifdef __cplusplus
extern "C" {
#endif


#include "tm1637_hal_config.h"
#include <inttypes.h>
#include <stdbool.h>
#ifdef USE_STM32F1
#include "stm32f1xx_hal.h"
#endif


#define SET_DIO_INPUT __setGPIODIOInput();
#define SET_DIO_OUTPUT __setGPIODIOOutput();
#define SET_CLK_INPUT __setGPIOClkInput();
#define SET_CLK_OUTPUT __setGPIOClkOutput();
#define READ_ACK HAL_GPIO_ReadPin(TM1637_DIO_PORT,TM1637_DIO_PIN);

/* Functions useful for handling waiting times us*/
#define DWT_INIT CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk; DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
#define DELAY_US(US) uint32_t us_count_tic =  US * (SystemCoreClock / 1000000U);DWT->CYCCNT = 0U;while(DWT->CYCCNT < us_count_tic);
#define MICROS DWT->CYCCNT / (SystemCoreClock / 1000000U); 

#define SEG_A   0b00000001
#define SEG_B   0b00000010
#define SEG_C   0b00000100
#define SEG_D   0b00001000
#define SEG_E   0b00010000
#define SEG_F   0b00100000
#define SEG_G   0b01000000
#define SEG_DP  0b10000000

#define __TM1637_DEFAULT_BIT_DELAY  100
#define __TM1637_DEFAULT_ON 1
#define __TM1637_DEFAULT_LENGTH 4
#define __TM1637_DEFAULT_POS 0
#define __TM1637_DEFAULT_LEADING_ZERO 0
#define __TM1637_DEFAULT_DOTS 0

void tm1637_init(void);
void tm1637_setBrightness(uint8_t brightness, bool on);
void tm1637_setSegments(const uint8_t segments[], uint8_t length, uint8_t pos);
void tm1637_clear(void);
void tm1637_showNumberDec(int num, bool leading_zero , uint8_t length , uint8_t pos );
void tm1637_showNumberDecEx(int num, uint8_t dots, bool leading_zero, uint8_t length , uint8_t pos );
void tm1637_showNumberHexEx(uint16_t num, uint8_t dots , bool leading_zero , uint8_t length , uint8_t pos );
void tm1637_showNumberBaseEx(int8_t base, uint16_t num, uint8_t dots , bool leading_zero , uint8_t length, uint8_t pos);
uint8_t tm1637_encodeDigit(uint8_t digit);


#ifdef __cplusplus
}
#endif

#endif // TM1637_HAL_H
