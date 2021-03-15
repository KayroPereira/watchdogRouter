/*
 * Function.h
 *
 *  Created on: 2021 Mar 11
 *      Author: kayro
 */

#ifndef FUNCTIONS_H_
	#define FUNCTIONS_H_

	#include "HardwareAccess.h"
	#include "WebCommunication.h"


	void copyVetor_8(uint8_t* v1,  uint8_t* v2, uint8_t len);
	void updateDataNumericEEPROM_8(uint16_t address, uint8_t *vetor, uint8_t len);



	extern FlagsCtrlSystem flagsCtrlSystem;
	extern FlagsFirebase flagsFirebase;
	extern FirebaseData fbdo2;
	extern FirebaseJson jsonBuffer;
	extern uint8_t datasFirebaseNumeric[];
	extern uint8_t datasFirebaseNumericLocal[];
	extern int	datasFirebaseNumericSend[];
	extern uint8_t regDelayHardReset;
	extern uint8_t regBaseDelayError;
	extern uint8_t regDelayError;


	extern void loadVariablesFirebase();
	extern void readVetor_8 (int address, uint8_t *data, uint8_t length);
	extern size_t writeVetor_8 (int address, uint8_t *vetor, uint8_t length);
	extern void swapPin(int8_t pin);
	extern void changeOutput(uint8_t pin, uint8_t status);
	extern void addDataBufferFirebase(const String path, int value);
	extern bool updateFirebase(FirebaseData &fbdo, String path, FirebaseJson &json);



#endif /* FUNCTIONS_H_ */
