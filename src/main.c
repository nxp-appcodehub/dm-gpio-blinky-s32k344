/*==================================================================================================
 * Project : RTD AUTOSAR 4.9
 * Platform : CORTEXM
 * Peripheral : S32K3XX
 * Dependencies : none
 *
 * Autosar Version : 4.9.0
 * Autosar Revision : ASR_REL_4_9_REV_0000
 * Autosar Conf.Variant :
 * SW Version : 7.0.1
 * Build Version : S32K3_RTD_7_0_1_D2602_ASR_REL_4_9_REV_0000_20260206
 *
 * Copyright 2020 - 2026 NXP
 *
 * 	 NXP Proprietary. This software is owned or controlled by NXP and may only be
 *   used strictly in accordance with the applicable license terms. By expressly
 *   accepting such terms or by downloading, installing, activating and/or otherwise
 *   using the software, you are agreeing that you have read, and that you agree to
 *   comply with and are bound by, such license terms. If you do not agree to be
 *   bound by the applicable license terms, then you may not retain, install,
 *   activate or otherwise use the software.
 ==================================================================================================*/

/**
 *   @file main.c
 *
 *   @addtogroup main_module main module documentation
 *   @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/*==================================================================================================
 *                                        INCLUDE FILES
 ==================================================================================================*/

#include "Mcal.h"
#include "Clock_Ip.h"
#include "OsIf.h"
#include "OsIf_Timer_System.h"
#include "OsIf_Cfg.h"
#include "Siul2_Port_Ip.h"
#include "Siul2_Dio_Ip.h"

/*==================================================================================================
 *                                      DEFINES AND MACROS
 ==================================================================================================*/

#define LED_DELAY    1000
#define LED_CONSTANT 1

/*==================================================================================================
 *                                      GLOBAL VARIABLES
 ==================================================================================================*/

volatile int exit_code = 0;

Siul2_Dio_Ip_GpioType * const LED_PORT[3] = { LED_BLUE_PORT, LED_GREEN_PORT, LED_RED_PORT };
uint8 LED_PINS[3] = { LED_BLUE_PIN, LED_GREEN_PIN, LED_RED_PIN };

/*==================================================================================================
 *                                   LOCAL FUNCTION PROTOTYPES
 ==================================================================================================*/

static void DelayMs(void);
static void TriggerLed(Siul2_Dio_Ip_GpioType * const port, uint16 pin);

/*==================================================================================================
 *                                       LOCAL FUNCTIONS
 ==================================================================================================*/

/* ============================================================================================= *
 *                                   TIMING FUNCTIONS
 * ============================================================================================= */

/**
 * @brief Blocking delay using OsIf system counter.
 * @param ms  Number of milliseconds to wait.
 */
static void DelayMs(void)
{
    uint32 cur     = OsIf_GetCounter(OSIF_COUNTER_SYSTEM);
    uint32 elapsed = 0U;
    uint32 timeout = OsIf_MicrosToTicks(LED_DELAY * 1000U, OSIF_COUNTER_SYSTEM);

    while (elapsed < timeout)
    {
        elapsed += OsIf_GetElapsed(&cur, OSIF_COUNTER_SYSTEM);
    }
}

/* ============================================================================================= *
 *                                   LED FUNCTIONS
 * ============================================================================================= */

/**
 * @brief Toggle an LED on for LED_DELAY ms then off for LED_DELAY ms.
 * @param port  Pointer to the GPIO port of the LED.
 * @param pin   Pin number of the LED.
 */
static void TriggerLed(Siul2_Dio_Ip_GpioType * const port, uint16 pin)
{
    Siul2_Dio_Ip_WritePin(port, pin, LED_CONSTANT);
    DelayMs();
    Siul2_Dio_Ip_WritePin(port, pin, !LED_CONSTANT);
    DelayMs();
}

/*==================================================================================================
 *                                       MAIN FUNCTION
 ==================================================================================================*/

/*!
 * \brief The main function for the project.
 * \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
 */
int main(void)
{
    /* ------------------------------------------------------------------
     *  HARDWARE INITIALISATION
     * ------------------------------------------------------------------ */

    Clock_Ip_Init(Clock_Ip_aClockConfig);
    OsIf_Init(NULL_PTR);
    OsIf_Timer_System_Init();
    Siul2_Port_Ip_Init(
            NUM_OF_CONFIGURED_PINS_PortContainer_0_BOARD_InitPeripherals,
            g_pin_mux_InitConfigArr_PortContainer_0_BOARD_InitPeripherals);

    /* ------------------------------------------------------------------
     *  SAFE INITIAL STATE
     * ------------------------------------------------------------------ */

    Siul2_Dio_Ip_ClearPins(LED_BLUE_PORT,  LED_BLUE_PIN);
    Siul2_Dio_Ip_ClearPins(LED_RED_PORT,   LED_RED_PIN);
    Siul2_Dio_Ip_ClearPins(LED_GREEN_PORT, LED_GREEN_PIN);

    /* ------------------------------------------------------------------
     *  NORMAL OPERATION LOOP
     * ------------------------------------------------------------------ */

    while (1)
    {
        int i;

        for (i = 0; i < 3; i++)
        {
            TriggerLed(LED_PORT[i], LED_PINS[i]);
        }
    }

    return exit_code;
}

#ifdef __cplusplus
}
#endif

/** @} */
