/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
//comment
#include "board.h"
#include "fsl_gpio.h"

#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define EXAMPLE_LED_GPIO BOARD_USER_LED_GPIO
#define EXAMPLE_LED_GPIO_PIN BOARD_USER_LED_GPIO_PIN


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t g_systickCounter;
volatile uint32_t millis_cnt = 0;
/* The PIN status */
volatile bool g_pinSet = false;


/*******************************************************************************
 * Code
 ******************************************************************************/
void SysTick_Handler(void)
{
    if (g_systickCounter != 0U)
    { 
        g_systickCounter--;
    }
    millis_cnt++;
}

uint32_t millis(void)
{
	return millis_cnt;
}

void SysTick_DelayTicks(uint32_t n)
{
    g_systickCounter = n;
    while(g_systickCounter != 0U)
    {
    }
}

/*!
 * @brief Main function
 */

uint32_t led_timer = 0;
uint32_t print_timer = 0;

int main(void)
{
    /* Define the init structure for the output LED pin*/
    gpio_pin_config_t led_config = {kGPIO_DigitalOutput, 0, kGPIO_NoIntmode};

    /* Board pin init */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    /* Init output LED GPIO. */
    GPIO_PinInit(EXAMPLE_LED_GPIO, EXAMPLE_LED_GPIO_PIN, &led_config);

    /* Set systick reload value to generate 1ms interrupt */
    if(SysTick_Config(SystemCoreClock / 1000U))
    {
        while(1)
        {
        }
    }

    while (1)
    {
    	if(millis() > led_timer + 2000)
		{
			led_timer = millis();
			if (g_pinSet)
			{
				GPIO_PinWrite(EXAMPLE_LED_GPIO, EXAMPLE_LED_GPIO_PIN, 0U);
				g_pinSet = false;
			}
			else
			{
				GPIO_PinWrite(EXAMPLE_LED_GPIO, EXAMPLE_LED_GPIO_PIN, 1U);
				g_pinSet = true;
			}
		}
		if(millis() > print_timer + 1000)
		{
			print_timer = millis();
			PRINTF("state = %d\r\n", GPIO_PinRead(EXAMPLE_LED_GPIO, EXAMPLE_LED_GPIO_PIN));
		}
    }
}
