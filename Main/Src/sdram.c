#include "sdram.h"

void sdramInit( void )
{

  __IO uint32_t tmpmrd =0;

  /*##-1- Configure the SDRAM device #########################################*/
  /* SDRAM device configuration */
  hsdram.Instance = FMC_SDRAM_DEVICE;

  /* FMC Configuration -------------------------------------------------------*/
    /* FMC SDRAM Bank configuration */
    /* Timing configuration for 90 Mhz of SD clock frequency (180Mhz/2) */
    /* TMRD: 2 Clock cycles */
  tsdram.LoadToActiveDelay    = 2;
    /* TXSR: min=70ns (7x11.11ns) */
  tsdram.ExitSelfRefreshDelay = 7;
    /* TRAS: min=42ns (4x11.11ns) max=120k (ns) */
  tsdram.SelfRefreshTime      = 4;
    /* TRC:  min=70 (7x11.11ns) */
  tsdram.RowCycleDelay        = 7;
    /* TWR:  min=1+ 7ns (1+1x11.11ns) */
  tsdram.WriteRecoveryTime    = 2;
    /* TRP:  20ns => 2x11.11ns*/
  tsdram.RPDelay              = 2;
    /* TRCD: 20ns => 2x11.11ns */
  tsdram.RCDDelay             = 2;

    /* FMC SDRAM control configuration */
  hsdram.Init.SDBank             = FMC_SDRAM_BANK2;
    /* Row addressing: [7:0] */
  hsdram.Init.ColumnBitsNumber   = FMC_SDRAM_COLUMN_BITS_NUM_8;
    /* Column addressing: [11:0] */
  hsdram.Init.RowBitsNumber      = FMC_SDRAM_ROW_BITS_NUM_12;
  hsdram.Init.MemoryDataWidth    = FMC_SDRAM_MEM_BUS_WIDTH_16;
  hsdram.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram.Init.CASLatency         = FMC_SDRAM_CAS_LATENCY_3;
  hsdram.Init.WriteProtection    = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram.Init.SDClockPeriod      = FMC_SDRAM_CLOCK_PERIOD_2;
  hsdram.Init.ReadBurst          = FMC_SDRAM_RBURST_DISABLE;
  hsdram.Init.ReadPipeDelay      = FMC_SDRAM_RPIPE_DELAY_1;

  HAL_SDRAM_Init(&hsdram, &tsdram);

  /* Program the SDRAM external device */
  csdram.CommandMode             = FMC_SDRAM_CMD_CLK_ENABLE;
  csdram.CommandTarget           = FMC_SDRAM_CMD_TARGET_BANK2;
  csdram.AutoRefreshNumber       = 1;
  csdram.ModeRegisterDefinition  = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(&hsdram, &csdram, 0x1000);

  /* Configure a PALL (precharge all) command */
  csdram.CommandMode             = FMC_SDRAM_CMD_PALL;
  csdram.CommandTarget           = FMC_SDRAM_CMD_TARGET_BANK2;
  csdram.AutoRefreshNumber       = 1;
  csdram.ModeRegisterDefinition  = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(&hsdram, &csdram, 0x1000);

  /* Configure a Auto-Refresh command */
  csdram.CommandMode             = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
  csdram.CommandTarget           = FMC_SDRAM_CMD_TARGET_BANK2;
  csdram.AutoRefreshNumber       = 4;
  csdram.ModeRegisterDefinition  = 0;

  /* Send the command */
  HAL_SDRAM_SendCommand(&hsdram, &csdram, 0x1000);

  /* Program the external memory mode register */
  tmpmrd = (uint32_t)   SDRAM_MODEREG_BURST_LENGTH_2            |
                        SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL     |
                        SDRAM_MODEREG_CAS_LATENCY_3             |
                        SDRAM_MODEREG_OPERATING_MODE_STANDARD   |
                        SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

  csdram.CommandMode             = FMC_SDRAM_CMD_LOAD_MODE;
  csdram.CommandTarget           = FMC_SDRAM_CMD_TARGET_BANK2;
  csdram.AutoRefreshNumber       = 1;
  csdram.ModeRegisterDefinition  = tmpmrd;

  /* Send the command */
  HAL_SDRAM_SendCommand(&hsdram, &csdram, 0x1000);

  /* Set the refresh rate counter */
  /* (15.62 us x Freq) - 20 */
  /* Set the device refresh counter */
  HAL_SDRAM_ProgramRefreshRate(&hsdram, REFRESH_COUNT );
}
