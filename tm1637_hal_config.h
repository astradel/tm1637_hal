#ifndef TM1637_HAL_CONFIG_H
#define TM1637_HAL_CONFIG_H
#define TM1637_CLK_PIN GPIO_PIN_12
#define TM1637_DIO_PIN GPIO_PIN_13
#define TM1637_CLK_PORT GPIOB
#define TM1637_DIO_PORT GPIOB
#define TM1637_ENABLE_GPIO_CLK  __HAL_RCC_GPIOB_CLK_ENABLE();
#define TM1637_ENABLE_GPIO_DIO  __HAL_RCC_GPIOB_CLK_ENABLE();
#define TM1637_DELAY_US 100
#define USE_STM32F1
#endif //TM1637_HAL_CONFIG_H
