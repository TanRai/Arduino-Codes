void setup() {
  pinMode(2, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  int sensorState = digitalRead(2);
  if(sensorState == HIGH)
  {
    Serial.println("Locked");
  }
  else
  {
    Serial.println("Unlocked");
  }

}
