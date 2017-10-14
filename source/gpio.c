/*##################################### GPIO ###################################
������� �������������:
1. ��������� ������������ � �������� RCC->AHB1ENR ���������� ���� RCC_AHB1ENR_GPIO�EN, ��� � - ����� �����.
2. ��������� � ���������� ������������ � �������� RCC->AHB1RSTR ���������� ���� RCC_AHB1RSTR_GPIO�RST, ��� �  - ����� �����.
3. ����� ������ ������ ����� ����� � �������� GPIO�->MODER:
  00 - ���� (~GPIO_MODER_MODER�);
  01 - ����� (GPIO_MODER_MODER�_0);
  10 - �������������� ������� (GPIO_MODER_MODER�_1);
  11 - ��������� ����� (GPIO_MODER_MODER�);
      ��� � - ����� ����.
4. ��������� ������ ������ (GPIO�->OTYPER):
  0 - ����������� ����� (~GPIO_OTYPER_OT_�);
  1 - ����� ��������� ����� (������ ���������� ������� �������) (GPIO_OTYPER_OT_�);
      ��� � - ����� ����.
5. ��������� �������������� ��������� (GPIO�->PUPDR):
  00 - ��� �������� (~GPIO_PUPDR_PUPDR�);
  01 - �������� � �������� ������ (GPIO_PUPDR_PUPDR�_0);
  10 - �������� � ������� ������ (GPIO_PUPDR_PUPDR�_1);
      ��� � - ����� ����
6. ��������� �������� ���� (GPIO�->OSPEEDR), ������ �� �������� ������� ������������ �����:
  00 - ������ ��������, 2 MHz (~GPIO_OSPEEDER_OSPEEDR�);
  01 - ������� ��������, 25 MHz (GPIO_OSPEEDER_OSPEEDR�_0);
  10 - ������� ��������, 50 MHz (GPIO_OSPEEDER_OSPEEDR�_1);
  11 - ����� ������� ��������, 100 MHz (GPIO_OSPEEDER_OSPEEDR�);
      ��� � - ����� ����
7. ��������� �������������� ������� (GPIO�->AFR[0]; GPIO�->FR[1]). GPIO�->AFR[0] - ��� ����� 0-7; GPIO�->AFR[1] - ��� ����� 8-15:
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

��� ���������� �� ����� ������������ ������� GPIO�->IDR (� - ����� �����) � ��������� GPIO_IDR_IDR_� (� - ����� ����).
��� ������ � ���� ������������ ������� GPIO�->ODR (� - ����� �����) � ��������� GPIO�->ODR_ODR_� (� - ����� ����).
��� ��������� ������ ������������ �������� GPIO�->BSRRL � GPIO�->BSRRH (� - ����� �����) � ��������� GPIO_BSRR_BS_� (� - ����� ����).
������ � ������� GPIO�->BSRRL - ��������� 1; � ������� GPIO�->BSRRH - ��������� 0.
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