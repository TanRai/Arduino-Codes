#include <ArduinoJson.h>
#include <ArduinoJson.hpp>

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  while(!Serial1.available());
  Serial.println(Serial1.readString());
  Serial.println("Began operation Arduino");
}

void loop() {
  
}

String getJSON(){
  Serial1.print("&getJSON*"+path+"#");
  while(!Serial1.available());
  String temp = Serial1.readString();
  return temp;
}

int getInt(String path){
  Serial1.print("&getInt*"+path+"#");
  while(!Serial1.available());
  String temp = Serial1.readString();
  return temp.toInt();
}
float getFloat(String path){
  Serial1.print("&getFloat*"+path+"#");
  while(!Serial1.available());
  String temp = Serial1.readString();
  return temp.toFloat();
}
String getString(String path){
  Serial1.print("&getString*"+path+"#");
  while(!Serial1.available());
  String temp = Serial1.readString();
  return temp;
}
bool getBool(String path){
  Serial1.print("&getBool*"+path+"#");
  while(!Serial1.available());
  String temp = Serial1.readString();
  if(temp.equals("1"))
    return true;
  return false;
}
void setInt(String path,int temp){
  Serial1.print("&setInt*"+path+"*"+temp+"#");
  while(!Serial1.available());
  Serial1.readString();
}
void setFloat(String path,float temp){
  Serial1.print("&setFloat*"+path+"*"+temp+"#");
  while(!Serial1.available());
  Serial1.readString();
}
void setString(String path,String temp){
  Serial1.print("&setString*"+path+"*"+temp+"#");
  while(!Serial1.available());
  Serial1.readString();
}
void setBool(String path,bool temp){
  if(temp == true){
    Serial1.print("&setBool*"+path+"*true#");
  }
  else{
    Serial1.print("&setBool*"+path+"*false#");
  }
  while(!Serial1.available());
  Serial1.readString();
}
