/**
 * @file
 * @brief Library to handle LED blinking
 */

#include "led.h"

/**
 * Tick count, when activity LED was last switched on
 */
static uint32_t activity_led_last_on = 0xFF;
static uint32_t error_led_last_on = 0xFF;


void led_init()
{
    GPIO_InitTypeDef GPIO_InitStruct;

    #ifdef LED_POWER_ENABLED
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = 0;
    GPIO_InitStruct.Pin = LED_POWER_PIN;
    HAL_GPIO_Init(LED_POWER_PORT, &GPIO_InitStruct);
    led_on(LED_POWER);
    #endif

    #ifdef LED_ACTIVITY_ENABLED
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = 0;
    GPIO_InitStruct.Pin = LED_ACTIVITY_PIN;
    HAL_GPIO_Init(LED_ACTIVITY_PORT, &GPIO_InitStruct);
    #endif

    #ifdef LED_ERROR_ENABLED
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct.Alternate = 0;
    GPIO_InitStruct.Pin = LED_ERROR_PIN;
    HAL_GPIO_Init(LED_ERROR_PORT, &GPIO_InitStruct);
    #endif
}


void led_on(led_index_t led)
{
    if (led == LED_POWER)
    {
        HAL_GPIO_WritePin(LED_POWER_PORT, LED_POWER_PIN, GPIO_PIN_SET);
        return;
    }

	// Make sure the LED has been off for at least LED_DURATION before turning on again.
	// This prevents a solid status LED on a busy canbus.
	if ((led == LED_ACTIVITY)
	 && (HAL_GetTick() - activity_led_last_on > 2*LED_ON_DURATION))
	{
		HAL_GPIO_WritePin(LED_ACTIVITY_PORT, LED_ACTIVITY_PIN, GPIO_PIN_RESET);
		activity_led_last_on = HAL_GetTick();
	}

	if ((led == LED_ERROR)
	 && (HAL_GetTick() - error_led_last_on > 2*LED_ON_DURATION))
	{
        HAL_GPIO_WritePin(LED_ERROR_PORT, LED_ERROR_PIN, GPIO_PIN_SET);
        error_led_last_on = HAL_GetTick();
	}
}


void led_off(led_index_t led)
{
    if (led == LED_POWER)
    {
        HAL_GPIO_WritePin(LED_POWER_PORT, LED_POWER_PIN, GPIO_PIN_RESET);
        return;
    }
    if (led == LED_ACTIVITY)
    {
        HAL_GPIO_WritePin(LED_ACTIVITY_PORT, LED_ACTIVITY_PIN, GPIO_PIN_SET);
        return;
    }
    if (led == LED_ERROR)
    {
        HAL_GPIO_WritePin(LED_ERROR_PORT, LED_ERROR_PIN, GPIO_PIN_RESET);
    }
}


void led_process()
{
	if ((HAL_GPIO_ReadPin(LED_ACTIVITY_PORT, LED_ACTIVITY_PIN) == GPIO_PIN_RESET)
     && (HAL_GetTick() - activity_led_last_on > LED_ON_DURATION))
	{
		led_off(LED_ACTIVITY);
	}

    if ((HAL_GPIO_ReadPin(LED_ERROR_PORT, LED_ERROR_PIN) == GPIO_PIN_SET)
     && (HAL_GetTick() - error_led_last_on > LED_ON_DURATION))
    {
        led_off(LED_ERROR);
    }
}
