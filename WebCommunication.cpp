/*
 * WebCommunication.c
 *
 *  Created on: 2021 Mar 10
 *      Author: kayro
 */

#include "WebCommunication.h"

FlagsCommunication flagsCommunication;

FirebaseData fbdo1;
FirebaseData fbdo2;

uint8_t regDelayTryConnection		= DELAY_TRY_CONNECTION,				//tempo para tentar uma nova conexão com a internet, caso não tenha efetuada uma primeira conexão
//		failureCount = 0,
		contCommunicationFailure 	= COUNT_COMM_FAILURE;

uint8_t datasFirebaseNumeric[LENGTH_DATA_EEPROM_NUMERIC] = {VALUE_ERRO, VALUE_FAVORITE, VALUE_HARD_RESET, VALUE_TIME_HARD_RESET},
		datasFirebaseNumericLocal[LENGTH_DATA_EEPROM_NUMERIC];

const char *ssidAP = "WatchdogRouter";
const char *passwordAP = "";


String	ssidST	= "",
		passwordST = "";


void loadVariablesCommunication(){

	//Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
	fbdo1.setBSSLBufferSize(1024, 1024);

	//Set the size of HTTP response buffers in the case where we want to work with large data.
	fbdo1.setResponseSize(1024);

	//Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
	fbdo2.setBSSLBufferSize(1024, 1024);

	//Set the size of HTTP response buffers in the case where we want to work with large data.
	fbdo2.setResponseSize(1024);
}


void configVariableControlCommunication(bool value){

	regDelayTryConnection = DELAY_TRY_CONNECTION;

	flagsCtrlSystem.flgBit.flgCommunicationOk = value;
//	flagsCtrlSystem.flgBit.flgRestarWifi = !value;
//	flagsCtrlSystem.flgBit.flgSystemStart = value;
	flagsCommunication.flgBit.flgDelayTryConnectionEn = !value;

	flagsCommunication.flgBit.flgDelayTryConnectionOk = 0;
}

//void changeFlagsCommunicationFirebase(bool status){
//
//	flagsCommunication.flgBit.flgFailureFirebase = !status;			//Falha no Firebase = true
//	flagsCommunication.flgBit.flgFirebaseFirsComm = status;			//Em falha = false
//}

void configurationWifi(uint8_t mode, uint8_t tryConnection){

	uint8_t attempt = 0;

//	updateErrorFlag();
	_delay(BASETIME);
	switch(mode){
		case WIFI_STA:

//			updateErrorFlag();
			WiFi.mode(WIFI_STA);
			_delay(BASETIME);
			WiFi.begin(ssidST.c_str(), passwordST.c_str());

			flagsCommunication.flgBit.flgTryingConnection = 0;			//desabilita a verificacao durante a interrupcao

			while (WiFi.status() != WL_CONNECTED && attempt <= tryConnection) {
				attempt++;
				Serial.print(".");
//				updateErrorFlag();
				_delay(BASETIME_2);
			}
			Serial.println();

			if(attempt < tryConnection){								//true se conexão bem sucedida
				configVariableControlCommunication(true);

				flagsCommunication.flgBit.flgTryingConnection = 1;		//habilita a verificacao após a interrupcão se a conexão for bem sucedida
				flagsCtrlSystem.flgBit.flgSystemStart = 1;
				WiFi.setAutoReconnect(true);
			}
			else{
				configVariableControlCommunication(false);
			}

		break;

		case WIFI_AP:
			//TODO - Verificar depois
//
//			flagsCommunication.flgBit.flgTryingConnection = 0;			//desabilita a verificacao durante a interrupcao
//
//			//set-up the custom IP address
//
//			updateErrorFlag();
//			WiFi.mode(WIFI_AP);
//
//			_delay(BASETIME);
//
//			WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
//			WiFi.softAP(ssidAP, passwordAP);
//			updateErrorFlag();
//			_delay(BASETIME);
//
//			ServerStar();
		break;
	}
}

//Testa a conexao com wifi
bool wifiConnectionStatus(){

	if (WiFi.status() == WL_CONNECTED){

		contCommunicationFailure = COUNT_COMM_FAILURE;
		return true;
	}else{
		if (!--contCommunicationFailure){								//caso uma certa quantidade de tentativas nao seja bem sucedida
			contCommunicationFailure = COUNT_COMM_FAILURE;				//uma conexao completa sera tentada

//			flagsCommunication.flgBit.flgRestarWifi = 1;
//
//			changeFlagsCommunicationFirebase(false);					//habilita uma nova conexão com o Firebase

			configVariableControlCommunication(false);

			flagsCommunication.flgBit.flgTryingConnection = 0;			//habilita a verificacao após a interrupcão se a conexão for bem sucedida
		}
		return false;
	}

//	if(WiFi.status() == WL_CONNECTED){
//		return true;
//	}else{
//		return false;
//	}
}

bool firebaseConnection(){

	//Iniciar Firebase
	Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
	Firebase.reconnectWiFi(true);

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

//void splitDataBase(FirebaseObject *obj){
//
//	for (int i = 0; i < LENGTH_PATH; i++){
//		*(datasFirebase+i) = (*obj).getInt(*(pathFirebase+i));
//	}
//}
//
//bool isCommunicationFirebase(){
//
//	if (!Firebase.success()){
//
//		if (failureCount++ > FAILURE_DB){
//
//			flagsCommunication.flgBit.flgFailureFirebase = 1;
//			failureCount = 0;
//		}
//		return false;
//	}else{
//		flagsCommunication.flgBit.flgFailureFirebase = 0;
//		failureCount = 0;
//
//		return true;
//	}
//}
//
//
//int getDataFirebase(String defaultPath, String objectPath, bool mode){
//
//	FirebaseObject obj = Firebase.get(defaultPath);
//
//	if (isCommunicationFirebase()){
//		int objectValue = obj.getInt(objectPath);
//
//		if (mode != DATA_LOCAL || objectValue != 0){
//			splitDataBase(&obj);
//			flagsCommunication.flgBit.flgReceivedData = 1;
//		}
//		return objectValue;
//	}
//
//	flagsCommunication.flgBit.flgReceivedData = 0;
//	return -1;
//}
//
//void getDataFirebaseEvent(){
//
//	if(isCommunicationFirebase()){
//		if (Firebase.available()){
//			FirebaseObject obj = Firebase.readEvent();
//
//			if (!obj.getString("path").equals("")){
//				getDataFirebase(DEFAULT_PATH, ALL_OBJECT_PATH, DATA_GLOBAL);
//			}
//		}
//	}else{
//
//		Firebase.stream(DEFAULT_PATH);  									//Enter mode of verification by event
//	}
//	_yield();
//}
//
//void refreshFirebase(String path, String objectPath, int value){
//
//	if(!flagsCommunication.flgBit.flgFailureFirebase){
//		String temp = "/";
//		temp += objectPath;
//
//		Firebase.setInt(path+temp, value);
//	}else{
//		flagsCommunication.flgBit.flgStuckDataFirebase = 1;
//	}
//}
//
//bool firebaseCommStatus(){
//
//	FirebaseObject obj = Firebase.get(OBJECT_DESCRIPTION);
//
//	return isCommunicationFirebase();
//}
//
////void parameterError(uint8_t option) {
////	switch(option){
////		case ERROR_LENGTH_SSID:
////			server.send (404, "text/plain", "SSID com mais de 35 caractere");
////		break;
/////*
////		case 1:
////			server.send (404, "text/plain", "Password com menos de 8 caractere");
////		break;
////*/
////		case ERROR_LENGTH_PASSWORD:
////			server.send (404, "text/plain", "Password com mais de 35 caractere");
////		break;
////	}
////}
////
////void errComm() {
////	webSend(404, "text/plain", "Erro no envio");
////}
////
////bool configLocalWifi () {
////	String ssidLocal = server.arg("ssid"),
////	passwordLocal = server.arg("password");
////
////	if (ssidLocal.length() < LENGTH_SSID)
////		writeString(ADDRESS_SSID, ssidLocal+"##");									//## é o verificador de sucesso
////	else{
////		parameterError(ERROR_LENGTH_SSID);
////		return false;
////	}
////
////	if (passwordLocal.length() < LENGTH_PASSWORD)
////		writeString(ADDRESS_PASSWORD, passwordLocal+"##");							//## é o verificador de sucesso
////	else{
////		parameterError(ERROR_LENGTH_PASSWORD);
////		return false;
////	}
////
////	flagsCtrlGeral.flagsCtrlGeralBit.flgConfigurationOk = 1;
////	server.send (200, "text/plain", "SSID: " + ssidLocal + " Password: " + passwordLocal);
////	_delay(DELAY_FAST_SWITCHING);																				//delay para aguardar atualizar a EEPROM
////	return true;
////}
////
////void ServerStar(){
////
////	server.on ("/configWifi", configLocalWifi);
////	server.onNotFound(errComm);
////	server.begin();
////}
////
////void webSend(int code, String content_type, String content){
////	server.send(code, content_type, content);
////}

bool tryGetAuthentication(){

	uint8_t repeatAuthentication = 0;
	bool success = false;

	do{
		ssidST = "";
		passwordST = "";
		success = getAuthenticationEEPROM(&ssidST, &passwordST);
		_delayMicroseconds(100);
	}while(!success && ++repeatAuthentication < 5);

	return success;
}

////void listenServer(){
////
////	//teste
////	//http://172.42.1.42/configWifi?ssid=teste&password=12345
////
////	while(!flagsCtrlGeral.flagsCtrlGeralBit.flgConfigurationOk){
////		server.handleClient();
////
////		if (switchAnalizeIO() == 0)
////			flagsCtrlGeral.flagsCtrlGeralBit.flgConfigurationOk = 1;
////
////		_yield();
////	}
////	flagsCtrlGeral.flagsCtrlGeralBit.flgConfigurationEn = 0;
////
////	flagsCommunication.flgBit.flgAuthenticationError = tryGetAuthentication();
////
////	flagsCtrlSystem.flgBit.flgSystemStart = 0;	// <- verificar
////	configurationWifi(WIFI_STA, WAIT_COMM);
////	firebaseConnection();
////
////	flagsCtrlGeral.flagsCtrlGeralBit.flgConfigurationOk = 0;
////	_delay(LOADTIME);
////}
//
void communicationStatusMonitoring(){

	if (flagsCtrlSystem.flgBit.flgCommunicationOk){											//Comunicação ok = True
//		if (!flagsCommunication.flgBit.flgFailureFirebase){									//Comunicação com Firebase ok
//			if (flagsCommunication.flgBit.flgGetFirebaseDelayMin){							//Realiza uma consulta completa no firebase em um período de tempo prédefinido
//				if (getDataFirebase(DEFAULT_PATH, OBJECT_PATH, DATA_LOCAL) >= 0){			//Pega os dados
//
//					//verificar necessidade de atualizar os dados
//					flagsCommunication.flgBit.flgGetFirebaseDelayMin = 0;					//inicia nova contagem caso a consulta tenha sido bem sucedida
//				}
//			}else{
//				if (flagsCommunication.flgBit.flgGetFirebaseOk){							//Se não chegou o  momento de realizar uma busca completa, realiza uma verificação
//
//					flagsCommunication.flgBit.flgGetFirebaseOk = 0;							//apenas de novos eventos.
//					getDataFirebaseEvent();
//					flagsCommunication.flgBit.flgGetFirebaseDelay = 1;						//Verifica novos eventos.
//				}
//			}
//		}else{
//			if (flagsCommunication.flgBit.flgGetFirebaseOk){								//Se hover problema na comunicação com o firebase, tenta uma nova conexão.
//
//				flagsCommunication.flgBit.flgGetFirebaseOk = 0;								//Em caso de falta de conexão com firebase, limpa o status de todas as caixas, fica funcionado apenas local
//				if (!flagsCommunication.flgBit.flgFirebaseFirsComm){						//Se o problema ocorreu na inicialização realiza o processo do início
//
//					firebaseConnection();
//				}else{
//
//					firebaseCommStatus();													//Verifica o status da conexão com firebase
//				}
//
//				flagsCommunication.flgBit.flgGetFirebaseDelay = 1;
//			}
//		}
	}else{
		if (!flagsCtrlSystem.flgBit.flgSystemStart){										//Busca nova conexão com a internet.

			if(!flagsCtrlSystem.flgBit.flgAuthenticationError){								//Falha na leitura da EEPROM = false

				flagsCtrlSystem.flgBit.flgAuthenticationError = tryGetAuthentication();
			}

			if(flagsCtrlSystem.flgBit.flgAuthenticationError){

				if(flagsCommunication.flgBit.flgDelayTryConnectionOk){

					flagsCommunication.flgBit.flgDelayTryConnectionOk = 0;

					configurationWifi(WIFI_STA, WAIT_COMM_FAST);

					if (flagsCtrlSystem.flgBit.flgCommunicationOk){
						firebaseConnection();
					}
				}
			}
		}
	}
}

////void updateErrorFirebase(){
////
////	if (flagsCtrlGeral.flagsCtrlGeralBit.flgSendErrorFirebase){
////		flagsCtrlGeral.flagsCtrlGeralBit.flgSendErrorFirebase = 0;
////		refreshFirebase(DEFAULT_PATH, PATH_ERROR, errorOld);
////	}
////}
