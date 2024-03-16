#define led_pin 10
#define sensor_pin 3

void setup() {
  Serial.begin(9600);
  pinMode(led_pin,OUTPUT);
  pinMode(sensor_pin, INPUT);
}

void loop() {
  int sensorValue = digitalRead(sensor_pin);

  if(sensorValue == HIGH)
  {
    Serial.println("HIGH");
  }
  else
  {
    Serial.println("LOW");
  }

}
