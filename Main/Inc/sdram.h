#ifndef __SDRAM_H
#define __SDRAM_H

#include "stm32f4xx_hal.h"


/* DEFINES */
#define SDRAM_DEVICE_ADDR                               ((uint32_t) 0xD0000000)
#define SDRAM_DEVICE_SIZE                               ((uint32_t) 0x800000)  /* SDRAM device size in MBytes */
#define REFRESH_COUNT                                   ((uint32_t) 0x0569)   /* SDRAM refresh counter (90Mhz SD clock) */
#define SDRAM_MODEREG_BURST_LENGTH_1                    ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2                    ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4                    ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8                    ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED            ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2                     ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3                     ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD           ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED        ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE            ((uint16_t)0x0200)

/* VARIABLES */
static SDRAM_HandleTypeDef hsdram;
static FMC_SDRAM_TimingTypeDef tsdram;
static FMC_SDRAM_CommandTypeDef csdram;

/* FUNCTIONS */
void sdramInit( void );

#endif /* __SDRAM_H */
