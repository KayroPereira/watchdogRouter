/*
 * FirebaseFunctions.h
 *
 *  Created on: 2021 Mar 13
 *      Author: kayro
 */

#ifndef FIREBASEFUNCTIONS_H_
	#define FIREBASEFUNCTIONS_H_

	#include "HardwareAccess.h"

	#define MAC_DEVICE "/34E2C412"

	//Valores default
	#define ALIAS 						0						//apelido equipamento
	#define DNS1 						1						//dns ping primário
	#define DNS2						2						//dns ping secundário
	#define ERRO 						3						//erro
	#define FAVORITE					4						//flag - equipamento favorito - utilizado no dashboard
	#define HARD_RESET					5						//flag - reset remoto
	#define TIME_HARD_RESET				6						//tempo máximo para reset local em caso de falha


	//Valores default
	#define VALUE_ALIAS					"default"				//apelido equipamento
	#define VALUE_DNS1 					"8.8.8.8"				//Valor default flag atualização
	#define VALUE_DNS2					"8.8.4.4"				//Valor default status bomba
	#define VALUE_ERRO 					0						//Valor default erro
	#define VALUE_FAVORITE				0						//Valor default
	#define VALUE_HARD_RESET			0						//Valor default
	#define VALUE_TIME_HARD_RESET		10						//Valor default para realização de reset local


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
	const uint8_t LIST_DATA_EEPROM_NUMERIC[] = {ERRO, FAVORITE, HARD_RESET, TIME_HARD_RESET};												//Dados numéricos salvos na EEPROM
	const uint8_t LIST_DATA_EEPROM_TEXT[] = {ALIAS, DNS1, DNS2};																			//Dados literais salvos na EEPROM


	void streamCallback(StreamData data);
	void streamTimeoutCallback(bool timeout);
	void printResult(FirebaseData &data);
	void printResult(StreamData &data);

#endif /* FIREBASEFUNCTIONS_H_ */
