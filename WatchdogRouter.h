
#ifndef _testEspFirebaseCallback_H_
	#define _testEspFirebaseCallback_H_

	#include "HardwareAccess.h"
	#include "WebCommunication.h"
	#include "FirebaseFunctions.h"

//	#define LED_2			2			//GPIO2	-	D4
//
//	#define OUT_8			12			//GPIO12 -	D6
//	#define OUT_11			15			//GPIO15 -	D8
//
//	#define BUTTON_9		13			//GPIO13 -	D7
//	#define BUTTON_10		14			//GPIO14 -	D5

//	#define LENGTH_PATH_FIREBASE		2
//	const String PATH_FIREBASE[] = {"btn1", "btn2"};
//	const uint8_t PATH_LEDS[] = {OUT_8, OUT_11};
//	const uint8_t PATH_BUTTONS[] = {BUTTON_9, BUTTON_10};



	void printResult(FirebaseData &data);
	void printResult(StreamData &data);
	void streamCallback(StreamData data);
	void streamTimeoutCallback(bool timeout);
	void refreshFirebase();


	extern FlagsCtrlSystem flagsCtrlSystem;
	extern FlagsFirebase flagsFirebase;
//	extern uint8_t datasFirebaseNumeric[LENGTH_DATA_EEPROM_NUMERIC];
	extern uint8_t datasFirebaseNumeric[];
	extern uint8_t datasFirebaseNumericLocal[];
	extern FirebaseData fbdo1;
	extern FirebaseData fbdo2;
	extern FirebaseData fbdo3;


	extern void hardwareConfig();
	extern void loadVariables();
	extern bool tryGetAuthentication();
	extern bool downloadDataNumericEEPROM_8(uint8_t address, uint8_t len, uint8_t *back);
	extern void configurationWifi(uint8_t mode, uint8_t tryConnection);
	extern bool firebaseConnection();
	extern void communicationStatusMonitoring();
	extern void loadVariablesControl();
	extern void sendDataFirebase();


	//TODO - Remover
	extern bool updateFirebase(FirebaseData &fbdo, String path, FirebaseJson &json);
	extern void splitDataJsonFirebase(FirebaseData &data);

#endif /* _testEspFirebaseCallback_H_ */
