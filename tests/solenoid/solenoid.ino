#define INHALE_RELEASE_VLV_PIN 34 /*!< Ouput for inhale high pressure release line solenoid On/Off *///Connector for j6
#define INHALE_VLV_PIN 32         /*!<A10 Ouput for Inhale line solenoid On/Off */ //Connector for j3
#define EXHALE_VLV_PIN 33         /*!< A11 Ouput for Exhale line solenoid On/Off *///Connector for j4
#define O2Cyl_VLV_PIN  35         /*!< Ouput for Cylinder Oxygen line solenoid On/Off *///Connector for j8
#define O2Hln_VLV_PIN 36          /*!< Ouput for Hospital Oxygen line solenoid On/Off *///Connector for j9

/*!< Normally Opened Valves */
#define EXHALE_VLV_OPEN()  digitalWrite(EXHALE_VLV_PIN, LOW)    /*!< Exhale line solenoid valve Open */
#define EXHALE_VLV_CLOSE() digitalWrite(EXHALE_VLV_PIN, HIGH)   /*!< Exhale line solenoid valve Close */
#define INHALE_VLV_OPEN()  digitalWrite(INHALE_VLV_PIN, LOW)    /*!< Inhale line solenoid valve Open */
#define INHALE_VLV_CLOSE() digitalWrite(INHALE_VLV_PIN, HIGH)   /*!< Inhale line solenoid valve Close */

/*!< Normally Closed Valves */
#define INHALE_RELEASE_VLV_OPEN()  digitalWrite(INHALE_RELEASE_VLV_PIN, HIGH)       /*!< Inhale line high pressure release solenoid valve Open */
#define INHALE_RELEASE_VLV_CLOSE() digitalWrite(INHALE_RELEASE_VLV_PIN, LOW)        /*!< Inhale line high pressure release solenoid valve Close */
#define O2Cyl_VLV_OPEN()  digitalWrite(O2Cyl_VLV_PIN, HIGH)                         /*!< Cylinder Oxygen line solenoid valve Open */
#define O2Cyl_VLV_CLOSE() digitalWrite(O2Cyl_VLV_PIN, LOW)                          /*!< Cylinder Oxygen line solenoid valve Close */
#define O2Hln_VLV_OPEN()  digitalWrite(O2Hln_VLV_PIN, HIGH)                         /*!< Hospital Oxygen line solenoid valve Open */
#define O2Hln_VLV_CLOSE() digitalWrite(O2Hln_VLV_PIN, LOW)                          /*!< Hospital Oxygen line solenoid valve Close */

#define SOLENOID_VALVES_ALL_TIME_TESTING 1
#define SOLENOID_VALVE_EXHALE_TIME_TESTING 1
#define SOLENOID_VALVE_INHALE_TIME_TESTING 1
#define SOLENOID_VALVE_O2Cyl_TIME_TESTING 1
#define SOLENOID_VALVE_O2Hln_TIME_TESTING 1
#define SOLENOID_VALVE_INHALE_RELEASE_TIME_TESTING 1
void setup() {
  // put your setup code here, to run once:
    //Valves
  pinMode(EXHALE_VLV_PIN, OUTPUT);
  pinMode(INHALE_VLV_PIN, OUTPUT);
  pinMode(O2Cyl_VLV_PIN, OUTPUT);
  pinMode(O2Hln_VLV_PIN, OUTPUT);
  pinMode(INHALE_RELEASE_VLV_PIN, OUTPUT);
  //Valves Pin Initialize
  digitalWrite(EXHALE_VLV_PIN, LOW);
  digitalWrite(INHALE_VLV_PIN, LOW);
  digitalWrite(O2Cyl_VLV_PIN, LOW);
  digitalWrite(O2Hln_VLV_PIN, LOW);
  digitalWrite(INHALE_RELEASE_VLV_PIN, LOW);

}

void loop() {
  // put your main code here, to run repeatedly:
  openValves();
  closeValves();
}
void openValves(){
  
  #if SOLENOID_VALVES_ALL_TIME_TESTING
  unsigned long dstarttime = millis();
#endif  
EXHALE_VLV_OPEN();
delay(2000);
INHALE_VLV_OPEN();
delay(2000);
INHALE_RELEASE_VLV_OPEN();
delay(2000);
O2Cyl_VLV_OPEN();
delay(2000);
O2Hln_VLV_OPEN();
delay(2000);  
#if ROW0_PROCESSING_TIME_TESTING
 Serial.print("Total Time taken by all Valves to open:");
  Serial.println((millis()-dstarttime-5000));
#endif

}
void closeValves(){
EXHALE_VLV_CLOSE();
delay(500);
INHALE_VLV_CLOSE();
delay(500);
INHALE_RELEASE_VLV_CLOSE();
delay(500);
O2Cyl_VLV_CLOSE();
delay(500);
O2Hln_VLV_CLOSE();  
}
