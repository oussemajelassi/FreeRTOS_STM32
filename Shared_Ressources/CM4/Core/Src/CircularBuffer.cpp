/*
 * CircularBuffer.cpp
 *
 *  Created on: Oct 16, 2023
 *      Author: ousjl
 */


#include "CircularBuffer.h"
#include "FreeRTOS.h"
#include "portable.h"

CircularBuffer::CircularBuffer (uint32_t size )
{
	uint8_t * tmp = static_cast <uint8_t *> ( pvPortMalloc(size * sizeof(uint8_t ) ) ) ;
	if ( tmp  )
	{
		this->CircularBufferHandle = tmp ;
		this->reset() ;
	}

	else
	{
		this->CircularBufferHandle = NULL ;
	}
}

void CircularBuffer::insert_item(uint8_t item )
{
	if ( this->GetFreeSpace() )
	{
		* ( this->CircularBufferHandle + this->head_index ) = item ;
		this->head_index ++ ;
		if ( this->head_index == this->size ) this->full = true ;
	}
	else
	{
		this->head_index = 0 ;
		this->insert_item(item) ;
	}
}

uint32_t CircularBuffer::GetFreeSpace()
{
	return ( this->size - this->head_index ) ;
}

/*
 * Brief : This Function suppose that user have already checked that there is unseen messages
 *
 */
uint8_t CircularBuffer::read_item (void)
{
	uint8_t tmp = * ( this->CircularBufferHandle + this->tail_index ) ;
	this->tail_index ++ ;
	this->tail_index = this->head_index % this->size ;
	return ( tmp ) ;
}

CircularBuffer::~CircularBuffer()
{
	vPortFree(CircularBufferHandle) ;
}

void CircularBuffer::reset ( void )
{
	this->head_index = 0 ;
	this->tail_index = 0 ;
}