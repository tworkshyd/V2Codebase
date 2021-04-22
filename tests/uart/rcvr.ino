
void setup() {
  Serial.begin(115200);
  Serial.print("Start sending characters from Serial Monitor");
}

void loop() {
  if(Serial.available()) {
    char data_rcvd = Serial.read();
    Serial.print(data_rcvd);
  }
}
