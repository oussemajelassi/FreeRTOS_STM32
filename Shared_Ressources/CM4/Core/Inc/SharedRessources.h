/*
 * SharedRessources.h
 *
 *  Created on: Oct 16, 2023
 *      Author: ousjl
 */

#ifndef INC_SHAREDRESSOURCES_H_
#define INC_SHAREDRESSOURCES_H_

#include "CircularBuffer.h"
#include "stm32h7xx.h"
class Producer
{
private :
	uint16_t GPIO_Pin ;
	GPIO_TypeDef * GPIOx ;
	uint8_t ProductId_u8 ;
public :
	uint8_t ProducedQuantity_u8 ;
	static uint8_t sProducedId_u8  ;
	Producer(GPIO_TypeDef *GPIOx , uint16_t GPIO_Pin , uint8_t ProducedQuantity_u8 )  ;
	~Producer(void) ;
	void InsertItem ( CircularBuffer* buff ) ;

};

class Consumer
{
private :

	uint8_t ConsumerID_u8 ;
public :
	uint8_t NeededQuantity_u8 ;
	static uint8_t sConsumerID_u8  ;
	Consumer(uint8_t NeededQuantity_u8) ;
	~Consumer(void);
	uint8_t RetrieveItem ( CircularBuffer* buff ) ;
};


#endif /* INC_SHAREDRESSOURCES_H_ */
