/*
 *  queue.h
 *
 *  Created on: Dec 25, 2020
 *      Author: Truong VV
 */

#ifndef _QUEUE_H_
#define _QUEUE_H

#include "stm32f10x.h"

/******************************************************************************/
/*                              DEFINE                                      */
/******************************************************************************/
typedef struct QUEUEx_t
{
    uint8_t *p;
    uint8_t  element_size;
    uint8_t  queue_size;
    uint8_t  count;
    uint16_t in;
    uint16_t out;
} QUEUEx_t;

/******************************************************************************/
/*                              FUNCTION                                      */
/******************************************************************************/
void  QUEUE_Init(QUEUEx_t *q,uint8_t* buffer,uint8_t queue_size, uint8_t element_size);
uint8_t    QUEUE_Push(QUEUEx_t *q,uint8_t* data);
uint8_t    QUEUE_Get(QUEUEx_t *q, uint8_t* buffer);
uint8_t    QUEUE_Empty(QUEUEx_t *q);
uint8_t    QUEUE_Count(QUEUEx_t *q);

#endif /*_QUEUE_H*/