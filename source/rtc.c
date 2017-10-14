/*##################################### RTC ###################################
������� �������������:
1.
################################################################################*/


#include "rtc.h"

void RTC_Init()
{
  RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN;

  //������� ������ �� ������
  PWR->CR |= PWR_CR_DBP;

  //�������� �������� ������������
  RCC->CSR |= RCC_CSR_LSION;
  while(!(RCC->CSR & RCC_CSR_LSIRDY));

  //reset
  RCC->BDCR |= RCC_BDCR_BDRST;
  RCC->BDCR &= ~RCC_BDCR_BDRST;

  //������������� �������� ������������ ��� RTC (LSI)
  RCC->BDCR &= ~RCC_BDCR_RTCSEL;
  RCC->BDCR |= RCC_BDCR_RTCSEL_LSI;
  RCC->BDCR |= RCC_BDCR_RTCEN;

  while(!(RTC->CRL & RTC_CRL_RTOFF));
  RTC->CRL |= RTC_CRL_CNF;

  RTC->PRLH = 0;
  RTC->PRLL = 0x9C3F;

  RTC->CRL &= ~RTC_CRL_CNF;
  while(!(RTC->CRL & RTC_CRL_RTOFF));

  //������������� ������ �� ������
  PWR->CR &= ~PWR_CR_DBP;
}
