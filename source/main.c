#include "stm32f103xb.h"
#include "configuration.h"

volatile unsigned long * takt;

int main()
{
  RCC->CR |= RCC_CR_HSEON;
  while(!(RCC->CR & RCC_CR_HSERDY));
  RCC->CFGR |= RCC_CFGR_PLLMULL9;
  RCC->CFGR |= RCC_CFGR_PLLSRC;
  RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
  FLASH->ACR |= FLASH_ACR_LATENCY_2;
  RCC->CR |= RCC_CR_PLLON;
  while(!(RCC->CR & RCC_CR_PLLRDY));
  RCC->CFGR |= RCC_CFGR_SW_1;
  while(!(RCC->CFGR & RCC_CFGR_SWS_1));

  configureIt();

  while(1)
  {

  }
}