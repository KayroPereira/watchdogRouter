#include "WatchdogRouter.h"

FlagsCtrlGeral flagsCtrlGeral;

//Define FirebaseESP8266 data object
//FirebaseData fbdo1;
//FirebaseData fbdo2;

FirebaseJson jsonBuffer;

unsigned long 	sendDataPrevMillis = 0,
				delayButton = 0,
				delaySendFirebase = 0,
				realTime = 0;

bool sendDataFirebase = false;

//String path = "/Test/Stream";
String path = "/34E2C412";

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

	flagsCtrlSystem.flgBit.flgDownloadDataEEPROM = downloadDataNumericEEPROM_8(ADDRESS_DATA_EEPROM_NUMERIC, LENGTH_DATA_EEPROM_NUMERIC, datasFirebaseNumericLocal);


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

	if (!Firebase.beginStream(fbdo1, path)) {
		Serial.println("------------------------------------");
		Serial.println("Can't begin stream connection...");
		Serial.println("REASON: " + fbdo1.errorReason());
		Serial.println("------------------------------------");
		Serial.println();
	}

	Firebase.setStreamCallback(fbdo1, streamCallback, streamTimeoutCallback);

	refreshFirebase();
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

void printStatusButton(String name, uint8_t button, uint8_t buttonTemp, uint8_t buttonStatus){
	Serial.printf("\n\n------------------------------------\nButton: %s\nStatus: %u \nStatusTemp: %u\nStatusLed: %u\n------------------------------------\n\n", &name, button, buttonTemp, buttonStatus);
}

bool updateFirebase(FirebaseData &fbdo, String path, FirebaseJson &json){

	if (Firebase.updateNode(fbdo, path, json)) {
		Serial.println("PASSED");
		Serial.println("PATH: " + fbdo.dataPath());
		Serial.println("TYPE: " + fbdo.dataType());
		Serial.print("VALUE: ");
		printResult(fbdo);
		Serial.println("------------------------------------");
		Serial.println();
		return true;
	} else {
		Serial.println("FAILED");
		Serial.println("REASON: " + fbdo.errorReason());
		Serial.println("------------------------------------");
		Serial.println();
		return false;
	}
}

void refreshFirebase(){
	FirebaseJson json;

	for(uint8_t i = 0; i < SIZE_PATH_TEXT; i++){
		json.set(PATH_FIREBASE[i], 0);
	}

	updateFirebase(fbdo2, path, json);
}


int teste = 0;

void loop() {

//	errorFlags();

//	//Caso exita falha na leitura da EEPROM na inicialização, tenta realizar uma nova atualização. Para de tentar quando consegui ler a EEPROM, ou até baixar os dados do Firebase
//	if(!flagsCtrlGeral.flagsCtrlGeralBit.flgDownloadDataEEPROM && !flagsCtrlSystem.flgBit.flgSystemStart){
//
//		if(downloadDataEEPROM()){															//Baixa todos os dados do Firebase
//			loadVariablesControl();
//		}
//	}
//
//	//Tenta baixar todos os dados do Firebase na inicialização
//	if(!flagsCommunication.flgBit.flgFailureFirebase && !flagsCtrlSystem.flgBit.flgSystemStart
//		&& flagsCtrlSystem.flgBit.flgCommunicationOk){
//
//		if(isReceivedData()){																//update processes with data received from Firebase
//			flagsCtrlSystem.flgBit.flgSystemStart = 1;
//			clearTimeRearmFirebase();
//		}
//	}

	communicationStatusMonitoring();							//Acompanha o estado da comunicação web e com o firebase, corrigindo erros eventuais;

	realTime = millis();

	if(realTime - delayButton > 30){

		jsonBuffer.set(PATH_FIREBASE[TIME_HARD_RESET], (int) realTime);

		if(teste++ >= 100){
			sendDataFirebase = true;
			teste = 0;
		}

//		uint8_t temp;
//
//		delayButton = millis();
//
//		for(uint8_t inputAnalyze = 0; inputAnalyze < LENGTH_PATH_FIREBASE; inputAnalyze++){
//			temp = digitalRead(PATH_BUTTONS[inputAnalyze]);
//
//			if(pushButton(temp, inputAnalyze)){
//				statusButtons[inputAnalyze] = !statusButtons[inputAnalyze];
//				jsonBuffer.set(PATH_FIREBASE[inputAnalyze],(int) statusButtons[inputAnalyze]);
//				updateOutput(inputAnalyze);
//				printStatusButton(PATH_FIREBASE[inputAnalyze], temp, pathButtonsTemp[inputAnalyze], statusButtons[inputAnalyze]);
//				sendDataFirebase = true;
//			}
//		}
	}

	if(sendDataFirebase){
		if(realTime - delaySendFirebase > 1000){

			delaySendFirebase = millis();

			if(updateFirebase(fbdo2, path, jsonBuffer)){
				jsonBuffer.clear();

				sendDataFirebase = false;
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

		updateFirebase(fbdo2, path, json);
	}
}

void streamCallback(StreamData data) {

	Serial.printf("\n-------------------------------------\n         Atualização Firebase            \n-------------------------------------\n");

	Serial.println("Stream Data1 available...");
	Serial.println("STREAM PATH: " + data.streamPath());
	Serial.println("EVENT PATH: " + data.dataPath());
	Serial.println("DATA TYPE: " + data.dataType());
	Serial.println("EVENT TYPE: " + data.eventType());
	Serial.print("VALUE: ");
	printResult(data);
	Serial.printf("\n-------------------------------------\n\n");
}

void streamTimeoutCallback(bool timeout) {
	if (timeout) {
		Serial.printf("\n\n-------------------------------------\n\n");
		Serial.println("Stream timeout, resume streaming...");
		Serial.printf("\n-------------------------------------\n\n");
	}
}

void printResult(FirebaseData &data) {

	if (data.dataType() == "int")
		Serial.println(data.intData());
	else if (data.dataType() == "float")
		Serial.println(data.floatData(), 5);
	else if (data.dataType() == "double")
		printf("%.9lf\n", data.doubleData());
	else if (data.dataType() == "boolean")
		Serial.println(data.boolData() == 1 ? "true" : "false");
	else if (data.dataType() == "string")
		Serial.println(data.stringData());
	else if (data.dataType() == "json") {
		Serial.println();

		FirebaseJson &json = data.jsonObject();
		//Print all object data
		Serial.println("Pretty printed JSON data:");

		String jsonStr;

		json.toString(jsonStr, true);

		Serial.println(jsonStr);
		Serial.println();
		Serial.println("Iterate JSON data:");
		Serial.println();

		size_t len = json.iteratorBegin();
		String key, value = "";

		int type = 0;
		for (size_t i = 0; i < len; i++) {
			json.iteratorGet(i, type, key, value);

			Serial.print(i);
			Serial.print(", ");
			Serial.print("Type: ");
			Serial.print(type == FirebaseJson::JSON_OBJECT ? "object" : "array");

			if (type == FirebaseJson::JSON_OBJECT) {
				Serial.print(", Key: ");
				Serial.print(key);
			}
			Serial.print(", Value: ");
			Serial.println(value);
		}
		json.iteratorEnd();
	} else if (data.dataType() == "array") {
		Serial.println();

		//get array data from FirebaseData using FirebaseJsonArray object
		FirebaseJsonArray &arr = data.jsonArray();
		//Print all array values

		Serial.println("Pretty printed Array:");

		String arrStr;
		arr.toString(arrStr, true);
		Serial.println(arrStr);
		Serial.println();
		Serial.println("Iterate array values:");
		Serial.println();

		for (size_t i = 0; i < arr.size(); i++) {
			Serial.print(i);
			Serial.print(", Value: ");

			FirebaseJsonData &jsonData = data.jsonData();

			//Get the result data from FirebaseJsonArray object
			arr.get(jsonData, i);
			if (jsonData.typeNum == FirebaseJson::JSON_BOOL)
				Serial.println(jsonData.boolValue ? "true" : "false");
			else if (jsonData.typeNum == FirebaseJson::JSON_INT)
				Serial.println(jsonData.intValue);
			else if (jsonData.typeNum == FirebaseJson::JSON_FLOAT)
				Serial.println(jsonData.floatValue);
			else if (jsonData.typeNum == FirebaseJson::JSON_DOUBLE)
				printf("%.9lf\n", jsonData.doubleValue);
			else if (jsonData.typeNum == FirebaseJson::JSON_STRING
					|| jsonData.typeNum == FirebaseJson::JSON_NULL
					|| jsonData.typeNum == FirebaseJson::JSON_OBJECT
					|| jsonData.typeNum == FirebaseJson::JSON_ARRAY)
				Serial.println(jsonData.stringValue);
		}
	} else if (data.dataType() == "blob") {

		Serial.println();

		for (int i = 0; i < data.blobData().size(); i++) {
			if (i > 0 && i % 16 == 0)
				Serial.println();

			if (i < 16)
				Serial.print("0");

			Serial.print(data.blobData()[i], HEX);
			Serial.print(" ");
		}
		Serial.println();
	} else if (data.dataType() == "file") {

		Serial.println();

		File file = data.fileStream();
		int i = 0;

		while (file.available()) {
			if (i > 0 && i % 16 == 0)
				Serial.println();

			int v = file.read();

			if (v < 16)
				Serial.print("0");

			Serial.print(v, HEX);
			Serial.print(" ");
			i++;
		}
		Serial.println();
		file.close();
	} else {
		Serial.println(data.payload());
	}
}

void printResult(StreamData &data) {

	if (data.dataType() == "int")
		Serial.println(data.intData());
	else if (data.dataType() == "float")
		Serial.println(data.floatData(), 5);
	else if (data.dataType() == "double")
		printf("%.9lf\n", data.doubleData());
	else if (data.dataType() == "boolean")
		Serial.println(data.boolData() == 1 ? "true" : "false");
	else if (data.dataType() == "string" || data.dataType() == "null")
		Serial.println(data.stringData());
	else if (data.dataType() == "json") {
		Serial.println();
		FirebaseJson *json = data.jsonObjectPtr();
		//Print all object data
		Serial.println("Pretty printed JSON data:");
		String jsonStr;
		json->toString(jsonStr, true);
		Serial.println(jsonStr);
		Serial.println();
		Serial.println("Iterate JSON data:");
		Serial.println();
		size_t len = json->iteratorBegin();
		String key, value = "";
		int type = 0;
		for (size_t i = 0; i < len; i++) {
			json->iteratorGet(i, type, key, value);
			Serial.print(i);
			Serial.print(", ");
			Serial.print("Type: ");
			Serial.print(
					type == FirebaseJson::JSON_OBJECT ? "object" : "array");
			if (type == FirebaseJson::JSON_OBJECT) {
				Serial.print(", Key: ");
				Serial.print(key);
			}
			Serial.print(", Value: ");
			Serial.println(value);
		}
		json->iteratorEnd();
	} else if (data.dataType() == "array") {
		Serial.println();
		//get array data from FirebaseData using FirebaseJsonArray object
		FirebaseJsonArray *arr = data.jsonArrayPtr();
		//Print all array values
		Serial.println("Pretty printed Array:");
		String arrStr;
		arr->toString(arrStr, true);
		Serial.println(arrStr);
		Serial.println();
		Serial.println("Iterate array values:");
		Serial.println();

		for (size_t i = 0; i < arr->size(); i++) {
			Serial.print(i);
			Serial.print(", Value: ");

			FirebaseJsonData *jsonData = data.jsonDataPtr();
			//Get the result data from FirebaseJsonArray object
			arr->get(*jsonData, i);
			if (jsonData->typeNum == FirebaseJson::JSON_BOOL)
				Serial.println(jsonData->boolValue ? "true" : "false");
			else if (jsonData->typeNum == FirebaseJson::JSON_INT)
				Serial.println(jsonData->intValue);
			else if (jsonData->typeNum == FirebaseJson::JSON_FLOAT)
				Serial.println(jsonData->floatValue);
			else if (jsonData->typeNum == FirebaseJson::JSON_DOUBLE)
				printf("%.9lf\n", jsonData->doubleValue);
			else if (jsonData->typeNum == FirebaseJson::JSON_STRING
					|| jsonData->typeNum == FirebaseJson::JSON_NULL
					|| jsonData->typeNum == FirebaseJson::JSON_OBJECT
					|| jsonData->typeNum == FirebaseJson::JSON_ARRAY)
				Serial.println(jsonData->stringValue);
		}
	} else if (data.dataType() == "blob") {

		Serial.println();

		for (int i = 0; i < data.blobData().size(); i++) {
			if (i > 0 && i % 16 == 0)
				Serial.println();

			if (i < 16)
				Serial.print("0");

			Serial.print(data.blobData()[i], HEX);
			Serial.print(" ");
		}
		Serial.println();
	} else if (data.dataType() == "file") {

		Serial.println();

		File file = data.fileStream();
		int i = 0;

		while (file.available()) {
			if (i > 0 && i % 16 == 0)
				Serial.println();

			int v = file.read();

			if (v < 16)
				Serial.print("0");

			Serial.print(v, HEX);
			Serial.print(" ");
			i++;
		}
		Serial.println();
		file.close();
	}
}
