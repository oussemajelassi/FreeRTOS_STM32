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
	uint32_t size ;
	uint8_t head_index ;
	uint8_t tail_index ;
	uint8_t * CircularBufferHandle ;
	bool full ;

public :
	CircularBuffer ( uint32_t size )   ;
	uint8_t read_item( void ) ;
	void insert_item ( uint8_t item ) ;
	uint32_t GetFreeSpace ( void ) ;
	void reset ( void ) ;
	uint8_t * GetBuff ( void ) ;
	~CircularBuffer (void) ;


};
#endif /* INC_CIRCULARBUFFER_H_ */
