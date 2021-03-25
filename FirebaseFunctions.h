/*
 * FirebaseFunctions.h
 *
 *  Created on: 2021 Mar 13
 *      Author: kayro
 */

#ifndef FIREBASEFUNCTIONS_H_
	#define FIREBASEFUNCTIONS_H_

	typedef union {
		struct {
//			unsigned flgGetFirebaseOk        	: 1;
//			unsigned flgGetFirebaseDelay     	: 1;
//			unsigned flgFailureFirebase      	: 1;
//			unsigned flgGetFirebaseDelayMin  	: 1;
//			unsigned flgFirebaseFirsComm	    : 1;
//
//			unsigned flgCommunicationOk			: 1;
//			unsigned flgAuthenticationError		: 1;
//			unsigned flgRestarWifi				: 1;
//			unsigned flgTryingConnection  		: 1;
//
//			unsigned flgDelayTryConnectionEn	: 1;
//			unsigned flgDelayTryConnectionOk	: 1;

			unsigned flgSendDataFirebase		: 1;
			unsigned flgAddDataSendFirebase		: 1;

			unsigned flgFirebaseStartOk			: 1;
//
//			unsigned flgReceivedData	  		: 1;
//			unsigned flgStuckDataFirebase		: 1;
		}flgBit;

		unsigned int flgByte;
	}FlagsFirebase;

	#include "HardwareAccess.h"

	#define MAC_DEVICE 				"/34E2C412"
	#define PATH_UPDATE_SHARE 		MAC_DEVICE  "/share"
	#define PATH_UPDATE_FLG 		MAC_DEVICE "/f"

	//default values of path positions
	#define DNS1 						0						//dns ping primário
	#define DNS2						1						//dns ping secundário
	#define ERRO 						2						//erro
	#define HARD_RESET					3						//flag - reset remoto
	#define TIME_HARD_RESET				4						//tempo máximo para reset local em caso de falha


//	//default values of position send firebase
//	#define DNS1 						0						//dns ping primário
//	#define DNS2						1						//dns ping secundário
	#define ERRO_PATH_SEND 				0						//erro
	#define HARD_RESET_PATH_SEND		1						//flag - reset remoto
//	#define TIME_HARD_RESET				4						//tempo máximo para reset local em caso de falha


	//Valores default
	#define VALUE_DNS1 					"8.8.8.8"				//Valor default flag atualização
	#define VALUE_DNS2					"8.8.4.4"				//Valor default status bomba
	#define VALUE_ERRO 					0						//Valor default erro
	#define VALUE_HARD_RESET			0						//Valor default
	#define VALUE_TIME_HARD_RESET		10						//Valor default para realização de reset local

//	//Valores default
////	#define ALIAS 						0						//apelido equipamento
//	#define DNS1 						1						//dns ping primário
//	#define DNS2						2						//dns ping secundário
//	#define ERRO 						3						//erro
////	#define FAVORITE					4						//flag - equipamento favorito - utilizado no dashboard
//	#define HARD_RESET					5						//flag - reset remoto
//	#define TIME_HARD_RESET				6						//tempo máximo para reset local em caso de falha
//
//
//	//Valores default
////	#define VALUE_ALIAS					"default"				//apelido equipamento
//	#define VALUE_DNS1 					"8.8.8.8"				//Valor default flag atualização
//	#define VALUE_DNS2					"8.8.4.4"				//Valor default status bomba
//	#define VALUE_ERRO 					0						//Valor default erro
////	#define VALUE_FAVORITE				0						//Valor default
//	#define VALUE_HARD_RESET			0						//Valor default
//	#define VALUE_TIME_HARD_RESET		10						//Valor default para realização de reset local



	#define LENGTH_PATH_FIREBASE		5
//	#define LENGTH_PATH_TEXT			3
	#define SIZE_PATH_TEXT				10

//	#define ERROR_LENGTH_SSID			0
//	#define ERROR_LENGTH_PASSWORD		2

	//configuration and address EEPROM
	#define ADDRESS_DATA_NUMERIC_EEPROM			100				//Inicio dos dados numéricos na EEPROM
	#define ADDRESS_DATA_TEXT_EEPROM			110				//Inicio dos dados texto na EEPROM
	#define LENGTH_DATA_NUMERIC_EEPROM			3				//Três dados serão salvos - erro - hardReset - timeReset + #AA -> este valor confirma a leitura correta
	#define LENGTH_DATA_FIREASE_NUMERIC_SEND	2				//dois dados serão compartilhados com o firebase - erro - hardReset

	#define LENGTH_DATA_TEXT_EEPROM				2				//dois dados serão salvos - dns1 - dns2 + #AA -> este valor confirma a leitura correta

	//TODO - definir para texto
//	#define LENGTH_DATA_EEPROM_NUMERIC			10				//Quatro dados serão salvos - erro - favorite - hardReset - timeReset + #AA -> este valor confirma a leitura correta

//	const char PATH_FIREBASE[][SIZE_PATH_TEXT] = {"alias", "dns1", "dns2", "erro", "favorite", "hardReset", "timeReset"};			//labels Firebase
//	const uint8_t LIST_DATA_EEPROM_NUMERIC[] = {ERRO, FAVORITE, HARD_RESET, TIME_HARD_RESET};										//Dados numéricos salvos na EEPROM
//	const uint8_t LIST_DATA_EEPROM_TEXT[] = {ALIAS, DNS1, DNS2};																	//Dados literais salvos na EEPROM

	const char PATH_FIREBASE[][SIZE_PATH_TEXT] = {"dns1", "dns2", "erro", "hardReset", "timeReset"};								//labels Firebase

	const uint8_t LIST_DATA_TEXT_EEPROM[] = {DNS1, DNS2};																			//Dados literais salvos na EEPROM
	const uint8_t LIST_DATA_NUMERIC_EEPROM[] = {ERRO, HARD_RESET, TIME_HARD_RESET};													//Dados numéricos salvos na EEPROM

	const uint8_t LIST_PATH_NUMERIC_SEND[] = {ERRO, HARD_RESET};																	//Endereço dos dados numéricos compartilhados com o firebase - link com PATH_FIREBASE
	const uint8_t LIST_DATA_NUMERIC_SEND[] = {ERRO_PATH_SEND, HARD_RESET_PATH_SEND};												//Dados numéricos compartilhados com o firebase


	void streamCallback(StreamData data);
	void streamTimeoutCallback(bool timeout);
	void printResult(FirebaseData &data);
	void printResult(StreamData &data);
	void splitDataJsonFirebase(StreamData &data);


	extern uint8_t datasFirebaseNumeric[];

#endif /* FIREBASEFUNCTIONS_H_ */
