/*
 * HardwareAccess.h
 *
 *  Created on: 2021 Mar 11
 *      Author: kayro
 */

#ifndef HARDWAREACCESS_H_
	#define HARDWAREACCESS_H_

	#include "Arduino.h"
	#include <EEPROM.h>

	#include <ESP8266WiFi.h>
	#include <FirebaseESP8266.h>

	#include "Security.h"


	typedef union {
		struct {
//			unsigned flgGetFirebaseOk        	: 1;
//			unsigned flgGetFirebaseDelay     	: 1;
//			unsigned flgFailureFirebase      	: 1;
//			unsigned flgGetFirebaseDelayMin  	: 1;
//			unsigned flgFirebaseFirsComm	    : 1;
//
			unsigned flgCommunicationOk			: 1;
			unsigned flgAuthenticationError		: 1;
//			unsigned flgRestarWifi				: 1;

			unsigned flgSystemStart		    	: 1;

			unsigned flgDownloadDataEEPROM		: 1;

//			unsigned flgTryingConnection  		: 1;
//
//			unsigned flgDelayTryConnectionEn	: 1;
//			unsigned flgDelayTryConnectionOk	: 1;
//
//			unsigned flgReceivedData	  		: 1;
//			unsigned flgStuckDataFirebase		: 1;

			unsigned flgRefreshLedError				: 1;
			unsigned flgErrorFlagsEn				: 1;
			unsigned flgErrorFlagsStartStop			: 1;

			unsigned flgConfigurationEn				: 1;
			unsigned flgConfigurationOk				: 1;
		}flgBit;

		unsigned int flgByte;
	}FlagsCtrlSystem;

typedef union {
		struct {
//			unsigned flgStartProtection        		: 1;
//			unsigned flgStopProtection        		: 1;
//			unsigned flgTurnOffPump		    		: 1;
//			unsigned flgMonitorFlowWaterEn			: 1;
//			unsigned flgProtectionWait				: 1;
//			unsigned flgProtectionRearm				: 1;
//
//			unsigned flgClearCountWater	    		: 1;
//			unsigned flgCicloCountWater	    		: 1;
//
//			unsigned flgVoltageSensorDelayEn   		: 1;
//			unsigned flgVoltageSensorDelayOn		: 1;
//			unsigned flgVoltageSensorTransition		: 1;
//
//			unsigned flgStatusPump			    	: 1;
//
//			unsigned flgSystemStart		    		: 1;
//			unsigned flgDownloadDataEEPROM    		: 1;
//
//			unsigned flgDebounceSwitchEn			: 1;
//			unsigned flgDebounceSwitchOk			: 1;
//			unsigned flgSwitchInputStatus			: 1;
//			unsigned flgSwitchInputTransition		: 1;
//
//			unsigned flgRefreshLedError				: 1;
//			unsigned flgStatusLedError				: 1;
//			unsigned flgSendErrorFirebase			: 1;
//
//			unsigned flgDelayConfigurationEn		: 1;
//			unsigned flgDelayConfigurationOk		: 1;
//
//			unsigned flgConfigurationEn				: 1;
//			unsigned flgConfigurationOk				: 1;
//
//			unsigned flgErrorFlagsEn				: 1;
//			unsigned flgErrorFlagsStartStop			: 1;
//			unsigned flgErroFlowWaterPumpOff		: 1;
////
//			unsigned flgSendTimeRearmFirebaseEn		: 1;
//			unsigned flgSendTimeRearmFirebaseOk		: 1;

		}flagsCtrlGeralBit;

		double flagsCtrlGeralByte;
	}FlagsCtrlGeral;


	#include "WebCommunication.h"


	//Outputs
	#define OUTPUT_ERRO_LED				2		//GPIO2	-	D4
	#define HARD_RESET_RELE				12		//GPIO12 -	D6
	#define LENGTH_LIST_OUTPUTS			2
	const uint8_t LIST_OUTPUTS[] = {OUTPUT_ERRO_LED, HARD_RESET_RELE};


	//Inputs
	#define BUTTON_CONFIG				13		//GPIO13 -	D7
	#define LENGTH_LIST_INPUTS			1
	const uint8_t LIST_INPUTS[] = {BUTTON_CONFIG};


	//Storage Data
	//configuration and address EEPROM
	#define LENGTH_EEPROM				300		//Comprimento total utilizado da EEPROM
	#define ADDRESS_SSID				10
	#define ADDRESS_PASSWORD			50
	#define LENGTH_SSID					35
	#define LENGTH_PASSWORD				35


	//flags error
	#define ERROR_CLEAR					0
	#define ERROR_AUTHENTICATON			1
	#define ERROR_COMM					2
	#define MODE_CONFIGURATION			4

	//base times error
	#define DELAY_ERROR_AUTHENTICATON	50
	#define DELAY_ERROR_COMM			100
	#define DELAY_MODE_CONFIGURATION	10
	#define DELAY_ERR_GENERIC			200


	//base times
	#define REC_DELAY_1S                100    	//Base de tempo 1s colocar em 100
	#define REC_DELAY_1M                60    	//Base de tempo 1min colocar em 60

	#define LOAD_TIME_SYSTEM            2000

	#define BASETIME_2                  500
	#define BASETIME                    1000
	#define LOADTIME                    2000
	#define DELAY_FAST_SWITCHING		100		//100ms


	void Timer_0 (void*);
	void _pinMode(uint8_t pin, uint8_t mode);
	void _delay(int temp);
	void changeOutput(uint8_t pin, uint8_t status);
	size_t writeString (int address, String value);


	extern FlagsCommunication flagsCommunication;
	extern uint8_t regDelayTryConnection;


	extern bool wifiConnectionStatus();
	extern void errorFlags();

#endif /* HARDWAREACCESS_H_ */
