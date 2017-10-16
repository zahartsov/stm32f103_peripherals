/*################## Инициализация системной периферии #########################
Порядок инициализации:
1.
################################################################################*/

#include "system.h"

void RTC_Init(Rtc* rtc)
{
  RCC->APB1ENR |= RCC_APB1ENR_PWREN | RCC_APB1ENR_BKPEN;

  //Снимаем защиту от записи
  PWR->CR |= PWR_CR_DBP;

  //Включаем источник тактирования
  RCC->CSR |= RCC_CSR_LSION;
  while(!(RCC->CSR & RCC_CSR_LSIRDY));

  //reset
  RCC->BDCR |= RCC_BDCR_BDRST;
  RCC->BDCR &= ~RCC_BDCR_BDRST;

  //Устанавливаем источник тактирования для RTC (LSI)
  RCC->BDCR &= ~RCC_BDCR_RTCSEL;
  RCC->BDCR |= RCC_BDCR_RTCSEL_LSI;
  RCC->BDCR |= RCC_BDCR_RTCEN;

  while(!(RTC->CRL & RTC_CRL_RTOFF));
  RTC->CRL |= RTC_CRL_CNF;

  RTC->PRLH = 0;
  RTC->PRLL = 0x9C3F;

  RTC->CRL &= ~RTC_CRL_CNF;
  while(!(RTC->CRL & RTC_CRL_RTOFF));

  //Устанавливаем защиту от записи
  PWR->CR &= ~PWR_CR_DBP;
}

void RCC_Init(Rcc* rcc)
{
  if(rcc->sysClockSource == RCC_CLOCK_SOURCE_PLL)
  {
    //Настраиваем тактирование от HSE через PLL
    if(rcc->pllClockSource == RCC_CLOCK_SOURCE_HSE)
    {
      if((rcc->sysclkFreq < 4000000) || (rcc->sysclkFreq > 72000000)) return;
      //if(((rcc->sysclkFreq / rcc->sourceFreq) > 16) || (rcc->sysclkFreq < rcc->sourceFreq)) return;
      //Включаем HSE
      RCC->CR |= RCC_CR_HSEON;
      while(!(RCC->CR & RCC_CR_HSERDY));
      //Устанавливаем HSE в качестве источника тактирования для PLL
      RCC->CFGR |= RCC_CFGR_PLLSRC;
      /*Вычисляем значение делителей источника тактирования
      Настройка будет удачной только если частота системного тика кратна частоте внешнего источника, делённой на 2
      */
      if((rcc->sysclkFreq % rcc->sourceFreq) == 0)
      {
        RCC->CFGR |= ((rcc->sysclkFreq / rcc->sourceFreq) - 2) << RCC_CFGR_PLLMULL9_Pos;
      }
      else if((rcc->sysclkFreq % (rcc->sourceFreq/2)) == 0)
      {
        RCC->CFGR |= RCC_CFGR_PLLXTPRE_HSE_DIV2;
        RCC->CFGR |= ((rcc->sysclkFreq / (rcc->sourceFreq/2)) - 2) << RCC_CFGR_PLLMULL9_Pos;
      }

      //Вычисляем значение предделителя системной шины
      //Предделитель - 2 в степени до 512
      //Если предделитель получается не равным двойке в степени, он округляется в меньшую сторону
      uint16_t tmp = (rcc->sysclkFreq / rcc->hclkFreq) >> 1;
      uint16_t div = 0;
      while(tmp)
      {
        tmp = tmp >> 1;
        div++;
      }
      if(div != 0) div = 8 + div - 1;
      RCC->CFGR |= div << RCC_CFGR_HPRE_Pos;

      //Выичсляем значение предделителя первой периферийное шины
      tmp = (rcc->hclkFreq / rcc->pclk1Freq) >> 1;
      div = 0;
      while(tmp)
      {
        tmp = tmp >> 1;
        div++;
      }
      if(div != 0) div = 4 + div - 1;
      RCC->CFGR |= div << RCC_CFGR_PPRE1_Pos;

      //Выичсляем значение предделителя второй периферийное шины
      tmp = (rcc->hclkFreq / rcc->pclk2Freq) >> 1;
      div = 0;
      while(tmp)
      {
        tmp = tmp >> 1;
        div++;
      }
      if(div != 0) div = 4 + div - 1;
      RCC->CFGR |= div << RCC_CFGR_PPRE2_Pos;

      //Настраиваем FLASH в соответствии с установленной частотой
      if(rcc->sysclkFreq > 48000000) FLASH->ACR |= FLASH_ACR_LATENCY_1;
      else if(rcc->sysclkFreq > 24000000) FLASH->ACR |= FLASH_ACR_LATENCY_0;
      else FLASH->ACR &= ~FLASH_ACR_LATENCY;

      //Включаем PLL
      RCC->CR |= RCC_CR_PLLON;
      while(!(RCC->CR & RCC_CR_PLLRDY));

      //Переключаемся на тактирование от PLL
      RCC->CFGR |= RCC_CFGR_SW_1;
      while(!(RCC->CFGR & RCC_CFGR_SWS_1));
    }
    //Настраиваем тактирование от HSI через PLL
    else
    {

    }

  }
  //Настраиваем тактирование от HSE напрямую
  else if(rcc->sysClockSource == RCC_CLOCK_SOURCE_HSE)
  {

  }
  //Настраиваем тактирование от HSI напрямую
  else if(rcc->sysClockSource == RCC_CLOCK_SOURCE_HSI)
  {

  }
}

void System_Init(System* system)
{
  system->i2cAddress = 0x01;

  RCC_Init(system->rcc);

  if(system->Clock)
  {
    RTC_Init(system->Clock);
  }

  if(system->LED)
  {
    system->LED->baseGPIO = (GPIO_TypeDef*)GPIOC;
    system->LED->pin = 13;
    system->LED->mode = GPIO_MODE_OUTPUT_PUSH_PULL;
    system->LED->speed = GPIO_SPEED_50MHZ;
    userGPIO_Init(system->LED);
  }

  /*if(system->Button)
  {
    system->Button->interruptFalling = EXTI_FALLING_DISABLE;
    system->Button->interruptRising = EXTI_RISING_ENABLE;
    system->Button->baseGPIO = (GPIO_TypeDef*)GPIOA_BASE;
    system->Button->pin = 0;
    system->Button->mode = GPIO_MODE_INPUT;
    system->Button->pull = GPIO_PULL_NOPULL;
    system->Button->speed = GPIO_SPEED_HIGH;
    system->Button->type = GPIO_TYPE_PUSHPULL;
    userGPIO_Init(system->Button);
    userEXTI_Init(system->Button);
  }

  if(system->IWatchDog)
  {
    system->IWatchDog->frequancy = 32000;
    system->IWatchDog->watchPeriod = 3000;
    system->IWatchDog->debugMode = IWDG_DEBUGMODE_STOP;
    IWDG_Init(system->IWatchDog);
    IWDG_Enable();
  }

  if(system->sleepMode)
  {
    system->sleepMode->debugMode = SLEEPMODE_DEBUGMODE_SLEEP_RUN | SLEEPMODE_DEBUGMODE_STOP_RUN | SLEEPMODE_DEBUGMODE_STANDBY_RUN;
    system->sleepMode->deepSleepModeEn = SLEEPMODE_SLEEPDEEP_ON;
    system->sleepMode->sleepOnExitMode = SLEEPMODE_SLEEPONEXIT_OFF;
    system->sleepMode->deepSleepMode = SLEEPMODE_DEEPSLEEP_STANDBY;
    system->sleepMode->wakeUpPinEn = SLEEPMODE_WKUPPIN_ON;
    sleepMode_Init(system->sleepMode);
  }*/

  system->timer->baseTimer = (TIM_TypeDef*)TIM2_BASE;
  system->timer->mode = TIMER_MODE_COUNTER;
  system->timer->timerAPBFreq = system->rcc->pclk1Freq * 2;
  system->timer->timerOneCount = 500;
  system->timer->timerPeriod = 500000;
  userTimer_Init(system->timer);
  userTimer_Enable(system->timer);

  SCB_DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;// разрешаем использовать DWT
  sys.takt = &DWT_CYCCNT;//смотрим сколько натикало
  DWT_CYCCNT = 0;// обнуляем значение
  DWT_CONTROL|= DWT_CTRL_CYCCNTENA_Msk; // включаем счётчик
}

uint8_t bouncingHandling(GPIO* GPIO)
{
  uint8_t tmp = 100;
  while((tmp > 0) && (tmp < 200))
  {
    if(GPIO->baseGPIO->IDR &(GPIO_IDR_IDR0 << GPIO->pin)) tmp++;
    else tmp--;
    DelayUs(200);
  }
  return(tmp);
}

//################### Функции программных задержек #############################
//Рассчитано для частоты 72 МГц (один такт ~14 нс), для другой нужно пересчитать
void DelayMs(uint32_t ms)
{
  DWT_CYCCNT = 0;
  ms = (ms*1000000)/14;
  while(DWT_CYCCNT < ms);
}

void DelayUs(uint32_t us)
{
  DWT_CYCCNT = 0;
  us = (us*1000)/14;
  while(DWT_CYCCNT < us);
}

void DelayNs(uint16_t ns)
{
  DWT_CYCCNT = 0;
  ns = ns/14;
  while(DWT_CYCCNT < ns);
}
//##############################################################################

void InterruptsPrioritySet()
{
  NVIC_SetPriority(SPI1_IRQn, 10);
  NVIC_SetPriority(EXTI1_IRQn, 13);
  //NVIC_SetPriority(TIM6_DAC_IRQn, 15);          //Таймер системный
}

void TIM2_IRQHandler()
{
  sys.timer->baseTimer->SR &= ~TIM_SR_UIF;

  uint32_t time = RTC->CNTH << 16;
  time |= RTC->CNTL;
  sys.Clock->hours = time / 3600;
  sys.Clock->minutes = time / 60;
  sys.Clock->seconds = time - (sys.Clock->hours * 3600) - (sys.Clock->minutes * 60);

  /*sys.Clock->year = 10 * ((RTC->DR >> 20) & 0x0F);
  sys.Clock->year += (RTC->DR >> 16) & 0x0F;
  sys.Clock->year += 2000;
  sys.Clock->month = 10 * ((RTC->DR >> 12) & 0x01);
  sys.Clock->month += (RTC->DR >> 8) & 0x0F;
  sys.Clock->day = 10 * ((RTC->DR >> 4) & 0x03);
  sys.Clock->day += RTC->DR & 0x0F;
  sys.Clock->weekDay = (RTC->DR >> 13) & 0x07;*/

  if(sys.LED->baseGPIO->IDR & (GPIO_IDR_IDR0 << sys.LED->pin)) sys.LED->baseGPIO->BSRR |= GPIO_BSRR_BR0 << sys.LED->pin;
  else sys.LED->baseGPIO->BSRR |= GPIO_BSRR_BS0 << sys.LED->pin;
}

/*void EXTI0_IRQHandler()
{
  EXTI->PR |= EXTI_PR_PR0;

  if(bouncingHandling(sys.Button))
  {
    sys.timer->baseTimer->CR1 |= TIM_CR1_UDIS;
    sys.timer->baseTimer->ARR /= 2;
    if(sys.timer->baseTimer->ARR < 100) sys.timer->baseTimer->ARR = 9999;
    sys.timer->baseTimer->CR1 &= ~TIM_CR1_UDIS;

    sys.timer->baseTimer->CR1 |= TIM_CR1_CEN;
    sys.sleepMode->sleepMode_ON = 0;
  }
}*/