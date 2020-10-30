
#include "Arduino.h"
#include "relayModule_v2.h"
#include "./libraries/MsTimer2/MsTimer2.cpp"
//*****************

//#include "Ventcomm.cpp"
#include "gauge_pressure.h"
//#include "relayModule.h"

void setup()
{

  Serial.begin(115200);                 // The Serial port of Arduino baud rate.
  Serial.println(F("Signum Techniks")); // say hello to check serial line
  Serial3.begin(115200);

  //pinMode(INDICATOR_LED, OUTPUT);
  pinMode(LED_1_PIN, OUTPUT);
  pinMode(LED_2_PIN, OUTPUT);
  pinMode(LED_3_PIN, OUTPUT);
  pinMode(LED_4_PIN, OUTPUT);
  pinMode(LED_5_PIN, OUTPUT);

  digitalWrite(LED_1_PIN, HIGH);
  digitalWrite(LED_2_PIN, HIGH);
  digitalWrite(LED_3_PIN, HIGH);
  digitalWrite(LED_4_PIN, HIGH);
  digitalWrite(LED_5_PIN, HIGH);

  //Stepper Motor step and direction
  pinMode(MOTOR_STEP_PIN, OUTPUT);
  digitalWrite(MOTOR_STEP_PIN, HIGH);
  pinMode(MOTOR_DIR_PIN, OUTPUT);
  digitalWrite(MOTOR_DIR_PIN, LOW);

  //motor controls & Home pins
  //pinMode(MOTOR_RUN_PIN, INPUT_PULLUP);
  pinMode(HOME_SENSOR_PIN, INPUT_PULLUP);
  //digitalWrite(MOTOR_RUN_PIN, HIGH);
  digitalWrite(HOME_SENSOR_PIN, HIGH);

  //Valves
  pinMode(EXHALE_VLV_PIN, OUTPUT);
  pinMode(INHALE_VLV_PIN, OUTPUT);
  pinMode(O2Cyl_VLV_PIN, OUTPUT);
  pinMode(INHALE_RELEASE_VLV_PIN, OUTPUT);
  //Valves Pin Initialize
  digitalWrite(EXHALE_VLV_PIN, LOW);
  digitalWrite(INHALE_VLV_PIN, LOW);
  digitalWrite(O2Cyl_VLV_PIN, LOW);
  digitalWrite(INHALE_RELEASE_VLV_PIN, LOW);

  inti_all_Valves();
  //stop_timer();

  //home cycle on power up
  home_cycle = true;
  motion_profile_count_temp = 0;
  Serial.print("Power On Home Cycle : ");
  run_pulse_count_temp = 0.0;
  run_pulse_count = 200000.0;
  digitalWrite(MOTOR_DIR_PIN, EXP_DIR);
  //This is mandatory to initate the Timer block properly
  initialize_timer1_for_set_RPM(home_speed_value * 10.0);
  run_motor = true;

  //delay(5000);
  flag_Serial_requested = false ;
 // Serial.println("Requesting paramemters : ");
 // Serial3.print("$VSP10001&");
}

void loop()
{
  if (send_pressure_data == true)
  {
    Ipressure = get_calibrated_pressure_MPX5010(INHALE_GAUGE_PRESSURE, &IRaw);
    Epressure = get_calibrated_pressure_MPX5010(EXHALE_GUAGE_PRESSURE, &ERaw);
    Serial3.print(Ctrl_CreateCommand(PARAMGP_PRS, Ipressure * 100, Epressure * 100));
    delay(100);
    // Serial.println(Ctrl_CreateCommand(PARAMGP_PRS, Ipressure * 100, Epressure * 100));
  }

  if (send_millivolts_data == true)
  {
    //  Ctrl_CreateCommand(PARAMGP_RAW, int value1, int value2)
    Ipressure = get_calibrated_pressure_MPX5010(INHALE_GAUGE_PRESSURE, &IRaw);
    Epressure = get_calibrated_pressure_MPX5010(EXHALE_GUAGE_PRESSURE, &ERaw);
    Serial3.print(Ctrl_CreateCommand(PARAMGP_RAW, IRaw, ERaw));
    delay(100);
    //Serial.print(Ctrl_CreateCommand(PARAMGP_RAW, IRaw, ERaw));
  }

  //Expansion completed & Compression start
  if ((cycle_start == true) && (exp_start == true) && (exp_end == true) && (exp_timer_end == true))
  {
    EXHALE_VLV_CLOSE();
    Epressure = get_calibrated_pressure_MPX5010(EXHALE_GUAGE_PRESSURE, &ERaw);
    PEEP = Epressure;
    Serial.print("PEEP:  ");    Serial.println(PEEP);
    INHALE_VLV_OPEN();
    Serial.print("IER: 1:");
    Serial.print(IER);
    Serial.print("  BPM: ");
    Serial.print(BPM);
    Serial.print("  TV: ");
    Serial.print(tidal_volume);
    Serial.print("  Stroke: ");
    Serial.println(Stroke_length);
    
    Serial.print("Peak Pressure: ");
    Serial.print(peak_prsur);
    Serial.print("  Cali. GP0: ");
    Serial.print(CAL_GP0);
    Serial.print("  Cali. GP1: ");
    Serial.println(CAL_GP1);
    
    Serial.print("comp : ");
    Serial.print((c_end_millis - c_start_millis) / 1000.0);
    Serial.print("/");
    Serial.print(inhale_time);
    Serial.print("  ExpTime : ");
    Serial.print((e_timer_end_millis - e_start_millis) / 1000.0);
    Serial.print("/");
    Serial.print(exhale_time);
    Serial.print("  Cycle : ");
    Serial.print((e_timer_end_millis - c_start_millis) / 1000.0);
    Serial.print("/");
    Serial.println(cycle_time);
    Serial.print("Inhale-hold : ");
    Serial.print(inhale_hold_time / 1000.0);
    Serial.print("  MotorRet. : ");
    Serial.println((e_end_millis - e_start_millis) / 1000.0);
    Serial.println();
    if ((BPM_new != BPM) || (tidal_volume_new != tidal_volume) || (IER_new != IER))
    {
      convert_all_set_params_2_machine_values();
    }
    Start_inhale_cycle();
    PIP = 0.0; // To catch max value we have t0 make it zero.
  }

  //compression started & is in progress
  if ((cycle_start == true) && (comp_start == true) && (comp_end == false))
  {

    Ipressure = get_calibrated_pressure_MPX5010(INHALE_GAUGE_PRESSURE, &IRaw);
    if (Ipressure > PIP)
    {
      PIP = Ipressure;
      // Serial.print("PIP:");
      // Serial.println(Ipressure);
    }
    if (Ipressure > peak_prsur)
    {
      INHALE_VLV_CLOSE();
      //Stop motor
      Emergency_motor_stop = true;
    }
  }

  //Compression completed & start Expansion
  if ((cycle_start == true) && (comp_start == true) && (comp_end == true))
  {
    inhale_hold_time = (inhale_time * (inhale_hold_percentage / 100)) * 1000;
    delay(inhale_hold_time); //expansion delay
    
    Epressure = get_calibrated_pressure_MPX5010(EXHALE_GUAGE_PRESSURE, &ERaw);
    PLAT = Epressure;

    Serial.print("PLAT:");
    Serial.println(PLAT);
    Start_exhale_cycle();
    EXHALE_VLV_OPEN();
    //Start_exhale_cycle();
  }

  //Expansion started & is in progress
  if ((cycle_start == true) && (exp_start == true) && (exp_end == false))
  {
    Epressure = get_calibrated_pressure_MPX5010(EXHALE_GUAGE_PRESSURE, &ERaw);
  }
}

ISR(TIMER1_COMPA_vect)
{ //timer1 interrupt 1Hz toggles pin 13 (LED)
  //generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)
  if (run_motor == true)
  {
    if ((motion_profile_count_temp == 0) && (run_pulse_count_temp == 0.0))
    {
      //compression cycle start only once
      if ((comp_start == true) & (comp_end == false))
      {
        //Serial.print("comp: "); Serial.println(motion_profile_count_temp);
        c_start_millis = millis();
        run_pulse_count = compression_step_array[motion_profile_count_temp];
        digitalWrite(MOTOR_DIR_PIN, COMP_DIR);
        OCR1A = OCR1A_comp_array[motion_profile_count_temp];
        load_TCCR1B_var(TCCR1B_comp_array[motion_profile_count_temp]);
        Emergency_motor_stop = false;
        INHALE_RELEASE_VLV_CLOSE();
        EXHALE_VLV_CLOSE();
        INHALE_VLV_OPEN();
      }

      //after inhale-hold time --> Expansion cycle start only once
      if ((exp_start == true) & (exp_end == false))
      {
        //Serial.print("exp: "); Serial.println(motion_profile_count_temp);
        e_start_millis = millis();
        run_pulse_count = expansion_step_array[motion_profile_count_temp];
        digitalWrite(MOTOR_DIR_PIN, EXP_DIR);
        OCR1A = OCR1A_exp_array[motion_profile_count_temp];
        load_TCCR1B_var(TCCR1B_exp_array[motion_profile_count_temp]);
        Emergency_motor_stop = false;
        INHALE_RELEASE_VLV_CLOSE();
        INHALE_VLV_CLOSE();
        //Commented to check if i can open the Valve after hold time.. so hold time and motor retraction will have overlap.
        //EXHALE_VLV_OPEN();
      }
    }

    //Actual motor pulse generation block
    if (run_pulse_count_temp < run_pulse_count)
    {
      if (Emergency_motor_stop == false)
        digitalWrite(MOTOR_STEP_PIN, digitalRead(MOTOR_STEP_PIN) ^ 1);
      run_pulse_count_temp = run_pulse_count_temp + 0.5;
      if (home_cycle == true)
      {
        if (digitalRead(HOME_SENSOR_PIN) == HOME_SENSE_VALUE)
        {
          run_motor = false;
          run_pulse_count_temp = 0.0;
          home_cycle = false;
          motion_profile_count_temp = 0;
          Serial.println("Home Cycle Complete...");
          if (cycle_start == true)
            inti_Start();
        }
      }
      if ((cycle_start == true) && (digitalRead(MOTOR_DIR_PIN) == EXP_DIR))
      {
        if (digitalRead(HOME_SENSOR_PIN) == HOME_SENSE_VALUE)
        {
          run_pulse_count_temp = run_pulse_count;
          motion_profile_count_temp = CURVE_EXP_STEPS;
          Emergency_motor_stop = true;
          //motion_profile_count_temp = 0;
          //run_pulse_count_temp = 0.0;
        }
      }
    }
    else
    {
      noInterrupts();
      run_motor = false;
      run_pulse_count_temp = 0.0;
      motion_profile_count_temp = motion_profile_count_temp + 1;
      //Compression end check
      if ((comp_start == true) & (comp_end == false))
      {
        if (motion_profile_count_temp < CURVE_COMP_STEPS)
        {
          //Serial.print("comp: "); Serial.println(motion_profile_count_temp);
          run_pulse_count = compression_step_array[motion_profile_count_temp];
          digitalWrite(MOTOR_DIR_PIN, COMP_DIR);
          OCR1A = OCR1A_comp_array[motion_profile_count_temp];
          load_TCCR1B_var(TCCR1B_comp_array[motion_profile_count_temp]);
          run_motor = true;
        }
        else
        {
          c_end_millis = millis();
          motion_profile_count_temp = 0;
          run_pulse_count_temp = 0.0;
          Emergency_motor_stop = false;
          Serial.print("PIP:");
          Serial.println(Ipressure);
          INHALE_RELEASE_VLV_CLOSE();
          INHALE_VLV_CLOSE();
          //commented as this will be Opened after Inhale-Hold Delay.
          //EXHALE_VLV_OPEN();
          comp_end = true;
        }
      }
      //expansion end check
      if ((exp_start == true) & (exp_end == false))
      {
        if (motion_profile_count_temp < CURVE_EXP_STEPS)
        {
          //Serial.print("exp: "); Serial.println(motion_profile_count_temp);
          run_pulse_count = expansion_step_array[motion_profile_count_temp];
          digitalWrite(MOTOR_DIR_PIN, EXP_DIR);
          OCR1A = OCR1A_exp_array[motion_profile_count_temp];
          load_TCCR1B_var(TCCR1B_exp_array[motion_profile_count_temp]);
          run_motor = true;
        }
        else
        {
          e_end_millis = millis();
          motion_profile_count_temp = 0;
          run_pulse_count_temp = 0.0;
          Emergency_motor_stop = false;
          INHALE_RELEASE_VLV_CLOSE();
          //these are skipped to enable motor fast retract and wait for timer to close the exhale valve
          //          EXHALE_VLV_CLOSE();
          //          INHALE_VLV_OPEN();
          exp_end = true;
        }
      }
      interrupts();
    }
  }
}

bool Start_exhale_cycle()
{
  Serial3.print(Ctrl_CreateCommand(EXPAN, PIP * 100, 0)); //expansion flag
  Serial.print(Ctrl_CreateCommand(EXPAN, PIP * 100, 0));  //expansion flag
  //Serial.print("CYCLE Exhale Time: " );Serial.println(exhale_time);
  MsTimer2::set(exhale_time * 1000, Exhale_timer_timout); //period
  MsTimer2::start();

  cycle_start = true;
  comp_start = false;
  comp_end = false;
  exp_start = true;
  exp_end = false;
  exp_timer_end = false;
  run_motor = true;
  return true;
}

bool Start_inhale_cycle()
{
  Serial3.print(Ctrl_CreateCommand(COMP, PEEP * 100, PLAT * 100)); //comp start flag
  Serial.print(Ctrl_CreateCommand(COMP, PEEP * 100, PLAT * 100));  //comp start flag
  cycle_start = true;
  comp_start = true;
  comp_end = false;
  exp_start = false;
  exp_end = false;
  exp_timer_end = false;
  run_motor = true;
  return true;
}
/*
   Function to build the command to be sent to Ventilator Master
*/
String Ctrl_CreateCommand(String paramName, long value1, int value2)
{

  String command;
  char paddedValue1[8];
  char paddedValue2[8];
  command = START_DELIM;
  command += VENT_MAST;
  command += paramName;
  if (paramName == GP0_PARAM || paramName == GP1_PARAM)
  {
    char paddedValue3[15];
    sprintf(paddedValue3, "%08lu", value1);
    command += paddedValue3;
    Serial.print("cal value sending :  ");
    Serial.print(paramName);
    Serial.print(" == ");
    Serial.println(paddedValue3);
  }
  else
  {
    sprintf(paddedValue1, "%04d", value1);
    sprintf(paddedValue2, "%04d", value2);
    command += paddedValue1;
    command += paddedValue2;
  }

  command += END_DELIM;
  return command;
}
bool Exhale_timer_timout()
{
  MsTimer2::stop();
  //digitalWrite(INDICATOR_LED, digitalRead(INDICATOR_LED) ^ 1);
  e_timer_end_millis = millis();
  EXHALE_VLV_CLOSE();
  INHALE_VLV_OPEN();
  exp_timer_end = true;
  return true;
}

void load_TCCR1B_var(int TCCR1B_var_temp)
{
  if (TCCR1B_var_temp == 1)
  {
    TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);
  }
  if (TCCR1B_var_temp == 8)
  {
    TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10);
  }
  if (TCCR1B_var_temp == 64)
  {
    TCCR1B |= (0 << CS12) | (1 << CS11) | (1 << CS10);
  }
  if (TCCR1B_var_temp == 256)
  {
    TCCR1B |= (1 << CS12) | (0 << CS11) | (0 << CS10);
  }
  if (TCCR1B_var_temp == 1024)
  {
    TCCR1B |= (1 << CS12) | (0 << CS11) | (1 << CS10);
  }

  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
}

boolean convert_all_set_params_2_machine_values()
{
  Serial.println(("Speed curve calculations : "));

  BPM = BPM_new;
  tidal_volume = tidal_volume_new;
  Stroke_length = Stroke_length_new;
  inhale_ratio = 1.0;
  IER = IER_new;
  exhale_ratio = IER_new;

  //global variable
  cycle_time = 60.0 / BPM;
  inhale_time = (cycle_time * inhale_ratio) / (inhale_ratio + exhale_ratio);
  exhale_time = (cycle_time * exhale_ratio) / (inhale_ratio + exhale_ratio);
  //  exhale_time = exhale_time - (inhale_hold_time/1000);

  MsTimer2::set(exhale_time * 1000, Exhale_timer_timout); //period
  Serial.print("Calculated Exhale Time: ");
  Serial.println(exhale_time);

  float inhale_vpeak = ((Stroke_length * 0.8) / (inhale_time * 0.8));
  float exhale_vpeak = (((Stroke_length + extra_exhale_travel) * 0.8) / (0.90 * 0.8)); //exhale_time

  compression_speed = (inhale_vpeak / LEAD_SCREW_PITCH) * 60;
  expansion_speed = (exhale_vpeak / LEAD_SCREW_PITCH) * 60;

  //this 1.8 degree step motor so 200 steps for 360 degree
  run_pulse_count_1_full_movement = ((micro_stepping * (Stroke_length / LEAD_SCREW_PITCH * 1.0)) / 2.0);
  run_pulse_count_1_piece_compression = (run_pulse_count_1_full_movement / 100.0); //CURVE_COMP_STEPS   '''taking 100 pieces to ease the % calculation
  run_pulse_count_1_full_movement = ((micro_stepping * ((Stroke_length + extra_exhale_travel) / LEAD_SCREW_PITCH * 1.0)) / 2.0);
  run_pulse_count_1_piece_expansion = (run_pulse_count_1_full_movement / 100.0); //CURVE_EXP_STEPS  '''taking 100 pieces to ease the % calculation

  //avoid accel/deccel below 300 rpm
  if (compression_speed > MIN_RPM_NO_ACCEL)
  {
    compression_speed = compression_speed + (compression_speed * 0.08);
    compression_min_speed = MIN_RPM_NO_ACCEL;
  }
  else
    compression_min_speed = compression_speed;
  if (expansion_speed > MIN_RPM_NO_ACCEL)
  {
    expansion_speed = expansion_speed + (expansion_speed * 0.08);
    expansion_min_speed = MIN_RPM_NO_ACCEL;
  }
  else
    expansion_min_speed = expansion_speed;

  compression_step_array[10] = run_pulse_count_1_piece_compression * 80;
  compression_speed_array[10] = compression_speed;
  pre_calculate_timer_values_4_different_RPM(compression_speed_array[10] * 10.0);
  TCCR1B_comp_array[10] = TCCR1B_var;
  OCR1A_comp_array[10] = OCR1A_var;

  expansion_step_array[10] = run_pulse_count_1_piece_expansion * 80;
  expansion_speed_array[10] = expansion_speed;
  pre_calculate_timer_values_4_different_RPM(expansion_speed_array[10] * 10.0);
  TCCR1B_exp_array[10] = TCCR1B_var;
  OCR1A_exp_array[10] = OCR1A_var;

  float comp_slope = (compression_speed - compression_min_speed) / 8.0;
  float exp_slope = (expansion_speed - expansion_min_speed) / 8.0;
  int i;
  for (i = 0; i < 10; i++)
  {
    compression_step_array[i] = run_pulse_count_1_piece_compression;
    compression_step_array[20 - i] = run_pulse_count_1_piece_compression;
    compression_speed_array[i] = (comp_slope * ((Stroke_length * 0.01) * (i))) + compression_min_speed;
    compression_speed_array[20 - i] = (comp_slope * ((Stroke_length * 0.01) * (i))) + compression_min_speed;

    //compression_speed_array[i] = compression_min_speed + (i * ((compression_speed - compression_min_speed) / 10.0));
    //compression_speed_array[20 - i] = compression_min_speed + (i * ((compression_speed - compression_min_speed) / 10.0));

    pre_calculate_timer_values_4_different_RPM(compression_speed_array[i] * 10.0);
    TCCR1B_comp_array[i] = TCCR1B_var;
    OCR1A_comp_array[i] = OCR1A_var;
    TCCR1B_comp_array[20 - i] = TCCR1B_var;
    OCR1A_comp_array[20 - i] = OCR1A_var;

    expansion_step_array[i] = run_pulse_count_1_piece_expansion;
    expansion_step_array[20 - i] = run_pulse_count_1_piece_expansion;
    expansion_speed_array[i] = (exp_slope * (((Stroke_length + extra_exhale_travel) * 0.01) * (i))) + expansion_min_speed;
    expansion_speed_array[20 - i] = (exp_slope * (((Stroke_length + extra_exhale_travel) * 0.01) * (i))) + expansion_min_speed;

    //expansion_speed_array[i] = expansion_min_speed + (i * ((expansion_speed - expansion_min_speed) / 10.0));
    //expansion_speed_array[20 - i] = expansion_min_speed + (i * ((expansion_speed - expansion_min_speed) / 10.0));

    pre_calculate_timer_values_4_different_RPM(expansion_speed_array[i] * 10.0);
    TCCR1B_exp_array[i] = TCCR1B_var;
    OCR1A_exp_array[i] = OCR1A_var;
    TCCR1B_exp_array[20 - i] = TCCR1B_var;
    OCR1A_exp_array[20 - i] = OCR1A_var;
  }

  //  for (i = 0; i < 21; i++) {
  //    Serial.print("Compression: "); Serial.print(i); Serial.print(" | step: "); Serial.print(compression_step_array[i]); Serial.print(" | rpm: "); Serial.println(compression_speed_array[i]);
  //    Serial.print("expansion  : "); Serial.print(i); Serial.print(" | step: "); Serial.print(expansion_step_array[i]);  Serial.print(" | rpm: "); Serial.println(expansion_speed_array[i]);
  //  }

  return true;
}

boolean stop_timer()
{
  //cli();  // One way to disable the timer, and all interrupts

  TCCR1B &= ~(1 << CS12); // turn off the clock altogether
  TCCR1B &= ~(1 << CS11);
  TCCR1B &= ~(1 << CS10);

  TIMSK1 &= ~(1 << OCIE1A); // turn off the timer interrupt

  return true;
}

void pre_calculate_timer_values_4_different_RPM(float rpm)
{

  double freq;
  //  long OCR1A_var;
  //  long TCCR1B_var;

  rpm = abs(rpm);
  freq = round(long((rpm * micro_stepping) / 600.0));
  //Serial.print(("rpm  : ")); Serial.print(rpm / 10); Serial.print(("  Freq : ")); Serial.print(freq); Serial.print(("  Micro stepping: ")); Serial.println(micro_stepping);

  // initialize timer1
  //noInterrupts(); // disable all interrupts
  //set timer1 interrupt at 1Hz
  //TCCR1A = 0;// set entire TCCR1A register to 0
  //TCCR1B = 0;// same for TCCR1B
  //TCNT1  = 0;//initialize counter value to 0

  // set compare match register for 1hz increments
  TCCR1B_var = 1;
  OCR1A_var = (16000000.0 / (freq * TCCR1B_var)) - 1; // (must be <65536)
  //  Serial.println("1 :");
  //  Serial.print(("TCCR : ")); Serial.print(TCCR1B_var);
  //  Serial.print(("       OCR1 : ")); Serial.println(OCR1A_var);

  if (OCR1A_var > 65536 || OCR1A_var <= 0)
  {
    TCCR1B_var = 8;
    OCR1A_var = ((16000000) / (freq * TCCR1B_var)) - 1; // (must be <65536)
  }
  //  Serial.println("2 :");
  //  Serial.print(("TCCR : ")); Serial.print(TCCR1B_var);
  //  Serial.print(("       OCR1 : ")); Serial.println(OCR1A_var);

  if (OCR1A_var > 65536 || OCR1A_var <= 0)
  {
    TCCR1B_var = 64;
    OCR1A_var = ((16000000) / (freq * TCCR1B_var)) - 1; // (must be <65536)
  }
  //  Serial.println("3 :");
  //  Serial.print(("TCCR : ")); Serial.print(TCCR1B_var);
  //  Serial.print(("       OCR1 : ")); Serial.println(OCR1A_var);

  if (OCR1A_var > 65536 || OCR1A_var <= 0)
  {
    TCCR1B_var = 256;
    OCR1A_var = ((16000000) / (freq * TCCR1B_var)) - 1; // (must be <65536)
  }
  //  Serial.println("4 :");
  //  Serial.print(("TCCR : ")); Serial.print(TCCR1B_var);
  //  Serial.print(("       OCR1 : ")); Serial.println(OCR1A_var);

  if (OCR1A_var > 65536 || OCR1A_var <= 0)
  {
    TCCR1B_var = 1024;
    OCR1A_var = ((16000000) / (freq * TCCR1B_var)) - 1; // (must be <65536)
  }
  //  Serial.println("5 :");
  //  Serial.print(("TCCR : ")); Serial.print(TCCR1B_var);
  //  Serial.print(("       OCR1 : ")); Serial.println(OCR1A_var);
}

boolean initialize_timer1_for_set_RPM(float rpm)
{
  double freq;
  //  long OCR1A_var;
  //  long TCCR1B_var;

  rpm = abs(rpm);
  //  Serial.print(("rpm  : ")); Serial.println(rpm / 10);
  //  Serial.print(("Micro: ")); Serial.println(micro_stepping);
  freq = round(long((rpm * micro_stepping) / 600.0));
  //  Serial.print(("Freq : ")); Serial.println(freq);

  // initialize timer1
  noInterrupts(); // disable all interrupts
  //set timer1 interrupt at 1Hz
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1 = 0;  //initialize counter value to 0

  // set compare match register for 1hz increments
  TCCR1B_var = 1;
  OCR1A_var = (16000000.0 / (freq * TCCR1B_var)) - 1; // (must be <65536)
  //  Serial.println("1 :");
  //  Serial.print(("TCCR : ")); Serial.print(TCCR1B_var);
  //  Serial.print(("       OCR1 : ")); Serial.println(OCR1A_var);
  if (OCR1A_var > 65536 || OCR1A_var <= 0)
  {
    TCCR1B_var = 8;
    OCR1A_var = ((16000000) / (freq * TCCR1B_var)) - 1; // (must be <65536)
  }
  //  Serial.println("2 :");
  //  Serial.print(("TCCR : ")); Serial.print(TCCR1B_var);
  //  Serial.print(("       OCR1 : ")); Serial.println(OCR1A_var);
  if (OCR1A_var > 65536 || OCR1A_var <= 0)
  {
    TCCR1B_var = 64;
    OCR1A_var = ((16000000) / (freq * TCCR1B_var)) - 1; // (must be <65536)
  }
  //  Serial.println("3 :");
  //  Serial.print(("TCCR : ")); Serial.print(TCCR1B_var);
  //  Serial.print(("       OCR1 : ")); Serial.println(OCR1A_var);
  if (OCR1A_var > 65536 || OCR1A_var <= 0)
  {
    TCCR1B_var = 256;
    OCR1A_var = ((16000000) / (freq * TCCR1B_var)) - 1; // (must be <65536)
  }
  //  Serial.println("4 :");
  //  Serial.print(("TCCR : ")); Serial.print(TCCR1B_var);
  //  Serial.print(("       OCR1 : ")); Serial.println(OCR1A_var);
  if (OCR1A_var > 65536 || OCR1A_var <= 0)
  {
    TCCR1B_var = 1024;
    OCR1A_var = ((16000000) / (freq * TCCR1B_var)) - 1; // (must be <65536)
  }
  //  Serial.println("5 :");
  //  Serial.print(("TCCR : ")); Serial.print(TCCR1B_var);
  //  Serial.print(("       OCR1 : ")); Serial.println(OCR1A_var);

  OCR1A = OCR1A_var;

  // Set CS12, CS11 and CS10 bits for X prescaler
  //001 = 1
  //010 = 8
  //011 = 64
  //100 = 256
  //101 = 1024
  if (TCCR1B_var == 1)
  {
    TCCR1B |= (0 << CS12) | (0 << CS11) | (1 << CS10);
  }
  if (TCCR1B_var == 8)
  {
    TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10);
  }
  if (TCCR1B_var == 64)
  {
    TCCR1B |= (0 << CS12) | (1 << CS11) | (1 << CS10);
  }
  if (TCCR1B_var == 256)
  {
    TCCR1B |= (1 << CS12) | (0 << CS11) | (0 << CS10);
  }
  if (TCCR1B_var == 1024)
  {
    TCCR1B |= (1 << CS12) | (0 << CS11) | (1 << CS10);
  }

  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  interrupts(); // enable all interrupts
}
int comcnt = 0;     /*!< counter to count serial recieved bytes */
String rxdata = ""; /*!< string to store serial recieved data  */

void serialEvent()
{
  while (Serial.available())
  {
    char inChar = (char)Serial.read();
    if (inChar == '$')
    {
      comcnt = 1;
      rxdata = "";
    }
    if (comcnt >= 1)
    {
      rxdata += inChar;
      comcnt = comcnt + 1;
      if (inChar == '&')
      {
        if (comcnt >= 10)
        {
          Serial.print(rxdata);
          Prcs_RxData();
        }
      }
    }
  }
}

void serialEvent3()
{

  while (Serial3.available())
  {
    char inChar = (char)Serial3.read();
    if (inChar == '$')
    {
      comcnt = 1;
      rxdata = "";
    }
    if (comcnt >= 1)
    {
      rxdata += inChar;
      comcnt = comcnt + 1;
      if (inChar == '&')
      {
        if (comcnt >= 10)
        {
          Serial.println(rxdata);
          Prcs_RxData();
        }
      }
    }
  }
}
bool Prcs_RxData()
{
  String p1;
  String p2;
  String p3;
  String p4;
  String payload;

  p1 = rxdata.substring(1, 3);
  p2 = rxdata.substring(3, 5);
  p3 = rxdata.substring(5, 7);
  p4 = rxdata.substring(7, 9);
  payload = p3 + p4;

  if (p1 == "VM")
  {
    if (p2 == "ST")
    {
      //stepper motor
      if (payload == "0000")
      {
        inti_Stop_n_Home();
      }
    }
    else if (p2 == "IN")
    {
      if (payload == "0000")
      {
        //if (cycle_start == false)
        inti_Home_n_Start();
      }
      if (payload == "0001")
      {
        if (cycle_start == true)
          inti_Stop_n_Home();
      }
      if (payload == "0003")
      {
        if ((cycle_start == true) && (digitalRead(HOME_SENSOR_PIN) == HOME_SENSE_VALUE))
          breathe_detected_skip_exhale_n_start_inhale();
      }
      if (payload == "0002")
      {
        inti_all_Valves();
      }
    }
    else if (p2 == "PP")
    {
      if (payload == "0000")
      {
        Par_editstat = 1;
      }
      if (payload == "1111")
      {
        Par_editstat = 0;
      }
    }
    else if (p2 == TV_PARAM)
    {
      tidal_volume_new = payload.toInt();
      Serial.print("TV : ");
      Serial.println(tidal_volume_new);

      //      if (tidal_volume_new == 50) Stroke_length_new = 16;
      //      if (tidal_volume_new == 100) Stroke_length_new = 24;
      //      if (tidal_volume_new == 150) Stroke_length_new = 31;
      //      if (tidal_volume_new == 200)        Stroke_length_new = 55.3;
      //      if (tidal_volume_new == 250)        Stroke_length_new = 58.5;
      //      if (tidal_volume_new == 300)        Stroke_length_new = 62.5;
      //      if (tidal_volume_new == 350)        Stroke_length_new = 65.8;
      //      if (tidal_volume_new == 400)        Stroke_length_new = 69.1;
      //      if (tidal_volume_new == 450)        Stroke_length_new = 73.0;
      //      if (tidal_volume_new == 500)        Stroke_length_new = 76.8;
      //      if (tidal_volume_new == 550)        Stroke_length_new = 80.9;
      //      if (tidal_volume_new == 600)        Stroke_length_new = 85.3;
      //      if (tidal_volume_new == 650)        Stroke_length_new = 91.0;
      //      if (tidal_volume_new == 700)        Stroke_length_new = 97.0;

      //      if (tidal_volume_new == 200) Stroke_length_new = 50;
      //      if (tidal_volume_new == 250) Stroke_length_new = 56;
      //      if (tidal_volume_new == 300) Stroke_length_new = 61.5;
      //      if (tidal_volume_new == 350) Stroke_length_new = 66.70;
      //      if (tidal_volume_new == 400) Stroke_length_new = 71.0;
      //      if (tidal_volume_new == 450) Stroke_length_new = 75.5;
      //      if (tidal_volume_new == 500) Stroke_length_new = 79.5;
      //      if (tidal_volume_new == 550) Stroke_length_new = 83.3;
      //      if (tidal_volume_new == 600) Stroke_length_new = 87.5;
      //      if (tidal_volume_new == 650) Stroke_length_new = 92.8;
      //      if (tidal_volume_new == 700) Stroke_length_new = 98;
      
      //      if (tidal_volume_new == 750) Stroke_length_new = 86;
      //      if (tidal_volume_new == 800) Stroke_length_new = 90;
      //      if (tidal_volume_new == 850) Stroke_length_new = 95;
      //      if (tidal_volume_new == 900) Stroke_length_new = 100;
      //      if (tidal_volume_new == 950) Stroke_length_new = 105;

      pick_stroke_length();
      //Stroke_length_new=tidal_volume_new/10;  //enable this to do calibration using serial cmd to control strok length with xx.x accuracy
      
      if (flag_Serial_requested == true)
      {
        Serial3.print("$VSP20002&");
      }
    }
    else if (p2 == RR_PARAM)
    {
      BPM_new = payload.toInt();
      Serial.print("BPM : ");
      Serial.println(BPM_new);
      cycle_time = 60.0 / BPM_new;
      Serial.print("cycle time : ");
      Serial.println(cycle_time);
      inhale_hold_time = (cycle_time * (inhale_hold_percentage / 100)) * 1000;
      Serial.print("Compression hold in mS: ");
      Serial.println(inhale_hold_time);
      pick_stroke_length();
      if (flag_Serial_requested == true)
      {
        Serial3.print("$VSP50004&");
      }
    }

    else if (p2 == "P3")
    {
      FiO2 = payload.toInt();
      Serial.print("FiO2 : "); Serial.println(FiO2);
    }
    else if (p2 == "P4")
    {
      PEEP_new = payload.toInt();
      Serial.print("PEEP_new : "); Serial.println(PEEP_new);
    }
    else if (p2 == IER_PARAM)
    {
      IER_new = payload.toInt();
      Serial.print("IER : "); Serial.println(IER_new);
      pick_stroke_length();
      //      IER = 1020;
      //      inhale_ratio = 1.0;
      //      exhale_ratio = 2.0;
      if (flag_Serial_requested == true)
      {
        // flag_Serial_requested = false;
        //  convert_all_set_params_2_machine_values();
        Serial3.print("$VSP60006&");
      }
    }
    else if (p2 == PEAK_PARAM)
    {
      peak_prsur = payload.toInt();
      Serial.print("peak_prsur_new : "); Serial.println(peak_prsur);
      if (flag_Serial_requested == true)
      {
        Serial3.print("$VSP70011&");
      }
    }
    else if (p2 == GP0_PARAM)
    {
      
      CAL_GP0_new = rxdata.substring(5, 13).toFloat()/100000;
      apply_zerocal_offset_MPX5010(SENSOR_PRESSURE_A1, CAL_GP0_new);
      CAL_GP0 = get_zerocal_offset_MPX5010(SENSOR_PRESSURE_A1);
      Serial.print("CAL_GP0 : "); Serial.println(CAL_GP0);
      if (flag_Serial_requested == true)
      {
        Serial3.print("$VSP80012&");
      }
    }
    else if (p2 == GP1_PARAM)
    {
      
      CAL_GP1_new = rxdata.substring(5, 13).toFloat()/100000;
      apply_zerocal_offset_MPX5010(SENSOR_PRESSURE_A0, CAL_GP1_new);
      CAL_GP1 = get_zerocal_offset_MPX5010(SENSOR_PRESSURE_A0);
      Serial.print("CAL_GP1 : "); Serial.println(CAL_GP1);
      if (flag_Serial_requested == true)
      {
        flag_Serial_requested = false;
        convert_all_set_params_2_machine_values();
        Serial3.print("$VSO20000&");
      }
    }
    else if (p2 == "F2")
    {
      if (1 == payload.toInt())
      {
        Serial.println("Pressure flag == true");
        send_pressure_data = true;
      }
      else if (0 == payload.toInt())
      {
        Serial.println("Pressure flag == false");
        send_pressure_data = false;
      }
    }

    else if (p2 == "F3")
    {
      if (1 == payload.toInt())
      {
        Serial.println("Milli volt flag == true");
        send_millivolts_data = true;
      }
      else if (0 == payload.toInt())
      {
        Serial.println("Milli volt flag == false");
        send_millivolts_data = false;
      }
    }
    else if (p2 == "F1")
    {
      if (1 == payload.toInt())
      {
        send_pressure_data = false;
        delay(500);
        perform_calib_gp = true;
        calibrate_MPX5010();
        Serial3.flush();
        Serial.print("sending claibration GP0 : ");
        Serial.println(get_zerocal_offset_MPX5010(SENSOR_PRESSURE_A1) * 100000);
        Serial3.print(Ctrl_CreateCommand(GP0_PARAM, (long)(get_zerocal_offset_MPX5010(SENSOR_PRESSURE_A1) * 100000), 0));
        delay(500);
        Serial.print("sending claibration GP1 : ");
        Serial.println(get_zerocal_offset_MPX5010(SENSOR_PRESSURE_A0) * 100000);
        Serial3.print(Ctrl_CreateCommand(GP1_PARAM, (long)(get_zerocal_offset_MPX5010(SENSOR_PRESSURE_A0) * 100000), 0));
        delay(500);
        perform_calib_gp = false;
        // send_pressure_data = true;
      }
      else if (0 == payload.toInt())
      {
        perform_calib_gp = false;
      }
    }
    else if (p2 == "SV")
    {
      if (p3 == "01")
      {
        if (p4 == "00")
        {
          //digitalWrite(INHALE_VLV_PIN, LOW);
          INHALE_VLV_CLOSE();
          //Stop motor
          if ((cycle_start == true) && (comp_start == true) && (comp_end == false))
            Emergency_motor_stop = true;
          //relief valve ON
          //INHALE_RELEASE_VLV_OPEN();
        }
        else if (p4 == "01")
        {
          //digitalWrite(INHALE_VLV_PIN, HIGH);
          INHALE_VLV_OPEN();
        }
      }
      else if (p3 == "02")
      {
        if (p4 == "00")
        {
          //digitalWrite(EXHALE_VLV_PIN, LOW);
          EXHALE_VLV_CLOSE();
        }
        else if (p4 == "01")
        {
          //digitalWrite(EXHALE_VLV_PIN, HIGH);
          EXHALE_VLV_OPEN();
        }
      }
      else if (p3 == "03")
      {
        if (p4 == "00")
        {
          INHALE_RELEASE_VLV_CLOSE();
        }
        else if (p4 == "01")
        {
          INHALE_RELEASE_VLV_OPEN();
        }
      }
    }
    else if (p2 == "O2")
    { //solanoide valve for Oxygen line
      if (p3 == "01")
      {
        if (p4 == "00")
        {
          //digitalWrite(O2Cyl_VLV_PIN, LOW);
          Serial.println("2Hln_VLV SELECTED");
          O2_line_option = 1;
          if (cycle_start == true)
          {
            O2Cyl_VLV_CLOSE();
          }
        }
        else if (p4 == "01")
        {
          //digitalWrite(O2Cyl_VLV_PIN, HIGH);
          Serial.println("O2Cyl_VLV SELECTED");
          O2_line_option = 0;
          if (cycle_start == true)
          {
            O2Cyl_VLV_OPEN();
          }
        }
      }
      else if (p3 == "02")
      {
        if (p4 == "00")
        {
          Serial.println("O2Cyl_VLV SELECTED");
          O2_line_option = 0;
          if (cycle_start == true)
          {
            O2Cyl_VLV_OPEN();
          }
        }
        else if (p4 == "01")
        {
          Serial.println("2Hln_VLV SELECTED");
          O2_line_option = 1;
          if (cycle_start == true)
          {
            O2Cyl_VLV_CLOSE();
          }
        }
      }
    }
  }
  return true;
}

bool open_selected_O2_value(void)
{
  if (O2_line_option == 0)
  {
    Serial.println("O2Cyl_VLV Opened...");
    O2Cyl_VLV_OPEN();
  }
  else
  {
    Serial.println("2Hln_VLV Opened...");
    O2Cyl_VLV_CLOSE();
  }
}

bool inti_all_Valves(void)
{
  //Normally Opened
  EXHALE_VLV_OPEN();
  INHALE_VLV_OPEN();

  //Normally closed
  INHALE_RELEASE_VLV_CLOSE();
  O2Cyl_VLV_CLOSE();
  return true;
}

bool breathe_detected_skip_exhale_n_start_inhale()
{
  Emergency_motor_stop = false;
  Serial.println("Skipping Home Cycle : ");
  home_cycle = false;
  cycle_start = true;
  comp_start = false;
  comp_end = false;
  exp_start = true;
  exp_end = true;
  //exp_timer_end = true;
  Exhale_timer_timout();
  //run_motor = true
  return true;
}

bool inti_Stop_n_Home()
{
  cycle_start = false;
  Emergency_motor_stop = false;
  run_motor = true;
  Exhale_timer_timout();
  Serial.println("Cycle Stop & goto Home : ");
  run_pulse_count = 200000;
  digitalWrite(MOTOR_DIR_PIN, EXP_DIR);
  initialize_timer1_for_set_RPM(home_speed_value * 10.0);
  comp_start = false;
  comp_end = false;
  exp_start = false;
  exp_end = false;
  home_cycle = true;
  cycle_start = false;
  run_motor = true;
  inti_all_Valves();
  return true;
}

bool inti_Home_n_Start()
{
  Emergency_motor_stop = false;
  motion_profile_count_temp = 0;
  run_pulse_count_temp = 0.0;
  if (digitalRead(HOME_SENSOR_PIN) == !(HOME_SENSE_VALUE))
  {
    Serial.println("Home Cycle : ");
    run_pulse_count = 200000;
    digitalWrite(MOTOR_DIR_PIN, EXP_DIR);
    initialize_timer1_for_set_RPM(home_speed_value * 10.0);
    comp_start = false;
    comp_end = false;
    exp_start = false;
    exp_end = false;
    home_cycle = true;
    run_motor = true;
    delay(200);
    cycle_start = true;
    Serial3.print("$VSSY0000&");
  }
  else
  {
    inti_Start();
  }
  return true;
}

bool inti_Start()
{
  convert_all_set_params_2_machine_values();
  open_selected_O2_value();
  Emergency_motor_stop = false;
  Serial.println("Skipping Home Cycle : ");
  home_cycle = false;
  cycle_start = true;
  comp_start = false;
  comp_end = false;
  exp_start = true;
  exp_end = true;
  //exp_timer_end = true;
  Exhale_timer_timout();
  //run_motor = true
  return true;
}


void pick_stroke_length()
{
  if(IER_new == 1 && BPM_new ==10 && tidal_volume_new ==200){ Stroke_length_new = 59.5;}
  if(IER_new == 1 && BPM_new ==11 && tidal_volume_new ==200){ Stroke_length_new = 58;}
  if(IER_new == 1 && BPM_new ==12 && tidal_volume_new ==200){ Stroke_length_new = 56.5;}
  if(IER_new == 1 && BPM_new ==13 && tidal_volume_new ==200){ Stroke_length_new = 55.5;}
  if(IER_new == 1 && BPM_new ==14 && tidal_volume_new ==200){ Stroke_length_new = 54.5;}
  if(IER_new == 1 && BPM_new ==15 && tidal_volume_new ==200){ Stroke_length_new = 53.5;}
  if(IER_new == 1 && BPM_new ==16 && tidal_volume_new ==200){ Stroke_length_new = 53;}
  if(IER_new == 1 && BPM_new ==17 && tidal_volume_new ==200){ Stroke_length_new = 52.5;}
  if(IER_new == 1 && BPM_new ==18 && tidal_volume_new ==200){ Stroke_length_new = 52;}
  if(IER_new == 1 && BPM_new ==19 && tidal_volume_new ==200){ Stroke_length_new = 51.5;}
  if(IER_new == 1 && BPM_new ==20 && tidal_volume_new ==200){ Stroke_length_new = 51.3;}
  if(IER_new == 1 && BPM_new ==21 && tidal_volume_new ==200){ Stroke_length_new = 51.1;}
  if(IER_new == 1 && BPM_new ==22 && tidal_volume_new ==200){ Stroke_length_new = 50.9;}
  if(IER_new == 1 && BPM_new ==23 && tidal_volume_new ==200){ Stroke_length_new = 50.7;}
  if(IER_new == 1 && BPM_new ==24 && tidal_volume_new ==200){ Stroke_length_new = 50.5;}
  if(IER_new == 1 && BPM_new ==25 && tidal_volume_new ==200){ Stroke_length_new = 50.3;}
  if(IER_new == 1 && BPM_new ==26 && tidal_volume_new ==200){ Stroke_length_new = 50.1;}
  if(IER_new == 1 && BPM_new ==27 && tidal_volume_new ==200){ Stroke_length_new = 49.9;}
  if(IER_new == 1 && BPM_new ==28 && tidal_volume_new ==200){ Stroke_length_new = 49.7;}
  if(IER_new == 1 && BPM_new ==29 && tidal_volume_new ==200){ Stroke_length_new = 49.5;}
  if(IER_new == 1 && BPM_new ==30 && tidal_volume_new ==200){ Stroke_length_new = 49.3;}
  
  if(IER_new == 1 && BPM_new ==10 && tidal_volume_new ==250){ Stroke_length_new = 62.5;}
  if(IER_new == 1 && BPM_new ==11 && tidal_volume_new ==250){ Stroke_length_new = 62;}
  if(IER_new == 1 && BPM_new ==12 && tidal_volume_new ==250){ Stroke_length_new = 61.2;}
  if(IER_new == 1 && BPM_new ==13 && tidal_volume_new ==250){ Stroke_length_new = 60;}
  if(IER_new == 1 && BPM_new ==14 && tidal_volume_new ==250){ Stroke_length_new = 59.5;}
  if(IER_new == 1 && BPM_new ==15 && tidal_volume_new ==250){ Stroke_length_new = 58.5;}
  if(IER_new == 1 && BPM_new ==16 && tidal_volume_new ==250){ Stroke_length_new = 58;}
  if(IER_new == 1 && BPM_new ==17 && tidal_volume_new ==250){ Stroke_length_new = 57.5;}
  if(IER_new == 1 && BPM_new ==18 && tidal_volume_new ==250){ Stroke_length_new = 57.2;}
  if(IER_new == 1 && BPM_new ==19 && tidal_volume_new ==250){ Stroke_length_new = 56.9;}
  if(IER_new == 1 && BPM_new ==20 && tidal_volume_new ==250){ Stroke_length_new = 56.6;}
  if(IER_new == 1 && BPM_new ==21 && tidal_volume_new ==250){ Stroke_length_new = 56.3;}
  if(IER_new == 1 && BPM_new ==22 && tidal_volume_new ==250){ Stroke_length_new = 56;}
  if(IER_new == 1 && BPM_new ==23 && tidal_volume_new ==250){ Stroke_length_new = 55.7;}
  if(IER_new == 1 && BPM_new ==24 && tidal_volume_new ==250){ Stroke_length_new = 55.4;}
  if(IER_new == 1 && BPM_new ==25 && tidal_volume_new ==250){ Stroke_length_new = 55.1;}
  if(IER_new == 1 && BPM_new ==26 && tidal_volume_new ==250){ Stroke_length_new = 55.1;}
  if(IER_new == 1 && BPM_new ==27 && tidal_volume_new ==250){ Stroke_length_new = 55.1;}
  if(IER_new == 1 && BPM_new ==28 && tidal_volume_new ==250){ Stroke_length_new = 55.1;}
  if(IER_new == 1 && BPM_new ==29 && tidal_volume_new ==250){ Stroke_length_new = 55.1;}
  if(IER_new == 1 && BPM_new ==30 && tidal_volume_new ==250){ Stroke_length_new = 55.1;}
  
  if(IER_new == 1 && BPM_new ==10 && tidal_volume_new ==300){ Stroke_length_new = 67;}
  if(IER_new == 1 && BPM_new ==11 && tidal_volume_new ==300){ Stroke_length_new = 66;}
  if(IER_new == 1 && BPM_new ==12 && tidal_volume_new ==300){ Stroke_length_new = 65;}
  if(IER_new == 1 && BPM_new ==13 && tidal_volume_new ==300){ Stroke_length_new = 64.1;}
  if(IER_new == 1 && BPM_new ==14 && tidal_volume_new ==300){ Stroke_length_new = 63.4;}
  if(IER_new == 1 && BPM_new ==15 && tidal_volume_new ==300){ Stroke_length_new = 62.7;}
  if(IER_new == 1 && BPM_new ==16 && tidal_volume_new ==300){ Stroke_length_new = 62.6;}
  if(IER_new == 1 && BPM_new ==17 && tidal_volume_new ==300){ Stroke_length_new = 62.2;}
  if(IER_new == 1 && BPM_new ==18 && tidal_volume_new ==300){ Stroke_length_new = 61.8;}
  if(IER_new == 1 && BPM_new ==19 && tidal_volume_new ==300){ Stroke_length_new = 61.4;}
  if(IER_new == 1 && BPM_new ==20 && tidal_volume_new ==300){ Stroke_length_new = 61;}
  if(IER_new == 1 && BPM_new ==21 && tidal_volume_new ==300){ Stroke_length_new = 60.9;}
  if(IER_new == 1 && BPM_new ==22 && tidal_volume_new ==300){ Stroke_length_new = 60.8;}
  if(IER_new == 1 && BPM_new ==23 && tidal_volume_new ==300){ Stroke_length_new = 60.7;}
  if(IER_new == 1 && BPM_new ==24 && tidal_volume_new ==300){ Stroke_length_new = 60.6;}
  if(IER_new == 1 && BPM_new ==25 && tidal_volume_new ==300){ Stroke_length_new = 60.5;}
  if(IER_new == 1 && BPM_new ==26 && tidal_volume_new ==300){ Stroke_length_new = 60.4;}
  if(IER_new == 1 && BPM_new ==27 && tidal_volume_new ==300){ Stroke_length_new = 60.3;}
  if(IER_new == 1 && BPM_new ==28 && tidal_volume_new ==300){ Stroke_length_new = 60.2;}
  if(IER_new == 1 && BPM_new ==29 && tidal_volume_new ==300){ Stroke_length_new = 60.1;}
  if(IER_new == 1 && BPM_new ==30 && tidal_volume_new ==300){ Stroke_length_new = 60;}
  
  if(IER_new == 1 && BPM_new ==10 && tidal_volume_new ==350){ Stroke_length_new = 70.4;}
  if(IER_new == 1 && BPM_new ==11 && tidal_volume_new ==350){ Stroke_length_new = 69;}
  if(IER_new == 1 && BPM_new ==12 && tidal_volume_new ==350){ Stroke_length_new = 68.5;}
  if(IER_new == 1 && BPM_new ==13 && tidal_volume_new ==350){ Stroke_length_new = 67.5;}
  if(IER_new == 1 && BPM_new ==14 && tidal_volume_new ==350){ Stroke_length_new = 66.8;}
  if(IER_new == 1 && BPM_new ==15 && tidal_volume_new ==350){ Stroke_length_new = 66.4;}
  if(IER_new == 1 && BPM_new ==16 && tidal_volume_new ==350){ Stroke_length_new = 66.2;}
  if(IER_new == 1 && BPM_new ==17 && tidal_volume_new ==350){ Stroke_length_new = 65.8;}
  if(IER_new == 1 && BPM_new ==18 && tidal_volume_new ==350){ Stroke_length_new = 65.5;}
  if(IER_new == 1 && BPM_new ==19 && tidal_volume_new ==350){ Stroke_length_new = 65.2;}
  if(IER_new == 1 && BPM_new ==20 && tidal_volume_new ==350){ Stroke_length_new = 65.1;}
  if(IER_new == 1 && BPM_new ==21 && tidal_volume_new ==350){ Stroke_length_new = 65;}
  if(IER_new == 1 && BPM_new ==22 && tidal_volume_new ==350){ Stroke_length_new = 64.9;}
  if(IER_new == 1 && BPM_new ==23 && tidal_volume_new ==350){ Stroke_length_new = 64.9;}
  if(IER_new == 1 && BPM_new ==24 && tidal_volume_new ==350){ Stroke_length_new = 64.9;}
  if(IER_new == 1 && BPM_new ==25 && tidal_volume_new ==350){ Stroke_length_new = 64.8;}
  if(IER_new == 1 && BPM_new ==26 && tidal_volume_new ==350){ Stroke_length_new = 64.5;}
  if(IER_new == 1 && BPM_new ==27 && tidal_volume_new ==350){ Stroke_length_new = 64.5;}
  if(IER_new == 1 && BPM_new ==28 && tidal_volume_new ==350){ Stroke_length_new = 64.4;}
  if(IER_new == 1 && BPM_new ==29 && tidal_volume_new ==350){ Stroke_length_new = 64.2;}
  if(IER_new == 1 && BPM_new ==30 && tidal_volume_new ==350){ Stroke_length_new = 64.2;}
  
  if(IER_new == 1 && BPM_new ==10 && tidal_volume_new ==400){ Stroke_length_new = 74.1;}
  if(IER_new == 1 && BPM_new ==11 && tidal_volume_new ==400){ Stroke_length_new = 73.7;}
  if(IER_new == 1 && BPM_new ==12 && tidal_volume_new ==400){ Stroke_length_new = 73;}
  if(IER_new == 1 && BPM_new ==13 && tidal_volume_new ==400){ Stroke_length_new = 72.4;}
  if(IER_new == 1 && BPM_new ==14 && tidal_volume_new ==400){ Stroke_length_new = 71.8;}
  if(IER_new == 1 && BPM_new ==15 && tidal_volume_new ==400){ Stroke_length_new = 71.4;}
  if(IER_new == 1 && BPM_new ==16 && tidal_volume_new ==400){ Stroke_length_new = 71.2;}
  if(IER_new == 1 && BPM_new ==17 && tidal_volume_new ==400){ Stroke_length_new = 70.8;}
  if(IER_new == 1 && BPM_new ==18 && tidal_volume_new ==400){ Stroke_length_new = 70.2;}
  if(IER_new == 1 && BPM_new ==19 && tidal_volume_new ==400){ Stroke_length_new = 70.1;}
  if(IER_new == 1 && BPM_new ==20 && tidal_volume_new ==400){ Stroke_length_new = 69.8;}
  if(IER_new == 1 && BPM_new ==21 && tidal_volume_new ==400){ Stroke_length_new = 69.6;}
  if(IER_new == 1 && BPM_new ==22 && tidal_volume_new ==400){ Stroke_length_new = 69.6;}
  if(IER_new == 1 && BPM_new ==23 && tidal_volume_new ==400){ Stroke_length_new = 69.2;}
  if(IER_new == 1 && BPM_new ==24 && tidal_volume_new ==400){ Stroke_length_new = 69;}
  if(IER_new == 1 && BPM_new ==25 && tidal_volume_new ==400){ Stroke_length_new = 68.8;}
  if(IER_new == 1 && BPM_new ==26 && tidal_volume_new ==400){ Stroke_length_new = 68.8;}
  if(IER_new == 1 && BPM_new ==27 && tidal_volume_new ==400){ Stroke_length_new = 68.7;}
  if(IER_new == 1 && BPM_new ==28 && tidal_volume_new ==400){ Stroke_length_new = 68.7;}
  if(IER_new == 1 && BPM_new ==29 && tidal_volume_new ==400){ Stroke_length_new = 68.7;}
  if(IER_new == 1 && BPM_new ==30 && tidal_volume_new ==400){ Stroke_length_new = 68.7;}
  
  if(IER_new == 1 && BPM_new ==10 && tidal_volume_new ==450){ Stroke_length_new = 77.5;}
  if(IER_new == 1 && BPM_new ==11 && tidal_volume_new ==450){ Stroke_length_new = 77;}
  if(IER_new == 1 && BPM_new ==12 && tidal_volume_new ==450){ Stroke_length_new = 76.5;}
  if(IER_new == 1 && BPM_new ==13 && tidal_volume_new ==450){ Stroke_length_new = 76;}
  if(IER_new == 1 && BPM_new ==14 && tidal_volume_new ==450){ Stroke_length_new = 75.5;}
  if(IER_new == 1 && BPM_new ==15 && tidal_volume_new ==450){ Stroke_length_new = 75;}
  if(IER_new == 1 && BPM_new ==16 && tidal_volume_new ==450){ Stroke_length_new = 74.8;}
  if(IER_new == 1 && BPM_new ==17 && tidal_volume_new ==450){ Stroke_length_new = 74.6;}
  if(IER_new == 1 && BPM_new ==18 && tidal_volume_new ==450){ Stroke_length_new = 74.4;}
  if(IER_new == 1 && BPM_new ==19 && tidal_volume_new ==450){ Stroke_length_new = 74.2;}
  if(IER_new == 1 && BPM_new ==20 && tidal_volume_new ==450){ Stroke_length_new = 74;}
  if(IER_new == 1 && BPM_new ==21 && tidal_volume_new ==450){ Stroke_length_new = 73.8;}
  if(IER_new == 1 && BPM_new ==22 && tidal_volume_new ==450){ Stroke_length_new = 73.6;}
  if(IER_new == 1 && BPM_new ==23 && tidal_volume_new ==450){ Stroke_length_new = 73.4;}
  if(IER_new == 1 && BPM_new ==24 && tidal_volume_new ==450){ Stroke_length_new = 73.4;}
  if(IER_new == 1 && BPM_new ==25 && tidal_volume_new ==450){ Stroke_length_new = 73.4;}
  if(IER_new == 1 && BPM_new ==26 && tidal_volume_new ==450){ Stroke_length_new = 73.4;}
  if(IER_new == 1 && BPM_new ==27 && tidal_volume_new ==450){ Stroke_length_new = 73.4;}
  if(IER_new == 1 && BPM_new ==28 && tidal_volume_new ==450){ Stroke_length_new = 73.4;}
  if(IER_new == 1 && BPM_new ==29 && tidal_volume_new ==450){ Stroke_length_new = 73.4;}
  if(IER_new == 1 && BPM_new ==30 && tidal_volume_new ==450){ Stroke_length_new = 73.4;}
  
  if(IER_new == 1 && BPM_new ==10 && tidal_volume_new ==500){ Stroke_length_new = 81.5;}
  if(IER_new == 1 && BPM_new ==11 && tidal_volume_new ==500){ Stroke_length_new = 81;}
  if(IER_new == 1 && BPM_new ==12 && tidal_volume_new ==500){ Stroke_length_new = 80.5;}
  if(IER_new == 1 && BPM_new ==13 && tidal_volume_new ==500){ Stroke_length_new = 80;}
  if(IER_new == 1 && BPM_new ==14 && tidal_volume_new ==500){ Stroke_length_new = 79.5;}
  if(IER_new == 1 && BPM_new ==15 && tidal_volume_new ==500){ Stroke_length_new = 79;}
  if(IER_new == 1 && BPM_new ==16 && tidal_volume_new ==500){ Stroke_length_new = 78.8;}
  if(IER_new == 1 && BPM_new ==17 && tidal_volume_new ==500){ Stroke_length_new = 78.6;}
  if(IER_new == 1 && BPM_new ==18 && tidal_volume_new ==500){ Stroke_length_new = 78.4;}
  if(IER_new == 1 && BPM_new ==19 && tidal_volume_new ==500){ Stroke_length_new = 78.2;}
  if(IER_new == 1 && BPM_new ==20 && tidal_volume_new ==500){ Stroke_length_new = 78;}
  if(IER_new == 1 && BPM_new ==21 && tidal_volume_new ==500){ Stroke_length_new = 78.1;}
  if(IER_new == 1 && BPM_new ==22 && tidal_volume_new ==500){ Stroke_length_new = 78.2;}
  if(IER_new == 1 && BPM_new ==23 && tidal_volume_new ==500){ Stroke_length_new = 78.3;}
  if(IER_new == 1 && BPM_new ==24 && tidal_volume_new ==500){ Stroke_length_new = 78.4;}
  if(IER_new == 1 && BPM_new ==25 && tidal_volume_new ==500){ Stroke_length_new = 78.5;}
  if(IER_new == 1 && BPM_new ==26 && tidal_volume_new ==500){ Stroke_length_new = 78.8;}
  if(IER_new == 1 && BPM_new ==27 && tidal_volume_new ==500){ Stroke_length_new = 79.1;}
  if(IER_new == 1 && BPM_new ==28 && tidal_volume_new ==500){ Stroke_length_new = 79.4;}
  if(IER_new == 1 && BPM_new ==29 && tidal_volume_new ==500){ Stroke_length_new = 79.7;}
  if(IER_new == 1 && BPM_new ==30 && tidal_volume_new ==500){ Stroke_length_new = 80;}
  
  if(IER_new == 1 && BPM_new ==10 && tidal_volume_new ==550){ Stroke_length_new = 86;}
  if(IER_new == 1 && BPM_new ==11 && tidal_volume_new ==550){ Stroke_length_new = 85.6;}
  if(IER_new == 1 && BPM_new ==12 && tidal_volume_new ==550){ Stroke_length_new = 85.2;}
  if(IER_new == 1 && BPM_new ==13 && tidal_volume_new ==550){ Stroke_length_new = 84.8;}
  if(IER_new == 1 && BPM_new ==14 && tidal_volume_new ==550){ Stroke_length_new = 84.4;}
  if(IER_new == 1 && BPM_new ==15 && tidal_volume_new ==550){ Stroke_length_new = 84;}
  if(IER_new == 1 && BPM_new ==16 && tidal_volume_new ==550){ Stroke_length_new = 83.8;}
  if(IER_new == 1 && BPM_new ==17 && tidal_volume_new ==550){ Stroke_length_new = 83.6;}
  if(IER_new == 1 && BPM_new ==18 && tidal_volume_new ==550){ Stroke_length_new = 83.4;}
  if(IER_new == 1 && BPM_new ==19 && tidal_volume_new ==550){ Stroke_length_new = 83.2;}
  if(IER_new == 1 && BPM_new ==20 && tidal_volume_new ==550){ Stroke_length_new = 83;}
  if(IER_new == 1 && BPM_new ==21 && tidal_volume_new ==550){ Stroke_length_new = 82.8;}
  if(IER_new == 1 && BPM_new ==22 && tidal_volume_new ==550){ Stroke_length_new = 82.6;}
  if(IER_new == 1 && BPM_new ==23 && tidal_volume_new ==550){ Stroke_length_new = 82.4;}
  if(IER_new == 1 && BPM_new ==24 && tidal_volume_new ==550){ Stroke_length_new = 82.2;}
  if(IER_new == 1 && BPM_new ==25 && tidal_volume_new ==550){ Stroke_length_new = 82;}
  if(IER_new == 1 && BPM_new ==26 && tidal_volume_new ==550){ Stroke_length_new = 82.4;}
  if(IER_new == 1 && BPM_new ==27 && tidal_volume_new ==550){ Stroke_length_new = 82.8;}
  if(IER_new == 1 && BPM_new ==28 && tidal_volume_new ==550){ Stroke_length_new = 83.2;}
  if(IER_new == 1 && BPM_new ==29 && tidal_volume_new ==550){ Stroke_length_new = 83.6;}
  if(IER_new == 1 && BPM_new ==30 && tidal_volume_new ==550){ Stroke_length_new = 84;}
  
  if(IER_new == 1 && BPM_new ==10 && tidal_volume_new ==600){ Stroke_length_new = 92.2;}
  if(IER_new == 1 && BPM_new ==11 && tidal_volume_new ==600){ Stroke_length_new = 91.8;}
  if(IER_new == 1 && BPM_new ==12 && tidal_volume_new ==600){ Stroke_length_new = 91.3;}
  if(IER_new == 1 && BPM_new ==13 && tidal_volume_new ==600){ Stroke_length_new = 90.5;}
  if(IER_new == 1 && BPM_new ==14 && tidal_volume_new ==600){ Stroke_length_new = 90;}
  if(IER_new == 1 && BPM_new ==15 && tidal_volume_new ==600){ Stroke_length_new = 89.4;}
  if(IER_new == 1 && BPM_new ==16 && tidal_volume_new ==600){ Stroke_length_new = 89;}
  if(IER_new == 1 && BPM_new ==17 && tidal_volume_new ==600){ Stroke_length_new = 88.6;}
  if(IER_new == 1 && BPM_new ==18 && tidal_volume_new ==600){ Stroke_length_new = 88.3;}
  if(IER_new == 1 && BPM_new ==19 && tidal_volume_new ==600){ Stroke_length_new = 87.5;}
  if(IER_new == 1 && BPM_new ==20 && tidal_volume_new ==600){ Stroke_length_new = 87.5;}
  if(IER_new == 1 && BPM_new ==21 && tidal_volume_new ==600){ Stroke_length_new = 87.7;}
  if(IER_new == 1 && BPM_new ==22 && tidal_volume_new ==600){ Stroke_length_new = 87.9;}
  if(IER_new == 1 && BPM_new ==23 && tidal_volume_new ==600){ Stroke_length_new = 88.1;}
  if(IER_new == 1 && BPM_new ==24 && tidal_volume_new ==600){ Stroke_length_new = 88.3;}
  if(IER_new == 1 && BPM_new ==25 && tidal_volume_new ==600){ Stroke_length_new = 89;}
  if(IER_new == 1 && BPM_new ==26 && tidal_volume_new ==600){ Stroke_length_new = 89.5;}
  if(IER_new == 1 && BPM_new ==27 && tidal_volume_new ==600){ Stroke_length_new = 90;}
  if(IER_new == 1 && BPM_new ==28 && tidal_volume_new ==600){ Stroke_length_new = 90.5;}
  if(IER_new == 1 && BPM_new ==29 && tidal_volume_new ==600){ Stroke_length_new = 91;}
  if(IER_new == 1 && BPM_new ==30 && tidal_volume_new ==600){ Stroke_length_new = 91.5;}
  
  if(IER_new == 1 && BPM_new ==10 && tidal_volume_new ==650){ Stroke_length_new = 97.1;}
  if(IER_new == 1 && BPM_new ==11 && tidal_volume_new ==650){ Stroke_length_new = 97.1;}
  if(IER_new == 1 && BPM_new ==12 && tidal_volume_new ==650){ Stroke_length_new = 97;}
  if(IER_new == 1 && BPM_new ==13 && tidal_volume_new ==650){ Stroke_length_new = 96.2;}
  if(IER_new == 1 && BPM_new ==14 && tidal_volume_new ==650){ Stroke_length_new = 96.1;}
  if(IER_new == 1 && BPM_new ==15 && tidal_volume_new ==650){ Stroke_length_new = 95.1;}
  if(IER_new == 1 && BPM_new ==16 && tidal_volume_new ==650){ Stroke_length_new = 95.1;}
  if(IER_new == 1 && BPM_new ==17 && tidal_volume_new ==650){ Stroke_length_new = 95.1;}
  if(IER_new == 1 && BPM_new ==18 && tidal_volume_new ==650){ Stroke_length_new = 95;}
  if(IER_new == 1 && BPM_new ==19 && tidal_volume_new ==650){ Stroke_length_new = 94.5;}
  if(IER_new == 1 && BPM_new ==20 && tidal_volume_new ==650){ Stroke_length_new = 94.5;}
  if(IER_new == 1 && BPM_new ==21 && tidal_volume_new ==650){ Stroke_length_new = 94.7;}
  if(IER_new == 1 && BPM_new ==22 && tidal_volume_new ==650){ Stroke_length_new = 94.9;}
  if(IER_new == 1 && BPM_new ==23 && tidal_volume_new ==650){ Stroke_length_new = 95.1;}
  if(IER_new == 1 && BPM_new ==24 && tidal_volume_new ==650){ Stroke_length_new = 95.3;}
  if(IER_new == 1 && BPM_new ==25 && tidal_volume_new ==650){ Stroke_length_new = 95.5;}
  if(IER_new == 1 && BPM_new ==26 && tidal_volume_new ==650){ Stroke_length_new = 95.9;}
  if(IER_new == 1 && BPM_new ==27 && tidal_volume_new ==650){ Stroke_length_new = 96.3;}
  if(IER_new == 1 && BPM_new ==28 && tidal_volume_new ==650){ Stroke_length_new = 96.7;}
  if(IER_new == 1 && BPM_new ==29 && tidal_volume_new ==650){ Stroke_length_new = 97.1;}
  if(IER_new == 1 && BPM_new ==30 && tidal_volume_new ==650){ Stroke_length_new = 97.5;}
  
  if(IER_new == 1 && BPM_new ==10 && tidal_volume_new ==700){ Stroke_length_new = 106;}
  if(IER_new == 1 && BPM_new ==11 && tidal_volume_new ==700){ Stroke_length_new = 105;}
  if(IER_new == 1 && BPM_new ==12 && tidal_volume_new ==700){ Stroke_length_new = 103.5;}
  if(IER_new == 1 && BPM_new ==13 && tidal_volume_new ==700){ Stroke_length_new = 102.5;}
  if(IER_new == 1 && BPM_new ==14 && tidal_volume_new ==700){ Stroke_length_new = 102.5;}
  if(IER_new == 1 && BPM_new ==15 && tidal_volume_new ==700){ Stroke_length_new = 102.4;}
  if(IER_new == 1 && BPM_new ==16 && tidal_volume_new ==700){ Stroke_length_new = 102.3;}
  if(IER_new == 1 && BPM_new ==17 && tidal_volume_new ==700){ Stroke_length_new = 101.2;}
  if(IER_new == 1 && BPM_new ==18 && tidal_volume_new ==700){ Stroke_length_new = 101.2;}
  if(IER_new == 1 && BPM_new ==19 && tidal_volume_new ==700){ Stroke_length_new = 101;}
  if(IER_new == 1 && BPM_new ==20 && tidal_volume_new ==700){ Stroke_length_new = 101;}
  if(IER_new == 1 && BPM_new ==21 && tidal_volume_new ==700){ Stroke_length_new = 101;}
  if(IER_new == 1 && BPM_new ==22 && tidal_volume_new ==700){ Stroke_length_new = 101;}
  if(IER_new == 1 && BPM_new ==23 && tidal_volume_new ==700){ Stroke_length_new = 101;}
  if(IER_new == 1 && BPM_new ==24 && tidal_volume_new ==700){ Stroke_length_new = 101;}
  if(IER_new == 1 && BPM_new ==25 && tidal_volume_new ==700){ Stroke_length_new = 101;}
  if(IER_new == 1 && BPM_new ==26 && tidal_volume_new ==700){ Stroke_length_new = 102;}
  if(IER_new == 1 && BPM_new ==27 && tidal_volume_new ==700){ Stroke_length_new = 102;}
//  if(IER_new == 1 && BPM_new ==28 && tidal_volume_new ==700){ Stroke_length_new = NA;}
//  if(IER_new == 1 && BPM_new ==29 && tidal_volume_new ==700){ Stroke_length_new = NA;}
//  if(IER_new == 1 && BPM_new ==30 && tidal_volume_new ==700){ Stroke_length_new = NA;}






  
  if(IER_new == 2 && BPM_new ==10 && tidal_volume_new ==200){ Stroke_length_new = 53.5;}
  if(IER_new == 2 && BPM_new ==11 && tidal_volume_new ==200){ Stroke_length_new = 52.5;}
  if(IER_new == 2 && BPM_new ==12 && tidal_volume_new ==200){ Stroke_length_new = 51.5;}
  if(IER_new == 2 && BPM_new ==13 && tidal_volume_new ==200){ Stroke_length_new = 51;}
  if(IER_new == 2 && BPM_new ==14 && tidal_volume_new ==200){ Stroke_length_new = 50.5;}
  if(IER_new == 2 && BPM_new ==15 && tidal_volume_new ==200){ Stroke_length_new = 50.2;}
  if(IER_new == 2 && BPM_new ==16 && tidal_volume_new ==200){ Stroke_length_new = 50;}
  if(IER_new == 2 && BPM_new ==17 && tidal_volume_new ==200){ Stroke_length_new = 49.8;}
  if(IER_new == 2 && BPM_new ==18 && tidal_volume_new ==200){ Stroke_length_new = 49.5;}
  if(IER_new == 2 && BPM_new ==19 && tidal_volume_new ==200){ Stroke_length_new = 49.5;}
  if(IER_new == 2 && BPM_new ==20 && tidal_volume_new ==200){ Stroke_length_new = 49.1;}
  if(IER_new == 2 && BPM_new ==21 && tidal_volume_new ==200){ Stroke_length_new = 49;}
  if(IER_new == 2 && BPM_new ==22 && tidal_volume_new ==200){ Stroke_length_new = 48.8;}
  if(IER_new == 2 && BPM_new ==23 && tidal_volume_new ==200){ Stroke_length_new = 48.8;}
  if(IER_new == 2 && BPM_new ==24 && tidal_volume_new ==200){ Stroke_length_new = 48.8;}
  if(IER_new == 2 && BPM_new ==25 && tidal_volume_new ==200){ Stroke_length_new = 48.8;}
  if(IER_new == 2 && BPM_new ==26 && tidal_volume_new ==200){ Stroke_length_new = 49;}
  if(IER_new == 2 && BPM_new ==27 && tidal_volume_new ==200){ Stroke_length_new = 48.8;}
  if(IER_new == 2 && BPM_new ==28 && tidal_volume_new ==200){ Stroke_length_new = 48.8;}
  if(IER_new == 2 && BPM_new ==29 && tidal_volume_new ==200){ Stroke_length_new = 48.8;}
  if(IER_new == 2 && BPM_new ==30 && tidal_volume_new ==200){ Stroke_length_new = 48.8;}
  
  if(IER_new == 2 && BPM_new ==10 && tidal_volume_new ==250){ Stroke_length_new = 58;}
  if(IER_new == 2 && BPM_new ==11 && tidal_volume_new ==250){ Stroke_length_new = 57.5;}
  if(IER_new == 2 && BPM_new ==12 && tidal_volume_new ==250){ Stroke_length_new = 56.6;}
  if(IER_new == 2 && BPM_new ==13 && tidal_volume_new ==250){ Stroke_length_new = 56.5;}
  if(IER_new == 2 && BPM_new ==14 && tidal_volume_new ==250){ Stroke_length_new = 56.2;}
  if(IER_new == 2 && BPM_new ==15 && tidal_volume_new ==250){ Stroke_length_new = 55.9;}
  if(IER_new == 2 && BPM_new ==16 && tidal_volume_new ==250){ Stroke_length_new = 55.6;}
  if(IER_new == 2 && BPM_new ==17 && tidal_volume_new ==250){ Stroke_length_new = 55.3;}
  if(IER_new == 2 && BPM_new ==18 && tidal_volume_new ==250){ Stroke_length_new = 55.1;}
  if(IER_new == 2 && BPM_new ==19 && tidal_volume_new ==250){ Stroke_length_new = 55;}
  if(IER_new == 2 && BPM_new ==20 && tidal_volume_new ==250){ Stroke_length_new = 54.9;}
  if(IER_new == 2 && BPM_new ==21 && tidal_volume_new ==250){ Stroke_length_new = 54.8;}
  if(IER_new == 2 && BPM_new ==22 && tidal_volume_new ==250){ Stroke_length_new = 54.7;}
  if(IER_new == 2 && BPM_new ==23 && tidal_volume_new ==250){ Stroke_length_new = 54.6;}
  if(IER_new == 2 && BPM_new ==24 && tidal_volume_new ==250){ Stroke_length_new = 54.5;}
  if(IER_new == 2 && BPM_new ==25 && tidal_volume_new ==250){ Stroke_length_new = 54.5;}
  if(IER_new == 2 && BPM_new ==26 && tidal_volume_new ==250){ Stroke_length_new = 54.5;}
  if(IER_new == 2 && BPM_new ==27 && tidal_volume_new ==250){ Stroke_length_new = 54.5;}
  if(IER_new == 2 && BPM_new ==28 && tidal_volume_new ==250){ Stroke_length_new = 54.5;}
  if(IER_new == 2 && BPM_new ==29 && tidal_volume_new ==250){ Stroke_length_new = 54.6;}
  if(IER_new == 2 && BPM_new ==30 && tidal_volume_new ==250){ Stroke_length_new = 54.6;}
  
  if(IER_new == 2 && BPM_new ==10 && tidal_volume_new ==300){ Stroke_length_new = 63;}
  if(IER_new == 2 && BPM_new ==11 && tidal_volume_new ==300){ Stroke_length_new = 62.5;}
  if(IER_new == 2 && BPM_new ==12 && tidal_volume_new ==300){ Stroke_length_new = 62.2;}
  if(IER_new == 2 && BPM_new ==13 && tidal_volume_new ==300){ Stroke_length_new = 61.9;}
  if(IER_new == 2 && BPM_new ==14 && tidal_volume_new ==300){ Stroke_length_new = 61;}
  if(IER_new == 2 && BPM_new ==15 && tidal_volume_new ==300){ Stroke_length_new = 60.7;}
  if(IER_new == 2 && BPM_new ==16 && tidal_volume_new ==300){ Stroke_length_new = 60.4;}
  if(IER_new == 2 && BPM_new ==17 && tidal_volume_new ==300){ Stroke_length_new = 60.2;}
  if(IER_new == 2 && BPM_new ==18 && tidal_volume_new ==300){ Stroke_length_new = 60.2;}
  if(IER_new == 2 && BPM_new ==19 && tidal_volume_new ==300){ Stroke_length_new = 59.8;}
  if(IER_new == 2 && BPM_new ==20 && tidal_volume_new ==300){ Stroke_length_new = 59.8;}
  if(IER_new == 2 && BPM_new ==21 && tidal_volume_new ==300){ Stroke_length_new = 59.8;}
  if(IER_new == 2 && BPM_new ==22 && tidal_volume_new ==300){ Stroke_length_new = 59.8;}
  if(IER_new == 2 && BPM_new ==23 && tidal_volume_new ==300){ Stroke_length_new = 59.8;}
  if(IER_new == 2 && BPM_new ==24 && tidal_volume_new ==300){ Stroke_length_new = 59.8;}
  if(IER_new == 2 && BPM_new ==25 && tidal_volume_new ==300){ Stroke_length_new = 59.8;}
  if(IER_new == 2 && BPM_new ==26 && tidal_volume_new ==300){ Stroke_length_new = 59.8;}
  if(IER_new == 2 && BPM_new ==27 && tidal_volume_new ==300){ Stroke_length_new = 59.6;}
  if(IER_new == 2 && BPM_new ==28 && tidal_volume_new ==300){ Stroke_length_new = 59.6;}
  if(IER_new == 2 && BPM_new ==29 && tidal_volume_new ==300){ Stroke_length_new = 59.8;}
  if(IER_new == 2 && BPM_new ==30 && tidal_volume_new ==300){ Stroke_length_new = 59.8;}
  
  if(IER_new == 2 && BPM_new ==10 && tidal_volume_new ==350){ Stroke_length_new = 67.5;}
  if(IER_new == 2 && BPM_new ==11 && tidal_volume_new ==350){ Stroke_length_new = 66.8;}
  if(IER_new == 2 && BPM_new ==12 && tidal_volume_new ==350){ Stroke_length_new = 66.4;}
  if(IER_new == 2 && BPM_new ==13 && tidal_volume_new ==350){ Stroke_length_new = 66.2;}
  if(IER_new == 2 && BPM_new ==14 && tidal_volume_new ==350){ Stroke_length_new = 65.8;}
  if(IER_new == 2 && BPM_new ==15 && tidal_volume_new ==350){ Stroke_length_new = 65.5;}
  if(IER_new == 2 && BPM_new ==16 && tidal_volume_new ==350){ Stroke_length_new = 65.2;}
  if(IER_new == 2 && BPM_new ==17 && tidal_volume_new ==350){ Stroke_length_new = 65.1;}
  if(IER_new == 2 && BPM_new ==18 && tidal_volume_new ==350){ Stroke_length_new = 65;}
  if(IER_new == 2 && BPM_new ==19 && tidal_volume_new ==350){ Stroke_length_new = 64.9;}
  if(IER_new == 2 && BPM_new ==20 && tidal_volume_new ==350){ Stroke_length_new = 64.9;}
  if(IER_new == 2 && BPM_new ==21 && tidal_volume_new ==350){ Stroke_length_new = 64.9;}
  if(IER_new == 2 && BPM_new ==22 && tidal_volume_new ==350){ Stroke_length_new = 64.8;}
  if(IER_new == 2 && BPM_new ==23 && tidal_volume_new ==350){ Stroke_length_new = 64.5;}
  if(IER_new == 2 && BPM_new ==24 && tidal_volume_new ==350){ Stroke_length_new = 64.5;}
  if(IER_new == 2 && BPM_new ==25 && tidal_volume_new ==350){ Stroke_length_new = 64.5;}
  if(IER_new == 2 && BPM_new ==26 && tidal_volume_new ==350){ Stroke_length_new = 64.5;}
  if(IER_new == 2 && BPM_new ==27 && tidal_volume_new ==350){ Stroke_length_new = 64.5;}
  if(IER_new == 2 && BPM_new ==28 && tidal_volume_new ==350){ Stroke_length_new = 64.5;}
  if(IER_new == 2 && BPM_new ==29 && tidal_volume_new ==350){ Stroke_length_new = 64.7;}
  if(IER_new == 2 && BPM_new ==30 && tidal_volume_new ==350){ Stroke_length_new = 64.7;}
  
  if(IER_new == 2 && BPM_new ==10 && tidal_volume_new ==400){ Stroke_length_new = 71.4;}
  if(IER_new == 2 && BPM_new ==11 && tidal_volume_new ==400){ Stroke_length_new = 71.2;}
  if(IER_new == 2 && BPM_new ==12 && tidal_volume_new ==400){ Stroke_length_new = 70.8;}
  if(IER_new == 2 && BPM_new ==13 && tidal_volume_new ==400){ Stroke_length_new = 70.2;}
  if(IER_new == 2 && BPM_new ==14 && tidal_volume_new ==400){ Stroke_length_new = 70.1;}
  if(IER_new == 2 && BPM_new ==15 && tidal_volume_new ==400){ Stroke_length_new = 69.8;}
  if(IER_new == 2 && BPM_new ==16 && tidal_volume_new ==400){ Stroke_length_new = 69.6;}
  if(IER_new == 2 && BPM_new ==17 && tidal_volume_new ==400){ Stroke_length_new = 69.6;}
  if(IER_new == 2 && BPM_new ==18 && tidal_volume_new ==400){ Stroke_length_new = 69.2;}
  if(IER_new == 2 && BPM_new ==19 && tidal_volume_new ==400){ Stroke_length_new = 69;}
  if(IER_new == 2 && BPM_new ==20 && tidal_volume_new ==400){ Stroke_length_new = 68.8;}
  if(IER_new == 2 && BPM_new ==21 && tidal_volume_new ==400){ Stroke_length_new = 68.8;}
  if(IER_new == 2 && BPM_new ==22 && tidal_volume_new ==400){ Stroke_length_new = 68.8;}
  if(IER_new == 2 && BPM_new ==23 && tidal_volume_new ==400){ Stroke_length_new = 68.8;}
  if(IER_new == 2 && BPM_new ==24 && tidal_volume_new ==400){ Stroke_length_new = 68.8;}
  if(IER_new == 2 && BPM_new ==25 && tidal_volume_new ==400){ Stroke_length_new = 68.8;}
  if(IER_new == 2 && BPM_new ==26 && tidal_volume_new ==400){ Stroke_length_new = 69;}
  if(IER_new == 2 && BPM_new ==27 && tidal_volume_new ==400){ Stroke_length_new = 69.3;}
  if(IER_new == 2 && BPM_new ==28 && tidal_volume_new ==400){ Stroke_length_new = 69.6;}
  if(IER_new == 2 && BPM_new ==29 && tidal_volume_new ==400){ Stroke_length_new = 69.9;}
  if(IER_new == 2 && BPM_new ==30 && tidal_volume_new ==400){ Stroke_length_new = 70.2;}
  
  if(IER_new == 2 && BPM_new ==10 && tidal_volume_new ==450){ Stroke_length_new = 75.2;}
  if(IER_new == 2 && BPM_new ==11 && tidal_volume_new ==450){ Stroke_length_new = 75;}
  if(IER_new == 2 && BPM_new ==12 && tidal_volume_new ==450){ Stroke_length_new = 74.5;}
  if(IER_new == 2 && BPM_new ==13 && tidal_volume_new ==450){ Stroke_length_new = 74.2;}
  if(IER_new == 2 && BPM_new ==14 && tidal_volume_new ==450){ Stroke_length_new = 73.8;}
  if(IER_new == 2 && BPM_new ==15 && tidal_volume_new ==450){ Stroke_length_new = 73.8;}
  if(IER_new == 2 && BPM_new ==16 && tidal_volume_new ==450){ Stroke_length_new = 73.6;}
  if(IER_new == 2 && BPM_new ==17 && tidal_volume_new ==450){ Stroke_length_new = 73.4;}
  if(IER_new == 2 && BPM_new ==18 && tidal_volume_new ==450){ Stroke_length_new = 73.3;}
  if(IER_new == 2 && BPM_new ==19 && tidal_volume_new ==450){ Stroke_length_new = 73.1;}
  if(IER_new == 2 && BPM_new ==20 && tidal_volume_new ==450){ Stroke_length_new = 73.1;}
  if(IER_new == 2 && BPM_new ==21 && tidal_volume_new ==450){ Stroke_length_new = 73.3;}
  if(IER_new == 2 && BPM_new ==22 && tidal_volume_new ==450){ Stroke_length_new = 73.5;}
  if(IER_new == 2 && BPM_new ==23 && tidal_volume_new ==450){ Stroke_length_new = 73.7;}
  if(IER_new == 2 && BPM_new ==24 && tidal_volume_new ==450){ Stroke_length_new = 73.9;}
  if(IER_new == 2 && BPM_new ==25 && tidal_volume_new ==450){ Stroke_length_new = 74.1;}
  if(IER_new == 2 && BPM_new ==26 && tidal_volume_new ==450){ Stroke_length_new = 74.3;}
  if(IER_new == 2 && BPM_new ==27 && tidal_volume_new ==450){ Stroke_length_new = 74.5;}
  if(IER_new == 2 && BPM_new ==28 && tidal_volume_new ==450){ Stroke_length_new = 74.7;}
  if(IER_new == 2 && BPM_new ==29 && tidal_volume_new ==450){ Stroke_length_new = 74.9;}
  if(IER_new == 2 && BPM_new ==30 && tidal_volume_new ==450){ Stroke_length_new = 75.1;}
  
  if(IER_new == 2 && BPM_new ==10 && tidal_volume_new ==500){ Stroke_length_new = 79.3;}
  if(IER_new == 2 && BPM_new ==11 && tidal_volume_new ==500){ Stroke_length_new = 78.9;}
  if(IER_new == 2 && BPM_new ==12 && tidal_volume_new ==500){ Stroke_length_new = 78.7;}
  if(IER_new == 2 && BPM_new ==13 && tidal_volume_new ==500){ Stroke_length_new = 78.3;}
  if(IER_new == 2 && BPM_new ==14 && tidal_volume_new ==500){ Stroke_length_new = 78;}
  if(IER_new == 2 && BPM_new ==15 && tidal_volume_new ==500){ Stroke_length_new = 77.7;}
  if(IER_new == 2 && BPM_new ==16 && tidal_volume_new ==500){ Stroke_length_new = 77.6;}
  if(IER_new == 2 && BPM_new ==17 && tidal_volume_new ==500){ Stroke_length_new = 77.6;}
  if(IER_new == 2 && BPM_new ==18 && tidal_volume_new ==500){ Stroke_length_new = 77.6;}
  if(IER_new == 2 && BPM_new ==19 && tidal_volume_new ==500){ Stroke_length_new = 77.6;}
  if(IER_new == 2 && BPM_new ==20 && tidal_volume_new ==500){ Stroke_length_new = 77.6;}
  if(IER_new == 2 && BPM_new ==21 && tidal_volume_new ==500){ Stroke_length_new = 77.9;}
  if(IER_new == 2 && BPM_new ==22 && tidal_volume_new ==500){ Stroke_length_new = 78.2;}
  if(IER_new == 2 && BPM_new ==23 && tidal_volume_new ==500){ Stroke_length_new = 78.5;}
  if(IER_new == 2 && BPM_new ==24 && tidal_volume_new ==500){ Stroke_length_new = 78.8;}
  if(IER_new == 2 && BPM_new ==25 && tidal_volume_new ==500){ Stroke_length_new = 79.1;}
  if(IER_new == 2 && BPM_new ==26 && tidal_volume_new ==500){ Stroke_length_new = 79.4;}
  if(IER_new == 2 && BPM_new ==27 && tidal_volume_new ==500){ Stroke_length_new = 79.7;}
  if(IER_new == 2 && BPM_new ==28 && tidal_volume_new ==500){ Stroke_length_new = 80;}
  if(IER_new == 2 && BPM_new ==29 && tidal_volume_new ==500){ Stroke_length_new = 80.3;}
  if(IER_new == 2 && BPM_new ==30 && tidal_volume_new ==500){ Stroke_length_new = 80.6;}
  
  if(IER_new == 2 && BPM_new ==10 && tidal_volume_new ==550){ Stroke_length_new = 83.5;}
  if(IER_new == 2 && BPM_new ==11 && tidal_volume_new ==550){ Stroke_length_new = 83.1;}
  if(IER_new == 2 && BPM_new ==12 && tidal_volume_new ==550){ Stroke_length_new = 82.8;}
  if(IER_new == 2 && BPM_new ==13 && tidal_volume_new ==550){ Stroke_length_new = 82.4;}
  if(IER_new == 2 && BPM_new ==14 && tidal_volume_new ==550){ Stroke_length_new = 82.2;}
  if(IER_new == 2 && BPM_new ==15 && tidal_volume_new ==550){ Stroke_length_new = 82.2;}
  if(IER_new == 2 && BPM_new ==16 && tidal_volume_new ==550){ Stroke_length_new = 82.2;}
  if(IER_new == 2 && BPM_new ==17 && tidal_volume_new ==550){ Stroke_length_new = 82;}
  if(IER_new == 2 && BPM_new ==18 && tidal_volume_new ==550){ Stroke_length_new = 82;}
  if(IER_new == 2 && BPM_new ==19 && tidal_volume_new ==550){ Stroke_length_new = 82;}
  if(IER_new == 2 && BPM_new ==20 && tidal_volume_new ==550){ Stroke_length_new = 82;}
  if(IER_new == 2 && BPM_new ==21 && tidal_volume_new ==550){ Stroke_length_new = 82.3;}
  if(IER_new == 2 && BPM_new ==22 && tidal_volume_new ==550){ Stroke_length_new = 82.6;}
  if(IER_new == 2 && BPM_new ==23 && tidal_volume_new ==550){ Stroke_length_new = 82.9;}
  if(IER_new == 2 && BPM_new ==24 && tidal_volume_new ==550){ Stroke_length_new = 83.2;}
  if(IER_new == 2 && BPM_new ==25 && tidal_volume_new ==550){ Stroke_length_new = 83.5;}
  if(IER_new == 2 && BPM_new ==26 && tidal_volume_new ==550){ Stroke_length_new = 83.9;}
  if(IER_new == 2 && BPM_new ==27 && tidal_volume_new ==550){ Stroke_length_new = 84.3;}
  if(IER_new == 2 && BPM_new ==28 && tidal_volume_new ==550){ Stroke_length_new = 84.7;}
  if(IER_new == 2 && BPM_new ==29 && tidal_volume_new ==550){ Stroke_length_new = 85.1;}
  if(IER_new == 2 && BPM_new ==30 && tidal_volume_new ==550){ Stroke_length_new = 85.5;}
  
  if(IER_new == 2 && BPM_new ==10 && tidal_volume_new ==600){ Stroke_length_new = 88.6;}
  if(IER_new == 2 && BPM_new ==11 && tidal_volume_new ==600){ Stroke_length_new = 88.3;}
  if(IER_new == 2 && BPM_new ==12 && tidal_volume_new ==600){ Stroke_length_new = 88;}
  if(IER_new == 2 && BPM_new ==13 && tidal_volume_new ==600){ Stroke_length_new = 87.7;}
  if(IER_new == 2 && BPM_new ==14 && tidal_volume_new ==600){ Stroke_length_new = 87.4;}
  if(IER_new == 2 && BPM_new ==15 && tidal_volume_new ==600){ Stroke_length_new = 87.1;}
  if(IER_new == 2 && BPM_new ==16 && tidal_volume_new ==600){ Stroke_length_new = 87.3;}
  if(IER_new == 2 && BPM_new ==17 && tidal_volume_new ==600){ Stroke_length_new = 87.5;}
  if(IER_new == 2 && BPM_new ==18 && tidal_volume_new ==600){ Stroke_length_new = 87.7;}
  if(IER_new == 2 && BPM_new ==19 && tidal_volume_new ==600){ Stroke_length_new = 87.9;}
  if(IER_new == 2 && BPM_new ==20 && tidal_volume_new ==600){ Stroke_length_new = 88.1;}
  if(IER_new == 2 && BPM_new ==21 && tidal_volume_new ==600){ Stroke_length_new = 88.3;}
  if(IER_new == 2 && BPM_new ==22 && tidal_volume_new ==600){ Stroke_length_new = 88.5;}
  if(IER_new == 2 && BPM_new ==23 && tidal_volume_new ==600){ Stroke_length_new = 88.7;}
  if(IER_new == 2 && BPM_new ==24 && tidal_volume_new ==600){ Stroke_length_new = 88.9;}
  if(IER_new == 2 && BPM_new ==25 && tidal_volume_new ==600){ Stroke_length_new = 89.4;}
  if(IER_new == 2 && BPM_new ==26 && tidal_volume_new ==600){ Stroke_length_new = 89.9;}
  if(IER_new == 2 && BPM_new ==27 && tidal_volume_new ==600){ Stroke_length_new = 90.4;}
  if(IER_new == 2 && BPM_new ==28 && tidal_volume_new ==600){ Stroke_length_new = 90.9;}
  if(IER_new == 2 && BPM_new ==29 && tidal_volume_new ==600){ Stroke_length_new = 91.4;}
  if(IER_new == 2 && BPM_new ==30 && tidal_volume_new ==600){ Stroke_length_new = 91.9;}
  
  if(IER_new == 2 && BPM_new ==10 && tidal_volume_new ==650){ Stroke_length_new = 94.5;}
  if(IER_new == 2 && BPM_new ==11 && tidal_volume_new ==650){ Stroke_length_new = 94.3;}
  if(IER_new == 2 && BPM_new ==12 && tidal_volume_new ==650){ Stroke_length_new = 94.1;}
  if(IER_new == 2 && BPM_new ==13 && tidal_volume_new ==650){ Stroke_length_new = 93.9;}
  if(IER_new == 2 && BPM_new ==14 && tidal_volume_new ==650){ Stroke_length_new = 93.7;}
  if(IER_new == 2 && BPM_new ==15 && tidal_volume_new ==650){ Stroke_length_new = 93.5;}
  if(IER_new == 2 && BPM_new ==16 && tidal_volume_new ==650){ Stroke_length_new = 93.7;}
  if(IER_new == 2 && BPM_new ==17 && tidal_volume_new ==650){ Stroke_length_new = 93.9;}
  if(IER_new == 2 && BPM_new ==18 && tidal_volume_new ==650){ Stroke_length_new = 94.1;}
  if(IER_new == 2 && BPM_new ==19 && tidal_volume_new ==650){ Stroke_length_new = 94.3;}
  if(IER_new == 2 && BPM_new ==20 && tidal_volume_new ==650){ Stroke_length_new = 94.5;}
  if(IER_new == 2 && BPM_new ==21 && tidal_volume_new ==650){ Stroke_length_new = 94.7;}
  if(IER_new == 2 && BPM_new ==22 && tidal_volume_new ==650){ Stroke_length_new = 94.9;}
  if(IER_new == 2 && BPM_new ==23 && tidal_volume_new ==650){ Stroke_length_new = 95.1;}
  if(IER_new == 2 && BPM_new ==24 && tidal_volume_new ==650){ Stroke_length_new = 95.3;}
  if(IER_new == 2 && BPM_new ==25 && tidal_volume_new ==650){ Stroke_length_new = 95.5;}
  if(IER_new == 2 && BPM_new ==26 && tidal_volume_new ==650){ Stroke_length_new = 96.5;}
  if(IER_new == 2 && BPM_new ==27 && tidal_volume_new ==650){ Stroke_length_new = 97;}
//  if(IER_new == 2 && BPM_new ==28 && tidal_volume_new ==650){ Stroke_length_new = NA;}
//  if(IER_new == 2 && BPM_new ==29 && tidal_volume_new ==650){ Stroke_length_new = NA;}
//  if(IER_new == 2 && BPM_new ==30 && tidal_volume_new ==650){ Stroke_length_new = NA;}
  
  if(IER_new == 2 && BPM_new ==10 && tidal_volume_new ==700){ Stroke_length_new = 102;}
  if(IER_new == 2 && BPM_new ==11 && tidal_volume_new ==700){ Stroke_length_new = 101.6;}
  if(IER_new == 2 && BPM_new ==12 && tidal_volume_new ==700){ Stroke_length_new = 101.2;}
  if(IER_new == 2 && BPM_new ==13 && tidal_volume_new ==700){ Stroke_length_new = 100.8;}
  if(IER_new == 2 && BPM_new ==14 && tidal_volume_new ==700){ Stroke_length_new = 100.4;}
  if(IER_new == 2 && BPM_new ==15 && tidal_volume_new ==700){ Stroke_length_new = 100.4;}
  if(IER_new == 2 && BPM_new ==16 && tidal_volume_new ==700){ Stroke_length_new = 100.6;}
  if(IER_new == 2 && BPM_new ==17 && tidal_volume_new ==700){ Stroke_length_new = 100.8;}
  if(IER_new == 2 && BPM_new ==18 && tidal_volume_new ==700){ Stroke_length_new = 101;}
  if(IER_new == 2 && BPM_new ==19 && tidal_volume_new ==700){ Stroke_length_new = 101.2;}
  if(IER_new == 2 && BPM_new ==20 && tidal_volume_new ==700){ Stroke_length_new = 101.4;}
  if(IER_new == 2 && BPM_new ==21 && tidal_volume_new ==700){ Stroke_length_new = 101.9;}
  if(IER_new == 2 && BPM_new ==22 && tidal_volume_new ==700){ Stroke_length_new = 102.4;}
  if(IER_new == 2 && BPM_new ==23 && tidal_volume_new ==700){ Stroke_length_new = 102.9;}
  if(IER_new == 2 && BPM_new ==24 && tidal_volume_new ==700){ Stroke_length_new = 103.4;}
//  if(IER_new == 2 && BPM_new ==25 && tidal_volume_new ==700){ Stroke_length_new = NA;}
//  if(IER_new == 2 && BPM_new ==26 && tidal_volume_new ==700){ Stroke_length_new = NA;}
//  if(IER_new == 2 && BPM_new ==27 && tidal_volume_new ==700){ Stroke_length_new = NA;}
//  if(IER_new == 2 && BPM_new ==28 && tidal_volume_new ==700){ Stroke_length_new = NA;}
//  if(IER_new == 2 && BPM_new ==29 && tidal_volume_new ==700){ Stroke_length_new = NA;}
//  if(IER_new == 2 && BPM_new ==30 && tidal_volume_new ==700){ Stroke_length_new = NA;}





  if(IER_new == 3 && BPM_new ==10 && tidal_volume_new ==200){ Stroke_length_new = 51;}
  if(IER_new == 3 && BPM_new ==11 && tidal_volume_new ==200){ Stroke_length_new = 50.5;}
  if(IER_new == 3 && BPM_new ==12 && tidal_volume_new ==200){ Stroke_length_new = 50.2;}
  if(IER_new == 3 && BPM_new ==13 && tidal_volume_new ==200){ Stroke_length_new = 50;}
  if(IER_new == 3 && BPM_new ==14 && tidal_volume_new ==200){ Stroke_length_new = 49.8;}
  if(IER_new == 3 && BPM_new ==15 && tidal_volume_new ==200){ Stroke_length_new = 49.5;}
  if(IER_new == 3 && BPM_new ==16 && tidal_volume_new ==200){ Stroke_length_new = 49.5;}
  if(IER_new == 3 && BPM_new ==17 && tidal_volume_new ==200){ Stroke_length_new = 49.1;}
  if(IER_new == 3 && BPM_new ==18 && tidal_volume_new ==200){ Stroke_length_new = 49;}
  if(IER_new == 3 && BPM_new ==19 && tidal_volume_new ==200){ Stroke_length_new = 48.8;}
  if(IER_new == 3 && BPM_new ==20 && tidal_volume_new ==200){ Stroke_length_new = 48.8;}
  if(IER_new == 3 && BPM_new ==21 && tidal_volume_new ==200){ Stroke_length_new = 48.8;}
  if(IER_new == 3 && BPM_new ==22 && tidal_volume_new ==200){ Stroke_length_new = 48.8;}
  if(IER_new == 3 && BPM_new ==23 && tidal_volume_new ==200){ Stroke_length_new = 48.8;}
  if(IER_new == 3 && BPM_new ==24 && tidal_volume_new ==200){ Stroke_length_new = 48.8;}
  if(IER_new == 3 && BPM_new ==25 && tidal_volume_new ==200){ Stroke_length_new = 48.8;}
  if(IER_new == 3 && BPM_new ==26 && tidal_volume_new ==200){ Stroke_length_new = 48.8;}
  if(IER_new == 3 && BPM_new ==27 && tidal_volume_new ==200){ Stroke_length_new = 48.8;}
  if(IER_new == 3 && BPM_new ==28 && tidal_volume_new ==200){ Stroke_length_new = 48.8;}
  if(IER_new == 3 && BPM_new ==29 && tidal_volume_new ==200){ Stroke_length_new = 48.8;}
  if(IER_new == 3 && BPM_new ==30 && tidal_volume_new ==200){ Stroke_length_new = 48.8;}
  
  if(IER_new == 3 && BPM_new ==10 && tidal_volume_new ==250){ Stroke_length_new = 56.5;}
  if(IER_new == 3 && BPM_new ==11 && tidal_volume_new ==250){ Stroke_length_new = 56.2;}
  if(IER_new == 3 && BPM_new ==12 && tidal_volume_new ==250){ Stroke_length_new = 55.9;}
  if(IER_new == 3 && BPM_new ==13 && tidal_volume_new ==250){ Stroke_length_new = 55.6;}
  if(IER_new == 3 && BPM_new ==14 && tidal_volume_new ==250){ Stroke_length_new = 55.3;}
  if(IER_new == 3 && BPM_new ==15 && tidal_volume_new ==250){ Stroke_length_new = 55.1;}
  if(IER_new == 3 && BPM_new ==16 && tidal_volume_new ==250){ Stroke_length_new = 55;}
  if(IER_new == 3 && BPM_new ==17 && tidal_volume_new ==250){ Stroke_length_new = 54.9;}
  if(IER_new == 3 && BPM_new ==18 && tidal_volume_new ==250){ Stroke_length_new = 54.8;}
  if(IER_new == 3 && BPM_new ==19 && tidal_volume_new ==250){ Stroke_length_new = 54.7;}
  if(IER_new == 3 && BPM_new ==20 && tidal_volume_new ==250){ Stroke_length_new = 54.6;}
  if(IER_new == 3 && BPM_new ==21 && tidal_volume_new ==250){ Stroke_length_new = 54.5;}
  if(IER_new == 3 && BPM_new ==22 && tidal_volume_new ==250){ Stroke_length_new = 54.5;}
  if(IER_new == 3 && BPM_new ==23 && tidal_volume_new ==250){ Stroke_length_new = 54.5;}
  if(IER_new == 3 && BPM_new ==24 && tidal_volume_new ==250){ Stroke_length_new = 54.5;}
  if(IER_new == 3 && BPM_new ==25 && tidal_volume_new ==250){ Stroke_length_new = 54.5;}
  if(IER_new == 3 && BPM_new ==26 && tidal_volume_new ==250){ Stroke_length_new = 54.6;}
  if(IER_new == 3 && BPM_new ==27 && tidal_volume_new ==250){ Stroke_length_new = 54.6;}
  if(IER_new == 3 && BPM_new ==28 && tidal_volume_new ==250){ Stroke_length_new = 54.5;}
  if(IER_new == 3 && BPM_new ==29 && tidal_volume_new ==250){ Stroke_length_new = 54.6;}
  if(IER_new == 3 && BPM_new ==30 && tidal_volume_new ==250){ Stroke_length_new = 54.6;}
  
  if(IER_new == 3 && BPM_new ==10 && tidal_volume_new ==300){ Stroke_length_new = 61.5;}
  if(IER_new == 3 && BPM_new ==11 && tidal_volume_new ==300){ Stroke_length_new = 61;}
  if(IER_new == 3 && BPM_new ==12 && tidal_volume_new ==300){ Stroke_length_new = 60.7;}
  if(IER_new == 3 && BPM_new ==13 && tidal_volume_new ==300){ Stroke_length_new = 60.4;}
  if(IER_new == 3 && BPM_new ==14 && tidal_volume_new ==300){ Stroke_length_new = 60.2;}
  if(IER_new == 3 && BPM_new ==15 && tidal_volume_new ==300){ Stroke_length_new = 60.2;}
  if(IER_new == 3 && BPM_new ==16 && tidal_volume_new ==300){ Stroke_length_new = 59.8;}
  if(IER_new == 3 && BPM_new ==17 && tidal_volume_new ==300){ Stroke_length_new = 59.8;}
  if(IER_new == 3 && BPM_new ==18 && tidal_volume_new ==300){ Stroke_length_new = 59.8;}
  if(IER_new == 3 && BPM_new ==19 && tidal_volume_new ==300){ Stroke_length_new = 59.8;}
  if(IER_new == 3 && BPM_new ==20 && tidal_volume_new ==300){ Stroke_length_new = 59.8;}
  if(IER_new == 3 && BPM_new ==21 && tidal_volume_new ==300){ Stroke_length_new = 59.9;}
  if(IER_new == 3 && BPM_new ==22 && tidal_volume_new ==300){ Stroke_length_new = 60;}
  if(IER_new == 3 && BPM_new ==23 && tidal_volume_new ==300){ Stroke_length_new = 60.1;}
  if(IER_new == 3 && BPM_new ==24 && tidal_volume_new ==300){ Stroke_length_new = 60.2;}
  if(IER_new == 3 && BPM_new ==25 && tidal_volume_new ==300){ Stroke_length_new = 60.3;}
  if(IER_new == 3 && BPM_new ==26 && tidal_volume_new ==300){ Stroke_length_new = 60.4;}
  if(IER_new == 3 && BPM_new ==27 && tidal_volume_new ==300){ Stroke_length_new = 60.5;}
  if(IER_new == 3 && BPM_new ==28 && tidal_volume_new ==300){ Stroke_length_new = 60.6;}
  if(IER_new == 3 && BPM_new ==29 && tidal_volume_new ==300){ Stroke_length_new = 60.7;}
  if(IER_new == 3 && BPM_new ==30 && tidal_volume_new ==300){ Stroke_length_new = 60.8;}
  
  if(IER_new == 3 && BPM_new ==10 && tidal_volume_new ==350){ Stroke_length_new = 66.2;}
  if(IER_new == 3 && BPM_new ==11 && tidal_volume_new ==350){ Stroke_length_new = 65.8;}
  if(IER_new == 3 && BPM_new ==12 && tidal_volume_new ==350){ Stroke_length_new = 65.5;}
  if(IER_new == 3 && BPM_new ==13 && tidal_volume_new ==350){ Stroke_length_new = 65.2;}
  if(IER_new == 3 && BPM_new ==14 && tidal_volume_new ==350){ Stroke_length_new = 65.1;}
  if(IER_new == 3 && BPM_new ==15 && tidal_volume_new ==350){ Stroke_length_new = 65;}
  if(IER_new == 3 && BPM_new ==16 && tidal_volume_new ==350){ Stroke_length_new = 65;}
  if(IER_new == 3 && BPM_new ==17 && tidal_volume_new ==350){ Stroke_length_new = 65;}
  if(IER_new == 3 && BPM_new ==18 && tidal_volume_new ==350){ Stroke_length_new = 65;}
  if(IER_new == 3 && BPM_new ==19 && tidal_volume_new ==350){ Stroke_length_new = 65;}
  if(IER_new == 3 && BPM_new ==20 && tidal_volume_new ==350){ Stroke_length_new = 65;}
  if(IER_new == 3 && BPM_new ==21 && tidal_volume_new ==350){ Stroke_length_new = 65.4;}
  if(IER_new == 3 && BPM_new ==22 && tidal_volume_new ==350){ Stroke_length_new = 65.5;}
  if(IER_new == 3 && BPM_new ==23 && tidal_volume_new ==350){ Stroke_length_new = 65.6;}
  if(IER_new == 3 && BPM_new ==24 && tidal_volume_new ==350){ Stroke_length_new = 65.7;}
  if(IER_new == 3 && BPM_new ==25 && tidal_volume_new ==350){ Stroke_length_new = 65.9;}
  if(IER_new == 3 && BPM_new ==26 && tidal_volume_new ==350){ Stroke_length_new = 66.1;}
  if(IER_new == 3 && BPM_new ==27 && tidal_volume_new ==350){ Stroke_length_new = 66.3;}
  if(IER_new == 3 && BPM_new ==28 && tidal_volume_new ==350){ Stroke_length_new = 66.5;}
  if(IER_new == 3 && BPM_new ==29 && tidal_volume_new ==350){ Stroke_length_new = 66.7;}
  if(IER_new == 3 && BPM_new ==30 && tidal_volume_new ==350){ Stroke_length_new = 66.9;}
  
  if(IER_new == 3 && BPM_new ==10 && tidal_volume_new ==400){ Stroke_length_new = 70.2;}
  if(IER_new == 3 && BPM_new ==11 && tidal_volume_new ==400){ Stroke_length_new = 70.1;}
  if(IER_new == 3 && BPM_new ==12 && tidal_volume_new ==400){ Stroke_length_new = 69.8;}
  if(IER_new == 3 && BPM_new ==13 && tidal_volume_new ==400){ Stroke_length_new = 69.6;}
  if(IER_new == 3 && BPM_new ==14 && tidal_volume_new ==400){ Stroke_length_new = 69.6;}
  if(IER_new == 3 && BPM_new ==15 && tidal_volume_new ==400){ Stroke_length_new = 69.2;}
  if(IER_new == 3 && BPM_new ==16 && tidal_volume_new ==400){ Stroke_length_new = 69.2;}
  if(IER_new == 3 && BPM_new ==17 && tidal_volume_new ==400){ Stroke_length_new = 69.2;}
  if(IER_new == 3 && BPM_new ==18 && tidal_volume_new ==400){ Stroke_length_new = 69.2;}
  if(IER_new == 3 && BPM_new ==19 && tidal_volume_new ==400){ Stroke_length_new = 69.2;}
  if(IER_new == 3 && BPM_new ==20 && tidal_volume_new ==400){ Stroke_length_new = 69.2;}
  if(IER_new == 3 && BPM_new ==21 && tidal_volume_new ==400){ Stroke_length_new = 69.4;}
  if(IER_new == 3 && BPM_new ==22 && tidal_volume_new ==400){ Stroke_length_new = 69.6;}
  if(IER_new == 3 && BPM_new ==23 && tidal_volume_new ==400){ Stroke_length_new = 69.8;}
  if(IER_new == 3 && BPM_new ==24 && tidal_volume_new ==400){ Stroke_length_new = 70;}
  if(IER_new == 3 && BPM_new ==25 && tidal_volume_new ==400){ Stroke_length_new = 70.4;}
  if(IER_new == 3 && BPM_new ==26 && tidal_volume_new ==400){ Stroke_length_new = 70.8;}
  if(IER_new == 3 && BPM_new ==27 && tidal_volume_new ==400){ Stroke_length_new = 71.2;}
  if(IER_new == 3 && BPM_new ==28 && tidal_volume_new ==400){ Stroke_length_new = 71.6;}
  if(IER_new == 3 && BPM_new ==29 && tidal_volume_new ==400){ Stroke_length_new = 72;}
  if(IER_new == 3 && BPM_new ==30 && tidal_volume_new ==400){ Stroke_length_new = 72.4;}
  
  if(IER_new == 3 && BPM_new ==10 && tidal_volume_new ==450){ Stroke_length_new = 74.2;}
  if(IER_new == 3 && BPM_new ==11 && tidal_volume_new ==450){ Stroke_length_new = 73.8;}
  if(IER_new == 3 && BPM_new ==12 && tidal_volume_new ==450){ Stroke_length_new = 73.8;}
  if(IER_new == 3 && BPM_new ==13 && tidal_volume_new ==450){ Stroke_length_new = 73.6;}
  if(IER_new == 3 && BPM_new ==14 && tidal_volume_new ==450){ Stroke_length_new = 73.4;}
  if(IER_new == 3 && BPM_new ==15 && tidal_volume_new ==450){ Stroke_length_new = 73.6;}
  if(IER_new == 3 && BPM_new ==16 && tidal_volume_new ==450){ Stroke_length_new = 73.8;}
  if(IER_new == 3 && BPM_new ==17 && tidal_volume_new ==450){ Stroke_length_new = 74;}
  if(IER_new == 3 && BPM_new ==18 && tidal_volume_new ==450){ Stroke_length_new = 74.2;}
  if(IER_new == 3 && BPM_new ==19 && tidal_volume_new ==450){ Stroke_length_new = 74.4;}
  if(IER_new == 3 && BPM_new ==20 && tidal_volume_new ==450){ Stroke_length_new = 74.6;}
  if(IER_new == 3 && BPM_new ==21 && tidal_volume_new ==450){ Stroke_length_new = 74.8;}
  if(IER_new == 3 && BPM_new ==22 && tidal_volume_new ==450){ Stroke_length_new = 75;}
  if(IER_new == 3 && BPM_new ==23 && tidal_volume_new ==450){ Stroke_length_new = 75.2;}
  if(IER_new == 3 && BPM_new ==24 && tidal_volume_new ==450){ Stroke_length_new = 75.4;}
  if(IER_new == 3 && BPM_new ==25 && tidal_volume_new ==450){ Stroke_length_new = 75.6;}
  if(IER_new == 3 && BPM_new ==26 && tidal_volume_new ==450){ Stroke_length_new = 76;}
  if(IER_new == 3 && BPM_new ==27 && tidal_volume_new ==450){ Stroke_length_new = 76.4;}
  if(IER_new == 3 && BPM_new ==28 && tidal_volume_new ==450){ Stroke_length_new = 76.8;}
  if(IER_new == 3 && BPM_new ==29 && tidal_volume_new ==450){ Stroke_length_new = 77.2;}
  if(IER_new == 3 && BPM_new ==30 && tidal_volume_new ==450){ Stroke_length_new = 77.6;}
  
  if(IER_new == 3 && BPM_new ==10 && tidal_volume_new ==500){ Stroke_length_new = 78;}
  if(IER_new == 3 && BPM_new ==11 && tidal_volume_new ==500){ Stroke_length_new = 77.9;}
  if(IER_new == 3 && BPM_new ==12 && tidal_volume_new ==500){ Stroke_length_new = 77.8;}
  if(IER_new == 3 && BPM_new ==13 && tidal_volume_new ==500){ Stroke_length_new = 77.7;}
  if(IER_new == 3 && BPM_new ==14 && tidal_volume_new ==500){ Stroke_length_new = 77.6;}
  if(IER_new == 3 && BPM_new ==15 && tidal_volume_new ==500){ Stroke_length_new = 77.5;}
  if(IER_new == 3 && BPM_new ==16 && tidal_volume_new ==500){ Stroke_length_new = 77.6;}
  if(IER_new == 3 && BPM_new ==17 && tidal_volume_new ==500){ Stroke_length_new = 77.9;}
  if(IER_new == 3 && BPM_new ==18 && tidal_volume_new ==500){ Stroke_length_new = 78.2;}
  if(IER_new == 3 && BPM_new ==19 && tidal_volume_new ==500){ Stroke_length_new = 78.5;}
  if(IER_new == 3 && BPM_new ==20 && tidal_volume_new ==500){ Stroke_length_new = 78.8;}
  if(IER_new == 3 && BPM_new ==21 && tidal_volume_new ==500){ Stroke_length_new = 79.2;}
  if(IER_new == 3 && BPM_new ==22 && tidal_volume_new ==500){ Stroke_length_new = 79.6;}
  if(IER_new == 3 && BPM_new ==23 && tidal_volume_new ==500){ Stroke_length_new = 80;}
  if(IER_new == 3 && BPM_new ==24 && tidal_volume_new ==500){ Stroke_length_new = 80.4;}
  if(IER_new == 3 && BPM_new ==25 && tidal_volume_new ==500){ Stroke_length_new = 80.8;}
  if(IER_new == 3 && BPM_new ==26 && tidal_volume_new ==500){ Stroke_length_new = 81.3;}
  if(IER_new == 3 && BPM_new ==27 && tidal_volume_new ==500){ Stroke_length_new = 81.8;}
  if(IER_new == 3 && BPM_new ==28 && tidal_volume_new ==500){ Stroke_length_new = 82.3;}
  if(IER_new == 3 && BPM_new ==29 && tidal_volume_new ==500){ Stroke_length_new = 82.8;}
  if(IER_new == 3 && BPM_new ==30 && tidal_volume_new ==500){ Stroke_length_new = 83.3;}
  
  if(IER_new == 3 && BPM_new ==10 && tidal_volume_new ==550){ Stroke_length_new = 82.4;}
  if(IER_new == 3 && BPM_new ==11 && tidal_volume_new ==550){ Stroke_length_new = 82.4;}
  if(IER_new == 3 && BPM_new ==12 && tidal_volume_new ==550){ Stroke_length_new = 82.4;}
  if(IER_new == 3 && BPM_new ==13 && tidal_volume_new ==550){ Stroke_length_new = 82.4;}
  if(IER_new == 3 && BPM_new ==14 && tidal_volume_new ==550){ Stroke_length_new = 82.4;}
  if(IER_new == 3 && BPM_new ==15 && tidal_volume_new ==550){ Stroke_length_new = 82.4;}
  if(IER_new == 3 && BPM_new ==16 && tidal_volume_new ==550){ Stroke_length_new = 82.8;}
  if(IER_new == 3 && BPM_new ==17 && tidal_volume_new ==550){ Stroke_length_new = 83.2;}
  if(IER_new == 3 && BPM_new ==18 && tidal_volume_new ==550){ Stroke_length_new = 83.6;}
  if(IER_new == 3 && BPM_new ==19 && tidal_volume_new ==550){ Stroke_length_new = 84;}
  if(IER_new == 3 && BPM_new ==20 && tidal_volume_new ==550){ Stroke_length_new = 84.5;}
  if(IER_new == 3 && BPM_new ==21 && tidal_volume_new ==550){ Stroke_length_new = 84.9;}
  if(IER_new == 3 && BPM_new ==22 && tidal_volume_new ==550){ Stroke_length_new = 85.3;}
  if(IER_new == 3 && BPM_new ==23 && tidal_volume_new ==550){ Stroke_length_new = 85.7;}
  if(IER_new == 3 && BPM_new ==24 && tidal_volume_new ==550){ Stroke_length_new = 86.1;}
  if(IER_new == 3 && BPM_new ==25 && tidal_volume_new ==550){ Stroke_length_new = 86.5;}
  if(IER_new == 3 && BPM_new ==26 && tidal_volume_new ==550){ Stroke_length_new = 87;}
  if(IER_new == 3 && BPM_new ==27 && tidal_volume_new ==550){ Stroke_length_new = 87.5;}
//  if(IER_new == 3 && BPM_new ==28 && tidal_volume_new ==550){ Stroke_length_new = NA;}
//  if(IER_new == 3 && BPM_new ==29 && tidal_volume_new ==550){ Stroke_length_new = NA;}
//  if(IER_new == 3 && BPM_new ==30 && tidal_volume_new ==550){ Stroke_length_new = NA;}
  
  if(IER_new == 3 && BPM_new ==10 && tidal_volume_new ==600){ Stroke_length_new = 87.7;}
  if(IER_new == 3 && BPM_new ==11 && tidal_volume_new ==600){ Stroke_length_new = 87.4;}
  if(IER_new == 3 && BPM_new ==12 && tidal_volume_new ==600){ Stroke_length_new = 87.1;}
  if(IER_new == 3 && BPM_new ==13 && tidal_volume_new ==600){ Stroke_length_new = 87.3;}
  if(IER_new == 3 && BPM_new ==14 && tidal_volume_new ==600){ Stroke_length_new = 87.5;}
  if(IER_new == 3 && BPM_new ==15 && tidal_volume_new ==600){ Stroke_length_new = 87.7;}
  if(IER_new == 3 && BPM_new ==16 && tidal_volume_new ==600){ Stroke_length_new = 88.2;}
  if(IER_new == 3 && BPM_new ==17 && tidal_volume_new ==600){ Stroke_length_new = 88.7;}
  if(IER_new == 3 && BPM_new ==18 && tidal_volume_new ==600){ Stroke_length_new = 89.2;}
  if(IER_new == 3 && BPM_new ==19 && tidal_volume_new ==600){ Stroke_length_new = 89.7;}
  if(IER_new == 3 && BPM_new ==20 && tidal_volume_new ==600){ Stroke_length_new = 90.2;}
  if(IER_new == 3 && BPM_new ==21 && tidal_volume_new ==600){ Stroke_length_new = 90.7;}
  if(IER_new == 3 && BPM_new ==22 && tidal_volume_new ==600){ Stroke_length_new = 91.2;}
  if(IER_new == 3 && BPM_new ==23 && tidal_volume_new ==600){ Stroke_length_new = 91.7;}
  if(IER_new == 3 && BPM_new ==24 && tidal_volume_new ==600){ Stroke_length_new = 92.2;}
//  if(IER_new == 3 && BPM_new ==25 && tidal_volume_new ==600){ Stroke_length_new = NA;}
//  if(IER_new == 3 && BPM_new ==26 && tidal_volume_new ==600){ Stroke_length_new = NA;}
//  if(IER_new == 3 && BPM_new ==27 && tidal_volume_new ==600){ Stroke_length_new = NA;}
//  if(IER_new == 3 && BPM_new ==28 && tidal_volume_new ==600){ Stroke_length_new = NA;}
//  if(IER_new == 3 && BPM_new ==29 && tidal_volume_new ==600){ Stroke_length_new = NA;}
//  if(IER_new == 3 && BPM_new ==30 && tidal_volume_new ==600){ Stroke_length_new = NA;}
  
  if(IER_new == 3 && BPM_new ==10 && tidal_volume_new ==650){ Stroke_length_new = 93.5;}
  if(IER_new == 3 && BPM_new ==11 && tidal_volume_new ==650){ Stroke_length_new = 93.7;}
  if(IER_new == 3 && BPM_new ==12 && tidal_volume_new ==650){ Stroke_length_new = 93.9;}
  if(IER_new == 3 && BPM_new ==13 && tidal_volume_new ==650){ Stroke_length_new = 94.1;}
  if(IER_new == 3 && BPM_new ==14 && tidal_volume_new ==650){ Stroke_length_new = 94.3;}
  if(IER_new == 3 && BPM_new ==15 && tidal_volume_new ==650){ Stroke_length_new = 94.6;}
  if(IER_new == 3 && BPM_new ==16 && tidal_volume_new ==650){ Stroke_length_new = 95;}
  if(IER_new == 3 && BPM_new ==17 && tidal_volume_new ==650){ Stroke_length_new = 95.4;}
  if(IER_new == 3 && BPM_new ==18 && tidal_volume_new ==650){ Stroke_length_new = 95.8;}
  if(IER_new == 3 && BPM_new ==19 && tidal_volume_new ==650){ Stroke_length_new = 96.2;}
  if(IER_new == 3 && BPM_new ==20 && tidal_volume_new ==650){ Stroke_length_new = 96.6;}
  if(IER_new == 3 && BPM_new ==21 && tidal_volume_new ==650){ Stroke_length_new = 97;}
//  if(IER_new == 3 && BPM_new ==22 && tidal_volume_new ==650){ Stroke_length_new = NA;}
//  if(IER_new == 3 && BPM_new ==23 && tidal_volume_new ==650){ Stroke_length_new = NA;}
//  if(IER_new == 3 && BPM_new ==24 && tidal_volume_new ==650){ Stroke_length_new = NA;}
//  if(IER_new == 3 && BPM_new ==25 && tidal_volume_new ==650){ Stroke_length_new = NA;}
//  if(IER_new == 3 && BPM_new ==26 && tidal_volume_new ==650){ Stroke_length_new = NA;}
//  if(IER_new == 3 && BPM_new ==27 && tidal_volume_new ==650){ Stroke_length_new = NA;}
//  if(IER_new == 3 && BPM_new ==28 && tidal_volume_new ==650){ Stroke_length_new = NA;}
//  if(IER_new == 3 && BPM_new ==29 && tidal_volume_new ==650){ Stroke_length_new = NA;}
//  if(IER_new == 3 && BPM_new ==30 && tidal_volume_new ==650){ Stroke_length_new = NA;}
  
  if(IER_new == 3 && BPM_new ==10 && tidal_volume_new ==700){ Stroke_length_new = 99.4;}
  if(IER_new == 3 && BPM_new ==11 && tidal_volume_new ==700){ Stroke_length_new = 99.6;}
  if(IER_new == 3 && BPM_new ==12 && tidal_volume_new ==700){ Stroke_length_new = 99.8;}
  if(IER_new == 3 && BPM_new ==13 && tidal_volume_new ==700){ Stroke_length_new = 100;}
  if(IER_new == 3 && BPM_new ==14 && tidal_volume_new ==700){ Stroke_length_new = 100.2;}
  if(IER_new == 3 && BPM_new ==15 && tidal_volume_new ==700){ Stroke_length_new = 100.6;}
  if(IER_new == 3 && BPM_new ==16 && tidal_volume_new ==700){ Stroke_length_new = 101;}
  if(IER_new == 3 && BPM_new ==17 && tidal_volume_new ==700){ Stroke_length_new = 101.4;}
  if(IER_new == 3 && BPM_new ==18 && tidal_volume_new ==700){ Stroke_length_new = 101.8;}
//  if(IER_new == 3 && BPM_new ==19 && tidal_volume_new ==700){ Stroke_length_new = NA;}
//  if(IER_new == 3 && BPM_new ==20 && tidal_volume_new ==700){ Stroke_length_new = NA;}
//  if(IER_new == 3 && BPM_new ==21 && tidal_volume_new ==700){ Stroke_length_new = NA;}
//  if(IER_new == 3 && BPM_new ==22 && tidal_volume_new ==700){ Stroke_length_new = NA;}
//  if(IER_new == 3 && BPM_new ==23 && tidal_volume_new ==700){ Stroke_length_new = NA;}
//  if(IER_new == 3 && BPM_new ==24 && tidal_volume_new ==700){ Stroke_length_new = NA;}
//  if(IER_new == 3 && BPM_new ==25 && tidal_volume_new ==700){ Stroke_length_new = NA;}
//  if(IER_new == 3 && BPM_new ==26 && tidal_volume_new ==700){ Stroke_length_new = NA;}
//  if(IER_new == 3 && BPM_new ==27 && tidal_volume_new ==700){ Stroke_length_new = NA;}
//  if(IER_new == 3 && BPM_new ==28 && tidal_volume_new ==700){ Stroke_length_new = NA;}
//  if(IER_new == 3 && BPM_new ==29 && tidal_volume_new ==700){ Stroke_length_new = NA;}
//  if(IER_new == 3 && BPM_new ==30 && tidal_volume_new ==700){ Stroke_length_new = NA;}

      Serial.print("IER_new : ");
      Serial.print(IER_new); 
      Serial.print("  BPM_new : ");
      Serial.print(BPM_new); 
      Serial.print("  tidal_volume_new : ");
      Serial.print(tidal_volume_new); 
      Serial.print("  SL_new : ");
      Serial.println(Stroke_length_new);
}
