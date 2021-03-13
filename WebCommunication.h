/*
 * WebCommunication.h
 *
 *  Created on: 2021 Mar 10
 *      Author: kayro
 */

#ifndef WEBCOMMUNICATION_H_
	#define WEBCOMMUNICATION_H_


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
			unsigned flgTryingConnection  		: 1;
//
			unsigned flgDelayTryConnectionEn	: 1;
			unsigned flgDelayTryConnectionOk	: 1;
//
//			unsigned flgReceivedData	  		: 1;
//			unsigned flgStuckDataFirebase		: 1;
		}flgBit;

		unsigned int flgByte;
	}FlagsCommunication;


	#include "HardwareAccess.h"


	#define DELAY_TRY_CONNECTION	60						//trocar para 60

////	#include "FirebaseArduino.h"
////	#include <ESP8266WebServer.h>
//
//	#include <ESP8266WiFi.h>
//	#include <FirebaseESP8266.h>
//	#include "security.h"
//
//	//Valores default erros
//	#define	ERRO_PING_DNS				11
//	#define	ERRO_PROTECAO_CLEAR			12
//	#define	ERRO_TURN_OFF_MANUAL		13
//	#define	ERRO_FLOW_WATER_PUMP_OFF	14
//	#define	ERRO_NAO_CADASTRADO			15
//


	//base timer
//	#define FAILURE_DB					10
	#define WAIT_COMM					30
	#define WAIT_COMM_FAST				3						//trocar para 3 - possivelmente
//	#define DELAY_GET_FIREBASE_1M		60						//trocar para 60
//
//	#define DELAY_GET_FIREBASE_1M		60						//trocar para 60
//
	#define DELAY_TRY_CONNECTION		60						//trocar para 60
	#define COUNT_COMM_FAILURE			30						//Mudar para 60 -> testando com 30 para melhor otimização


	//Valores default
	#define ALIAS 						0						//apelido equipamento
	#define DNS1 						1						//dns ping primário
	#define DNS2						2						//dns ping secundário
	#define ERRO 						3						//erro
	#define FAVORITE					4						//flag - equipamento favorito - utilizado no dashboard
	#define HARD_RESET					5						//flag - reset remoto
	#define TIME_RESET					6						//tempo máximo para reset local em caso de falha


	//Valores default
	#define VALUE_ALIAS					"default"				//apelido equipamento
	#define VALUE_DNS1 					"8.8.8.8"				//Valor default flag atualização
	#define VALUE_DNS2					"8.8.4.4"				//Valor default status bomba
	#define VALUE_ERRO 					0						//Valor default erro
	#define VALUE_FAVORITE				0						//Valor default
	#define VALUE_HARD_RESET			0						//Valor default
	#define VALUE_TIME_RESET			10						//Valor default para realização de reset local


//	#define LENGTH_PATH_NUMERIC			4
	#define LENGTH_PATH_TEXT			3
	#define SIZE_PATH_TEXT				10

	#define ERROR_LENGTH_SSID			0
	#define ERROR_LENGTH_PASSWORD		2

	//configuration and address EEPROM
	#define ADDRESS_DATA_EEPROM_NUMERIC			100				//Inicio dos dados numéricos na EEPROM
	#define ADDRESS_DATA_EEPROM_TEXT			110				//Inicio dos dados texto na EEPROM
	#define LENGTH_DATA_EEPROM_NUMERIC			4				//Quatro dados serão salvos - erro - favorite - hardReset - timeReset + #AA -> este valor confirma a leitura correta

	//TODO - definir para texto
//	#define LENGTH_DATA_EEPROM_NUMERIC			10				//Quatro dados serão salvos - erro - favorite - hardReset - timeReset + #AA -> este valor confirma a leitura correta

	const char PATH_FIREBASE[][SIZE_PATH_TEXT] = {"alias", "dns1", "dns2", "erro", "favorite", "hardReset", "timeReset"};					//labels Firebase
	const uint8_t LIST_DATA_EEPROM_NUMERIC[] = {ERRO, FAVORITE, HARD_RESET, TIME_RESET};													//Dados numéricos salvos na EEPROM
	const uint8_t LIST_DATA_EEPROM_TEXT[] = {ALIAS, DNS1, DNS2};																			//Dados literais salvos na EEPROM


//	extern void _delayMicroseconds(unsigned int delay);


extern FlagsCtrlSystem flagsCtrlSystem;


extern bool getAuthenticationEEPROM(String *ssid, String *password);
extern void _delayMicroseconds(unsigned int delay);

#endif /* WEBCOMMUNICATION_H_ */
