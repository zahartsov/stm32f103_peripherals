#ifndef __GPIO_H__
#define __GPIO_H__

#include "stm32f103xb.h"

//Alternative function
#define AF0  0x00
#define AF1  0x01
#define AF2  0x02
#define AF3  0x03
#define AF4  0x04
#define AF5  0x05
#define AF6  0x06
#define AF7  0x07
#define AF8  0x08
#define AF9  0x09
#define AF10 0x0A
#define AF11 0x0B
#define AF12 0x0C
#define AF13 0x0D
#define AF14 0x0E
#define AF15 0x0F

//Mode
#define GPIO_MODE_INPUT_ANALOG          0x00
#define GPIO_MODE_INPUT_FLOATING        0x01
#define GPIO_MODE_INPUT_WITH_PULL       0x02
#define GPIO_MODE_OUTPUT_PUSH_PULL      0x03
#define GPIO_MODE_OUTPUT_OPEN_DRAIN     0x04
#define GPIO_MODE_OUTPUT_AF_PUSH_PULL   0x05
#define GPIO_MODE_OUTPUT_AF_OPEN_DRAIN  0x06

//Speed
#define GPIO_SPEED_NONE         0x00
#define GPIO_SPEED_10MHZ        0x01
#define GPIO_SPEED_20MHZ        0x02
#define GPIO_SPEED_50MHZ        0x03

//Rising interrupt
#define EXTI_RISING_DISABLE     0x00
#define EXTI_RISING_ENABLE      0x01

//Falling interrupt
#define EXTI_FALLING_DISABLE    0x00
#define EXTI_FALLING_ENABLE     0x01

typedef struct
{
  GPIO_TypeDef* baseGPIO;
  uint8_t pin;
  uint8_t mode;
  uint8_t pull;
  uint8_t speed;
  uint8_t AF;
  uint8_t interruptRising;
  uint8_t interruptFalling;
}GPIO;     // Параметры таймера

void userGPIO_shortInit(GPIO_TypeDef* GPIO, uint8_t pin, uint8_t mode, uint8_t pull, uint8_t AF);
void userGPIO_Init(GPIO*);
void userEXTI_Init(GPIO*);

#endif