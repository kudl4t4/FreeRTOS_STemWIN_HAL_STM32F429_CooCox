#include "gpio.h"

void gpioInit( void ) {

    /*## Configure the SDRAM device #########################################*/
    /* Enable FMC clock */
      __FMC_CLK_ENABLE();

      /* Enable GPIOs clock */
      __GPIOB_CLK_ENABLE();
      __GPIOC_CLK_ENABLE();
      __GPIOD_CLK_ENABLE();
      __GPIOE_CLK_ENABLE();
      __GPIOF_CLK_ENABLE();
      __GPIOG_CLK_ENABLE();

    /*-- GPIOs Configuration -----------------------------------------------------*/
    /*
     +-------------------+--------------------+--------------------+--------------------+
     +                       SDRAM pins assignment                                      +
     +-------------------+--------------------+--------------------+--------------------+
     | PD0  <-> FMC_D2   | PE0  <-> FMC_NBL0  | PF0  <-> FMC_A0    | PG0  <-> FMC_A10   |
     | PD1  <-> FMC_D3   | PE1  <-> FMC_NBL1  | PF1  <-> FMC_A1    | PG1  <-> FMC_A11   |
     | PD8  <-> FMC_D13  | PE7  <-> FMC_D4    | PF2  <-> FMC_A2    | PG8  <-> FMC_SDCLK |
     | PD9  <-> FMC_D14  | PE8  <-> FMC_D5    | PF3  <-> FMC_A3    | PG15 <-> FMC_NCAS  |
     | PD10 <-> FMC_D15  | PE9  <-> FMC_D6    | PF4  <-> FMC_A4    |--------------------+
     | PD14 <-> FMC_D0   | PE10 <-> FMC_D7    | PF5  <-> FMC_A5    |
     | PD15 <-> FMC_D1   | PE11 <-> FMC_D8    | PF11 <-> FMC_NRAS  |
     +-------------------| PE12 <-> FMC_D9    | PF12 <-> FMC_A6    |
                         | PE13 <-> FMC_D10   | PF13 <-> FMC_A7    |
                         | PE14 <-> FMC_D11   | PF14 <-> FMC_A8    |
                         | PE15 <-> FMC_D12   | PF15 <-> FMC_A9    |
     +-------------------+--------------------+--------------------+
     | PB5 <-> FMC_SDCKE1|
     | PB6 <-> FMC_SDNE1 |
     | PC0 <-> FMC_SDNWE |
     +-------------------+

    */

      /* Common GPIO configuration */
      GPIO_InitStructure.Mode  = GPIO_MODE_AF_PP;
      GPIO_InitStructure.Speed = GPIO_SPEED_FAST;
      GPIO_InitStructure.Pull  = GPIO_NOPULL;
      GPIO_InitStructure.Alternate = GPIO_AF12_FMC;

      /* GPIOB configuration */
      GPIO_InitStructure.Pin = GPIO_PIN_5 | GPIO_PIN_6;
      HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

      /* GPIOC configuration */
      GPIO_InitStructure.Pin = GPIO_PIN_0;
      HAL_GPIO_Init(GPIOC, &GPIO_InitStructure);

      /* GPIOD configuration */
      GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1  | GPIO_PIN_8 |
                               GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_14 |
                               GPIO_PIN_15;
      HAL_GPIO_Init(GPIOD, &GPIO_InitStructure);

      /* GPIOE configuration */
      GPIO_InitStructure.Pin = GPIO_PIN_0  | GPIO_PIN_1  | GPIO_PIN_7 |
                               GPIO_PIN_8  | GPIO_PIN_9  | GPIO_PIN_10 |
                               GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 |
                               GPIO_PIN_14 | GPIO_PIN_15;
      HAL_GPIO_Init(GPIOE, &GPIO_InitStructure);

      /* GPIOF configuration */
      GPIO_InitStructure.Pin = GPIO_PIN_0  | GPIO_PIN_1 | GPIO_PIN_2 |
                               GPIO_PIN_3  | GPIO_PIN_4 | GPIO_PIN_5 |
                               GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 |
                               GPIO_PIN_14 | GPIO_PIN_15;
      HAL_GPIO_Init(GPIOF, &GPIO_InitStructure);

      /* GPIOG configuration */
      GPIO_InitStructure.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 |
                               GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_15;
      HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

      /*## Configure the LEDs device #########################################*/
      /* Configure the GPIO_LED pin */
      GPIO_InitStructure.Pin = GPIO_PIN_13 | GPIO_PIN_14;
      GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
      GPIO_InitStructure.Pull = GPIO_PULLUP;
      GPIO_InitStructure.Speed = GPIO_SPEED_FAST;

      HAL_GPIO_Init(GPIOG, &GPIO_InitStructure);

      HAL_GPIO_WritePin(GPIOG, GPIO_PIN_13 | GPIO_PIN_14, GPIO_PIN_RESET);
}
