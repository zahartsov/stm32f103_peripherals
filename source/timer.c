/*##################################### TIM ###################################
������� �������������:
1. ��������� ������������ ������� � �������� RCC->APB2ENR (TIM 1,8,9,10,11) � RCC->APB1ENR (���������) ���������� ���� RCC_APB1ENR_TIM�EN, ��� � - ����� �������.
2. ��������� ������������������� ������ ������ ���������� �������� � TIM�->CR2, ��� � - ����� �������.
3. ��������� ���������� ������� � �������� TIM�->DIER, ��� � - ����� �������:
    TIM_DIER_UIE - ��������� ���������� �� ������������ (?);
    TIM_DIER_UDE - ��������� ������� � DMA �� ������������ (?).
4. ��������� ������������ � �������� TIM�->PSC
5. ��������� ������� ����� TIM�->ARR
    ������
    ������� ���� ���1 = 84��� (��. ���� system_stm32f4xx.c ������� SetSysClock())
    ������������ PSC = 8400
    !!!!!� ������� ������������ 8400 - 1 = 8399!!!!!
    ������ ARR = 10 000
    !!!!!� ������� ������������ 10 000 - 1 = 9999!!!!!
    (84 000 000 / 8400) * 10 000 = 1 ���
    ������ ����������� 1 ��� � �������.
6. ��������� ���������� � NVIC:
    NVIC_EnableIRQ(IRQn_Type IRQn).
7. ��������� ������ ������� � ��� ��������� � �������� TIM�->CR1:
    TIM_CR1_CEN - ��������� �������;
    TIM_CR1_ARPE - ��������� ������������ ��������� ������� ����� �������.
################################################################################*/


#include "timer.h"

void userTimer_Init(Timer* timer)
{
  if((timer->timerPeriod / timer->timerOneCount) > 0xFFFF) return;
  if(((timer->timerAPBFreq / 1000000) * timer->timerOneCount) > 0xFFFF) return;
  if(timer->baseTimer == (TIM_TypeDef*)TIM1_BASE)
  {
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);
    NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
    NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
    NVIC_EnableIRQ(TIM1_CC_IRQn);
  }
  else if(timer->baseTimer == (TIM_TypeDef*)TIM2_BASE)
  {
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    NVIC_EnableIRQ(TIM2_IRQn);
  }
  else if(timer->baseTimer == (TIM_TypeDef*)TIM3_BASE)
  {
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    NVIC_EnableIRQ(TIM3_IRQn);
  }
  else if(timer->baseTimer == (TIM_TypeDef*)TIM4_BASE)
  {
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    NVIC_EnableIRQ(TIM4_IRQn);
  }

  timer->baseTimer->PSC = ((timer->timerAPBFreq / 1000000) * timer->timerOneCount) - 1;
  timer->baseTimer->ARR = (timer->timerPeriod / timer->timerOneCount) - 1;

  if(timer->mode == TIMER_MODE_COUNTER)
  {
    timer->baseTimer->DIER |= TIM_DIER_UIE;
    timer->baseTimer->CR1 |= TIM_CR1_ARPE;
  }
  else if(timer->mode == TIMER_MODE_PWM)
  {
    if(timer->channel == 1) timer->baseTimer->CCER |= TIM_CCER_CC1E;
    if(timer->channel == 2) timer->baseTimer->CCER |= TIM_CCER_CC2E;
    if(timer->channel == 3) timer->baseTimer->CCER |= TIM_CCER_CC3E;
    if(timer->channel == 4) timer->baseTimer->CCER |= TIM_CCER_CC4E;

    timer->baseTimer->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;
    timer->baseTimer->CCR2 = ((timer->baseTimer->ARR) * timer->pulseWidth) / 100;
  }
}

void userTimer_Enable(Timer* timer)
{
  timer->baseTimer->CR1 |= TIM_CR1_CEN;
}

void userTimer_Disable(Timer* timer)
{
  timer->baseTimer->CR1 &= ~TIM_CR1_CEN;
}