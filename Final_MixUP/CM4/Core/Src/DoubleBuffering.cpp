/*
 * DoubleBuffering.cpp
 *
 *  Created on: Nov 6, 2023
 *      Author: Flooki
 */

#include "DoubleBuffering.h"

extern TaskHandle_t ComputaionTaskHandle ;


DoubleBuffer::DoubleBuffer ( uint8_t Size )
{
	this->NextBuffer.resize(0) ;
	this->CurrentBuffer.resize(0);
	this->FreeSpace = Size ;
	this->Size = Size ;
}

DoubleBuffer::~DoubleBuffer ( void )
{
	this->NextBuffer.clear() ;
	this->CurrentBuffer.clear() ;
}

void DoubleBuffer::Swap ( void )
{
	this->CurrentBuffer.swap(this->NextBuffer) ;
	this->NextBuffer.clear() ;
	this->CurrentBuffer.resize(this->Size) ;
	this->FreeSpace = this->Size ;
}

void DoubleBuffer::NotifyComputaionTask ( void )
{
	BaseType_t pxHigherPriorityTaskWoken ;
	xTaskNotifyFromISR(ComputaionTaskHandle , 1  , eSetValueWithOverwrite , & pxHigherPriorityTaskWoken ) ;
}

double DoubleBuffer::ComputeAvg( void )
{
	double Avg = 0 ;

	for ( uint8_t cnt = 0 ; cnt < this->Size ; cnt ++  )
	{
		Avg += ( ( (double) this->CurrentBuffer[cnt] ) / (double) this->Size ) ;
	}
	this->CurrentBufferReady = false ;


	return ( Avg ) ;
}

/*
 * Brief : Insert data mechanism is also where we swap the two buffers in advance
 */

void DoubleBuffer::InsertData( int Data )
{
	if  ( this->FreeSpace )
	{
		this->NextBuffer.push_back(Data) ;
		this->FreeSpace -- ;
		if ( this->FreeSpace == 0 )
		{
			this->Swap() ;
			this->CurrentBufferReady = true ;
		}
	}
}

bool DoubleBuffer::GetCurrentBufferState( void )
{
	bool tmp = false ;
	if ( this->CurrentBufferReady ) { tmp = true ; }
	else { tmp = false ; }
	return ( tmp ) ;
}

void DoubleBuffer::SetCurrentBufferState ( bool State )
{
	this->CurrentBufferReady = State ;
}
