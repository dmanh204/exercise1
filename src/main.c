/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "diag/trace.h"
#include "stm32f4xx.h"
// ----------------------------------------------------------------------------
//
// Standalone STM32F4 empty sample (trace via DEBUG).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace-impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

// Private variables
UART_HandleTypeDef huart2;
ADC_HandleTypeDef hadc1;
// Private function prototype
static void MX_ADC1_Init(void);

int
main(int argc, char* argv[])
{
  // At this stage the system clock should have already been configured
  // at high speed.
	HAL_Init();
	//Nucleo_BSP_Init();
	/* Initialize all configured peripherals*/
	MX_ADC1_Init();
	HAL_ADC_Start(&hadc1);

  // Infinite loop
  while (1)
    {
       // Add your code here.
	  char msg[20];
	  uint16_t rawValue;
	  float temp;

	  HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);

	  rawValue = HAL_ADC_GetValue(&hadc1);
	  temp = ((float)rawValue)/4095*3000;
	  temp = ((temp - 760.0)/2.5) +25;

	  sprintf(msg, "rawValue: %hu\r\n", rawValue);
	  HAL_UART_Transmit(&huart2, (uint8_t*) msg, strlen(msg), HAL_MAX_DELAY);

	  sprintf(msg, "Temperature: %f\r\n", temp);
	  HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
    }
}
// ADC1 initialize function
void MX_ADC1_Init()
{
	ADC_ChannelConfTypeDef sConfig;
	// Enable ADC peripheral
	__HAL_RCC_ADC1_CLK_ENABLE();
	// Configure the global feature of ADC (Clock, Resolution, Data Alignment, number conversion)
	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.ScanConvMode = DISABLE;
	hadc1.Init.ContinuousConvMode = ENABLE;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DMAContinuousRequests = DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
	HAL_ADC_Init(&hadc1);

	sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);
}


#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
