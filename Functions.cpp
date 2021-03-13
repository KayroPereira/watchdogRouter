/*
 * Functions.c
 *
 *  Created on: 2021 Mar 11
 *      Author: kayro
 */

#include "Function.h"

int 	errorOld = -1;

void loadVariables(){

	flagsCtrlSystem.flgByte = 0;
	flagsCommunication.flgByte = 0;

	loadVariablesCommunication();

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
	copyVetor_8(datasFirebaseNumericLocal, datasFirebaseNumeric, LENGTH_DATA_EEPROM_NUMERIC);		//Iguala os dados dos vetores Local <-> Firebase - O vetor Firebase é iniciado com dados padrão

//	loadVariablesControl();

	//TODO - Remover usado para teste inicial
//	uint8_t startTemp[LENGTH_DATA_EEPROM_NUMERIC] = {12, 14, 16, 18};
//	updateDataNumericEEPROM_8(ADDRESS_DATA_EEPROM_NUMERIC, startTemp, LENGTH_DATA_EEPROM_NUMERIC);

}

void loadVariablesControl(){

	regDelayHardReset = datasFirebaseNumericLocal[TIME_HARD_RESET];					//tempo para entre o acionamento da bomba e a chegada da água no reservatório
}

void copyVetor_8(uint8_t* v1,  uint8_t* v2, uint8_t len){

	for(uint8_t i = 0; i < len; i++){
		*(v1+i) = *(v2+i);
	}
}

void updateErrorFlag(){

	if (flagsCtrlSystem.flgBit.flgRefreshLedError){
		flagsCtrlSystem.flgBit.flgRefreshLedError = 0;

		if (flagsCtrlSystem.flgBit.flgErrorFlagsStartStop){

			swapPin(OUTPUT_ERRO_LED);
		}else{
			flagsCtrlSystem.flgBit.flgErrorFlagsEn = 0;

			changeOutput(OUTPUT_ERRO_LED, HIGH);
		}
	}
}

void setFlagsErroLocal(){
	flagsCtrlSystem.flgBit.flgErrorFlagsEn = 1;
	flagsCtrlSystem.flgBit.flgErrorFlagsStartStop = 1;
}

void clearFlagsErroLocal(){
	flagsCtrlSystem.flgBit.flgErrorFlagsEn = 0;
	flagsCtrlSystem.flgBit.flgErrorFlagsStartStop = 0;
	flagsCtrlSystem.flgBit.flgRefreshLedError = 0;

	changeOutput(OUTPUT_ERRO_LED, HIGH);
}

void errorFlags(){

	uint8_t error = -1;

	if (flagsCtrlSystem.flgBit.flgConfigurationEn){
		error = MODE_CONFIGURATION;
//	}else if(flagsCtrlGeral.flagsCtrlGeralBit.flgTurnOffPump){
//		error = ERRO_TURN_OFF_MANUAL;
//	}else if(flagsCtrlGeral.flagsCtrlGeralBit.flgStatusPump){
//			error = ERRO_PROTECAO_CLEAR;
//	}else if (flagsCtrlGeral.flagsCtrlGeralBit.flgProtectionWait){
//		error = ERRO_POCO_VAZIO;
//	}else if (flagsCtrlGeral.flagsCtrlGeralBit.flgErroFlowWaterPumpOff){
//			error = ERRO_FLOW_WATER_PUMP_OFF;
	}else if(!flagsCtrlSystem.flgBit.flgAuthenticationError){
		error = ERROR_AUTHENTICATON;
	}else if (!flagsCtrlSystem.flgBit.flgCommunicationOk){
		error = ERROR_COMM;
	}else if (errorOld > 0){
		error = ERROR_CLEAR;
	}

	if (error >= 0){

		setFlagsErroLocal();

		switch(error){
			case ERROR_CLEAR:
				flagsCtrlSystem.flgBit.flgErrorFlagsStartStop = 0;
				break;

			case ERROR_AUTHENTICATON:
				regBaseDelayError = DELAY_ERROR_AUTHENTICATON;
				break;

			case ERROR_COMM:
				regBaseDelayError = DELAY_ERROR_COMM;
				break;

			case MODE_CONFIGURATION:
				regBaseDelayError = DELAY_MODE_CONFIGURATION;
				break;

//			case ERRO_POCO_VAZIO:
//			case ERRO_PROTECAO_CLEAR:
//			case ERRO_TURN_OFF_MANUAL:
//			case ERRO_FLOW_WATER_PUMP_OFF:
//
//				regBaseDelayError = DELAY_ERR_GENERIC;
//				break;
		}

//		if(regDelayError <= 0){
//			regDelayError = regBaseDelayError;
//		}

		if(errorOld != error){
			regDelayError = regBaseDelayError;
			flagsCommunication.flgBit.flgSendErrorFirebase = 1;
		}
		errorOld = error;

		if(flagsCtrlSystem.flgBit.flgErrorFlagsEn){
			if (flagsCtrlSystem.flgBit.flgErrorFlagsStartStop){

				if (flagsCtrlSystem.flgBit.flgRefreshLedError){
					flagsCtrlSystem.flgBit.flgRefreshLedError = 0;

					swapPin(OUTPUT_ERRO_LED);
				}
			}else{
				clearFlagsErroLocal();
			}
		}
	}
}


//Functions EEPROM
bool getDataDefaultEEPROM_8(uint16_t address, uint8_t *vetor, uint8_t len){

	readVetor_8(address, vetor, len);

	return vetor[len-1] == 0x0AA ? true : false;
}

void updateDataNumericEEPROM_8(uint16_t address, uint8_t *vetor, uint8_t len){

	uint8_t listEEPROM[len+1];

	for(int i = 0; i < len; i++){
		listEEPROM[i] = *(vetor+i);
	}

	*(listEEPROM+len) = 0x0AA;

	writeVetor_8(address, listEEPROM, len+1);
}

//bool downloadDataEEPROM_8(uint16_t address, uint8_t len, uint8_t &back){
bool downloadDataNumericEEPROM_8(uint8_t address, uint8_t len, uint8_t *back){

	uint8_t dataTemp[len+1];

	if(getDataDefaultEEPROM_8(address, dataTemp, len+1)){

		for(int i = 0; i < len; i++){														//Descarta o último dado, devido ser apenas de verificação
			*(back+i) = *(dataTemp+i);
		}

		return true;
	}else{
		return false;
	}
}
