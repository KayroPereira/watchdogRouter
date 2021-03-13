/*
 * Functions.c
 *
 *  Created on: 2021 Mar 11
 *      Author: kayro
 */

#include "Function.h"

void loadVariables(){

	flagsCtrlSystem.flgByte = 0;
	flagsCommunication.flgByte = 0;

//	flagsCtrlGeral.flagsCtrlGeralByte = 0;
//	flagsCommunication.flgByte = 0;
//
//	flagsCommunication.flgBit.flgFirebaseFirsComm = 0;										//inicializa com falha de comnucação no firebase
//	flagsCtrlGeral.flagsCtrlGeralBit.flgStatusLedError	= 0;
//
//
//	flagsCtrlGeral.flagsCtrlGeralBit.flgErrorFlagsEn = 1;									//clear error conditions initially
//
//	flagsCtrlGeral.flagsCtrlGeralBit.flgDebounceSwitchEn = 1;								//defines initial button status conditions
//	flagsCtrlGeral.flagsCtrlGeralBit.flgSwitchInputStatus = 1;
//	flagsCtrlGeral.flagsCtrlGeralBit.flgSwitchInputTransition = 1;
//
//	flagsCommunication.flgBit.flgGetFirebaseDelay = 1;
//
//	flagsCommunication.flgBit.flgFailureFirebase = 1;										//Inicializa com falha na comunicação com o Firebase
//
//
//	flagsCtrlGeral.flagsCtrlGeralBit.flgClearCountWater = 1;
//
//	flagsCtrlGeral.flagsCtrlGeralBit.flgVoltageSensorTransition = 1;						//Evita que o acionamento da boia não seja detecnada na inicialização


//	configVariableControlCommunication(false);												//define os flags de controle da comunicação com falha
//
//	copyVetorInt(datasFirebaseLocal, datasFirebase, LENGTH_PATH);							//Iguala os dados dos vetores Local <-> Firebase - O vetor Firebase é iniciado com dados padrão

//	loadVariablesControl();
}

bool getDataDefaultEEPROM_8(uint16_t address, uint8_t *vetor, uint8_t len){

	readVetor_8(address, vetor, len);

	return vetor[len-1] == 0x0AA ? true : false;
}

//bool downloadDataEEPROM_8(uint16_t address, uint8_t len, uint8_t &back){
bool downloadDataEEPROM_8(uint8_t address, uint8_t len, uint8_t *back){

//	uint8_t dataTemp[LENGTH_DATA_EEPROM];
	uint8_t dataTemp[len];

	if(getDataDefaultEEPROM_8(address, dataTemp, len)){

		for(int i = 0; i < len-1; i++){														//Descarta o último dado, devido ser apenas de verificação
			*(back+i) = *(dataTemp+i);
			Serial.println("-------");
			Serial.println(*(back+i));
			Serial.println(*(dataTemp+i));
			Serial.println("-------");
//			datasFirebaseLocal[LIST_DATA_EEPROM[i]] = dataTemp[i];
		}

		return true;
	}else{
		return false;
	}
}
