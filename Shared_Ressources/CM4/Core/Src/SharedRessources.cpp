/*
 * SharedRessources.cpp
 *
 *  Created on: Oct 16, 2023
 *      Author: ousjl
 */

#include "SharedRessources.h"
#include "stm32h7xx.h"

/*** Static Attributes ***/

uint8_t Producer::sProducedId_u8 = 0 ;
uint8_t Consumer::sConsumerID_u8 = 0 ;

/*** Methods Producer **/

Producer::Producer(GPIO_TypeDef *GPIOx , uint16_t GPIO_Pin , uint8_t ProducedQuantity_u8)
{
	this->GPIO_Pin = GPIO_Pin ;
	this->GPIOx = GPIOx ;
	this-> ProducedQuantity_u8 = ProducedQuantity_u8  ;
	this->ProductId_u8 = this->sProducedId_u8 ;
	this->sProducedId_u8 ++ ;
	HAL_GPIO_WritePin(GPIOx , this->GPIO_Pin , GPIO_PIN_SET ) ;
}

Producer::~Producer(void)
{
	HAL_GPIO_WritePin(this->GPIOx , this->GPIO_Pin , GPIO_PIN_RESET ) ;
	this->sProducedId_u8 -- ;
}

void Producer::InsertItem( CircularBuffer * buff )
{
	buff->insert_item(this->ProductId_u8) ;
}

/*** Methods Consumer **/

Consumer::Consumer(uint8_t NeededQuantity_u8)
{
	this->ConsumerID_u8 =  this->sConsumerID_u8  ;
	this->sConsumerID_u8 ++ ;
	this->NeededQuantity_u8 = NeededQuantity_u8  ;
}

Consumer::~Consumer()
{
	this->sConsumerID_u8 -- ;
}

uint8_t Consumer::RetrieveItem(CircularBuffer* buff)
{
	return  ( buff->read_item() ) ;

}

