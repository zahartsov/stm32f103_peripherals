/*##################################### TIM ###################################
Порядок инициализации:
1. Включение тактирования таймера в регистре RCC->APB2ENR (TIM 1,8,9,10,11) и RCC->APB1ENR (остальные) установкой бита RCC_APB1ENR_TIMхEN, где х - номер таймера.
2. Настройка синхронизированного режима работы нескольких таймероа в TIMх->CR2, где х - номер таймера.
3. Настройка прерывания таймера в регистре TIMх->DIER, где х - номер таймера:
    TIM_DIER_UIE - включение прерывания по переполнению (?);
    TIM_DIER_UDE - включение запроса к DMA по переполнению (?).
4. Установка предделителя в регистре TIMх->PSC
5. Установка периода счёта TIMх->ARR
    Пример
    частота шины АВР1 = 84МГц (см. файл system_stm32f4xx.c функция SetSysClock())
    предделитель PSC = 8400
    !!!!!в регистр записывается 8400 - 1 = 8399!!!!!
    период ARR = 10 000
    !!!!!в регистр записывается 10 000 - 1 = 9999!!!!!
    (84 000 000 / 8400) * 10 000 = 1 сек
    Таймер срабатывает 1 раз в секунду.
6. Включение Прерывания в NVIC:
    NVIC_EnableIRQ(IRQn_Type IRQn).
7. Настройка работы таймера и его включение в регистре TIMх->CR1:
    TIM_CR1_CEN - включение таймера;
    TIM_CR1_ARPE - установка немедленного изменения периода счёта таймера.
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