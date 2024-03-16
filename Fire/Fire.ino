void setup() {
  pinMode(2, INPUT);
  Serial.begin(9600);
}

void loop() {
  int sensorValue = digitalRead(2);

  if(sensorValue == LOW)
  {
    Serial.println("FIRE !!!!!!!!!");
  }
  else
  {
    Serial.println("None");
  }

}
