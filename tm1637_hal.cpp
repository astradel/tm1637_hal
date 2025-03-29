extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>
}

#include <tm1637_hal.h>

static void __bitDelay();
static void __start();
static void __stop();
static bool __writeByte(uint8_t b);
static void __showDots(uint8_t dots, uint8_t* digits);
static void __setGPIODIOInput(void);
static void __setGPIODIOOutput(void);
static void __setGPIOClkInput(void);
static void __setGPIOClkOutput(void);

static uint8_t __m_brightness;
static GPIO_InitTypeDef m_gpioStruct;

#define TM1637_I2C_COMM1    0x40
#define TM1637_I2C_COMM2    0xC0
#define TM1637_I2C_COMM3    0x80

#define CLK_LOW HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);

//
//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---
//      D
static const uint8_t digitToSegment[] = {
 // XGFEDCBA
  0b00111111,    // 0
  0b00000110,    // 1
  0b01011011,    // 2
  0b01001111,    // 3
  0b01100110,    // 4
  0b01101101,    // 5
  0b01111101,    // 6
  0b00000111,    // 7
  0b01111111,    // 8
  0b01101111,    // 9
  0b01110111,    // A
  0b01111100,    // b
  0b00111001,    // C
  0b01011110,    // d
  0b01111001,    // E
  0b01110001     // F
  };

static const uint8_t minusSegments = 0b01000000;

void tm1637_init(void) {
  DWT_INIT
  TM1637_ENABLE_GPIO_CLK
  TM1637_ENABLE_GPIO_DIO
}

void tm1637_setBrightness(uint8_t brightness, bool on) {
	__m_brightness = (brightness & 0x7) | (on? 0x08 : 0x00);
}

void tm1637_setSegments(const uint8_t segments[], uint8_t length, uint8_t pos) {
    // Write COMM1
	__start();
	__writeByte(TM1637_I2C_COMM1);
	__stop();

	// Write COMM2 + first digit address
	__start();
	__writeByte(TM1637_I2C_COMM2 + (pos & 0x03));

	// Write the data bytes
	for (uint8_t k=0; k < length; k++)
	  __writeByte(segments[k]);

	__stop();

	// Write COMM3 + brightness
	__start();
	__writeByte(TM1637_I2C_COMM3 + (__m_brightness & 0x0f));
	__stop();

}

void tm1637_clear(void) {
    uint8_t data[] = { 0, 0, 0, 0 };
	tm1637_setSegments(data,__TM1637_DEFAULT_LENGTH ,__TM1637_DEFAULT_POS);
}

void tm1637_showNumberDec(int num, bool leading_zero , uint8_t length , uint8_t pos)
{
  tm1637_showNumberDecEx(num, 0, leading_zero, length, pos);
}

void tm1637_showNumberDecEx(int num, uint8_t dots, bool leading_zero, uint8_t length , uint8_t pos)
{
  tm1637_showNumberBaseEx(num < 0? -10 : 10, num < 0? -num : num, dots, leading_zero, length, pos);
}

void tm1637_showNumberHexEx(uint16_t num, uint8_t dots , bool leading_zero , uint8_t length , uint8_t pos )
{
  tm1637_showNumberBaseEx(16, num, dots, leading_zero, length, pos);
}

void tm1637_showNumberBaseEx(int8_t base, uint16_t num, uint8_t dots, bool leading_zero,
                                    uint8_t length, uint8_t pos)
{
    bool negative = false;
	if (base < 0) {
	    base = -base;
		negative = true;
	}


    uint8_t digits[4];

	if (num == 0 && !leading_zero) {
		// Singular case - take care separately
		for(uint8_t i = 0; i < (length-1); i++)
			digits[i] = 0;
		digits[length-1] = tm1637_encodeDigit(0);
	}
	else {
		//uint8_t i = length-1;
		//if (negative) {
		//	// Negative number, show the minus sign
		//    digits[i] = minusSegments;
		//	i--;
		//}
		
		for(int i = length-1; i >= 0; --i)
		{
		    uint8_t digit = num % base;
			
			if (digit == 0 && num == 0 && leading_zero == false)
			    // Leading zero is blank
				digits[i] = 0;
			else
			    digits[i] = tm1637_encodeDigit(digit);
				
			if (digit == 0 && num == 0 && negative) {
			    digits[i] = minusSegments;
				negative = false;
			}

			num /= base;
		}
    }
	
	if(dots != 0)
	{
		__showDots(dots, digits);
	}
    
    tm1637_setSegments(digits, length, pos);
}

void __bitDelay()
{
	DELAY_US(TM1637_DELAY_US)
}

void __showDots(uint8_t dots, uint8_t* digits)
{
    for(int i = 0; i < 4; ++i)
    {
        digits[i] |= (dots & 0x80);
        dots <<= 1;
    }
}

uint8_t tm1637_encodeDigit(uint8_t digit)
{
	return digitToSegment[digit & 0x0f];
}


void __start()
{
  SET_DIO_OUTPUT;
  __bitDelay();
}

void __stop()
{
  SET_DIO_OUTPUT;
	__bitDelay();
  SET_CLK_INPUT;
	__bitDelay();
  SET_DIO_INPUT;
	__bitDelay();
}

bool __writeByte(uint8_t b)
{
  uint8_t data = b;
  for(uint8_t i = 0; i < 8; i++) {
    SET_CLK_OUTPUT;
    __bitDelay();
    if (data & 0x01) {
      SET_DIO_INPUT;
    }
    else {
      SET_DIO_OUTPUT;
    }
    __bitDelay();
    SET_CLK_INPUT;
    __bitDelay();
    data = data >> 1;
  }
  SET_CLK_OUTPUT;
  SET_DIO_INPUT;
  __bitDelay();
  SET_CLK_INPUT;
  __bitDelay();
  uint8_t ack = READ_ACK;
  if (ack == 0) {
    SET_DIO_OUTPUT;
  }
  __bitDelay();
  SET_CLK_OUTPUT;
  __bitDelay();
  return ack;
}

void __setGPIODIOInput(void) {
  m_gpioStruct={0};
	m_gpioStruct.Pin = TM1637_DIO_PIN; 
	m_gpioStruct.Mode = GPIO_MODE_INPUT;
	m_gpioStruct.Pull = GPIO_NOPULL;
	m_gpioStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(TM1637_DIO_PORT, &m_gpioStruct);  
}

void __setGPIODIOOutput(void) {
  m_gpioStruct={0};  
	m_gpioStruct.Pin = TM1637_DIO_PIN; 
	m_gpioStruct.Mode = GPIO_MODE_OUTPUT_PP;
	m_gpioStruct.Pull = GPIO_NOPULL;
	m_gpioStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(TM1637_DIO_PORT, &m_gpioStruct);  
}

void __setGPIOClkInput(void) {
  m_gpioStruct={0};  
	m_gpioStruct.Pin = TM1637_CLK_PIN; 
	m_gpioStruct.Mode = GPIO_MODE_INPUT;
	m_gpioStruct.Pull = GPIO_PULLDOWN;
	m_gpioStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(TM1637_CLK_PORT, &m_gpioStruct);  
}

void __setGPIOClkOutput(void) {
  m_gpioStruct={0};
	m_gpioStruct.Pin = TM1637_CLK_PIN; 
	m_gpioStruct.Mode = GPIO_MODE_OUTPUT_PP;
	m_gpioStruct.Pull = GPIO_PULLDOWN;
	m_gpioStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(TM1637_CLK_PORT, &m_gpioStruct);  
}
