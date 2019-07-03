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

//! @file    Hal_Interrupts.hpp
//! @author  Juho Lepistö <juho.lepisto(a)gmail.com>
//! @date    1 Jul 2019
//!
//! @brief   Interrupts for STM32F429ZI.
//! 
//! This enum is needed by ISR HAL.

#ifndef HAL_INTERRUPTS_HPP_
#define HAL_INTERRUPTS_HPP_

//-----------------------------------------------------------------------------------------------------------------------------
// 1. Include Dependencies
//-----------------------------------------------------------------------------------------------------------------------------

#include <Utils_Types.hpp>

//-----------------------------------------------------------------------------------------------------------------------------
// 2. Typedefs, Structs, Enums and Constants
//-----------------------------------------------------------------------------------------------------------------------------

namespace Hal
{

//! @brief   Different interrupt vectors available in the system.
enum class Interrupt
{
    nonMaskableInt = 2,             //!< Non Maskable Interrupt                
    memoryManagement = 4,           //!< Cortex-M4 Memory Management Interrupt 
    busFault = 5,                   //!< Cortex-M4 Bus Fault Interrupt         
    usageFault = 6,                 //!< Cortex-M4 Usage Fault Interrupt       
    svCall = 11,                    //!< Cortex-M4 SV Call Interrupt          
    debugMonitor = 12,              //!< Cortex-M4 Debug Monitor Interrupt    
    pendSV = 14,                    //!< Cortex-M4 Pend SV Interrupt          
    sysTick = 15,                   //!< SysTick Interrupt.
    wwdg = 16,                      //!< Window WatchDog Interrupt
    pvd = 17,                       //!< PVD through EXTI Line detection Interrupt
    tamperAndTimestamp = 18,        //!< Tamper and TimeStamp interrupts through the EXTI line
    rtcWakeup = 19,                 //!< RTC Wakeup interrupt through the EXTI line
    flash = 20,                     //!< FLASH global Interrupt
    rcc = 21,                       //!< RCC global Interrupt
    exti0 = 22,                     //!< EXTI Line0 Interrupt
    exti1 = 23,                     //!< EXTI Line1 Interrupt
    exti2 = 24,                     //!< EXTI Line2 Interrupt
    exti3 = 25,                     //!< EXTI Line3 Interrupt
    exti4 = 26,                     //!< EXTI Line4 Interrupt
    dma1_stream0 = 27,              //!< DMA1 Stream 0 global Interrupt
    dma1_stream1 = 28,              //!< DMA1 Stream 1 global Interrupt
    dma1_stream2 = 29,              //!< DMA1 Stream 2 global Interrupt
    dma1_stream3 = 30,              //!< DMA1 Stream 3 global Interrupt
    dma1_stream4 = 31,              //!< DMA1 Stream 4 global Interrupt
    dma1_stream5 = 32,              //!< DMA1 Stream 5 global Interrupt
    dma1_stream6 = 33,              //!< DMA1 Stream 6 global Interrupt
    adc = 34,                       //!< ADC1, ADC2 and ADC3 global Interrupts
    can1_tx = 35,                   //!< CAN1 TX Interrupt
    can1_rx0 = 36,                  //!< CAN1 RX0 Interrupt
    can1_rx1 = 37,                  //!< CAN1 RX1 Interrupt
    can1_sce = 38,                  //!< CAN1 SCE Interrupt
    exti9_5 = 39,                   //!< External Line[9:5] Interrupts
    tim1Break = 40,                 //!< TIM1 Break interrupt (shared with TIM9 global interrupt)
    tim9Global = 40,                //!< TIM9 global interrupt (shared with TIM1 Break interrupt)
    tim1Update = 41,                //!< TIM1 Update Interrupt (shared with TIM10 global interrupt)
    tim10Global = 41,               //!< TIM10 global interrupt (shared with TIM1 Update Interrupt)
    tim1TriggerAndCommutation = 42, //!< TIM1 Trigger and Commutation Interrupt (shared with TIM11 global interrupt)
    tim11Global = 42,               //!< TIM11 global interrupt (shared with TIM1 Trigger and Commutation Interrupt)
    tim1_CC = 43,                   //!< TIM1 Capture Compare Interrupt
    tim2 = 44,                      //!< TIM2 global Interrupt
    tim3 = 45,                      //!< TIM3 global Interrupt
    tim4 = 46,                      //!< TIM4 global Interrupt
    i2c1_event = 47,                //!< I2C1 Event Interrupt
    i2c1_error = 48,                //!< I2C1 Error Interrupt
    i2c2_event = 49,                //!< I2C2 Event Interrupt
    i2c2_error = 50,                //!< I2C2 Error Interrupt
    spi1 = 51,                      //!< SPI1 global Interrupt
    spi2 = 52,                      //!< SPI2 global Interrupt
    usart1 = 53,                    //!< USART1 global Interrupt
    usart2 = 54,                    //!< USART2 global Interrupt
    usart3 = 55,                    //!< USART3 global Interrupt
    exti15_10 = 56,                 //!< External Line[15:10] Interrupts
    rtc_alarm = 57,                 //!< RTC Alarm (A and B) through EXTI Line Interrupt
    OtgFsWakeup = 58,               //!< USB OTG FS Wakeup through EXTI line interrupt
    tim8Break = 59,                 //!< TIM8 Break Interrupt (shared with TIM12 global interrupt)
    tim12 = 59,                     //!< TIM12 global interrupt (shared with TIM8 Break Interrupt)
    tim8Update = 60,                //!< TIM8 Update Interrupt (shared with TIM13 global interrupt)
    tim13 = 60,                     //!< TIM13 global interrupt (shared with TIM8 Update Interrupt)
    tim8TriggerAndCommutation = 61, //!< TIM8 Trigger and Commutation Interrupt (shared with TIM14 global interrupt)
    tim14 = 61,                     //!< TIM14 global interrupt (shared with TIM8 Trigger and Commutation Interrupt)
    tim8_CC = 62,                   //!< TIM8 Capture Compare global interrupt
    dma1_stream7 = 63,              //!< DMA1 Stream7 Interrupt
    fmc = 64,                       //!< FMC global Interrupt
    sdio = 65,                      //!< SDIO global Interrupt
    tim5 = 66,                      //!< TIM5 global Interrupt
    spi3 = 67,                      //!< SPI3 global Interrupt
    uart4 = 68,                     //!< UART4 global Interrupt
    uart5 = 69,                     //!< UART5 global Interrupt
    tim6Global = 70,                //!< TIM6 global interrupt (shared with DAC1&2 underrun error interrupt)
    dacUnderrun = 70,               //!< DAC1&2 underrun error interrupt (shared with TIM6 global interrupt)
    tim7 = 71,                      //!< TIM7 global interrupt
    dma2_stream0 = 72,              //!< DMA2 Stream 0 global Interrupt
    dma2_stream1 = 73,              //!< DMA2 Stream 1 global Interrupt
    dma2_stream2 = 74,              //!< DMA2 Stream 2 global Interrupt
    dma2_stream3 = 75,              //!< DMA2 Stream 3 global Interrupt
    dma2_stream4 = 76,              //!< DMA2 Stream 4 global Interrupt
    ethernet = 77,                  //!< Ethernet global Interrupt
    ethernetWakeup = 78,            //!< Ethernet Wakeup through EXTI line Interrupt
    can2_tx = 79,                   //!< CAN2 TX Interrupt
    can2_rx0 = 80,                  //!< CAN2 RX0 Interrupt
    can2_rx1 = 81,                  //!< CAN2 RX1 Interrupt
    can2_sce = 82,                  //!< CAN2 SCE Interrupt
    usbOtgFs = 83,                  //!< USB OTG FS global Interrupt
    dma2_stream5 = 84,              //!< DMA2 Stream 5 global interrupt
    dma2_stream6 = 85,              //!< DMA2 Stream 6 global interrupt
    dma2_stream7 = 86,              //!< DMA2 Stream 7 global interrupt
    usart6 = 87,                    //!< USART6 global interrupt
    i2c3_event = 88,                //!< I2C3 event interrupt
    i2c3_error = 89,                //!< I2C3 error interrupt
    usbOtgHsEndpoint1_out = 90,     //!< USB OTG HS End Point 1 Out global interrupt
    usbOtgHsEndpoint1_in = 91,      //!< USB OTG HS End Point 1 In global interrupt
    UsbOtgHsWakeupThroughExti = 92, //!< USB OTG HS Wakeup through EXTI interrupt
    usbOtgHs = 93,                  //!< USB OTG HS global interrupt
    dcmi = 94,                      //!< DCMI global interrupt
    hashAndRng = 95,                //!< Hash and Rng global interrupt
    fpu = 96,                       //!< FPU global interrupt
    uart7 = 97,                     //!< UART7 global interrupt
    uart8 = 98,                     //!< UART8 global interrupt
    spi4 = 99,                      //!< SPI4 global Interrupt
    spi5 = 100,                     //!< SPI5 global Interrupt
    spi6 = 101,                     //!< SPI6 global Interrupt
    sai1 = 102,                     //!< SAI1 global Interrupt
    ltdc = 103,                     //!< LTDC global Interrupt
    ltdcError = 104,                //!< LTDC Error global Interrupt
    dma2D = 105,                    //!< DMA2D global Interrupt
    max                             //!< The total number of vectors. Must be last on the enum!
};

} // namespace Hal

//-----------------------------------------------------------------------------------------------------------------------------
// 3. Inline Functions
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 4. Global Functions
//-----------------------------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------------------------
// 5. Class Declaration
//-----------------------------------------------------------------------------------------------------------------------------

#endif // HAL_INTERRUPTS_HPP_
