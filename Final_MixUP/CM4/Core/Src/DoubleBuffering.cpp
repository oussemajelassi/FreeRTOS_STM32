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

void DoubleBuffer::Swap ( void )
{
	this->CurrentBuffer.swap(this->NextBuffer) ;
	this->NextBuffer.clear() ;
	this->CurrentBuffer.resize(this->Size) ;
	this->FreeSpace = this->Size ;
}

void DoubleBuffer::NotifyComputaionTask ( void )
{
	xTaskNotify(ComputaionTaskHandle ,  1  , eSetValueWithOverwrite ) ;
}

double DoubleBuffer::ComputeAvg( void )
{
	double Avg = 0 ;
	double tmp = 0.0;
	if ( this->CurrentBufferReady )
	{
		for ( uint8_t cnt = 0 ; cnt < this->Size ; cnt ++  )
		{
			tmp = this->CurrentBuffer[cnt] ;
			Avg += ( ( (double) this->CurrentBuffer[cnt] ) / (double) this->Size ) ;
		}
		this->CurrentBufferReady = false ;
		return ( Avg ) ;
	}
}

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
