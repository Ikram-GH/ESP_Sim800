#include <Arduino.h>
// #include <ArduinoOTA.h>
#define PS 4
#define KEY 19
#define NS 18
#define VIO 5


void powerUp();
void powerDown();
void gsmCheck();
uint8_t getGsmStat();
void flushSim();
bool sendAtCom(long timeout, char* atCom, char* Rep, char* Error, int nbRep);
bool gprsOn();

void setup() {
  Serial.begin(9600);
  Serial.println("Begun communication");
  pinMode(VIO, OUTPUT);//VIO
  digitalWrite(VIO, HIGH);
  pinMode(PS, INPUT);//sim Power Status
  pinMode(NS, INPUT);//sim Power Status
  powerDown();
  powerUp();
  // Serial2.begin(4800);
  // gsmCheck();
  // Serial.println("gsm O K");
  // gprsOn();
  Serial.println("power on ok");
}

void loop() {
  Serial.println("ok");
  delay(1500);
}

void powerUp(){
  if ((analogRead(PS) < 200)) {
    pinMode(KEY, OUTPUT);//PWR KEY
    digitalWrite(KEY, LOW);
    delay(1200);
    pinMode(KEY, INPUT_PULLUP);
    delay(400);
  }
}
void powerDown() {
  if ((analogRead(PS) > 200)) {
    pinMode(KEY, OUTPUT);//PWR KEY  
    digitalWrite(KEY, LOW);
    delay(1200);
    pinMode(KEY, INPUT_PULLUP);// Turn On the module
    delay(400);
  }
}
void gsmCheck(){
  uint8_t gsmStatInt=getGsmStat();
  while((gsmStatInt != 1)&&(gsmStatInt != 5)){
    gsmStatInt=getGsmStat();delay(1000);
  }
}
uint8_t getGsmStat() {
  flushSim();
  Serial2.println("AT+CREG?");
  String tempGSM = Serial2.readString();
  int ind1 = tempGSM.indexOf(',');
  String gsmStat = tempGSM.substring(ind1 + 1, ind1 + 2);
  return gsmStat.toInt();
}
void flushSim() {
  uint16_t timeoutlp = 0;
  while (timeoutlp++ < 40) {
    while (Serial.available()) {
      Serial2.read();
      timeoutlp = 0;  // If char was received reset the timer
    }
    delay(1);
  }
}
bool gprsOn() {
    sendAtCom(5000, "AT+CFUN=1", "OK", "ERROR", 5); //"AT+CFUN=1"
    sendAtCom(5000, "AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"", "OK", "OK", 5); //"AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\""
    if (sendAtCom(5000, "AT+SAPBR=1,1", "OK", "OK", 5)) { //"AT+SAPBR=1,1"
      if (sendAtCom(5000, "AT+CIICR", "OK", "ERROR", 5)) { //"AT+CIICR"
        if (sendAtCom(5000, "AT+CIFSR", ">", "ERROR", 5)) { //"AT+CIFSR"
          return true;
      } else { return false;}
    } else {return false;}
  } else {return false;}
}
bool sendAtCom(long timeout, char* atCom, char* Rep, char* Error, int nbRep) {
  flushSim();
  Serial2.setTimeout(timeout);
  for (uint16_t a = 0; a < strlen(atCom); a++)
  {
    Serial2.print(atCom[a]);
  } Serial2.println("");

  int compteur = 0;
  while ((!Serial2.findUntil(Rep, Error)) && (compteur < nbRep)) {
    flushSim();
      for (uint16_t a = 0; a < strlen(atCom); a++)
    {
      Serial2.print(atCom[a]);
    } Serial2.println("");
    compteur++;
    delay(50);
  }
  if (compteur <= nbRep)
  {
    return true;
  } else
  {
    return false;
  }
  Serial2.setTimeout(1000);
}




















































// //#include<ESPhttpupdate.h>
// #include "WiFi.h"
// #include <ESP32httpUpdate.h>
// #define TRUC_VERSION "0_4_99"
// #define THIS_DEVICE "ESP32-Feather"
// const char* ssid = "HUAWEI-8e4e";
// const char* password = "ifran123";
// //#define SPIFFS_VERSION "0_5_0"
// // THIS_DEVICE is set earlier depending on various compile-time defines 
// // which eventually define the hw type, e.g. #define THIS_DEVICE "d1_mini"
// const char* updateUrl = "http://192.168.1.4:1880/update/"THIS_DEVICE;
// // this is my raspberry Pi server, the 1880 is the default NODE-RED port
// // /update is the url I chose for the server to "listen" for, followed by the device type
// //bool actualUpdate(bool sketch=false);

// bool actualUpdate(bool sketch=false){
//     String msg;
//     t_httpUpdate_return ret;
     
//     ESPhttpUpdate.rebootOnUpdate(false);
//     if(sketch){
//       ret=ESPhttpUpdate.update(updateUrl,TRUC_VERSION);     // ***This is the line that "does the business"   
//     }
//     // else {
//     //   ret=ESPhttpUpdate.updateSpiffs(updateUrl,SPIFFS_VERSION);
//     // }
//     if(ret!=HTTP_UPDATE_NO_UPDATES){
//       if(ret==HTTP_UPDATE_OK){

//         Serial.printf("UPDATE SUCCEEDED");
//         return true;
//         }
//       else {
//         if(ret==HTTP_UPDATE_FAILED){

//          Serial.printf("Upgrade Failed");
//           }
//         }
//       }
//   return false;}
  
// void setup() {
 
//   Serial.begin(115200);
 
//   WiFi.begin(ssid, password);
 
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.println("Connecting to WiFi..");
//   }
 
//   Serial.println("Connected to the WiFi network");


// }
// void loop() { 
//   if(actualUpdate(true)) {ESP.restart();}
// }









// #include "WiFi.h"
// #include <WebServer.h>
// #include <AutoConnect.h>

// WebServer server;                          // Step #1
// AutoConnect portal;                               // Step #2
// AutoConnectUpdate update("192.168.100.11", 8000);  // Step #3

 
// const char* ssid = "HUAWEI-8e4e";
// const char* password = "ifran123";
 
// void setup() {
 
//   Serial.begin(115200);
 
//   WiFi.begin(ssid, password);
 
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.println("Connecting to WiFi..");
//   }
 
//   Serial.println("Connected to the WiFi network");

//   if (portal.begin()) {     // Step #4
//     update.attach(portal);  // Step #5
//   }
 
// }
 
// void loop() {
//    portal.handleClient();    // Step #6
// }







