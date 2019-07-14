//-----------------------------------------------------------------------------------------------------------------------------
// Copyright (c) 2018 Juho Lepistö
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

//! @file    Hal_Isr.cpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    29 Aug 2018
//!
//! @class   Isr
//! @brief   HAL interface for ISRs.
//! 
//! The ISR module manages interrupt vectors, global interrupt enable state, and NVIC configurations.

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Files
//-----------------------------------------------------------------------------------------------------------------------------

#include <Hal_Isr.hpp>
#include <Utils_Assert.hpp>
#include <stm32f4xx.h>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Typedefs, Structs, Enums and Constants
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Local Variables
//-----------------------------------------------------------------------------------------------------------------------------

namespace
{

/// @brief Conversion table to easily convert Interrupt enum into Cortex IRQns.
/// The reserved interrupts are marked as non-maskable since there is no "invalid_IRQn"
/// defined in the enum.
const IRQn_Type irqNumbers[] =
{
    NonMaskableInt_IRQn,        // reserved
    NonMaskableInt_IRQn,        // reserved
    NonMaskableInt_IRQn,        // nonMaskableInt
    NonMaskableInt_IRQn,        // reserved
    MemoryManagement_IRQn,      // memoryManagement
    BusFault_IRQn,              // busFault
    UsageFault_IRQn,            // usageFault
    NonMaskableInt_IRQn,        // reserved
    NonMaskableInt_IRQn,        // reserved
    NonMaskableInt_IRQn,        // reserved
    NonMaskableInt_IRQn,        // reserved
    SVCall_IRQn,                // svCall
    DebugMonitor_IRQn,          // debugMonitor
    NonMaskableInt_IRQn,        // reserved
    PendSV_IRQn,                // pendSV
    SysTick_IRQn,               // sysTick
    WWDG_IRQn,                  // wwd
    PVD_IRQn,                   // pv
    TAMP_STAMP_IRQn,            // tamperAndTimestamp
    RTC_WKUP_IRQn,              // rtcWakeup
    FLASH_IRQn,                 // flash
    RCC_IRQn,                   // rcc
    EXTI0_IRQn,                 // exti0
    EXTI1_IRQn,                 // exti1
    EXTI2_IRQn,                 // exti2
    EXTI3_IRQn,                 // exti3
    EXTI4_IRQn,                 // exti4
    DMA1_Stream0_IRQn,          // dma1_stream0
    DMA1_Stream1_IRQn,          // dma1_stream1
    DMA1_Stream2_IRQn,          // dma1_stream2
    DMA1_Stream3_IRQn,          // dma1_stream3
    DMA1_Stream4_IRQn,          // dma1_stream4
    DMA1_Stream5_IRQn,          // dma1_stream5
    DMA1_Stream6_IRQn,          // dma1_stream6
    ADC_IRQn,                   // adc
    CAN1_TX_IRQn,               // can1_tx
    CAN1_RX0_IRQn,              // can1_rx0
    CAN1_RX1_IRQn,              // can1_rx1
    CAN1_SCE_IRQn,              // can1_sce
    EXTI9_5_IRQn,               // exti9_5
    TIM1_BRK_TIM9_IRQn,         // tim1Break, tim9Global
    TIM1_UP_TIM10_IRQn,         // tim1Update, tim10Global
    TIM1_TRG_COM_TIM11_IRQn,    // tim1TriggerAndCommutation, tim11Global
    TIM1_CC_IRQn,               // tim1_CC
    TIM2_IRQn,                  // tim2
    TIM3_IRQn,                  // tim3
    TIM4_IRQn,                  // tim4
    I2C1_EV_IRQn,               // i2c1_event
    I2C1_ER_IRQn,               // i2c1_error
    I2C2_EV_IRQn,               // i2c2_event
    I2C2_ER_IRQn,               // i2c2_error
    SPI1_IRQn,                  // spi1
    SPI2_IRQn,                  // spi2
    USART1_IRQn,                // usart1
    USART2_IRQn,                // usart2
    USART3_IRQn,                // usart3
    EXTI15_10_IRQn,             // exti15_10
    RTC_Alarm_IRQn,             // rtc_alarm
    OTG_FS_WKUP_IRQn,           // OtgFsWakeup
    TIM8_BRK_TIM12_IRQn,        // tim8Break, tim12
    TIM8_UP_TIM13_IRQn,         // tim8Update, tim13
    TIM8_TRG_COM_TIM14_IRQn,    // tim8TriggerAndCommutation, tim14
    TIM8_CC_IRQn,               // tim8_CC
    DMA1_Stream7_IRQn,          // dma1_stream7
    FMC_IRQn,                   // fmc
    SDIO_IRQn,                  // sdio
    TIM5_IRQn,                  // tim5
    SPI3_IRQn,                  // spi3
    UART4_IRQn,                 // uart4
    UART5_IRQn,                 // uart5
    TIM6_DAC_IRQn,              // tim6Global, dacUnderrun
    TIM7_IRQn,                  // tim7
    DMA2_Stream0_IRQn,          // dma2_stream0
    DMA2_Stream1_IRQn,          // dma2_stream1
    DMA2_Stream2_IRQn,          // dma2_stream2
    DMA2_Stream3_IRQn,          // dma2_stream3
    DMA2_Stream4_IRQn,          // dma2_stream4
    ETH_IRQn,                   // ethernet
    ETH_WKUP_IRQn,              // ethernetWakeup
    CAN2_TX_IRQn,               // can2_tx
    CAN2_RX0_IRQn,              // can2_rx0
    CAN2_RX1_IRQn,              // can2_rx1
    CAN2_SCE_IRQn,              // can2_sce
    OTG_FS_IRQn,                // usbOtgFs
    DMA2_Stream5_IRQn,          // dma2_stream5
    DMA2_Stream6_IRQn,          // dma2_stream6
    DMA2_Stream7_IRQn,          // dma2_stream7
    USART6_IRQn,                // usart6
    I2C3_EV_IRQn,               // i2c3_event
    I2C3_ER_IRQn,               // i2c3_error
    OTG_HS_EP1_OUT_IRQn,        // usbOtgHsEndpoint1_out
    OTG_HS_EP1_IN_IRQn,         // usbOtgHsEndpoint1_in
    OTG_HS_WKUP_IRQn,           // UsbOtgHsWakeupThroughExti
    OTG_HS_IRQn,                // usbOtgHs
    DCMI_IRQn,                  // dcmi
    HASH_RNG_IRQn,              // hashAndRng
    FPU_IRQn,                   // fpu
    UART7_IRQn,                 // uart7
    UART8_IRQn,                 // uart8
    SPI4_IRQn,                  // spi4
    SPI5_IRQn,                  // spi5
    SPI6_IRQn,                  // spi6
    SAI1_IRQn,                  // sai1
    LTDC_IRQn,                  // ltdc
    LTDC_ER_IRQn,               // ltdcError
    DMA2D_IRQn                  // dma2D
};

#if (UNIT_TEST == 0)
    Hal::interruptHandler_t __attribute__((section(".vector_array"))) Handlers[static_cast<std::size_t>(Hal::Interrupt::max)] = {0};
#else
    Hal::interruptHandler_t Handlers[static_cast<std::size_t>(Hal::Interrupt::max)] = {0};
#endif

}

//-----------------------------------------------------------------------------------------------------------------------------
// 4. Inline Functions
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 5. Static Function Prototypes
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 6. Class Member Definitions
//-----------------------------------------------------------------------------------------------------------------------------

namespace Hal
{

//---------------------------------------
// Functions
//---------------------------------------
void Isr::Init(void)
{
    if (UNIT_TEST == 0)
    {
        // Check that the handler array aligns to 0x80.
        Utils::Assert((reinterpret_cast<uint32_t>(Handlers) & 0x7FUL) == 0x0UL);
    }
    for (uint32_t i = 0U; i < static_cast<uint32_t>(Interrupt::max); ++i)
    {
        Handlers[i] = 0;
    }
    SCB->VTOR = reinterpret_cast<uint32_t>(Handlers);
    return;
}

void Isr::EnableGlobal(void)
{
    __enable_irq();
    return;
}

void Isr::DisableGlobal(void)
{
    __disable_irq();
    return;
}

void Isr::SetHandler(Interrupt type, const interruptHandler_t Handler)
{
    Utils::Assert(Handler != 0);
    Handlers[static_cast<std::size_t>(type)] = Handler;
    return;
}

interruptHandler_t Isr::GetHandler(Interrupt type)
{
    return Handlers[static_cast<std::size_t>(type)];
}

void Isr::Enable(Interrupt type)
{
    Utils::Assert(Handlers[static_cast<std::size_t>(type)] != 0);
    NVIC_EnableIRQ(irqNumbers[static_cast<std::size_t>(type)]);
    return;
}

void Isr::Disable(Interrupt type)
{
    NVIC_DisableIRQ(irqNumbers[static_cast<std::size_t>(type)]);
    return;
}

void Isr::SetPriority(Interrupt type, uint32_t priority)
{
    Utils::Assert(type != Interrupt::nonMaskableInt);
    NVIC_SetPriority(irqNumbers[static_cast<std::size_t>(type)], priority);
    return;
}

uint8_t Isr::GetPriority(Interrupt type)
{
    return NVIC_GetPriority(irqNumbers[static_cast<std::size_t>(type)]);
}

void Isr::SetPending(Interrupt type)
{
    NVIC_SetPendingIRQ(irqNumbers[static_cast<std::size_t>(type)]);
    return;
}

bool Isr::GetPending(Interrupt type)
{
    return NVIC_GetPendingIRQ(irqNumbers[static_cast<std::size_t>(type)]);
}

void Isr::Clear(Interrupt type)
{
    NVIC_ClearPendingIRQ(irqNumbers[static_cast<std::size_t>(type)]);
    return;
}

} // namespace Hal

//-----------------------------------------------------------------------------------------------------------------------------
// 7. Global Functions
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 8. Static Functions
//-----------------------------------------------------------------------------------------------------------------------------
