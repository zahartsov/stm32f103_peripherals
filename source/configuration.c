/*################## Инициализация системной периферии #########################
Порядок инициализации:
1.
################################################################################*/

#include "configuration.h"

System sys;
Timer sysTimer;
GPIO sysLED;
/*GPIO sysButton;*/
rtc sysClock;

void configureIt()
{
  sys.hclk = 72000000;
  sys.pclk1 = 36000000;
  sys.pclk2 = 72000000;
  sys.timer = &sysTimer;
  sys.LED = &sysLED;
  /*sys.Button = &sysButton;*/
  sys.Clock = &sysClock;

  //IndependentWatchDog IWatchDog;
  //sys.IWatchDog = &IWatchDog;
  //SleepMode sleepMode;
  //sys.sleepMode = &sleepMode;

  InterruptsPrioritySet();
  System_Init(&sys);
}