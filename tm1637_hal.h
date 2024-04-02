#ifndef __TM1637_HAL__
#define __TM1637_HAL__

#ifdef __cplusplus
extern "C" {
#endif


#include "tm1637_hal_config.h"
#include <inttypes.h>
#include <stdbool.h>
#ifdef USE_STM32F1
#include "stm32f1xx_hal.h"
#endif

#define ENABLE_GPIOS  __HAL_RCC_GPIOB_CLK_ENABLE();
#define SET_DIO_INPUT __setGPIODIOInput();
#define SET_DIO_OUTPUT __setGPIODIOOutput();
#define SET_CLK_INPUT __setGPIOClkInput();
#define SET_CLK_OUTPUT __setGPIOClkOutput();
#define READ_ACK HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_13);


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

void tm1637_init(unsigned int bitDelay);
void tm1637_setBrightness(uint8_t brightness, bool on);
void tm1637_setSegments(const uint8_t segments[], uint8_t length, uint8_t pos);
void tm1637_clear(void);
void tm1637_showNumberDec(int num, bool leading_zero , uint8_t length , uint8_t pos );
void tm1637_showNumberDecEx(int num, uint8_t dots, bool leading_zero, uint8_t length , uint8_t pos );
void tm1637_showNumberHexEx(uint16_t num, uint8_t dots , bool leading_zero , uint8_t length , uint8_t pos );
void tm1637_showNumberBaseEx(int8_t base, uint16_t num, uint8_t dots , bool leading_zero , uint8_t length, uint8_t pos);
uint8_t tm1637_encodeDigit(uint8_t digit);

/*class TM1637Display{

public:
  TM1637Display(unsigned int bitDelay = DEFAULT_BIT_DELAY);
  void setBrightness(uint8_t brightness, bool on = true);
  void setSegments(const uint8_t segments[], uint8_t length = 4, uint8_t pos = 0);
  void clear();
  void showNumberDec(int num, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);
  void showNumberDecEx(int num, uint8_t dots = 0, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);
  void showNumberHexEx(uint16_t num, uint8_t dots = 0, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);
  uint8_t encodeDigit(uint8_t digit);

protected:
   void bitDelay();
   void start();
   void stop();
   bool writeByte(uint8_t b);
   void showDots(uint8_t dots, uint8_t* digits);
   void showNumberBaseEx(int8_t base, uint16_t num, uint8_t dots = 0, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);
   void setGPIODIOInput(void);
   void setGPIODIOOutput(void);
   void setGPIOClkInput(void);
   void setGPIOClkOutput(void);
   inline void mDelayMicroseconds(uint32_t us);

private:
  uint8_t m_brightness;
  unsigned int m_bitDelay;
  GPIO_InitTypeDef m_gpioDIO;	
  GPIO_InitTypeDef m_gpioClk;	
};*/

#ifdef __cplusplus
}
#endif

#endif // __TM1637_HAL__
