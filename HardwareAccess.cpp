/*
 * HardwareAccess.c
 *
 *  Created on: 2021 Mar 11
 *      Author: kayro
 */

#include "HardwareAccess.h"

FlagsCtrlSystem flagsCtrlSystem;

os_timer_t tmr0;


uint8_t		regDelay1s              	= REC_DELAY_1S,
			regDelay1Min				= REC_DELAY_1M,
			regDelayHardReset			= VALUE_TIME_HARD_RESET,			//tempo para entre o acionamento da bomba e a chegada da água no reservatório
//			regDelayButtonConfig     	= DELAY_TIME_BUTTON_CONFIG,
//			regDelayDebounce			= DELAY_DEBOUNCE,
			regDelayError	 			= 0,
			regBaseDelayError 			= 0;
//			regDelayConfiguration		= DELAY_TIMER_CONFIGURATION,
//			regDelayFirebaseMin			= DELAY_GET_FIREBASE_1M,
//			regDelayFirebase        	= DELAY_GET_FIREBASE,
//			regDelayTryConnection		= DELAY_TRY_CONNECTION;				//tempo para tentar uma nova conexão com a internet, caso a mesma seja perdida


//unsigned int 	datasFirebaseLocal[LENGTH_PATH];

void hardwareConfig(){

	os_timer_setfn(&tmr0, Timer_0, NULL);     //Indica ao Timer qual sera sua Sub rotina.
	os_timer_arm(&tmr0, 10, true);            //Configura o timer 0 para 10ms com loop.


	EEPROM.begin(LENGTH_EEPROM);


	for(uint8_t i = 0; i < LENGTH_LIST_INPUTS; i++){
		_pinMode(LIST_INPUTS[i], INPUT_PULLUP);
	}

	for(uint8_t i = 0; i < LENGTH_LIST_OUTPUTS; i++){
		_pinMode(LIST_OUTPUTS[i], OUTPUT);
		changeOutput(LIST_OUTPUTS[i], HIGH);
	}

	_delay(LOAD_TIME_SYSTEM);

	Serial.begin(57600);

	//remover - RODAR APENAS NA PRIMEIRA GRAVAÇÃO - REMOVER APÓS
	delay(1000);
	writeString(ADDRESS_SSID, "NetWifi##");
	writeString(ADDRESS_PASSWORD, "Pedepano21@@##");
	_delay(1000);

//	delay(1000);
//	writeString(ADDRESS_SSID, "NetWifi2##");
//	writeString(ADDRESS_PASSWORD, "Pedepano21@@##");
//	_delay(1000);

//	delay(1000);
//	writeString(ADDRESS_SSID, "Kayro##");
//	writeString(ADDRESS_PASSWORD, "123456789##");
//	_delay(1000);
}

void Timer_0 (void*){

	//#Error Flags Start
	if (flagsCtrlSystem.flgBit.flgErrorFlagsEn){
		if (flagsCtrlSystem.flgBit.flgErrorFlagsStartStop){
			if (!--regDelayError){
				regDelayError = regBaseDelayError;
				flagsCtrlSystem.flgBit.flgRefreshLedError = 1;
			}
		}

//		if (flagsCtrlSystem.flgBit.flgErrorFlagsStartStop){
//			if (!--regDelayError){
//				flagsCtrlSystem.flgBit.flgRefreshLedError = 1;
//				regDelayError = regBaseDelayError;
//			}
//		}else{
//			flagsCtrlSystem.flgBit.flgRefreshLedError = 1;
//		}
	}
	//#Error Flags End

	//#Controle de tempo sequência de teste Início
	if (!--regDelay1s){
		regDelay1s = REC_DELAY_1S;


		//#Delay try connection - Início
		if(flagsCommunication.flgBit.flgDelayTryConnectionEn){
			if(!--regDelayTryConnection){
				regDelayTryConnection = DELAY_TRY_CONNECTION;
				flagsCommunication.flgBit.flgDelayTryConnectionOk = 1;
			}
		}
		//#Delay try connection - Fim

//		errorFlags();
//
//		//#Tempo máximo de configuração - via server - Início
//		if(flagsCtrlGeral.flagsCtrlGeralBit.flgConfigurationEn){
//			if(!--regDelayConfiguration){
//				flagsCtrlGeral.flagsCtrlGeralBit.flgConfigurationEn = 0;
//				flagsCtrlGeral.flagsCtrlGeralBit.flgConfigurationOk = 1;
//			}
//		}
//		//#Tempo máximo de configuração - via server - Fim
//
//		//#Timer para proteção da bomba - Início - Tempo máximo entre o acionamento e a chegada da água no reservatório
//		if(flagsCtrlGeral.flagsCtrlGeralBit.flgMonitorFlowWaterEn){
//			if(!--regDelayTimeWater){
//				regDelayTimeWater = datasFirebaseLocal[TDW];
//				flagsCtrlGeral.flagsCtrlGeralBit.flgMonitorFlowWaterEn = 0;
//				flagsCtrlGeral.flagsCtrlGeralBit.flgStartProtection = 1;			//Ativa proteção bomba
//			}
//		}
//		//#Timer para proteção da bomba - Fim
//
//		if (flagsCtrlGeral.flagsCtrlGeralBit.flgDelayConfigurationEn){
//			if (!--regDelayButtonConfig){
//				regDelayButtonConfig = DELAY_TIME_BUTTON_CONFIG;
//				flagsCtrlGeral.flagsCtrlGeralBit.flgDelayConfigurationEn = 0;
//				flagsCtrlGeral.flagsCtrlGeralBit.flgDelayConfigurationOk = 1;
//			}
//		}
//
//		if (!--regDelayFirebaseMin){										//controla o tipo de conexao com firebase
//			regDelayFirebaseMin = DELAY_GET_FIREBASE_1M;					//por evento ou por download de dados
//			flagsCommunication.flgBit.flgGetFirebaseDelayMin = 1;			//força o download a cada periodo tempo definido
//		}
//
		if (flagsCommunication.flgBit.flgTryingConnection)							//não realiza consulta caso esteja tentando na função configurationWifi
			flagsCtrlSystem.flgBit.flgCommunicationOk = wifiConnectionStatus();	//Verifica conexao wifi
//
//
//		if(!--regDelay1Min){
//			regDelay1Min = REC_DELAY_1M;
//
//			//#Tempo sem detectar pulsos - Início - poço seco
//			if(flagsCtrlGeral.flagsCtrlGeralBit.flgStartProtection){
//				if(!flagsCtrlGeral.flagsCtrlGeralBit.flgProtectionRearm){
//					if(!--regDelayRearm){
//						flagsCtrlGeral.flagsCtrlGeralBit.flgProtectionRearm = 1;
//					}
//				}
//
//				//#Tempo envio delay rearme - Início
//				if(flagsCtrlGeral.flagsCtrlGeralBit.flgSendTimeRearmFirebaseEn){
//					flagsCtrlGeral.flagsCtrlGeralBit.flgSendTimeRearmFirebaseOk = 1;
//				}
//				//#Tempo envio delay rearme - Fim
//			}
//			//#Tempo sem detectar pulsos - Fim
//		}
	}
	errorFlags();
}


//Functions use general

void _pinMode(uint8_t pin, uint8_t mode){
	pinMode(pin, mode);
}

void swapPin(int8_t pin){
	digitalWrite(pin, !digitalRead(pin));
}

void changeOutput(uint8_t pin, uint8_t status){
	digitalWrite(pin, status);
}

void _delay(int temp){
	delay(temp);
}

void _delayMicroseconds(unsigned int delay){
	delayMicroseconds(delay);
}


//Functions EEPROM

void write_8(int address, uint8_t value){
	EEPROM.write(address, value);
	EEPROM.commit();
}

void readVetor_8 (int address, uint8_t *data, uint8_t length){

	for (uint8_t i = 0; i < length; i++){
		*(data + i) = EEPROM.read(address+i);
	}
}

size_t writeVetor_8 (int address, uint8_t *vetor, uint8_t length){

	for(uint8_t i = 0; i < length; i++){
		write_8(address+i, *(vetor+i));
	}
	EEPROM.commit();

	return length;
}

String readString (int address, uint8_t length){

	char data[length+1];
	uint8_t len = 0;

	while(data[len-1] != '\0' && len < (length+1)){
		data[len] = EEPROM.read(address+len);
		len++;
	}

	if (data[len-1] == '\0'){
		return String(data);
	}else{
		return "";
	}
}

size_t writeString (int address, String value){
	int _size = value.length();

	for(int i = 0; i < _size; i++){
		EEPROM.write(address+i, value[i]);
	}
	EEPROM.write(address+_size,'\0');

	EEPROM.commit();
	return _size;
}

bool getAuthenticationEEPROM(String *ssid, String *password){

	*ssid = readString(ADDRESS_SSID, LENGTH_SSID);
	*password = readString(ADDRESS_PASSWORD, LENGTH_PASSWORD);

	uint8_t lenSSID = ssid->length(),
			lenPass = password->length();

	if ((*ssid).equals("") || !(*ssid).endsWith("##") || !(*password).endsWith("##")){
		return false;
	}

	*ssid = ssid->substring(0, lenSSID-2);
	*password = password->substring(0, lenPass-2);

	return true;
}
