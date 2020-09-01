
#define TIDAL_VOLUME_PIN   A0      
#define RR_PIN             A1     
#define PMAX_PIN           A2     
#define FiO2_PIN           A3  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:

   int tidal_volume = analogRead(TIDAL_VOLUME_PIN);
  tidal_volume = map(tidal_volume, 0, 1023, 200, 650);
  Serial.print("Tidal_volume:");
  Serial.println(tidal_volume);
  int resp_rate = analogRead(RR_PIN);
   resp_rate = map(resp_rate, 0, 1023, 4, 39);
    Serial.print("Resp Rate:");
  Serial.println(resp_rate);
   int pmax = analogRead(PMAX_PIN);
   pmax = map(pmax, 0, 1023, 29, 99);
    Serial.println("Pmax");
  Serial.print(pmax);
   int fio2 = analogRead(FiO2_PIN);
   fio2 = map(fio2, 0, 1023, 15, 100);
    Serial.print("FIO2:");
  Serial.println(fio2);
}
