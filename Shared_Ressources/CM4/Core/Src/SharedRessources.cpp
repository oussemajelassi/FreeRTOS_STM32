/*
 * SharedRessources.cpp
 *
 *  Created on: Oct 16, 2023
 *      Author: ousjl
 */

#include "SharedRessources.h"
#include "stm32h7xx.h"

/*** Static Attribute ***/

uint8_t Producer::sProducedId_u8 = 0 ;

/*** Methods Producer **/

Producer::Producer(GPIO_TypeDef *GPIOx , uint16_t GPIO_Pin)
{
	this->GPIO_Pin = GPIO_Pin ;
	this->GPIOx = GPIOx ;
	this->ProductId_u8 = this->sProducedId_u8 ;
	this->sProducedId_u8 ++ ;
	HAL_GPIO_WritePin(GPIOx , this->GPIO_Pin , GPIO_PIN_SET ) ;
}

Producer::~Producer(void)
{
	HAL_GPIO_WritePin(this->GPIOx , this->GPIO_Pin , GPIO_PIN_RESET ) ;
}

void Producer::InsertItem( CircularBuffer buff )
{
	buff.insert_item(this->ProductId_u8) ;
}
