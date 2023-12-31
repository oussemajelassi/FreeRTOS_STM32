/*
 * CircularBuffer.h
 *
 *  Created on: Oct 16, 2023
 *      Author: ousjl
 */

#ifndef INC_CIRCULARBUFFER_H_
#define INC_CIRCULARBUFFER_H_

#include "memory.h"
#include "stdint.h"
/*
 * This Class is meant to instantiate its objects globally.
 */
class  CircularBuffer
{
private :
	uint32_t head_index ;
	uint32_t tail_index ;
	uint8_t * CircularBufferHandle ;
	uint32_t GetFreeSpace ;
	bool full ;

public :
	uint32_t size ;
	CircularBuffer ( uint32_t size )   ;
	uint8_t read_item( void ) ;
	void insert_item ( uint8_t item ) ;
	void reset ( void ) ;
	uint8_t * GetBuff ( void ) ;
	~CircularBuffer (void) ;


};
#endif /* INC_CIRCULARBUFFER_H_ */
