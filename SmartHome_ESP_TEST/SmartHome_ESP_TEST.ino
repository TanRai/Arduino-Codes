#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "esptanrai-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "TCNX7wtzHUuTqNQwpX3RV4AwwHseBcndKaEgPmWW"
#define WIFI_SSID "Please_Hack_It"
#define WIFI_PASSWORD "killernet"

void setup() {
  Serial.begin(9600);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    //Serial.print(".");
    delay(500);
  }
  //Serial.println();
  //Serial.print("connected: ");
  //Serial.println(WiFi.localIP());
  Serial.println("1");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

String recievedString = "";
String str1 = "";
String str2 = "";
String str3 = "";
int strNum = 1;
bool stringReady = false;

void loop() {
  while(!Serial.available());
  while(Serial.available()){
    recievedString = Serial.readString();
    stringReady = true;
  }
  print("Recieved String = "+recievedString);
  if(stringReady){
    if(recievedString.startsWith("&") && recievedString.endsWith("#")){
      decodeString();
      if(str1.equals("setInt")){
        Firebase.setInt(str2, str3.toInt());
        Serial.print("success");
      }
      else if(str1.equals("setFloat")){
        Firebase.setFloat(str2, str3.toFloat());
        Serial.print("success");
      }
      else if(str1.equals("setString")){
        Firebase.setString(str2, str3);
        Serial.print("success");
      }
      else if(str1.equals("setBool")){
        if(str3.equals("true")){
          Firebase.setBool(str2, true);
          Serial.print("success");
        }
        else{
          Firebase.setBool(str2, false);
          Serial.print("success");
        }
      }
      else if(str1.equals("setJSON")){
        DynamicJsonBuffer jb(100);
        JsonObject& obj = jb.parseObject(str3);
        Firebase.set(str2,obj);
        Serial.print("success");
      }
      else if(str1.equals("getInt")){
        int temp = Firebase.getInt(str2);
        Serial.print(temp);
      }
      else if(str1.equals("getFloat")){
        float temp = Firebase.getFloat(str2);
        Serial.print(temp);
      }
      else if(str1.equals("getString")){
        String temp = Firebase.getString(str2);
        Serial.print(temp);
      }
      else if(str1.equals("getBool")){
        bool temp = Firebase.getBool(str2);
        Serial.print(temp);
      }
      else if(str1.equals("getJSON")){
        FirebaseObject object1 = Firebase.get(str2);
        JsonVariant variant = object1.getJsonVariant();
        variant.prettyPrintTo(Serial);
      }
      else{
        Firebase.pushString("errors", "Wrong decode");
        reset();
      }
    }
    else{
      Firebase.pushString("errors", "String wrong format");
      reset();
    }
    reset();
  }
}

void decodeString(){
  for(int i=1; recievedString[i]!='#';i++){
    if(recievedString[i]=='*'){
      strNum++;
    }
    else{
      if(strNum == 1){
        str1.concat(recievedString[i]);
      }
      else if(strNum == 2){
        str2.concat(recievedString[i]);
      }
      else if(strNum == 3){
        str3.concat(recievedString[i]);
      }
      else{
        Firebase.pushString("errors", "strNum invalid");
        reset();
      }
    }
  }
}

void reset(){
  recievedString = "";
  str1 = "";
  str2 = "";
  str3 = "";
  strNum = 1;
  stringReady = false;
}

void print(String str){
  Firebase.pushString("debug", str);
}




