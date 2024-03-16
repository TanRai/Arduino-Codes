#include <ArduinoJson.h>
#include <ArduinoJson.hpp>
#include <Servo.h>
#include "DHT.h"

#define DHTPIN 13
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

Servo s1;

#define FIREPIN 3

#define RELAYPIN1 4

#define RELAYPIN2 5

#define SERVOPIN 6

#define DOORPIN 7

#define PIRPIN1 8

#define PIRPIN2 12

#define BUZZERPIN 10


#define STEPPER_PIN_1 14
#define STEPPER_PIN_2 15
#define STEPPER_PIN_3 16
#define STEPPER_PIN_4 17
int step_number = 0;

bool rollerLocal = false;
bool motionLocal = false;
bool securityLocal = false;


void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  while (!Serial1.available());
  Serial.println(Serial1.readString());
  Serial.println("Began operation Arduino");
  s1.write(0);
  dht.begin();
  pinMode(FIREPIN, INPUT);
  pinMode(RELAYPIN1, OUTPUT);
  pinMode(RELAYPIN2, OUTPUT);
  digitalWrite(RELAYPIN1, HIGH);
  digitalWrite(RELAYPIN2, HIGH);
  s1.attach(SERVOPIN);
  pinMode(DOORPIN, INPUT_PULLUP);
  pinMode(STEPPER_PIN_1, OUTPUT);
  pinMode(STEPPER_PIN_2, OUTPUT);
  pinMode(STEPPER_PIN_3, OUTPUT);
  pinMode(STEPPER_PIN_4, OUTPUT);
  pinMode(PIRPIN1, INPUT);
  pinMode(PIRPIN2, INPUT);
  pinMode(BUZZERPIN, OUTPUT);
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);
  delay(2000);
}

void loop() {
  DynamicJsonBuffer jBuffer;
  JsonObject& root = jBuffer.createObject();
  int h = dht.readHumidity();
  int t = dht.readTemperature();
  int sensorValue = digitalRead(FIREPIN);
  bool fire;
  if (sensorValue == LOW) {
    fire = true;
  } else {
    fire = false;
  }
  root["temperature"] = t;
  root["humidity"] = h;
  root["fire"] = fire;
  String door;
  sensorValue = digitalRead(DOORPIN);
  if (sensorValue == HIGH) {
    door = "Unlocked";
  } else {
    door = "Locked";
  }
  root["door"] = door;
  String json;
  root.printTo(json);
  setJSON("Arduino", json);

  String android = getJSON("Android");
  DynamicJsonBuffer jb(100);
  JsonObject& obj = jb.parseObject(android);
  if (obj.success()) {
    Serial.println("Success");
    bool light1 = obj["light1"];
    bool light2 = obj["light2"];
    bool garage = obj["garage"];
    bool roller = obj["roller"];
    bool motionMode = obj["motionMode"];
    bool securityMode = obj["securityMode"];
    if (securityMode) {
      Serial.println("IN SECURITY");
      if(securityLocal != securityMode){
        securityLocal = true;
        digitalWrite(RELAYPIN1, HIGH);
        digitalWrite(RELAYPIN2, HIGH);
        s1.write(0);
        if(rollerLocal == true){
          rollerLocal = false;
          blindsClose();
        }
      }
      int pir1 = digitalRead(PIRPIN1);
      int pir2 = digitalRead(PIRPIN2);
      if(pir1 || pir2){
        setBool("intruder",true);
        tone(BUZZERPIN, 1000);
      }

    } else if (motionMode) {
      Serial.println("IN MOTION");
      if(motionLocal != motionMode){
        motionLocal = true;
        digitalWrite(RELAYPIN1, HIGH);
        digitalWrite(RELAYPIN2, HIGH);
      }
      int pir1 = digitalRead(PIRPIN1);
      int pir2 = digitalRead(PIRPIN2);
      if(pir1){
        digitalWrite(RELAYPIN1, LOW);
      }
      else{
        digitalWrite(RELAYPIN1, HIGH);
      }

      if(pir2){
        digitalWrite(RELAYPIN2, LOW);
      }
      else{
        digitalWrite(RELAYPIN2, HIGH);
      }


      if (garage) {
        s1.write(90);
      } else {
        s1.write(0);
      }

      if (roller == true && roller != rollerLocal) {
        blindsOpen();
        rollerLocal = roller;
      } else if (roller == false && roller != rollerLocal) {
        blindsClose();
        rollerLocal = roller;
      }
    } else {
      Serial.println("IN DEFAULT");
      if (light1) {
        digitalWrite(RELAYPIN1, LOW);
      } else {
        digitalWrite(RELAYPIN1, HIGH);
      }

      if (light2) {
        digitalWrite(RELAYPIN2, LOW);
      } else {
        digitalWrite(RELAYPIN2, HIGH);
      }

      if (garage) {
        s1.write(90);
      } else {
        s1.write(0);
      }

      if (roller == true && roller != rollerLocal) {
        blindsOpen();
        rollerLocal = roller;
      } else if (roller == false && roller != rollerLocal) {
        blindsClose();
        rollerLocal = roller;
      }
    }
    if(securityLocal == true && securityMode == false){
      setBool("intruder",false);
      noTone(BUZZERPIN);
    }
    motionLocal = motionMode;
    securityLocal = securityMode;

  } else {
    Serial.println("Fail");
  }
}


String getJSON(String path) {
  Serial1.print("&getJSON*" + path + "#");
  while (!Serial1.available())
    ;
  String temp = Serial1.readString();
  return temp;
}

int getInt(String path) {
  Serial1.print("&getInt*" + path + "#");
  while (!Serial1.available())
    ;
  String temp = Serial1.readString();
  return temp.toInt();
}
float getFloat(String path) {
  Serial1.print("&getFloat*" + path + "#");
  while (!Serial1.available())
    ;
  String temp = Serial1.readString();
  return temp.toFloat();
}
String getString(String path) {
  Serial1.print("&getString*" + path + "#");
  while (!Serial1.available())
    ;
  String temp = Serial1.readString();
  return temp;
}
bool getBool(String path) {
  Serial1.print("&getBool*" + path + "#");
  while (!Serial1.available())
    ;
  String temp = Serial1.readString();
  if (temp.equals("1"))
    return true;
  return false;
}
void setInt(String path, int temp) {
  Serial1.print("&setInt*" + path + "*" + temp + "#");
  while (!Serial1.available())
    ;
  Serial1.readString();
}
void setFloat(String path, float temp) {
  Serial1.print("&setFloat*" + path + "*" + temp + "#");
  while (!Serial1.available())
    ;
  Serial1.readString();
}
void setString(String path, String temp) {
  Serial1.print("&setString*" + path + "*" + temp + "#");
  while (!Serial1.available())
    ;
  Serial1.readString();
}
void setJSON(String path, String temp) {
  Serial1.print("&setJSON*" + path + "*" + temp + "#");
  while (!Serial1.available())
    ;
  Serial1.readString();
}
void setBool(String path, bool temp) {
  if (temp == true) {
    Serial1.print("&setBool*" + path + "*true#");
  } else {
    Serial1.print("&setBool*" + path + "*false#");
  }
  while (!Serial1.available())
    ;
  Serial1.readString();
}


void testJSON() {
  DynamicJsonBuffer jBuffer;
  JsonObject& root = jBuffer.createObject();

  root["day"] = "Monday";
  int temp = 101;
  int humidity = 53;
  root["temperature"] = temp;
  root["humidity"] = humidity;
  String abc;
  root.printTo(abc);
  setJSON("Arduino", abc);
}

void OneStep(bool dir) {
  if (dir) {
    switch (step_number) {
      case 0:
        digitalWrite(STEPPER_PIN_1, HIGH);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 1:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, HIGH);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 2:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, HIGH);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 3:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, HIGH);
        break;
    }
  } else {
    switch (step_number) {
      case 0:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, HIGH);
        break;
      case 1:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, HIGH);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 2:
        digitalWrite(STEPPER_PIN_1, LOW);
        digitalWrite(STEPPER_PIN_2, HIGH);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
        break;
      case 3:
        digitalWrite(STEPPER_PIN_1, HIGH);
        digitalWrite(STEPPER_PIN_2, LOW);
        digitalWrite(STEPPER_PIN_3, LOW);
        digitalWrite(STEPPER_PIN_4, LOW);
    }
  }
  step_number++;
  if (step_number > 3) {
    step_number = 0;
  }
}

void blindsOpen() {
  for (int i = 0; i < 4096; i++) {
    OneStep(true);
    delay(3);
  }
}
void blindsClose() {
  for (int i = 0; i < 4096; i++) {
    OneStep(false);
    delay(3);
  }
}

// RELAY LOW IS ON AND HIGH IS OFF  (OPPOSITE)
// DOOR HIGH IS UNLOCKED AND LOW IS LOCKED

// PIR

// int sensorValue = digitalRead(8);
// if(sensorValue == HIGH){
//   Serial.println("DETECTED");
// }
// else{
//   Serial.println("NONE");
// }
