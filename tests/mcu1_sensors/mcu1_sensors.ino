void setup() {
  // put your setup code here, to run once:
 Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  int adc1 = analogRead(1);//Inhale
  int adc2 = analogRead(0);//Exhale
  //adc3 = ads.readADC_SingleEnded(3);
  Serial.print("AIN0_GP1: "); Serial.println(adc1);
  Serial.print("AIN1_GP2: "); Serial.println(adc2);
  delay(1000);
  
  Serial.print("Press the Inhalation and exhalation pipe to see the variations in the Values read through adc (channel 1,0)");
}
