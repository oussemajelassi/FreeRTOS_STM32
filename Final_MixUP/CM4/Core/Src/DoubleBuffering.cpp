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
	this->CurrentBuffer.resize(Size) ;
	this->NextBuffer.resize(Size) ;
	this->FreeSpace = Size ;
	this->Size = Size ;
}

void DoubleBuffer::Swap ( void )
{
	this->CurrentBuffer.swap(NextBuffer) ;
	this->CurrentBuffer.clear() ;
	this->FreeSpace = this->Size ;
}

void DoubleBuffer::NotifyComputaionTask ( void )
{
	xTaskNotify(ComputaionTaskHandle ,  1  , eSetValueWithOverwrite ) ;
}

int DoubleBuffer::ComputeAvg( void )
{
	int Avg = 0 ;
	for ( uint8_t cnt = 0 ; cnt <  (this->CurrentBuffer.size()) ; cnt ++  )
	{
		Avg += ( ( this->CurrentBuffer[cnt] ) / this->CurrentBuffer.size() ) ;
	}
	return ( Avg ) ;
}

void DoubleBuffer::InsertData( int Data )
{
	if  ( this->FreeSpace )
	{
		this->NextBuffer.push_back(Data) ;
		this->FreeSpace -- ;

	}
	else
	{
		this->CurrentBuffer.swap(NextBuffer) ;
		this->NextBuffer.clear() ;
		this->FreeSpace = this->Size ;
		this->InsertData(Data) ;

	}
}
