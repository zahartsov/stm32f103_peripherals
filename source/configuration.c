/*################## Инициализация системной периферии #########################
Порядок инициализации:
1.
################################################################################*/

#include "configuration.h"

System sys;
Timer sysTimer;
GPIO sysLED;
/*GPIO sysButton;*/
Rtc sysClock;
Rcc sysRcc;
Iwdg sysIwdg;

void configureIt()
{
  sys.rcc = &sysRcc;
  sys.rcc->sysClockSource = RCC_CLOCK_SOURCE_PLL;
  sys.rcc->pllClockSource = RCC_CLOCK_SOURCE_HSE;
  sys.rcc->sourceFreq = 8000000;
  sys.rcc->sysclkFreq = 72000000;
  sys.rcc->hclkFreq = 72000000;
  sys.rcc->pclk1Freq = 36000000;
  sys.rcc->pclk2Freq = 72000000;
  sys.timer = &sysTimer;
  sys.LED = &sysLED;
  /*sys.Button = &sysButton;*/
  sys.Clock = &sysClock;
  sys.iwdg = &sysIwdg;

  //IndependentWatchDog IWatchDog;
  //sys.IWatchDog = &IWatchDog;
  //SleepMode sleepMode;
  //sys.sleepMode = &sleepMode;

  InterruptsPrioritySet();
  System_Init(&sys);
}