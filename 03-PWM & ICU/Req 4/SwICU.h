/*
 * SwICU.h
 *
 *  Created on: Oct 27, 2019
 *      Author: Sprints
 */

#ifndef SWICU_H_
#define SWICU_H_

#include "interrupts.h"
#include "timers.h"
#define TRIGGER_SEND			(1)
#define TRIGGER_DONOT_SEND		(0)

#define TRIGGER_GPIO		(GPIOB)
#define TRIGGER_PIn			(BIT1)

#define ECHO_GPIO			(GPIOB)
#define ECHO_PIn			(BIT2)

volatile extern uint8 trigger_flag;
volatile extern uint32 ovf_counts;

typedef enum {
	SwICU_EdgeFalling = 0x00,
	SwICU_EdgeRisiging = 0x40
}SwICU_Edge_TypeDef;

void SwICU_Init(SwICU_Edge_TypeDef inputCaptureEdge);
SwICU_Edge_TypeDef SwICU_GetCfgEdge(void);
void SwICU_SetCfgEdge(SwICU_Edge_TypeDef edge);
void SwICU_Read(volatile uint8 * capt);
void SwICU_Stop(void);
void SwICU_Start(void);
void SwICU_Enable(void);
void SwICU_Disable(void);

#endif /* SWICU_H_ */
