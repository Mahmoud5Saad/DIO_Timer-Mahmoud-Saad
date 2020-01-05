/*
 * sevenSeg_BCD.h
 *
 *  Created on: Oct 24, 2019
 *      Author: Sprints
 */

#ifndef SEVENSEG_BCD_H_
#define SEVENSEG_BCD_H_

#include "gpio.h"
#include "sevenSeg_BCD_CFG.h"


typedef enum En_SevenSegId_t{
	SEG_0,
	SEG_1,
}En_SevenSegId_t;

/**
 * Description: 
 * @param 
 */
void sevenSeg_Init(void);

/**
 * Description: 
 * @param 
 */
void sevenSeg_On(En_SevenSegId_t segment);

/**
 * Description: 
 * @param 
 */
void sevenSeg_Off(En_SevenSegId_t segment);

/**
 * Description: 
 * @param 
 */
void sevenSeg_Write(En_SevenSegId_t segment, uint8 number);


#endif /* SEVENSEG_BCD_H_ */
