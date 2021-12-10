/*
 *  queue.c
 *
 *  Created on: Dec 25, 2020
 *      Author: Truong VV
 */

#include "queue.h"
#include <string.h>
/******************************************************************************/
/*                              FUNCTION                                      */
/******************************************************************************/

 /*!
 * @brief functions QUEUE_Init.
 *
 */
void QUEUE_Init(QUEUEx_t *q, uint8_t* buffer, uint8_t queue_size, uint8_t element_size)
{
    q->in = 0;
    q->out = 0;
    q->count = 0;
    q->element_size = element_size;
    q->queue_size = queue_size;
    q->p = buffer;
}

 /*!
 * @brief functions QUEUE_Push.
 *
 */
uint8_t QUEUE_Push(QUEUEx_t *q, uint8_t* data)
{
    if(QUEUE_Count(q)>=q->queue_size)
    {       
        if(q->out>=((q->element_size)*(q->queue_size)))
        {
            q->out=0;
        }

        q->out+=q->element_size;        
        q->count--;
    }

    if(q->in>=((q->element_size)*(q->queue_size)))
    {
        q->in=0;
    }

    memcpy((uint8_t*)&(q->p[q->in]),(uint8_t*)data,q->element_size);
    q->in +=q->element_size;

    q->count++;

    return 1;
}

 /*!
 * @brief functions QUEUE_Get.
 *
 */
uint8_t QUEUE_Get(QUEUEx_t *q, uint8_t* buffer)
{
    if(QUEUE_Count(q))
    {
        if(q->out>=((q->element_size)*(q->queue_size)))
        {
            q->out=0;
        }

        memcpy((uint8_t*)buffer,(uint8_t*)&(q->p[q->out]),q->element_size);
        q->out+=q->element_size;        
        q->count--;

        return 1;
    }

    return 0;
}

 /*!
 * @brief functions QUEUE_Empty.
 *
 */
uint8_t QUEUE_Empty(QUEUEx_t *q)
{
    if(q->count)
    {
        return 0;
    }
    return 1;
}

 /*!
 * @brief functions QUEUE_Count.
 *
 */
uint8_t QUEUE_Count(QUEUEx_t *q)
{
    return q->count;
}