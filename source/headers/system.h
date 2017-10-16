#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "stm32f103xb.h"
#include "gpio.h"
#include "timer.h"

#define    DWT_CYCCNT    *(volatile unsigned long *)0xE0001004
#define    DWT_CONTROL   *(volatile unsigned long *)0xE0001000
#define    SCB_DEMCR     *(volatile unsigned long *)0xE000EDFC

#define OFF     0x01;
#define ON      0x00;

//Clock source
#define RCC_CLOCK_SOURCE_LSI    0x01
#define RCC_CLOCK_SOURCE_HSI    0x02
#define RCC_CLOCK_SOURCE_HSE    0x03
#define RCC_CLOCK_SOURCE_PLL    0x04

typedef struct
{
  uint8_t sysClockSource;
  uint8_t pllClockSource;
  uint32_t sysclkFreq;
  uint32_t sourceFreq;
  uint32_t hclkFreq;
  uint32_t pclk1Freq;
  uint32_t pclk2Freq;
}Rcc;

typedef struct
{
  uint8_t hours;
  uint8_t minutes;
  uint8_t seconds;
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t weekDay;
}Rtc;     // Параметры

typedef struct
{
  Rtc* Clock;
  Timer* timer;
  GPIO* LED;
  Rcc* rcc;
  //GPIO* Button;
  //SleepMode* sleepMode;
  //IndependentWatchDog* IWatchDog;
  uint8_t i2cAddress;
  uint8_t CPUtemperature;
  volatile unsigned long * takt;
}System;     // Параметры системы

extern System sys;
extern Rtc sysClock;
extern Timer sysTimer;
extern GPIO sysLED;
extern Rcc sysRcc;
//extern GPIO sysButton;
//extern SleepMode sleepMode;
//extern IndependentWatchDog IWatchDog;

void RCC_Init(Rcc*);
void RTC_Init(Rtc*);
void System_Init(System*);
void DelayMs(uint32_t);
void DelayUs(uint32_t);
void DelayNs(uint16_t);
void InterruptsPrioritySet(void);

uint32_t toPowerOfTwo(uint32_t);

#endif