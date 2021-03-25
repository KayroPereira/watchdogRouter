/*
 * FirebaseFunctions.cpp
 *
 *  Created on: 2021 Mar 13
 *      Author: kayro
 */

#include "FirebaseFunctions.h"

FlagsFirebase flagsFirebase;

FirebaseData fbdo1;
FirebaseData fbdo2;

FirebaseJson jsonBuffer;

void loadVariablesFirebase(){

	//Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
	fbdo1.setBSSLBufferSize(1024, 1024);

	//Set the size of HTTP response buffers in the case where we want to work with large data.
	fbdo1.setResponseSize(1024);

	//Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
	fbdo2.setBSSLBufferSize(1024, 1024);

	//Set the size of HTTP response buffers in the case where we want to work with large data.
	fbdo2.setResponseSize(1024);
}

bool firebaseConnection(){

	//Iniciar Firebase
	Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
	Firebase.reconnectWiFi(true);

//	Firebase.setStreamCallback(fbdo1, streamCallback, streamTimeoutCallback);

//	if (!Firebase.beginStream(fbdo1, PATH_UPDATE_SHARE)) {
//		Serial.println("------------------------------------");
//		Serial.println("Can't begin stream connection...");
//		Serial.println("REASON: " + fbdo1.errorReason());
//		Serial.println("------------------------------------");
//		Serial.println();
//	}

	if (!Firebase.beginStream(fbdo1, PATH_UPDATE_SHARE)) {
		Serial.println("------------------------------------");
		Serial.println("Can't begin stream connection...");
		Serial.println("REASON: " + fbdo1.errorReason());
		Serial.println("------------------------------------");
		Serial.println();
	}

	Firebase.setStreamCallback(fbdo1, streamCallback, streamTimeoutCallback);

	//TODO - Implementar mais tarde
//	if(firebaseCommStatus()){
//
//		changeFlagsCommunicationFirebase(true);
//
//		getDataFirebase(DEFAULT_PATH, ALL_OBJECT_PATH, DATA_GLOBAL);		//pega dados Firebase
//		_delay(DELAY_FAST_SWITCHING);
//
//		Firebase.stream(DEFAULT_PATH); 										//Enter mode of verification by event
//
//		flagsCommunication.flgBit.flgGetFirebaseDelay = 1;
//
//	} else {
//		changeFlagsCommunicationFirebase(false);
//		return false;
//	}

	return true;
}

void streamCallback(StreamData data) {

	//TODO - Remover
	Serial.printf("\n-------------------------------------\n         Download Firebase            \n-------------------------------------\n");

//	Serial.println("Stream Data1 available...");
//	Serial.println("STREAM PATH: " + data.streamPath());
//	Serial.println("EVENT PATH: " + data.dataPath());
//	Serial.println("DATA TYPE: " + data.dataType());
//	Serial.println("EVENT TYPE: " + data.eventType());
//	Serial.print("VALUE: ");
//	printResult(data);
//	Serial.printf("\n-------------------------------------\n\n");

	flagsFirebase.flgBit.flgFirebaseStartOk = 1;

	splitDataJsonFirebase(data);

	Serial.printf("\n\n------------------------\n\n");
	for(int i = 0; i < LENGTH_DATA_NUMERIC_EEPROM; i++){
		Serial.printf("%s: %u\n", PATH_FIREBASE[LIST_DATA_NUMERIC_EEPROM[i]], datasFirebaseNumeric[i]);
	}
}

#define TIME_OUT_MAX	2

int contTimeOut = TIME_OUT_MAX;

void streamTimeoutCallback(bool timeout) {
	if (timeout) {
		Serial.printf("\n\n-------------------------------------\n\n");
		Serial.println("Stream timeout, resume streaming...");
		Serial.printf("\n-------------------------------------\n\n");
	}else{
		Serial.printf("\n\n-------------------------------------\n\n");
		Serial.println("Stream timeout, NOK");
		Serial.printf("\n-------------------------------------\n\n");

//		if (--contTimeOut <= 0){
//			contTimeOut = TIME_OUT_MAX;
//
//			Firebase.setStreamCallback(fbdo1, streamCallback, streamTimeoutCallback);
//
//			Serial.println("Stream timeout, REFRESH");
		}

//		if(Firebase.getInt(fbdo1, PATH_UPDATE_FLG)){
//
////			FirebaseJson &jsonTest = fbdo1.jsonObject();
//			int flg = fbdo1.intData();
//
//			if(flg > 0){
//				Serial.println("Flag OK");
//			}else{
//				Serial.println("Flag NOK");
//			}
//
//		}
//
//			Firebase.setStreamCallback(fbdo1, streamCallback, streamTimeoutCallback);

//		if (!Firebase.beginStream(fbdo1, PATH_UPDATE_SHARE)) {
//			Serial.println("------------------------------------");
//			Serial.println("Can't begin stream connection...");
//			Serial.println("REASON: " + fbdo1.errorReason());
//			Serial.println("------------------------------------");
//			Serial.println();
//		}
//	}
}

bool updateFirebase(FirebaseData &fbdo, String path, FirebaseJson &json){

	return Firebase.updateNodeSilent(fbdo, path, json);
//	return Firebase.updateNode(fbdo, path, json);
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

void updateAllDataFirebase(){
	FirebaseJson json;

	for(uint8_t i = 0; i < SIZE_PATH_TEXT; i++){
		json.set(PATH_FIREBASE[i], 0);
	}

	updateFirebase(fbdo2, PATH_UPDATE_SHARE, json);
}

void addDataBufferFirebase(const String path, int value){

	jsonBuffer.set(path, value);
}

void printResult(FirebaseData &data) {

	Serial.printf("\n\n--------- printResult ---------\n");

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
		Serial.printf("\n\n------------Pretty printed JSON data:---------\n");

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

		for (uint16_t i = 0; i < data.blobData().size(); i++) {
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
			Serial.print(type == FirebaseJson::JSON_OBJECT ? "object" : "array");

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

		for (uint16_t i = 0; i < data.blobData().size(); i++) {
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

//sint8_t testePrint(String keyToFind, const char pathFirebase [][SIZE_PATH_TEXT], uint8_t len){
//
//	Serial.printf("\n\n--------------------testePrint---------------\n\n");
//
//	for(uint8_t i = 0; i < len; i++){
//		Serial.println(pathFirebase[i]);
//	}
//
//	for(uint8_t i = 0; i < len; i++){
//
//		if(keyToFind.equals(pathFirebase[i])){
//			return i;
//		}
//	}
//
//	return -1;
//}



//delay(2000);
//	Serial.printf("\n\nResultado: ");
//	Serial.print(testePrint("hardReset", PATH_FIREBASE, LENGTH_PATH_FIREBASE));


sint8_t findKeyPathFirebase(String keyToFind, const char pathFirebase [][SIZE_PATH_TEXT], uint8_t len){

	for(uint8_t i = 0; i < len; i++){

		if(keyToFind.equals(pathFirebase[i])){
			return i;
		}
	}
	return -1;
}

//void addNewDataBufferFirebaseLocal(String key, String value){
//
//	if(key[0] == '/')
//		key.remove(0, 1);
//
//	int keyFound = findKeyPathFirebase(key, PATH_FIREBASE, LENGTH_PATH_FIREBASE);
//
//	Serial.printf("\n\n----------addNewDataBufferFirebaseLocal\n\nkey: %s\nvalue: %s\nkeyFound: %d\n\n------------------", key.c_str(), value.c_str(), keyFound);
//
//	if(keyFound > -1 && keyFound < LENGTH_DATA_TEXT_EEPROM){
//
//		//FIXME - desenvolver para string
//		keyFound = -2;
//	}else if(keyFound >= LENGTH_DATA_TEXT_EEPROM && keyFound < (LENGTH_DATA_TEXT_EEPROM + LENGTH_DATA_NUMERIC_EEPROM)){
//		datasFirebaseNumeric[keyFound-LENGTH_DATA_TEXT_EEPROM] = value.toInt();
//	}
//}

void addNewDataBufferFirebaseLocal(String key, String value){

	if(key[0] == '/')
		key.remove(0, 1);

	int keyFound = findKeyPathFirebase(key, PATH_FIREBASE, LENGTH_PATH_FIREBASE);

	Serial.printf("\n----------addNewDataBufferFirebaseLocal\n\nkey: %s\nvalue: %s\nkeyFound: %d\n------------------", key.c_str(), value.c_str(), keyFound);

	if(keyFound > -1 && keyFound < LENGTH_DATA_TEXT_EEPROM){

		//FIXME - desenvolver para string
		keyFound = -2;
	}else if(keyFound >= LENGTH_DATA_TEXT_EEPROM && keyFound < (LENGTH_DATA_TEXT_EEPROM + LENGTH_DATA_NUMERIC_EEPROM)){
		datasFirebaseNumeric[keyFound-LENGTH_DATA_TEXT_EEPROM] = value.toInt();
	}
}

void splitDataJsonFirebase(StreamData &data) {

//	if (data.dataType() == "int")
//			Serial.println(data.intData());
//		else if (data.dataType() == "float")
//			Serial.println(data.floatData(), 5);
//		else if (data.dataType() == "double")
//			printf("%.9lf\n", data.doubleData());
//		else if (data.dataType() == "boolean")
//			Serial.println(data.boolData() == 1 ? "true" : "false");
//		else if (data.dataType() == "string" || data.dataType() == "null")
//			Serial.println(data.stringData());
//		else if (data.dataType() == "json") {
//

	if (data.dataType() == "int") {
		addNewDataBufferFirebaseLocal(data.dataPath(), (String) data.intData());
	}else if (data.dataType() == "json") {
		Serial.println();

		FirebaseJson *json = data.jsonObjectPtr();

		//Print all object data
		Serial.printf("\n\n-------------------splitDataJsonFirebase-------------------\n\n");

//		String jsonStr;
//
//		json->toString(jsonStr, true);
//
//		Serial.println(jsonStr);
//		Serial.println();
//		Serial.println("Iterate JSON data:");
//		Serial.println();


		size_t len = json->iteratorBegin();
		String key, value = "";
		int type = 0;

		for (size_t i = 0; i < len; i++) {
			json->iteratorGet(i, type, key, value);
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

			addNewDataBufferFirebaseLocal(key, value);

//			int keyFound = findKeyPathFirebase(key, PATH_FIREBASE, LENGTH_PATH_FIREBASE);
//			if(keyFound > -1 && keyFound < LENGTH_DATA_TEXT_EEPROM){
//
//				//FIXME - desenvolver para string
//				keyFound = -2;
//			}else if(keyFound >= LENGTH_DATA_TEXT_EEPROM && keyFound < (LENGTH_DATA_TEXT_EEPROM + LENGTH_DATA_NUMERIC_EEPROM)){
//				datasFirebaseNumeric[keyFound-LENGTH_DATA_TEXT_EEPROM] = value.toInt();
//			}
		}

		json->iteratorEnd();


	}

}

//void splitDataJsonFirebase(StreamData &data) {
//
////	if (data.dataType() == "int")
////			Serial.println(data.intData());
////		else if (data.dataType() == "float")
////			Serial.println(data.floatData(), 5);
////		else if (data.dataType() == "double")
////			printf("%.9lf\n", data.doubleData());
////		else if (data.dataType() == "boolean")
////			Serial.println(data.boolData() == 1 ? "true" : "false");
////		else if (data.dataType() == "string" || data.dataType() == "null")
////			Serial.println(data.stringData());
////		else if (data.dataType() == "json") {
////
//
//	if (data.dataType() == "int") {
//		addNewDataBufferFirebaseLocal(data.dataPath(), (String) data.intData());
//	}else if (data.dataType() == "json") {
//		Serial.println();
//
//		FirebaseJson *json = data.jsonObjectPtr();
//
//		//Print all object data
//		Serial.printf("\n\n-------------------splitDataJsonFirebase-------------------\n\n");
//
////		String jsonStr;
////
////		json->toString(jsonStr, true);
////
////		Serial.println(jsonStr);
////		Serial.println();
////		Serial.println("Iterate JSON data:");
////		Serial.println();
//
//		size_t len = json->iteratorBegin();
//		String key, value = "";
//		int type = 0;
//
//		for (size_t i = 0; i < len; i++) {
//			json->iteratorGet(i, type, key, value);
//			Serial.print(i);
//			Serial.print(", ");
//			Serial.print("Type: ");
//			Serial.print(type == FirebaseJson::JSON_OBJECT ? "object" : "array");
//
//			if (type == FirebaseJson::JSON_OBJECT) {
//				Serial.print(", Key: ");
//				Serial.print(key);
//			}
//			Serial.print(", Value: ");
//			Serial.println(value);
//
//			addNewDataBufferFirebaseLocal(key, value);
//
////			int keyFound = findKeyPathFirebase(key, PATH_FIREBASE, LENGTH_PATH_FIREBASE);
////			if(keyFound > -1 && keyFound < LENGTH_DATA_TEXT_EEPROM){
////
////				//FIXME - desenvolver para string
////				keyFound = -2;
////			}else if(keyFound >= LENGTH_DATA_TEXT_EEPROM && keyFound < (LENGTH_DATA_TEXT_EEPROM + LENGTH_DATA_NUMERIC_EEPROM)){
////				datasFirebaseNumeric[keyFound-LENGTH_DATA_TEXT_EEPROM] = value.toInt();
////			}
//		}
//
//		json->iteratorEnd();
//
//
//	}
////		else if (data.dataType() == "array") {
////		Serial.println();
////
////		//get array data from FirebaseData using FirebaseJsonArray object
////		FirebaseJsonArray *arr = data.jsonArrayPtr();
////
////		//Print all array values
////		Serial.println("Pretty printed Array:");
////
////		String arrStr;
////		arr->toString(arrStr, true);
////
////		Serial.println(arrStr);
////		Serial.println();
////		Serial.println("Iterate array values:");
////		Serial.println();
////
////		for (size_t i = 0; i < arr->size(); i++) {
////			Serial.print(i);
////			Serial.print(", Value: ");
////
////			FirebaseJsonData *jsonData = data.jsonDataPtr();
////
////			//Get the result data from FirebaseJsonArray object
////			arr->get(*jsonData, i);
////
////			if (jsonData->typeNum == FirebaseJson::JSON_BOOL)
////				Serial.println(jsonData->boolValue ? "true" : "false");
////			else if (jsonData->typeNum == FirebaseJson::JSON_INT)
////				Serial.println(jsonData->intValue);
////			else if (jsonData->typeNum == FirebaseJson::JSON_FLOAT)
////				Serial.println(jsonData->floatValue);
////			else if (jsonData->typeNum == FirebaseJson::JSON_DOUBLE)
////				printf("%.9lf\n", jsonData->doubleValue);
////			else if (jsonData->typeNum == FirebaseJson::JSON_STRING
////					|| jsonData->typeNum == FirebaseJson::JSON_NULL
////					|| jsonData->typeNum == FirebaseJson::JSON_OBJECT
////					|| jsonData->typeNum == FirebaseJson::JSON_ARRAY)
////				Serial.println(jsonData->stringValue);
////		}
////	}else if (data.dataType() == "blob") {
////
////		Serial.println();
////
////		for (uint16_t i = 0; i < data.blobData().size(); i++) {
////			if (i > 0 && i % 16 == 0)
////				Serial.println();
////
////			if (i < 16)
////				Serial.print("0");
////
////			Serial.print(data.blobData()[i], HEX);
////			Serial.print(" ");
////		}
////		Serial.println();
////	} else if (data.dataType() == "file") {
////
////		Serial.println();
////
////		File file = data.fileStream();
////		int i = 0;
////
////		while (file.available()) {
////			if (i > 0 && i % 16 == 0)
////				Serial.println();
////
////			int v = file.read();
////
////			if (v < 16)
////				Serial.print("0");
////
////			Serial.print(v, HEX);
////			Serial.print(" ");
////			i++;
////		}
////		Serial.println();
////		file.close();
////	}
//}
