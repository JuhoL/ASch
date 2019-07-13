//-----------------------------------------------------------------------------------------------------------------------------
// Copyright (c) 2019 Juho Lepistö
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the "Software"), to deal in the Software without restriction, including without 
// limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial
// portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
// CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------------------------------------------------------

//! @file    stm32f429xx_mock.cpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    23 Jun 2019
//!
//! @brief   Mocks for STM32F429xx.
//! 
//! These is mock of STM32F429xx registers. The file is based on ST original MCU header stm32f429xx.h
    
//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#include <stm32f429xx_mock.h>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Mock Structs
//-----------------------------------------------------------------------------------------------------------------------------

bool isGlobalInterrtupEnabled = false;

namespace Hal_Mock
{

TIM_TypeDef timers[timersMax];
RTC_TypeDef rtc;
WWDG_TypeDef wwdg;
IWDG_TypeDef iwdg;
SPI_TypeDef spis[spisMax];
USART_TypeDef usarts[usartsMax];
I2C_TypeDef i2cs[i2csMax];
CAN_TypeDef cans[cansMax];
PWR_TypeDef pwr;
DAC_TypeDef dac;
ADC_TypeDef adcs[adcsMax];
ADC_Common_TypeDef adcCommon;
SDIO_TypeDef sdio;
SYSCFG_TypeDef sysCfg;
EXTI_TypeDef exti;
SAI_Block_TypeDef sais[saisMax];
LTDC_TypeDef ltdc;
LTDC_Layer_TypeDef ltdcLayers[ltdcLayersMax];
GPIO_TypeDef gpios[gpiosMax];
CRC_TypeDef crc;
RCC_TypeDef rcc;
FLASH_TypeDef flash;
DMA_TypeDef dmas[dmasMax];
DMA_Stream_TypeDef dmaStreams[dmaStreamsMax];
ETH_TypeDef eth;
DMA2D_TypeDef dma2d;
DCMI_TypeDef dcmi;
RNG_TypeDef rng;
FMC_Bank1_TypeDef fmcBank1;
FMC_Bank1E_TypeDef fmcBank1E;
FMC_Bank2_3_TypeDef fmcBank2_3;
FMC_Bank4_TypeDef fmcBank4;
FMC_Bank5_6_TypeDef fmcBank5_6;
DBGMCU_TypeDef dbgMcu;
USB_OTG_GlobalTypeDef usbs[usbsMax];

SCnSCB_Type scnScb;
SCB_Type scb;
SysTick_Type sysTick;
NVIC_Type nvic;
ITM_Type itm;
DWT_Type dwt;
TPI_Type tpi;
CoreDebug_Type coreDebug;
MPU_Type mpu;
FPU_Type fpu;

void InitRegisters(void)
{
    InitTimerRegisters();
    InitRtcRegisters();
    InitWwdgRegisters();
    InitIwdgRegisters();
    InitSpiRegisters();
    InitUsartRegisters();
    InitI2cRegisters();
    InitCanRegisters();
    InitPwrRegisters();
    InitDacRegisters();
    InitAdcRegisters();
    InitSdioRegisters();
    InitSysCfgRegisters();
    InitExtiRegisters();
    InitSaiRegisters();
    InitLtdcRegisters();
    InitGpioRegisters();
    InitCrcRegisters();
    InitRccRegisters();
    InitFlashRegisters();
    InitDmaRegisters();
    InitEthRegisters();
    InitDma2dRegisters();
    InitDcmiRegisters();
    InitRngRegisters();
    InitFmcRegisters();
    InitDbgMcuRegisters();
    InitUsbRegisters();

    InitScnScbRegisters();
    InitScbRegisters();
    InitSysTickRegisters();
    InitNvicRegisters();
    InitItmRegisters();
    InitDwtRegisters();
    InitTpiRegisters();
    InitCoreDebugRegisters();
    InitMpuRegisters();
    InitFpuRegisters();

    return;
}

void InitTimerRegisters(void)
{
    for (uint32_t i = 0UL; i < timersMax; ++i)
    {
        timers[i].CR1   = 0x0UL;
        timers[i].CR2   = 0x0UL;
        timers[i].SMCR  = 0x0UL;
        timers[i].DIER  = 0x0UL;
        timers[i].SR    = 0x0UL;
        timers[i].EGR   = 0x0UL;
        timers[i].CCMR1 = 0x0UL;
        timers[i].CCMR2 = 0x0UL;
        timers[i].CCER  = 0x0UL;
        timers[i].CNT   = 0x0UL;
        timers[i].PSC   = 0x0UL;
        timers[i].ARR   = 0xFFFFUL;
        timers[i].RCR   = 0x0UL;
        timers[i].CCR1  = 0x0UL;
        timers[i].CCR2  = 0x0UL;
        timers[i].CCR3  = 0x0UL;
        timers[i].CCR4  = 0x0UL;
        timers[i].BDTR  = 0x0UL;
        timers[i].DCR   = 0x0UL;
        timers[i].DMAR  = 0x0UL;
        timers[i].OR    = 0x0UL;
    }
    return;
}

void InitRtcRegisters(void)
{
    rtc.TR          = 0x0UL;
    rtc.DR          = 0x2101UL;
    rtc.CR          = 0x0UL;
    rtc.ISR         = 0x7UL;
    rtc.PRER        = 0x7F00FFUL;
    rtc.WUTR        = 0xFFFFUL;
    rtc.CALIBR      = 0x0UL;
    rtc.ALRMAR      = 0x0UL;
    rtc.ALRMBR      = 0x0UL;
    rtc.WPR         = 0x0UL;
    rtc.SSR         = 0x0UL;
    rtc.SHIFTR      = 0x0UL;
    rtc.TSTR        = 0x0UL;
    rtc.TSDR        = 0x0UL;
    rtc.TSSSR       = 0x0UL;
    rtc.CALR        = 0x0UL;
    rtc.TAFCR       = 0x0UL;
    rtc.ALRMASSR    = 0x0UL;
    rtc.ALRMBSSR    = 0x0UL;
    rtc.RESERVED7   = 0x0UL;
    rtc.BKP0R       = 0x0UL;
    rtc.BKP1R       = 0x0UL;
    rtc.BKP2R       = 0x0UL;
    rtc.BKP3R       = 0x0UL;
    rtc.BKP4R       = 0x0UL;
    rtc.BKP5R       = 0x0UL;
    rtc.BKP6R       = 0x0UL;
    rtc.BKP7R       = 0x0UL;
    rtc.BKP8R       = 0x0UL;
    rtc.BKP9R       = 0x0UL;
    rtc.BKP10R      = 0x0UL;
    rtc.BKP11R      = 0x0UL;
    rtc.BKP12R      = 0x0UL;
    rtc.BKP13R      = 0x0UL;
    rtc.BKP14R      = 0x0UL;
    rtc.BKP15R      = 0x0UL;
    rtc.BKP16R      = 0x0UL;
    rtc.BKP17R      = 0x0UL;
    rtc.BKP18R      = 0x0UL;
    rtc.BKP19R      = 0x0UL;
    return;
}

void InitWwdgRegisters(void)
{
    wwdg.CR     = 0x7FUL;
    wwdg.CFR    = 0x7FUL;
    wwdg.SR     = 0x0UL;
    return;
}

void InitIwdgRegisters(void)
{
    iwdg.KR     = 0x0UL;
    iwdg.PR     = 0x0UL;
    iwdg.RLR    = 0xFFFUL;
    iwdg.SR     = 0x0UL;
    return;
}

void InitSpiRegisters(void)
{
    for (uint32_t i = 0UL; i < spisMax; ++i)
    {
        spis[i].CR1     = 0x0UL;
        spis[i].CR2     = 0x0UL;
        spis[i].SR      = 0x2UL;
        spis[i].DR      = 0x0UL;
        spis[i].CRCPR   = 0x7UL;
        spis[i].RXCRCR  = 0x0UL;
        spis[i].TXCRCR  = 0x0UL;
        spis[i].I2SCFGR = 0x0UL;
        spis[i].I2SPR   = 0x2UL;
    }
    return;
}

void InitUsartRegisters(void)
{
    for (uint32_t i = 0UL; i < usartsMax; ++i)
    {
        usarts[i].SR    = 0xC0UL;
        usarts[i].DR    = 0x0UL;
        usarts[i].BRR   = 0x0UL;
        usarts[i].CR1   = 0x0UL;
        usarts[i].CR2   = 0x0UL;
        usarts[i].CR3   = 0x0UL;
        usarts[i].GTPR  = 0x0UL;
    }
    return;
}

void InitI2cRegisters(void)
{
    for (uint32_t i = 0UL; i < i2csMax; ++i)
    {
        i2cs[i].CR1     = 0x0UL;
        i2cs[i].CR2     = 0x0UL;
        i2cs[i].OAR1    = 0x0UL;
        i2cs[i].OAR2    = 0x0UL;
        i2cs[i].DR      = 0x0UL;
        i2cs[i].SR1     = 0x0UL;
        i2cs[i].SR2     = 0x0UL;
        i2cs[i].CCR     = 0x0UL;
        i2cs[i].TRISE   = 0x2UL;
        i2cs[i].FLTR    = 0x0UL;
    }
    return;
}

void InitCanRegisters(void)
{
    for (uint32_t i = 0UL; i < cansMax; ++i)
    {
        cans[i].MCR             = 0x10002UL;
        cans[i].MSR             = 0xC02UL;
        cans[i].TSR             = 0x1C000000UL;
        cans[i].RF0R            = 0x0UL;
        cans[i].RF1R            = 0x0UL;
        cans[i].IER             = 0x0UL;
        cans[i].ESR             = 0x0UL;
        cans[i].BTR             = 0x1230000UL;
        for (uint32_t k = 0UL; k < 88UL; ++k)
        {
            cans[i].RESERVED0[k] = 0x0UL;
        }
        for (uint32_t k = 0UL; k < 3UL; ++k)
        {
            cans[i].sTxMailBox[k].TIR  = 0x0UL;
            cans[i].sTxMailBox[k].TDTR = 0x0UL;
            cans[i].sTxMailBox[k].TDLR = 0x0UL;
            cans[i].sTxMailBox[k].TDHR = 0x0UL;
        }
        for (uint32_t k = 0UL; k < 2UL; ++k)
        {
            cans[i].sFIFOMailBox[k].RIR     = 0x0UL;
            cans[i].sFIFOMailBox[k].RDTR    = 0x0UL;
            cans[i].sFIFOMailBox[k].RDLR    = 0x0UL;
            cans[i].sFIFOMailBox[k].RDHR    = 0x0UL;
        }
        for (uint32_t k = 0UL; k < 12UL; ++k)
        {
            cans[i].RESERVED1[k] = 0x0UL;
        }
        cans[i].FMR             = 0x2A1C0E01UL;
        cans[i].FM1R            = 0x0UL;
        cans[i].RESERVED2       = 0x0UL;
        cans[i].FS1R            = 0x0UL;
        cans[i].RESERVED3       = 0x0UL;
        cans[i].FFA1R           = 0x0UL;
        cans[i].RESERVED4       = 0x0UL;
        cans[i].FA1R            = 0x0UL;
        for (uint32_t k = 0UL; k < 8UL; ++k)
        {
            cans[i].RESERVED5[k] = 0x0UL;
        }
        for (uint32_t k = 0UL; k < 28UL; ++k)
        {
            cans[i].sFilterRegister[k].FR1 = 0x0UL;
            cans[i].sFilterRegister[k].FR2 = 0x0UL;
        }
    }
    return;
}

void InitPwrRegisters(void)
{
    pwr.CR  = 0xC000UL;
    pwr.CSR = 0x0UL;
    return;
}

void InitDacRegisters(void)
{
    dac.CR      = 0x0UL;
    dac.SWTRIGR = 0x0UL;
    dac.DHR12R1 = 0x0UL;
    dac.DHR12L1 = 0x0UL;
    dac.DHR8R1  = 0x0UL;
    dac.DHR12R2 = 0x0UL;
    dac.DHR12L2 = 0x0UL;
    dac.DHR8R2  = 0x0UL;
    dac.DHR12RD = 0x0UL;
    dac.DHR12LD = 0x0UL;
    dac.DHR8RD  = 0x0UL;
    dac.DOR1    = 0x0UL;
    dac.DOR2    = 0x0UL;
    dac.SR      = 0x0UL;
    return;
}

void InitAdcRegisters(void)
{
    for (uint32_t i = 0UL; i < adcsMax; ++i)
    {
        adcs[i].SR      = 0x0UL;
        adcs[i].CR1     = 0x0UL;
        adcs[i].CR2     = 0x0UL;
        adcs[i].SMPR1   = 0x0UL;
        adcs[i].SMPR2   = 0x0UL;
        adcs[i].JOFR1   = 0x0UL;
        adcs[i].JOFR2   = 0x0UL;
        adcs[i].JOFR3   = 0x0UL;
        adcs[i].JOFR4   = 0x0UL;
        adcs[i].HTR     = 0xFFFUL;
        adcs[i].LTR     = 0x0UL;
        adcs[i].SQR1    = 0x0UL;
        adcs[i].SQR2    = 0x0UL;
        adcs[i].SQR3    = 0x0UL;
        adcs[i].JSQR    = 0x0UL;
        adcs[i].JDR1    = 0x0UL;
        adcs[i].JDR2    = 0x0UL;
        adcs[i].JDR3    = 0x0UL;
        adcs[i].JDR4    = 0x0UL;
        adcs[i].DR      = 0x0UL;
    }
    adcCommon.CSR = 0x0UL;
    adcCommon.CCR = 0x0UL;
    adcCommon.CDR = 0x0UL;
    return;
}

void InitSdioRegisters(void)
{
    sdio.POWER      = 0x0UL;
    sdio.CLKCR      = 0x0UL;
    sdio.ARG        = 0x0UL;
    sdio.CMD        = 0x0UL;
    sdio.RESPCMD    = 0x0UL;
    sdio.RESP1      = 0x0UL;
    sdio.RESP2      = 0x0UL;
    sdio.RESP3      = 0x0UL;
    sdio.RESP4      = 0x0UL;
    sdio.DTIMER     = 0x0UL;
    sdio.DLEN       = 0x0UL;
    sdio.DCTRL      = 0x0UL;
    sdio.DCOUNT     = 0x0UL;
    sdio.STA        = 0x0UL;
    sdio.ICR        = 0x0UL;
    sdio.MASK       = 0x0UL;
    for (uint32_t k = 0UL; k < 2UL; ++k)
    {
        sdio.RESERVED0[k] = 0x0UL;
    }
    sdio.FIFOCNT    = 0x0UL;
    for (uint32_t k = 0UL; k < 13UL; ++k)
    {
        sdio.RESERVED1[k] = 0x0UL;
    }
    sdio.FIFO       = 0x0UL;
    return;
}

void InitSysCfgRegisters(void)
{
    sysCfg.MEMRMP   = 0x0UL;
    sysCfg.PMC      = 0x0UL;
    for (uint32_t k = 0UL; k < 4UL; ++k)
    {
        sysCfg.EXTICR[k] = 0x0UL;
    }
    for (uint32_t k = 0UL; k < 2UL; ++k)
    {
        sysCfg.RESERVED[k] = 0x0UL;
    }
    sysCfg.CMPCR    = 0x0UL;
    return;
}

void InitExtiRegisters(void)
{
    exti.IMR    = 0x0UL;
    exti.EMR    = 0x0UL;
    exti.RTSR   = 0x0UL;
    exti.FTSR   = 0x0UL;
    exti.SWIER  = 0x0UL;
    exti.PR     = 0x0UL;
    return;
}

void InitSaiRegisters(void)
{
    for (uint32_t i = 0UL; i < saisMax; ++i)
    {
        sais[i].CR1     = 0x40UL;
        sais[i].CR2     = 0x0UL;
        sais[i].FRCR    = 0x7UL;
        sais[i].SLOTR   = 0x0UL;
        sais[i].IMR     = 0x0UL;
        sais[i].SR      = 0x8UL;
        sais[i].CLRFR   = 0x0UL;
        sais[i].DR      = 0x0UL;
    }
    return;
}

void InitLtdcRegisters(void)
{
    for (uint32_t k = 0UL; k < 2UL; ++k)
    {
        ltdc.RESERVED0[k] = 0x0UL;
    }
    ltdc.SSCR       = 0x0UL;
    ltdc.BPCR       = 0x0UL;
    ltdc.AWCR       = 0x0UL;
    ltdc.TWCR       = 0x0UL;
    ltdc.GCR        = 0x2220UL;
    for (uint32_t k = 0UL; k < 2UL; ++k)
    {
        ltdc.RESERVED1[k] = 0x0UL;
    }
    ltdc.SRCR       = 0x0UL;
    ltdc.RESERVED2[0] = 0x0UL;
    ltdc.BCCR       = 0x0UL;
    ltdc.RESERVED3[0] = 0x0UL;
    ltdc.IER        = 0x0UL;
    ltdc.ISR        = 0x0UL;
    ltdc.ICR        = 0x0UL;
    ltdc.LIPCR      = 0x0UL;
    ltdc.CPSR       = 0x0UL;
    ltdc.CDSR       = 0xFUL;
    for (uint32_t i = 0UL; i < ltdcLayersMax; ++i)
    {
        ltdcLayers[i].CR        = 0x0UL;
        ltdcLayers[i].WHPCR     = 0x0UL;
        ltdcLayers[i].WVPCR     = 0x0UL;
        ltdcLayers[i].CKCR      = 0x0UL;
        ltdcLayers[i].PFCR      = 0x0UL;
        ltdcLayers[i].CACR      = 0xFFUL;
        ltdcLayers[i].DCCR      = 0x0UL;
        ltdcLayers[i].BFCR      = 0x607UL;
        for (uint32_t k = 0UL; k < 2UL; ++k)
        {
            ltdcLayers[i].RESERVED0[k] = 0x0UL;
        }
        ltdcLayers[i].CFBAR     = 0x0UL;
        ltdcLayers[i].CFBLR     = 0x0UL;
        ltdcLayers[i].CFBLNR    = 0x0UL;
        for (uint32_t k = 0UL; k < 3UL; ++k)
        {
            ltdcLayers[i].RESERVED1[k] = 0x0UL;
        }
        ltdcLayers[i].CLUTWR    = 0x0UL;
    }
    return;
}

void InitGpioRegisters(void)
{
    for (uint32_t i = 0UL; i < gpiosMax; ++i)
    {
        if (i == 0UL)
        {
            gpios[i].MODER   = 0xA8000000UL;
            gpios[i].OSPEEDR = 0x0C000000UL;
            gpios[i].PUPDR   = 0x64000000UL;
        }
        else if (i == 1UL)
        {
            gpios[i].MODER   = 0x280UL;
            gpios[i].OSPEEDR = 0x0C0UL;
            gpios[i].PUPDR   = 0x100UL;
        }
        else
        {
            gpios[i].MODER   = 0x0UL;
            gpios[i].OSPEEDR = 0x0UL;
            gpios[i].PUPDR   = 0x0UL;
        }
        gpios[i].OTYPER  = 0x0UL;
        gpios[i].PUPDR   = 0x0UL;
        gpios[i].IDR     = 0x0UL;
        gpios[i].ODR     = 0x0UL;
        gpios[i].BSRR    = 0x0UL;
        gpios[i].LCKR    = 0x0UL;
        for (uint32_t k = 0UL; k < 2UL; ++k)
        {
            gpios[i].AFR[k] = 0x0UL;
        }
    }
    return;
}

void InitCrcRegisters(void)
{
    crc.DR          = 0xFFFFFFFFUL;
    crc.IDR         = 0x0UL;
    crc.RESERVED0   = 0x0UL;
    crc.RESERVED1   = 0x0UL;
    crc.CR          = 0x0UL;
    return;
}

void InitRccRegisters(void)
{
    rcc.CR          = 0x83UL;
    rcc.PLLCFGR     = 0x24003010UL;
    rcc.CFGR        = 0x0UL;
    rcc.CIR         = 0x0UL;
    rcc.AHB1RSTR    = 0x0UL;
    rcc.AHB2RSTR    = 0x0UL;
    rcc.AHB3RSTR    = 0x0UL;
    rcc.RESERVED0   = 0x0UL;
    rcc.APB1RSTR    = 0x0UL;
    rcc.APB2RSTR    = 0x0UL;
    for (uint32_t k = 0UL; k < 2UL; ++k)
    {
        rcc.RESERVED1[k] = 0x0UL;
    }
    rcc.AHB1ENR     = 0x100000UL;
    rcc.AHB2ENR     = 0x0UL;
    rcc.AHB3ENR     = 0x0UL;
    rcc.RESERVED2   = 0x0UL;
    rcc.APB1ENR     = 0x0UL;
    rcc.APB2ENR     = 0x0UL;
    for (uint32_t k = 0UL; k < 2UL; ++k)
    {
        rcc.RESERVED3[k] = 0x0UL;
    }
    rcc.AHB1LPENR   = 0x7EEF97FFUL;
    rcc.AHB2LPENR   = 0xF1UL;
    rcc.AHB3LPENR   = 0x1UL;
    rcc.RESERVED4   = 0x0UL;
    rcc.APB1LPENR   = 0xF6FEC9FFUL;
    rcc.APB2LPENR   = 0x4777F33UL;
    for (uint32_t k = 0UL; k < 2UL; ++k)
    {
        rcc.RESERVED5[k] = 0x0UL;
    }
    rcc.BDCR        = 0x0UL;
    rcc.CSR         = 0xE000000UL;
    for (uint32_t k = 0UL; k < 2UL; ++k)
    {
        rcc.RESERVED6[k] = 0x0UL;
    }
    rcc.SSCGR       = 0x0UL;
    rcc.PLLI2SCFGR  = 0x24003000UL;
    rcc.PLLSAICFGR  = 0x24003000UL;
    rcc.DCKCFGR     = 0x0UL;
    return;
}

void InitFlashRegisters(void)
{
    flash.ACR       = 0x0UL;
    flash.KEYR      = 0x0UL;
    flash.OPTKEYR   = 0x0UL;
    flash.SR        = 0x0UL;
    flash.CR        = 0x80000000UL;
    flash.OPTCR     = 0xFFFAAEDUL;
    flash.OPTCR1    = 0xFFF0000UL;
    return;
}

void InitDmaRegisters(void)
{
    for (uint32_t i = 0UL; i < dmasMax; ++i)
    {
        dmas[i].LISR    = 0x0UL;
        dmas[i].HISR    = 0x0UL;
        dmas[i].LIFCR   = 0x0UL;
        dmas[i].HIFCR   = 0x0UL;
    }
    for (uint32_t i = 0UL; i < dmaStreamsMax; ++i)
    {
        dmaStreams[i].CR    = 0x0UL;
        dmaStreams[i].NDTR  = 0x0UL;
        dmaStreams[i].PAR   = 0x0UL;
        dmaStreams[i].M0AR  = 0x0UL;
        dmaStreams[i].M1AR  = 0x0UL;
        dmaStreams[i].FCR   = 0x21UL;
    }
    return;
}

void InitEthRegisters(void)
{
    eth.MACCR       = 0x8000UL;
    eth.MACFFR      = 0x0UL;
    eth.MACHTHR     = 0x0UL;
    eth.MACHTLR     = 0x0UL;
    eth.MACMIIAR    = 0x0UL;
    eth.MACMIIDR    = 0x0UL;
    eth.MACFCR      = 0x0UL;
    eth.MACVLANTR   = 0x0UL;
    for (uint32_t k = 0UL; k < 2UL; ++k)
    {
        eth.RESERVED0[k] = 0x0UL;
    }
    eth.MACRWUFFR   = 0x0UL;
    eth.MACPMTCSR   = 0x0UL;
    eth.RESERVED1   = 0x0UL;
    eth.MACDBGR     = 0x0UL;
    eth.MACSR       = 0x0UL;
    eth.MACIMR      = 0x0UL;
    eth.MACA0HR     = 0x8000FFFFUL;
    eth.MACA0LR     = 0xFFFFFFFFUL;
    eth.MACA1HR     = 0xFFFFUL;
    eth.MACA1LR     = 0xFFFFFFFFUL;
    eth.MACA2HR     = 0xFFFFUL;
    eth.MACA2LR     = 0xFFFFFFFFUL;
    eth.MACA3HR     = 0xFFFFUL;
    eth.MACA3LR     = 0xFFFFFFFFUL;
    for (uint32_t k = 0UL; k < 40UL; ++k)
    {
        eth.RESERVED2[k] = 0x0UL;
    }
    eth.MMCCR       = 0x0UL;
    eth.MMCRIR      = 0x0UL;
    eth.MMCTIR      = 0x0UL;
    eth.MMCRIMR     = 0x0UL;
    eth.MMCTIMR     = 0x0UL;
    for (uint32_t k = 0UL; k < 14UL; ++k)
    {
        eth.RESERVED3[k] = 0x0UL;
    }
    eth.MMCTGFSCCR  = 0x0UL;
    eth.MMCTGFMSCCR = 0x0UL;
    for (uint32_t k = 0UL; k < 5UL; ++k)
    {
        eth.RESERVED4[k] = 0x0UL;
    }
    eth.MMCTGFCR    = 0x0UL;
    for (uint32_t k = 0UL; k < 10UL; ++k)
    {
        eth.RESERVED5[k] = 0x0UL;
    }
    eth.MMCRFCECR   = 0x0UL;
    eth.MMCRFAECR   = 0x0UL;
    for (uint32_t k = 0UL; k < 10UL; ++k)
    {
        eth.RESERVED6[k] = 0x0UL;
    }
    eth.MMCRGUFCR   = 0x0UL;
    for (uint32_t k = 0UL; k < 334UL; ++k)
    {
        eth.RESERVED7[k] = 0x0UL;
    }
    eth.PTPTSCR     = 0x2000UL;
    eth.PTPSSIR     = 0x0UL;
    eth.PTPTSHR     = 0x0UL;
    eth.PTPTSLR     = 0x0UL;
    eth.PTPTSHUR    = 0x0UL;
    eth.PTPTSLUR    = 0x0UL;
    eth.PTPTSAR     = 0x0UL;
    eth.PTPTTHR     = 0x0UL;
    eth.PTPTTLR     = 0x0UL;
    eth.RESERVED8   = 0x0UL;
    eth.PTPTSSR     = 0x0UL;
    for (uint32_t k = 0UL; k < 565UL; ++k)
    {
        eth.RESERVED9[k] = 0x0UL;
    }
    eth.DMABMR      = 0x20101UL;
    eth.DMATPDR     = 0x0UL;
    eth.DMARPDR     = 0x0UL;
    eth.DMARDLAR    = 0x0UL;
    eth.DMATDLAR    = 0x0UL;
    eth.DMASR       = 0x0UL;
    eth.DMAOMR      = 0x0UL;
    eth.DMAIER      = 0x0UL;
    eth.DMAMFBOCR   = 0x0UL;
    eth.DMARSWTR    = 0x0UL;
    for (uint32_t k = 0UL; k < 8UL; ++k)
    {
        eth.RESERVED10[k] = 0x0UL;
    }
    eth.DMACHTDR    = 0x0UL;
    eth.DMACHRDR    = 0x0UL;
    eth.DMACHTBAR   = 0x0UL;
    eth.DMACHRBAR   = 0x0UL;
    return;
}

void InitDma2dRegisters(void)
{
    dma2d.CR        = 0x0UL;
    dma2d.ISR       = 0x0UL;
    dma2d.IFCR      = 0x0UL;
    dma2d.FGMAR     = 0x0UL;
    dma2d.FGOR      = 0x0UL;
    dma2d.BGMAR     = 0x0UL;
    dma2d.BGOR      = 0x0UL;
    dma2d.FGPFCCR   = 0x0UL;
    dma2d.FGCOLR    = 0x0UL;
    dma2d.BGPFCCR   = 0x0UL;
    dma2d.BGCOLR    = 0x0UL;
    dma2d.FGCMAR    = 0x0UL;
    dma2d.BGCMAR    = 0x0UL;
    dma2d.OPFCCR    = 0x0UL;
    dma2d.OCOLR     = 0x0UL;
    dma2d.OMAR      = 0x0UL;
    dma2d.OOR       = 0x0UL;
    dma2d.NLR       = 0x0UL;
    dma2d.LWR       = 0x0UL;
    dma2d.AMTCR     = 0x0UL;
    for (uint32_t k = 0UL; k < 236UL; ++k)
    {
        dma2d.RESERVED[k] = 0x0UL;
    }
    for (uint32_t k = 0UL; k < 256UL; ++k)
    {
        dma2d.FGCLUT[k] = 0x0UL;
        dma2d.BGCLUT[k] = 0x0UL;
    }
    return;
}

void InitDcmiRegisters(void)
{
    dcmi.CR         = 0x0UL;
    dcmi.SR         = 0x0UL;
    dcmi.RISR       = 0x0UL;
    dcmi.IER        = 0x0UL;
    dcmi.MISR       = 0x0UL;
    dcmi.ICR        = 0x0UL;
    dcmi.ESCR       = 0x0UL;
    dcmi.ESUR       = 0x0UL;
    dcmi.CWSTRTR    = 0x0UL;
    dcmi.CWSIZER    = 0x0UL;
    dcmi.DR         = 0x0UL;
    return;
}

void InitRngRegisters(void)
{
    rng.CR = 0x0UL;
    rng.SR = 0x0UL;
    rng.DR = 0x0UL;
    return;
}

void InitFmcRegisters(void)
{
    for (uint32_t k = 0UL; k < 8UL; ++k)
    {
        if (k == 0UL)
        {
            fmcBank1.BTCR[k] = 0x30DBUL;
        }
        else if (k < 4UL)
        {
            fmcBank1.BTCR[k] = 0x30D2UL;
        }
        else
        {
            fmcBank1.BTCR[k] = 0xFFFFFFFUL;
        }
    }

    for (uint32_t k = 0UL; k < 7UL; ++k)
    {
        fmcBank1E.BWTR[k] = 0xFFFFFFFUL;
    }

    fmcBank2_3.PCR2         = 0x18UL;
    fmcBank2_3.SR2          = 0x40UL;
    fmcBank2_3.PMEM2        = 0xFCFCFCFCUL;
    fmcBank2_3.PATT2        = 0xFCFCFCFCUL;
    fmcBank2_3.RESERVED0    = 0x0UL;
    fmcBank2_3.ECCR2        = 0x0UL;
    fmcBank2_3.RESERVED1    = 0x0UL;
    fmcBank2_3.RESERVED2    = 0x0UL;
    fmcBank2_3.PCR3         = 0x18UL;
    fmcBank2_3.SR3          = 0x40UL;
    fmcBank2_3.PMEM3        = 0xFCFCFCFCUL;
    fmcBank2_3.PATT3        = 0xFCFCFCFCUL;
    fmcBank2_3.RESERVED3    = 0x0UL;
    fmcBank2_3.ECCR3        = 0x0UL;
    
    fmcBank4.PCR4   = 0x18UL;
    fmcBank4.SR4    = 0x40UL;
    fmcBank4.PMEM4  = 0xFCFCFCFCUL;
    fmcBank4.PATT4  = 0xFCFCFCFCUL;
    fmcBank4.PIO4   = 0xFCFCFCFCUL;
    
    for (uint32_t k = 0UL; k < 2UL; ++k)
    {
        fmcBank5_6.SDCR[k] = 0x2D0UL;
        fmcBank5_6.SDTR[k] = 0xFFFFFFFUL;
    }
    fmcBank5_6.SDCMR    = 0x0UL;
    fmcBank5_6.SDRTR    = 0x0UL;
    fmcBank5_6.SDSR     = 0x0UL;
    
    return;
}

void InitDbgMcuRegisters(void)
{
    dbgMcu.IDCODE   = 0x0UL;
    dbgMcu.CR       = 0x0UL;
    dbgMcu.APB1FZ   = 0x0UL;
    dbgMcu.APB2FZ   = 0x0UL;
    return;
}

void InitUsbRegisters(void)
{
    for (uint32_t i = 0UL; i < usbsMax; ++i)
    {
        usbs[i].GOTGCTL             = 0x10000UL;
        usbs[i].GOTGINT             = 0x0UL;
        usbs[i].GAHBCFG             = 0x0UL;
        usbs[i].GUSBCFG             = 0x1440UL;
        usbs[i].GRSTCTL             = 0x80000000UL;
        usbs[i].GINTSTS             = 0x4000020UL;
        usbs[i].GINTMSK             = 0x0UL;
        usbs[i].GRXSTSR             = 0x0UL;
        usbs[i].GRXSTSP             = 0x0UL;
        usbs[i].GRXFSIZ             = 0x200UL;
        usbs[i].DIEPTXF0_HNPTXFSIZ  = 0x200UL;
        usbs[i].HNPTXSTS            = 0x80200UL;
        for (uint32_t k = 0UL; k < 2UL; ++k)
        {
            usbs[i].Reserved30[k]   = 0x0UL;
        }
        usbs[i].GCCFG               = 0x0UL;
        usbs[i].CID                 = 0x1200UL;
        for (uint32_t k = 0UL; k < 48UL; ++k)
        {
            usbs[i].Reserved40[k]   = 0x0UL;
        }
        usbs[i].HPTXFSIZ            = 0x2000400UL;
        for (uint32_t k = 0UL; k < 0xFUL; ++k)
        {
            usbs[i].DIEPTXF[k]      = 0x2000200UL;
        }
    }
    return;
}

void InitScnScbRegisters(void)
{
    scnScb.RESERVED0[0] = 0x0UL;
    scnScb.ICTR         = 0x0UL;
    scnScb.ACTLR        = 0x0UL;
    return;
}

void InitScbRegisters(void)
{
    scb.CPUID       = 0x0UL;
    scb.ICSR        = 0x0UL;
    scb.VTOR        = 0x0UL;
    scb.AIRCR       = 0x0UL;
    scb.SCR         = 0x0UL;
    scb.CCR         = 0x0UL;
    for (uint32_t k = 0UL; k < 12UL; ++k)
    {
        scb.SHP[k]  = 0x0UL;
    }
    scb.SHCSR       = 0x0UL;
    scb.CFSR        = 0x0UL;
    scb.HFSR        = 0x0UL;
    scb.DFSR        = 0x0UL;
    scb.MMFAR       = 0x0UL;
    scb.BFAR        = 0x0UL;
    scb.AFSR        = 0x0UL;
    for (uint32_t k = 0UL; k < 2UL; ++k)
    {
        scb.PFR[k]  = 0x0UL;
    }
    scb.DFR         = 0x0UL;
    scb.ADR         = 0x0UL;
    for (uint32_t k = 0UL; k < 4UL; ++k)
    {
        scb.MMFR[k] = 0x0UL;
    }
    for (uint32_t k = 0UL; k < 5UL; ++k)
    {
        scb.ISAR[k] = 0x0UL;
    }
    for (uint32_t k = 0UL; k < 5UL; ++k)
    {
        scb.RESERVED0[k] = 0x0UL;
    }
    scb.CPACR       = 0x0UL;
    return;
}

void InitSysTickRegisters(void)
{
    sysTick.CTRL    = 0x0UL;
    sysTick.LOAD    = 0x0UL;
    sysTick.VAL     = 0x0UL;
    sysTick.CALIB   = 1000UL;
    return;
}

void InitNvicRegisters(void)
{
    for (uint32_t k = 0UL; k < 8UL; ++k)
    {
        nvic.ISER[k] = 0x0UL;
        nvic.ICER[k] = 0x0UL;
        nvic.ISPR[k] = 0x0UL;
        nvic.ICPR[k] = 0x0UL;
        nvic.IABR[k] = 0x0UL;
    }
    for (uint32_t k = 0UL; k < 240UL; ++k)
    {
        nvic.IP[k] = 0x0UL;
    }
    
    for (uint32_t k = 0UL; k < 24UL; ++k)
    {
        nvic.RESERVED0[k] = 0x0UL;
        nvic.RSERVED1[k]  = 0x0UL;
        nvic.RESERVED2[k] = 0x0UL;
        nvic.RESERVED3[k] = 0x0UL;
    }
    for (uint32_t k = 0UL; k < 56UL; ++k)
    {
        nvic.RESERVED4[k]   = 0x0UL;
    }
    for (uint32_t k = 0UL; k < 644UL; ++k)
    {
        nvic.RESERVED5[k]   = 0x0UL;
    }
    nvic.STIR = 0x0UL;
    return;
}

void InitItmRegisters(void)
{
    for (uint32_t k = 0UL; k < 32UL; ++k)
    {
        itm.PORT[k].u32 = 0x0UL;
    }
    for (uint32_t k = 0UL; k < 864UL; ++k)
    {
        itm.RESERVED0[k] = 0x0UL;
    }
    itm.TER         = 0x0UL;
    for (uint32_t k = 0UL; k < 15UL; ++k)
    {
        itm.RESERVED1[k] = 0x0UL;
    }
    itm.TPR         = 0x0UL;
    for (uint32_t k = 0UL; k < 15UL; ++k)
    {
        itm.RESERVED2[k] = 0x0UL;
    }
    itm.TCR         = 0x0UL;
    for (uint32_t k = 0UL; k < 29UL; ++k)
    {
        itm.RESERVED3[k] = 0x0UL;
    }
    itm.IWR         = 0x0UL;
    itm.IRR         = 0x0UL;
    itm.IMCR        = 0x0UL;
    for (uint32_t k = 0UL; k < 43UL; ++k)
    {
        itm.RESERVED4[k] = 0x0UL;
    }
    itm.LAR         = 0x0UL;
    itm.LSR         = 0x0UL;
    for (uint32_t k = 0UL; k < 6UL; ++k)
    {
        itm.RESERVED5[k] = 0x0UL;
    }
    itm.PID4        = 0x0UL;
    itm.PID5        = 0x0UL;
    itm.PID6        = 0x0UL;
    itm.PID7        = 0x0UL;
    itm.PID0        = 0x0UL;
    itm.PID1        = 0x0UL;
    itm.PID2        = 0x0UL;
    itm.PID3        = 0x0UL;
    itm.CID0        = 0x0UL;
    itm.CID1        = 0x0UL;
    itm.CID2        = 0x0UL;
    itm.CID3        = 0x0UL;
    return;
}

void InitDwtRegisters(void)
{
    dwt.CTRL            = 0x0UL;
    dwt.CYCCNT          = 0x0UL;
    dwt.CPICNT          = 0x0UL;
    dwt.EXCCNT          = 0x0UL;
    dwt.SLEEPCNT        = 0x0UL;
    dwt.LSUCNT          = 0x0UL;
    dwt.FOLDCNT         = 0x0UL;
    dwt.PCSR            = 0x0UL;
    dwt.COMP0           = 0x0UL;
    dwt.MASK0           = 0x0UL;
    dwt.FUNCTION0       = 0x0UL;
    dwt.RESERVED0[0]    = 0x0UL;
    dwt.COMP1           = 0x0UL;
    dwt.MASK1           = 0x0UL;
    dwt.FUNCTION1       = 0x0UL;
    dwt.RESERVED1[0]    = 0x0UL;
    dwt.COMP2           = 0x0UL;
    dwt.MASK2           = 0x0UL;
    dwt.FUNCTION2       = 0x0UL;
    dwt.RESERVED2[0]    = 0x0UL;
    dwt.COMP3           = 0x0UL;
    dwt.MASK3           = 0x0UL;
    dwt.FUNCTION3       = 0x0UL;
    return;
}

void InitTpiRegisters(void)
{
    tpi.SSPSR           = 0x0UL;
    tpi.CSPSR           = 0x0UL;
    for (uint32_t k = 0UL; k < 2UL; ++k)
    {
        tpi.RESERVED0[k] = 0x0UL;
    }
    tpi.ACPR            = 0x0UL;
    for (uint32_t k = 0UL; k < 55UL; ++k)
    {
        tpi.RESERVED1[k] = 0x0UL;
    }
    tpi.SPPR        = 0x0UL;
    for (uint32_t k = 0UL; k < 131UL; ++k)
    {
        tpi.RESERVED2[k] = 0x0UL;
    }
    tpi.FFSR            = 0x0UL;
    tpi.FFCR            = 0x0UL;
    tpi.FSCR            = 0x0UL;
    for (uint32_t k = 0UL; k < 759UL; ++k)
    {
        tpi.RESERVED3[k] = 0x0UL;
    }
    tpi.TRIGGER         = 0x0UL;
    tpi.FIFO0           = 0x0UL;
    tpi.ITATBCTR2       = 0x0UL;
    tpi.RESERVED4[0]    = 0x0UL;
    tpi.ITATBCTR0       = 0x0UL;
    tpi.FIFO1           = 0x0UL;
    tpi.ITCTRL          = 0x0UL;
    for (uint32_t k = 0UL; k < 39UL; ++k)
    {
        tpi.RESERVED5[k] = 0x0UL;
    }
    tpi.CLAIMSET        = 0x0UL;
    tpi.CLAIMCLR        = 0x0UL;
    for (uint32_t k = 0UL; k < 8UL; ++k)
    {
        tpi.RESERVED7[k] = 0x0UL;
    }
    tpi.DEVID           = 0x0UL;
    tpi.DEVTYPE         = 0x0UL;
    return;
}

void InitCoreDebugRegisters(void)
{
    coreDebug.DHCSR = 0x0UL;
    coreDebug.DCRSR = 0x0UL;
    coreDebug.DCRDR = 0x0UL;
    coreDebug.DEMCR = 0x0UL;
    return;
}

void InitMpuRegisters(void)
{
    mpu.TYPE    = 0x0UL;
    mpu.CTRL    = 0x0UL;
    mpu.RNR     = 0x0UL;
    mpu.RBAR    = 0x0UL;
    mpu.RASR    = 0x0UL;
    mpu.RBAR_A1 = 0x0UL;
    mpu.RASR_A1 = 0x0UL;
    mpu.RBAR_A2 = 0x0UL;
    mpu.RASR_A2 = 0x0UL;
    mpu.RBAR_A3 = 0x0UL;
    mpu.RASR_A3 = 0x0UL;
    return;
}

void InitFpuRegisters(void)
{
    fpu.RESERVED0[0]    = 0x0UL;
    fpu.FPCCR           = 0x0UL;
    fpu.FPCAR           = 0x0UL;
    fpu.FPDSCR          = 0x0UL;
    fpu.MVFR0           = 0x0UL;
    fpu.MVFR1           = 0x0UL;
    return;
}

} // namespace Hal_Mock
