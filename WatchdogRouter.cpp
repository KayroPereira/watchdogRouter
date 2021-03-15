#include "WatchdogRouter.h"

FlagsCtrlGeral flagsCtrlGeral;

//Define FirebaseESP8266 data object
//FirebaseData fbdo1;
//FirebaseData fbdo2;

FirebaseJson jsonBufferLocal;

unsigned long 	sendDataPrevMillis = 0,
				delayButton = 0,
				delaySendFirebase = 0,
				realTime = 0;

bool sendDataFirebaseFlag = false;

//String path = "/Test/Stream";
//String path = "/34E2C412";

uint8_t statusButtons[2] = {0, 0};
uint8_t pathButtonsTemp[2] = {1, 1};

uint16_t count = 0;


void setup() {


	hardwareConfig();
	loadVariables();


//	//TODO - Remover
//	flagsCtrlSystem.flgBit.flgConfigurationEn = 1;

	flagsCtrlSystem.flgBit.flgAuthenticationError = tryGetAuthentication();

//	//TODO - Remover
//	Serial.printf("\n-------------\nvalor default\n");
//	for(int j = 0; j < LENGTH_DATA_EEPROM_NUMERIC; j++){
//		Serial.printf("%s: %u\n", PATH_FIREBASE[LIST_DATA_EEPROM_NUMERIC[j]], datasFirebaseNumericLocal[j]);
//	}
//	Serial.printf("\n-------------\n");

	flagsCtrlSystem.flgBit.flgDownloadDataEEPROM = downloadDataNumericEEPROM_8(ADDRESS_DATA_NUMERIC_EEPROM, LENGTH_DATA_NUMERIC_EEPROM, datasFirebaseNumericLocal);


	if(flagsCtrlSystem.flgBit.flgDownloadDataEEPROM){
		loadVariablesControl();
	}

	if(flagsCtrlSystem.flgBit.flgAuthenticationError){
		configurationWifi(WIFI_STA, WAIT_COMM);
	}

	if (flagsCtrlSystem.flgBit.flgCommunicationOk){
		firebaseConnection();
	}else{
		flagsCommunication.flgBit.flgDelayTryConnectionEn = 1;
	}

//	//Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
//	fbdo1.setBSSLBufferSize(1024, 1024);
//
//	//Set the size of HTTP response buffers in the case where we want to work with large data.
//	fbdo1.setResponseSize(1024);
//
//	//Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
//	fbdo2.setBSSLBufferSize(1024, 1024);
//
//	//Set the size of HTTP response buffers in the case where we want to work with large data.
//	fbdo2.setResponseSize(1024);

//	if (!Firebase.beginStream(fbdo1, MAC_DEVICE)) {
//		Serial.println("------------------------------------");
//		Serial.println("Can't begin stream connection...");
//		Serial.println("REASON: " + fbdo1.errorReason());
//		Serial.println("------------------------------------");
//		Serial.println();
//	}
//
//	Firebase.setStreamCallback(fbdo1, streamCallback, streamTimeoutCallback);






//	refreshFirebase();
//	}
}


bool pushButton(uint8_t button, uint8_t buttonOld){

	if(button == 1 && pathButtonsTemp[buttonOld] == 0){
		pathButtonsTemp[buttonOld] = 1;
		return false;
	}

	if(button == 0 && pathButtonsTemp[buttonOld] == 1){
		pathButtonsTemp[buttonOld] = 0;
		return false;
	}

	if(button == 0 && pathButtonsTemp[buttonOld] == 0){
		pathButtonsTemp[buttonOld] = 2;
		return true;
	}

	if(button == 1 && pathButtonsTemp[buttonOld] == 2){
		pathButtonsTemp[buttonOld] = 1;
		return false;
	}

	return false;
}

void updatePin(uint8_t pin, uint8_t val){
	digitalWrite(pin, val);
}

void updateOutput(uint8_t output){
	updatePin(LIST_OUTPUTS[output], !statusButtons[output]);
}

void printStatusButton(const String &name, uint8_t button, uint8_t buttonTemp, uint8_t buttonStatus){
	Serial.printf("\n\n------------------------------------\nButton: %s\nStatus: %u \nStatusTemp: %u\nStatusLed: %u\n------------------------------------\n\n", name.c_str(), button, buttonTemp, buttonStatus);
}

//bool updateFirebase(FirebaseData &fbdo, String path, FirebaseJson &json){
//
//	if (Firebase.updateNode(fbdo, path, json)) {
//		Serial.println("PASSED");
//		Serial.println("PATH: " + fbdo.dataPath());
//		Serial.println("TYPE: " + fbdo.dataType());
//		Serial.print("VALUE: ");
//		printResult(fbdo);
//		Serial.println("------------------------------------");
//		Serial.println();
//		return true;
//	} else {
//		Serial.println("FAILED");
//		Serial.println("REASON: " + fbdo.errorReason());
//		Serial.println("------------------------------------");
//		Serial.println();
//		return false;
//	}
//}

//void updateAllDataFirebase(){
//	FirebaseJson json;
//
//	for(uint8_t i = 0; i < SIZE_PATH_TEXT; i++){
//		json.set(PATH_FIREBASE[i], 0);
//	}
//
//	updateFirebase(fbdo2, MAC_DEVICE, json);
//}


	FirebaseJson jsonGet;
	String jsonStr = "";
	int teste = 0;


void loop() {

	//Caso exita falha na leitura da EEPROM na inicialização, tenta realizar uma nova atualização. Para de tentar quando consegui ler a EEPROM, ou até baixar os dados do Firebase
	if(!flagsCtrlSystem.flgBit.flgDownloadDataEEPROM && !flagsFirebase.flgBit.flgFirebaseStartOk){

		flagsCtrlSystem.flgBit.flgDownloadDataEEPROM = downloadDataNumericEEPROM_8(ADDRESS_DATA_NUMERIC_EEPROM, LENGTH_DATA_NUMERIC_EEPROM, datasFirebaseNumericLocal);

		if(flagsCtrlSystem.flgBit.flgDownloadDataEEPROM){
			loadVariablesControl();
		}
	}

	communicationStatusMonitoring();														//Acompanha o estado da comunicação web e com o firebase, corrigindo erros eventuais;

	sendDataFirebase();																		//envia os dados represados para o firebase


//	if (!Firebase.readStream(fbdo1))
//	{
//	  Serial.println(fbdo1.errorReason());
//	}
//
//	if (fbdo1.streamTimeout())
//	{
//	  Serial.println("Stream timeout, resume streaming...");
//	  Serial.println();
//	}
//
//	if (fbdo1.streamAvailable())
//	{
//
//	  if (fbdo1.dataType() == "int")
//	    Serial.println(fbdo1.intData());
//	  else if (fbdo1.dataType() == "float")
//	    Serial.println(fbdo1.floatData(), 5);
//	  else if (fbdo1.dataType() == "double")
//	    printf("%.9lf\n", fbdo1.doubleData());
//	  else if (fbdo1.dataType() == "boolean")
//	    Serial.println(fbdo1.boolData() == 1 ? "true" : "false");
//	  else if (fbdo1.dataType() == "string")
//	    Serial.println(fbdo1.stringData());
//	  else if (fbdo1.dataType() == "json")
//	    Serial.println(fbdo1.jsonString());
//
//	}

	realTime = millis();


	if(realTime - delayButton > 1000){

		Serial.printf("\n\n-------------- setStreamCallback --------------\n\n");

		delayButton = millis();

		Firebase.setStreamCallback(fbdo1, streamCallback, streamTimeoutCallback);
//		Serial.println(Firebase.getJSON(fbdo1, MAC_DEVICE));

//		Firebase.get(fbdo1, MAC_DEVICE);
//
//
//		jsonGet = fbdo1.jsonObject();
//
//
//		jsonGet.toString(jsonStr, true);
//
//		Serial.println(jsonStr);

	}

//	if(realTime - delayButton > 30){
//
//		jsonBufferLocal.set(PATH_FIREBASE[TIME_HARD_RESET], (int) realTime);
//
//		if(teste++ >= 100){
//			sendDataFirebaseFlag = true;
//			teste = 0;
//		}
//
//
//		uint8_t temp;
//
//		delayButton = millis();
//
//		for(uint8_t inputAnalyze = 0; inputAnalyze < LENGTH_LIST_INPUTS; inputAnalyze++){
//			temp = digitalRead(LIST_INPUTS[inputAnalyze]);
//
//			if(pushButton(temp, inputAnalyze)){
//				statusButtons[inputAnalyze] = !statusButtons[inputAnalyze];
//				jsonBufferLocal.set(PATH_FIREBASE[inputAnalyze],(int) statusButtons[inputAnalyze]);
//				updateOutput(inputAnalyze);
//				printStatusButton(PATH_FIREBASE[inputAnalyze], temp, pathButtonsTemp[inputAnalyze], statusButtons[inputAnalyze]);
//				sendDataFirebaseFlag = true;
//			}
//		}
//	}
//
	if(sendDataFirebaseFlag){
		if(realTime - delaySendFirebase > 1000){

			delaySendFirebase = millis();

			if(updateFirebase(fbdo2, MAC_DEVICE, jsonBufferLocal)){
				jsonBufferLocal.clear();

				sendDataFirebaseFlag = false;
			}
		}
	}

	if (realTime - sendDataPrevMillis > 15000) {


//		digitalWrite(OUTPUT_ERRO_LED, !digitalRead(OUTPUT_ERRO_LED));

		sendDataPrevMillis = millis();
		count++;

		FirebaseJson json;

		Serial.println("------------------------------------");
		Serial.println("Set JSON...");

		json.set("value", count);

		json.set("test", "dfadl");

		updateFirebase(fbdo2, "/34E2C412", json);
	}
}
