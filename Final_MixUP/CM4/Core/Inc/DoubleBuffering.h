/*
 * DoubleBuffering.h
 *
 *  Created on: Nov 6, 2023
 *      Author: Flooki
 */

#ifndef INC_DOUBLEBUFFERING_H_
#define INC_DOUBLEBUFFERING_H_

#include <vector>
#include "stdint.h"
#include"FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

class DoubleBuffer
{
private :
	uint8_t FreeSpace ;
	std::vector <int> CurrentBuffer ;
	std::vector <int> NextBuffer ;
public :
	DoubleBuffer ( uint8_t Size ) ;
	~DoubleBuffer ( void ) ;
	void Swap() ;
	void NotifyComputaionTask ( void ) ;
	int ComputeAvg ( void )  ;
	void InsertData ( int Data ) ;
};


#endif /* INC_DOUBLEBUFFERING_H_ */
