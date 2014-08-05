#include "stm32f4xx_hal.h"
#include "cmsis_os.h"

#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"

#include "main.h"
#include "WM.h"

#include "sdram.h"
#include "gpio.h"

xTimerHandle    TouchScreenTimer;

GUI_PID_STATE pState;
TS_StateTypeDef TsState;
TS_StateTypeDef prev_state;

static void BackgroundTask ( void );
static void BlinkyTaskGREEN ( void );
static void BlinkyTaskRED ( void );
static void basicSetupHardware( void );
static void SystemClock_Config( void );
void BSP_Pointer_Update(void);
static void vTimerCallback( xTimerHandle pxTimer );

extern WM_HWIN CreateWindow(void);

int main( void )
{
        basicSetupHardware();

        xTaskCreate( BackgroundTask, ( signed char * ) "BackgroundTask", 2048, NULL, 0, NULL );
        vTaskStartScheduler();

        while( 1 ) { }
}

static void BackgroundTask ( void ) {
  char str[100];
  /* Ports init */
  gpioInit();
  /* External SDRAM configuration */
  sdramInit();

  if ( BSP_TS_Init(240, 320) == TS_OK ) {
      HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13, GPIO_PIN_SET);
  }


  /* Ensure all priority bits are assigned as preemption priority bits. */
  /* System interrupt init*/
  /* Sets the priority grouping field */
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

  /* Launch Touchscreen Timer */
  TouchScreenTimer = xTimerCreate ("Timer", 50, pdTRUE, ( void * ) 1, vTimerCallback );

  if( TouchScreenTimer == NULL ) {
      HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, GPIO_PIN_SET);
  } else {
      if( xTimerStart( TouchScreenTimer, 0 ) != pdPASS ) {

      }
  }

  /* Activate the use of memory device feature */
  WM_SetCreateFlags(WM_CF_MEMDEV);

  GUI_Init();
  CreateWindow();

  GUI_Exec();

  xTaskCreate( BlinkyTaskGREEN, ( signed char * ) "BlinkyGREEN", configMINIMAL_STACK_SIZE, NULL, 1, NULL );
  xTaskCreate( BlinkyTaskRED, ( signed char * ) "BlinkyRED", configMINIMAL_STACK_SIZE, NULL, 1, NULL );

  while( 1 ) {
      GUI_Delay(250);
      GUI_Exec();
  }
}

static void BlinkyTaskGREEN ( void ) {

  while( 1 ) {
      vTaskDelay( 1000 / portTICK_RATE_MS );
      HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_13);
  }
}

static void BlinkyTaskRED ( void ) {
  while( 1 ) {
      vTaskDelay( 500 / portTICK_RATE_MS );
      HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_14);
  }
}

static void basicSetupHardware( void )
{
  /**
    * @brief  This function is used to initialize the HAL Library; it must be the first
    *         instruction to be executed in the main program (before to call any other
    *         HAL function)
    */
  HAL_Init();

  /* Setup STM32 system (clock, PLL and Flash configuration) */
  SystemClock_Config();
}

/**
  * @brief  SystemClock configuration
  */
static void SystemClock_Config( void )
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  __PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  /* Main PLL */
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  /* SYSCLK 180 MHz */

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  /* HCLK 180 MHz */
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  /* APB1 peripheral clocks 45 MHz */
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  /* APB2 peripheral clocks 90 MHz */
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

  /* (RCC->AHB1ENR |= (RCC_AHB1ENR_CRCEN)) */
  __CRC_CLK_ENABLE();
}

/**
  * @brief  Provide the GUI with current state of the touch screen
  * @param  None
  * @retval None
  */
void BSP_Pointer_Update(void)
{

  HAL_GPIO_TogglePin(GPIOG, GPIO_PIN_14);
  uint16_t xDiff, yDiff;

  BSP_TS_GetState(&TsState);

  pState.Pressed = TsState.TouchDetected;

  xDiff = (prev_state.X > TsState.X) ? (prev_state.X - TsState.X) : (TsState.X - prev_state.X);
  yDiff = (prev_state.Y > TsState.Y) ? (prev_state.Y - TsState.Y) : (TsState.Y - prev_state.Y);

  if( (prev_state.TouchDetected != TsState.TouchDetected ) || (xDiff > 3 )|| (yDiff > 3) ) {
          prev_state = TsState;
          pState.Layer = 0;
          pState.x = TsState.X;
          pState.y = TsState.Y;
          GUI_TOUCH_StoreStateEx(&pState);
  }
}

static void vTimerCallback( xTimerHandle pxTimer )
{
   BSP_Pointer_Update();
}

