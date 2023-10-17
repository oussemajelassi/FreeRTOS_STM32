/*
 * CircularBuffer.cpp
 *
 *  Created on: Oct 16, 2023
 *      Author: ousjl
 */


#include "CircularBuffer.h"
#include "FreeRTOS.h"
#include "portable.h"
#include "stdlib.h"

CircularBuffer::CircularBuffer (uint32_t size )
{
	uint8_t * tmp = static_cast <uint8_t *> ( malloc(size * sizeof(uint8_t ) ) ) ;
	if ( tmp  )
	{
		this->CircularBufferHandle = tmp ;
		this->size = size ;
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
		if ( this->head_index == this->size ) { this->full = true ; }
		else { this->full =false ; }
	}
	else
	{
		this->tail_index ++ ;
		this->tail_index = this->tail_index % this->size ;
		this->head_index = 0 ;
		this->insert_item(item) ;
	}
}
/*
 * TODO : this is not correct sometimes head will get bacj to 0 !!!!
 */
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
	this->tail_index = this->tail_index % this->size ;
	return ( tmp ) ;
}

CircularBuffer::~CircularBuffer()
{
	free(CircularBufferHandle) ;
}

void CircularBuffer::reset ( void )
{
	this->head_index = 0 ;
	this->tail_index = 0 ;
}
