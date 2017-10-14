/*##################################### GPIO ###################################
Порядок инициализации:
1. Включение тактирования в регистре RCC->AHB1ENR установкой бита RCC_AHB1ENR_GPIOхEN, где х - буква порта.
2. Включение и выключение перезагрузки в регистре RCC->AHB1RSTR установкой бита RCC_AHB1RSTR_GPIOхRST, где х  - буква порта.
3. Выбор режима работы пинов порта в регистре GPIOх->MODER:
  00 - вход (~GPIO_MODER_MODERх);
  01 - выход (GPIO_MODER_MODERх_0);
  10 - альтернативная функция (GPIO_MODER_MODERх_1);
  11 - аналоговы режим (GPIO_MODER_MODERх);
      где х - номер пина.
4. Установка режима выхода (GPIOх->OTYPER):
  0 - двухтактный режим (~GPIO_OTYPER_OT_х);
  1 - режим открытого стока (нельзя установить высокий уровень) (GPIO_OTYPER_OT_х);
      где х - номер пина.
5. Установка подтягивающего резистора (GPIOх->PUPDR):
  00 - нет подтяжки (~GPIO_PUPDR_PUPDRх);
  01 - подтяжка к высокому уровню (GPIO_PUPDR_PUPDRх_0);
  10 - подтяжка к низкому уровню (GPIO_PUPDR_PUPDRх_1);
      где х - номер пина
6. Установка скорости пина (GPIOх->OSPEEDR), влияет на крутизну фронтов переключения пинов:
  00 - низкая скорость, 2 MHz (~GPIO_OSPEEDER_OSPEEDRх);
  01 - средняя скорость, 25 MHz (GPIO_OSPEEDER_OSPEEDRх_0);
  10 - высокая скорость, 50 MHz (GPIO_OSPEEDER_OSPEEDRх_1);
  11 - очень высокая скорость, 100 MHz (GPIO_OSPEEDER_OSPEEDRх);
      где х - номер пина
7. Установка альтернативной функции (GPIOх->AFR[0]; GPIOх->FR[1]). GPIOх->AFR[0] - для пинов 0-7; GPIOх->AFR[1] - для пинов 8-15:
  0000 - AF0 (system)
  0001 - AF1 (TIM1/TIM2)
  0010 - AF2 (TIM3..5)
  0011 - AF3 (TIM8..11)
  0100 - AF4 (I2C1..3)
  0101 - AF5 (SPI1/SPI2)
  0110 - AF6 (SPI3)
  0111 - AF7 (USART1..3)
  1000 - AF8 (USART4..6)
  1001 - AF9 (CAN1/CAN2, TIM12..14)
  1010 - AF10 (OTG_FS, OTG_HS)
  1011 - AF11 (ETH)
  1100 - AF12 (FSMC, SDIO, OTG_HS)
  1101 - AF13 (DCMI)
  1110 - AF14
  1111 - AF15 (EVENTOUT)

Для считывания из порта используется регистр GPIOх->IDR (х - буква порта) и константа GPIO_IDR_IDR_х (х - номер пина).
Для записи в порт используется регистр GPIOх->ODR (х - буква порта) и константа GPIOх->ODR_ODR_х (х - номер пина).
Для побитовой записи используются регистры GPIOх->BSRRL и GPIOх->BSRRH (х - буква порта) и константы GPIO_BSRR_BS_х (х - номер пина).
Запись в регистр GPIOх->BSRRL - установка 1; в регситр GPIOх->BSRRH - установка 0.
################################################################################*/

#include "gpio.h"

/*void userGPIO_shortInit(GPIO_TypeDef* GPIO, uint8_t pin, uint8_t mode, uint8_t pull, uint8_t AF)
{
  if(GPIO == (GPIO_TypeDef*)GPIOA) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  else if(GPIO == (GPIO_TypeDef*)GPIOB) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
  else if(GPIO == (GPIO_TypeDef*)GPIOC) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
  else if(GPIO == (GPIO_TypeDef*)GPIOD) RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
  else if(GPIO == (GPIO_TypeDef*)GPIOE) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
  else if(GPIO == (GPIO_TypeDef*)GPIOF) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;

  if(mode == GPIO_MODE_INPUT) GPIO->MODER &= ~(GPIO_MODER_MODER0 << pin*2);
  else GPIO->MODER |= mode << pin*2;
  GPIO->OTYPER &= ~GPIO_OTYPER_OT_0 << pin*2;
  if(pull == GPIO_PULL_NOPULL) GPIO->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << pin*2);
  else GPIO->PUPDR |= pull << pin*2;
  GPIO->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR0_1 << pin*2;

  if(mode == GPIO_MODE_AF)
  {
    if(pin < 8) GPIO->AFR[0] |= AF << pin*4;
    else GPIO->AFR[1] |= AF << pin*4;
  }
}*/

void userGPIO_Init(GPIO* GPIO)
{
  if(GPIO->baseGPIO == (GPIO_TypeDef*)GPIOA) RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
  else if(GPIO->baseGPIO == (GPIO_TypeDef*)GPIOB) RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
  else if(GPIO->baseGPIO == (GPIO_TypeDef*)GPIOC) RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
  else if(GPIO->baseGPIO == (GPIO_TypeDef*)GPIOD) RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
  else if(GPIO->baseGPIO == (GPIO_TypeDef*)GPIOE) RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;

  uint32_t volatile *reg;
  uint8_t pin = 0;
  if(GPIO->pin > 7)
  {
    reg = &GPIO->baseGPIO->CRH;
    pin = GPIO->pin - 8;
  }
  else reg = &GPIO->baseGPIO->CRL;

  *reg &= ~((GPIO_CRL_CNF0 | GPIO_CRL_MODE0) << 4*pin);
  if(GPIO->mode == GPIO_MODE_INPUT_FLOATING) *reg |= GPIO_CRL_CNF0_0 << (4*pin);
  else if(GPIO->mode == GPIO_MODE_INPUT_WITH_PULL) *reg |= GPIO_CRL_CNF0_1 << (4*pin);
  else *reg |= GPIO->speed << (4*pin);

  if(GPIO->mode == GPIO_MODE_OUTPUT_OPEN_DRAIN) *reg |= GPIO_CRL_CNF0_0 << (4*pin);
  else if(GPIO->mode == GPIO_MODE_OUTPUT_AF_PUSH_PULL) *reg |= GPIO_CRL_CNF0_1 << (4*pin);
  else if(GPIO->mode == GPIO_MODE_OUTPUT_AF_OPEN_DRAIN) *reg |= (GPIO_CRL_CNF0_0 | GPIO_CRL_CNF0_1) << (4*pin);
}

/*void userEXTI_Init(GPIO* GPIO)
{
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

  uint16_t tmp = 0;
  if(GPIO->baseGPIO == (GPIO_TypeDef*)GPIOA) tmp |= SYSCFG_EXTICR1_EXTI0_PA << GPIO->pin*4;
  else if(GPIO->baseGPIO == (GPIO_TypeDef*)GPIOB) tmp |= SYSCFG_EXTICR1_EXTI0_PB << GPIO->pin*4;
  else if(GPIO->baseGPIO == (GPIO_TypeDef*)GPIOC) tmp |= SYSCFG_EXTICR1_EXTI0_PC << GPIO->pin*4;
  else if(GPIO->baseGPIO == (GPIO_TypeDef*)GPIOD) tmp |= SYSCFG_EXTICR1_EXTI0_PD << GPIO->pin*4;
  else if(GPIO->baseGPIO == (GPIO_TypeDef*)GPIOE) tmp |= SYSCFG_EXTICR1_EXTI0_PE << GPIO->pin*4;
  else if(GPIO->baseGPIO == (GPIO_TypeDef*)GPIOF) tmp |= SYSCFG_EXTICR1_EXTI0_PF << GPIO->pin*4;
  else return;

  if(GPIO->pin < 4) SYSCFG->EXTICR[0] |= tmp;
  else if(GPIO->pin < 8) SYSCFG->EXTICR[1] |= tmp;
  else if(GPIO->pin < 12) SYSCFG->EXTICR[2] |= tmp;
  else if(GPIO->pin < 16) SYSCFG->EXTICR[3] |= tmp;
  else return;

  EXTI->IMR |= EXTI_IMR_MR0 << GPIO->pin;
  //EXTI->EMR |= EXTI_EMR_MR0 << GPIO->pin;

  if(GPIO->interruptRising == EXTI_RISING_DISABLE) EXTI->RTSR &= ~(EXTI_RTSR_TR0 << GPIO->pin);
  else EXTI->RTSR |= EXTI_RTSR_TR0 << GPIO->pin;

  if(GPIO->interruptFalling == EXTI_FALLING_DISABLE) EXTI->FTSR &= ~(EXTI_FTSR_TR0 << GPIO->pin);
  else EXTI->FTSR |= EXTI_FTSR_TR0 << GPIO->pin;

  if(GPIO->pin == 0) NVIC_EnableIRQ(EXTI0_IRQn);
  else if(GPIO->pin == 1) NVIC_EnableIRQ(EXTI1_IRQn);
  else if(GPIO->pin == 2) NVIC_EnableIRQ(EXTI1_IRQn);
  else if(GPIO->pin == 3) NVIC_EnableIRQ(EXTI1_IRQn);
  else if(GPIO->pin == 4) NVIC_EnableIRQ(EXTI1_IRQn);
  else if(GPIO->pin < 10) NVIC_EnableIRQ(EXTI9_5_IRQn);
  else if(GPIO->pin < 16) NVIC_EnableIRQ(EXTI15_10_IRQn);
}*/