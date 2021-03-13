/*
 * Function.h
 *
 *  Created on: 2021 Mar 11
 *      Author: kayro
 */

#ifndef FUNCTION_H_
	#define FUNCTION_H_

	#include "HardwareAccess.h"
	#include "WebCommunication.h"


	void updateDataNumericEEPROM_8(uint16_t address, uint8_t *vetor, uint8_t len);



	extern FlagsCtrlSystem flagsCtrlSystem;


	extern void readVetor_8 (int address, uint8_t *data, uint8_t length);
	extern size_t writeVetor_8 (int address, uint8_t *vetor, uint8_t length);



#endif /* FUNCTION_H_ */
