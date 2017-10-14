#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "stm32f103xb.h"
#include "gpio.h"
#include "timer.h"
#include "rtc.h"

#define    DWT_CYCCNT    *(volatile unsigned long *)0xE0001004
#define    DWT_CONTROL   *(volatile unsigned long *)0xE0001000
#define    SCB_DEMCR     *(volatile unsigned long *)0xE000EDFC

#define OFF     0x01;
#define ON      0x00;

typedef struct
{
  rtc* Clock;
  Timer* timer;
  GPIO* LED;
  //GPIO* Button;
  //SleepMode* sleepMode;
  //IndependentWatchDog* IWatchDog;
  uint32_t hclk;        //частота процессора, √ц
  uint32_t pclk1;       //частота первой периферийной шины, √ц
  uint32_t pclk2;       //частота второй периферийной шины, √ц
  uint8_t i2cAddress;
  uint8_t CPUtemperature;
  volatile unsigned long * takt;
}System;     // ѕараметры системы

extern System sys;
extern rtc sysClock;
extern Timer sysTimer;
extern GPIO sysLED;
//extern GPIO sysButton;
//extern SleepMode sleepMode;
//extern IndependentWatchDog IWatchDog;

void System_Init(System*);
void DelayMs(uint32_t);
void DelayUs(uint32_t);
void DelayNs(uint16_t);
void InterruptsPrioritySet(void);

#endif