
#include "Arduino.h"
#include "BoardDefines.h"
#include "control_board.h"
#include "gauge_pressure.h"
#include "./libraries/MsTimer2/MsTimer2.cpp"




void booting_up (void) {

    int     i;

    for (i = 1; i < 5; i++)
    {
        digitalWrite(LED_1_PIN, i & 1);
        delay(100);
        digitalWrite(LED_2_PIN, i & 1);
        delay(100);
        digitalWrite(LED_3_PIN, i & 1);
        delay(100);
        digitalWrite(LED_4_PIN, i & 1);
        delay(100);
        digitalWrite(LED_5_PIN, i & 1);
        delay(100);
    }
    
}

void led_running (uint8_t inhale) {     // inhale = 1 => EXP_DIR, inhale = 0 => COMP_DIR.

    static uint8_t     mask = 7;
    static uint8_t     local_delay;

    local_delay++;
    if (local_delay)
        return;

    if (inhale) {
        mask = mask << 1;
    }
    else {
        mask = mask >> 1;
    }

    if (mask && ((mask & 0x1F) == 0))  {
        mask = 7;
    }
    else {
        if (mask == 0)  {
            mask = (0x07 << 2);   // 0x1C;
        }
    }
    

    digitalWrite(LED_1_PIN, mask & 0x01);
    digitalWrite(LED_2_PIN, mask & 0x02);
    digitalWrite(LED_3_PIN, mask & 0x04);
    digitalWrite(LED_4_PIN, mask & 0x08);
    digitalWrite(LED_5_PIN, mask & 0x10);

    
}

void setup (void)   {

    // i. UART Ports initializations
    DebugPort.begin(115200);                 // The Serial port of Arduino baud rate.
    DebugPort.println(F("Signum Techniks")); // say hello to check serial line
    Serial3.begin(115200);

    // ii. LEDs initializations
    pinMode(LED_1_PIN, OUTPUT);
    pinMode(LED_2_PIN, OUTPUT);
    pinMode(LED_3_PIN, OUTPUT);
    pinMode(LED_4_PIN, OUTPUT);
    pinMode(LED_5_PIN, OUTPUT);
    booting_up ();
  
    // iii. Stepper Motor step and direction
    pinMode(MOTOR_STEP_PIN, OUTPUT);
    digitalWrite(MOTOR_STEP_PIN, HIGH);
    pinMode(MOTOR_DIR_PIN, OUTPUT);
    digitalWrite(MOTOR_DIR_PIN, LOW);
    
    // iv. Home pin
    pinMode(HOME_SENSOR_PIN, INPUT_PULLUP);
    digitalWrite(HOME_SENSOR_PIN, HIGH);

    // v. Valves mode & pin Initialize
    pinMode(EXHALE_VLV_PIN, OUTPUT);
    pinMode(INHALE_VLV_PIN, OUTPUT);
    pinMode(O2Cyl_VLV_PIN, OUTPUT);
    pinMode(INHALE_RELEASE_VLV_PIN, OUTPUT);
    
    digitalWrite(EXHALE_VLV_PIN, LOW);
    digitalWrite(INHALE_VLV_PIN, LOW);
    digitalWrite(O2Cyl_VLV_PIN, LOW);
    digitalWrite(INHALE_RELEASE_VLV_PIN, LOW);

    inti_all_Valves();
    // stop_timer();

  
    // vi. home cycle on power up
    home_cycle = true;
    motion_profile_count_temp = 0;
    
    DebugPort.println("Power On Home Cycle : ");
    Home_attempt_count++;
    DebugPort.print("Homing attempt count before start : "); 
    DebugPort.println(Home_attempt_count);
    run_pulse_count_temp = 0.0;
    run_pulse_count =  ((micro_stepping * (power_on_home_travel / LEAD_SCREW_PITCH * 1.0)) / 2.0);
    DebugPort.print("travel in 'mm' back to home on power up : "); 
    DebugPort.println(power_on_home_travel);
    DebugPort.print("Pulses required to travel back to home on power up : "); 
    DebugPort.println(run_pulse_count);
    digitalWrite(MOTOR_DIR_PIN, EXP_DIR);
    
    // This is mandatory to initate the Timer block properly
    initialize_timer1_for_set_RPM (home_speed_value * 10.0);
    run_motor = true;


    flag_Serial_requested = false ;
    // DebugPort.println("Requesting paramemters : ");
    // Serial3.print("$VSP10001&");
  
}


int     toggle = 0;
int     f_test_data_updated;

void loop (void) {
    
    // for heart beat..
    toggle = ~toggle;
//    digitalWrite(LED_1_PIN, toggle);

   
    if (send_pressure_data == true)  {
        Ipressure = get_calibrated_pressure_MPX5010((sensor_e)INHALE_GAUGE_PRESSURE, &IRaw);
        Epressure = get_calibrated_pressure_MPX5010((sensor_e)EXHALE_GUAGE_PRESSURE, &ERaw);
        Serial3.print(Ctrl_CreateCommand(PARAMGP_PRS, Ipressure * 100, Epressure * 100));
        DebugPort.println(Ctrl_CreateCommand(PARAMGP_PRS, Ipressure * 100, Epressure * 100));
        delay(100);
        // DebugPort.println(Ctrl_CreateCommand(PARAMGP_PRS, Ipressure * 100, Epressure * 100));
    }
    
    if (send_millivolts_data == true)  {
        //  Ctrl_CreateCommand(PARAMGP_RAW, int value1, int value2)
        Ipressure = get_calibrated_pressure_MPX5010((sensor_e)INHALE_GAUGE_PRESSURE, &IRaw);
        Epressure = get_calibrated_pressure_MPX5010((sensor_e)EXHALE_GUAGE_PRESSURE, &ERaw);
        Serial3.print(Ctrl_CreateCommand(PARAMGP_RAW, IRaw, ERaw));
        DebugPort.println(Ctrl_CreateCommand(PARAMGP_RAW, IRaw, ERaw));
        delay(100);
        //DebugPort.print(Ctrl_CreateCommand(PARAMGP_RAW, IRaw, ERaw));
    }



    // 1. Expansion completed & Compression start
    if ((cycle_start == true) && (exp_start == true) && (exp_end == true) && (exp_timer_end == true))     {
        // digitalWrite(LED_4_PIN, HIGH);
        // digitalWrite(LED_5_PIN, LOW);
        
        EXHALE_VLV_CLOSE();
        Epressure = get_calibrated_pressure_MPX5010((sensor_e)EXHALE_GUAGE_PRESSURE, &ERaw);
        PEEP = Epressure;
        
        INHALE_VLV_OPEN();
        DebugPort.print("IER: 1:");
        DebugPort.print(IER);
        DebugPort.print("  BPM: ");
        DebugPort.print(BPM);
        DebugPort.print("  TV: ");
        DebugPort.print(tidal_volume);
        DebugPort.print("  Stroke: ");
        DebugPort.println(Stroke_length);
        
        DebugPort.print("Peak Pressure: ");
        DebugPort.print(peak_prsur);
        DebugPort.print("  Cali. GP0: ");
        DebugPort.print(CAL_GP0);
        DebugPort.print("  Cali. GP1: ");
        DebugPort.println(CAL_GP1);
        
        DebugPort.print("comp : ");
        DebugPort.print((c_end_millis - c_start_millis) / 1000.0);
        DebugPort.print("/");
        DebugPort.print(inhale_time);
        DebugPort.print("  ExpTime : ");
        DebugPort.print((e_timer_end_millis - e_start_millis) / 1000.0);
        DebugPort.print("/");
        DebugPort.print(exhale_time);
        DebugPort.print("  Cycle : ");
        DebugPort.print((e_timer_end_millis - c_start_millis) / 1000.0);
        DebugPort.print("/");
        DebugPort.println(cycle_time);
        DebugPort.print("Inhale-hold : ");
        DebugPort.print(inhale_hold_time / 1000.0);
        DebugPort.print("  MotorRet. : ");
        DebugPort.println((e_end_millis - e_start_millis) / 1000.0);
        DebugPort.println();
        // if ((BPM_new != BPM) || (tidal_volume_new != tidal_volume) || (IER_new != IER)) 
        if (f_test_data_updated || (BPM_new != BPM) || (tidal_volume_new != tidal_volume) || (IER_new != IER))    {
            f_test_data_updated = 0;
            convert_all_set_params_2_machine_values();
        }
        Start_inhale_cycle();
        PIP = 0.0; // To catch max value we have t0 make it zero.
    }


    // 2. compression started & is in progress
    if ((cycle_start == true) && (comp_start == true) && (comp_end == false))  {
        Ipressure = get_calibrated_pressure_MPX5010((sensor_e)INHALE_GAUGE_PRESSURE, &IRaw);
        if (Ipressure > PIP) {
            PIP = Ipressure;
            //       DebugPort.print("PIP:");
            //       DebugPort.println(Ipressure);
        }
        if (Ipressure > peak_prsur)   {
            INHALE_VLV_CLOSE();
            //Stop motor
            Emergency_motor_stop = true;
            DebugPort.print("\npeak detected for PIP:");  DebugPort.println(Ipressure);
        }

        led_running (COMP_DIR);
    }

    // 3. Compression completed & start Expansion
    if ((cycle_start == true) && (comp_start == true) && (comp_end == true))    {
        // digitalWrite(LED_4_PIN, LOW );
        // digitalWrite(LED_5_PIN, HIGH );        
        //    Epressure = get_calibrated_pressure_MPX5010((sensor_e)EXHALE_GUAGE_PRESSURE, &ERaw);
        //    PIP = Epressure;       
        inhale_hold_time = (inhale_time * (inhale_hold_percentage / 100)) * 1000;
        delay(inhale_hold_time); //expansion delay      
        Epressure = get_calibrated_pressure_MPX5010((sensor_e)EXHALE_GUAGE_PRESSURE, &ERaw);
        PLAT = Epressure;      
        Start_exhale_cycle();
        EXHALE_VLV_OPEN();
        // Start_exhale_cycle();
    }

    // 4. Expansion started & is in progress
    if ((cycle_start == true) && (exp_start == true) && (exp_end == false))   {
        Epressure = get_calibrated_pressure_MPX5010((sensor_e)EXHALE_GUAGE_PRESSURE, &ERaw);
        led_running (EXP_DIR);
    }

}




ISR (TIMER1_COMPA_vect) {
    
    // timer1 interrupt 1Hz toggles pin 13 (LED)
    // generates pulse wave of frequency 1Hz/2 = 0.5kHz (takes two cycles for full wave- toggle high then toggle low)
  
    // for heart beat..
    static uint8_t	blink;
    
    blink = ~blink;
    // digitalWrite(LED_3_PIN, blink);

  
    if (run_motor == true)    {
        
        if ((motion_profile_count_temp == 0) && (run_pulse_count_temp == 0.0))     {
            // compression cycle start only once
            if ((comp_start == true) & (comp_end == false))   {
                //DebugPort.print("comp: "); DebugPort.println(motion_profile_count_temp);
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
        
            // after inhale-hold time --> Expansion cycle start only once
            if ((exp_start == true) & (exp_end == false))   {
                // DebugPort.print("exp: "); DebugPort.println(motion_profile_count_temp);
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
        
        // Actual motor pulse generation block
        if (run_pulse_count_temp < run_pulse_count)   {
            if (Emergency_motor_stop == false)
                digitalWrite(MOTOR_STEP_PIN, digitalRead(MOTOR_STEP_PIN) ^ 1);
                
            run_pulse_count_temp = run_pulse_count_temp + 0.5;
            if (home_cycle == true) {
                if (digitalRead(HOME_SENSOR_PIN) == HOME_SENSE_VALUE)   {
                    run_motor = false;
                    run_pulse_count_temp = 0.0;
                    home_cycle = false;
                    motion_profile_count_temp = 0;
                    DebugPort.println("Home Cycle Complete...");
                    Home_attempt_count = 0;
                    if (cycle_start == true)
                        inti_Start();
                }
            }
            if ((cycle_start == true) && (digitalRead(MOTOR_DIR_PIN) == EXP_DIR))    {
                if (digitalRead(HOME_SENSOR_PIN) == HOME_SENSE_VALUE)   {
                    run_pulse_count_temp = run_pulse_count;
                    motion_profile_count_temp = CURVE_EXP_STEPS;
                    Emergency_motor_stop = true;
                    // motion_profile_count_temp = 0;
                    // run_pulse_count_temp = 0.0;
                }
            }
        }
        
        else    {
            noInterrupts();
            run_motor = false;
            run_pulse_count_temp = 0.0;
            motion_profile_count_temp = motion_profile_count_temp + 1;
            //Compression end check
            if ((comp_start == true) & (comp_end == false))  {
                if (motion_profile_count_temp < CURVE_COMP_STEPS)    {
                    //DebugPort.print("comp: "); DebugPort.println(motion_profile_count_temp);
                    run_pulse_count = compression_step_array[motion_profile_count_temp];
                    digitalWrite(MOTOR_DIR_PIN, COMP_DIR);
                    OCR1A = OCR1A_comp_array[motion_profile_count_temp];
                    load_TCCR1B_var(TCCR1B_comp_array[motion_profile_count_temp]);
                    run_motor = true;
                }
                else   {
                    c_end_millis = millis();
                    motion_profile_count_temp = 0;
                    run_pulse_count_temp = 0.0;
                    Emergency_motor_stop = false;
                    //DebugPort.print("\nPIP:");
                    //DebugPort.println(PIP);
                    INHALE_RELEASE_VLV_CLOSE();
                    INHALE_VLV_CLOSE();
                    //commented as this will be Opened after Inhale-Hold Delay.
                    //EXHALE_VLV_OPEN();
                    comp_end = true;
                }
            }
            //expansion end check
            if ((exp_start == true) & (exp_end == false))       {
                if (motion_profile_count_temp < CURVE_EXP_STEPS)      {
                    //DebugPort.print("exp: "); DebugPort.println(motion_profile_count_temp);
                    run_pulse_count = expansion_step_array[motion_profile_count_temp];
                    digitalWrite(MOTOR_DIR_PIN, EXP_DIR);
                    OCR1A = OCR1A_exp_array[motion_profile_count_temp];
                    load_TCCR1B_var(TCCR1B_exp_array[motion_profile_count_temp]);
                    run_motor = true;
                }
                else   {
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




bool Start_exhale_cycle (void)  {
    
    Serial3.print(Ctrl_CreateCommand(EXPAN, PIP * 10, 0)); //expansion flag
    DebugPort.print(Ctrl_CreateCommand(EXPAN, PIP * 10, 0));  //expansion flag
    DebugPort.print("\nPIP : ");  DebugPort.println(PIP);
    
    //DebugPort.print("CYCLE Exhale Time: " );DebugPort.println(exhale_time);
    MsTimer2::set(exhale_time * 1000, (void (*)())Exhale_timer_timout); //period
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

bool Start_inhale_cycle (void)  {
    
    Serial3.print(Ctrl_CreateCommand(COMP, PEEP * 10, PLAT * 10)); //comp start flag
    DebugPort.print(Ctrl_CreateCommand(COMP, PEEP * 10, PLAT * 10));  //comp start flag
    DebugPort.print("\nPEEP: "); DebugPort.println(PEEP);
    DebugPort.print("PLAT: ");   DebugPort.println(PLAT);
    
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
String Ctrl_CreateCommand (String paramName, long value1, int value2)    {
    
    String command;
    char paddedValue1[8];
    char paddedValue2[8];
    
    command = START_DELIM;
    command += VENT_MAST;
    command += paramName;
    
    if (paramName == GP0_PARAM || paramName == GP1_PARAM)  {
    
        char paddedValue3[15];
        
        sprintf(paddedValue3, "%08lu", value1);
        command += paddedValue3;
        DebugPort.print(paramName);
        DebugPort.print(" : cal value sending :  ");
        DebugPort.print(paramName);
        DebugPort.print(" == ");
        DebugPort.println(paddedValue3);
    }
    else    {
        sprintf(paddedValue1, "%04d", value1);
        sprintf(paddedValue2, "%04d", value2);
        command += paddedValue1;
        command += paddedValue2;
    }
    
    command += END_DELIM;
    
    return command;
    
}


bool Exhale_timer_timout (void)     {
    
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
  DebugPort.println(("Speed curve calculations : "));

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

  MsTimer2::set(exhale_time * 1000, (void (*)())Exhale_timer_timout); //period
  DebugPort.print("Calculated Exhale Time: ");
  DebugPort.println(exhale_time);

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
  //    DebugPort.print("Compression: "); DebugPort.print(i); DebugPort.print(" | step: "); DebugPort.print(compression_step_array[i]); DebugPort.print(" | rpm: "); DebugPort.println(compression_speed_array[i]);
  //    DebugPort.print("expansion  : "); DebugPort.print(i); DebugPort.print(" | step: "); DebugPort.print(expansion_step_array[i]);  DebugPort.print(" | rpm: "); DebugPort.println(expansion_speed_array[i]);
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
  //DebugPort.print(("rpm  : ")); DebugPort.print(rpm / 10); DebugPort.print(("  Freq : ")); DebugPort.print(freq); DebugPort.print(("  Micro stepping: ")); DebugPort.println(micro_stepping);

  // initialize timer1
  //noInterrupts(); // disable all interrupts
  //set timer1 interrupt at 1Hz
  //TCCR1A = 0;// set entire TCCR1A register to 0
  //TCCR1B = 0;// same for TCCR1B
  //TCNT1  = 0;//initialize counter value to 0

  // set compare match register for 1hz increments
  TCCR1B_var = 1;
  OCR1A_var = (16000000.0 / (freq * TCCR1B_var)) - 1; // (must be <65536)
  //  DebugPort.println("1 :");
  //  DebugPort.print(("TCCR : ")); DebugPort.print(TCCR1B_var);
  //  DebugPort.print(("       OCR1 : ")); DebugPort.println(OCR1A_var);

  if (OCR1A_var > 65536 || OCR1A_var <= 0)
  {
    TCCR1B_var = 8;
    OCR1A_var = ((16000000) / (freq * TCCR1B_var)) - 1; // (must be <65536)
  }
  //  DebugPort.println("2 :");
  //  DebugPort.print(("TCCR : ")); DebugPort.print(TCCR1B_var);
  //  DebugPort.print(("       OCR1 : ")); DebugPort.println(OCR1A_var);

  if (OCR1A_var > 65536 || OCR1A_var <= 0)
  {
    TCCR1B_var = 64;
    OCR1A_var = ((16000000) / (freq * TCCR1B_var)) - 1; // (must be <65536)
  }
  //  DebugPort.println("3 :");
  //  DebugPort.print(("TCCR : ")); DebugPort.print(TCCR1B_var);
  //  DebugPort.print(("       OCR1 : ")); DebugPort.println(OCR1A_var);

  if (OCR1A_var > 65536 || OCR1A_var <= 0)
  {
    TCCR1B_var = 256;
    OCR1A_var = ((16000000) / (freq * TCCR1B_var)) - 1; // (must be <65536)
  }
  //  DebugPort.println("4 :");
  //  DebugPort.print(("TCCR : ")); DebugPort.print(TCCR1B_var);
  //  DebugPort.print(("       OCR1 : ")); DebugPort.println(OCR1A_var);

  if (OCR1A_var > 65536 || OCR1A_var <= 0)
  {
    TCCR1B_var = 1024;
    OCR1A_var = ((16000000) / (freq * TCCR1B_var)) - 1; // (must be <65536)
  }
  //  DebugPort.println("5 :");
  //  DebugPort.print(("TCCR : ")); DebugPort.print(TCCR1B_var);
  //  DebugPort.print(("       OCR1 : ")); DebugPort.println(OCR1A_var);
}

boolean initialize_timer1_for_set_RPM(float rpm)
{
  double freq;
  //  long OCR1A_var;
  //  long TCCR1B_var;

  rpm = abs(rpm);
  //  DebugPort.print(("rpm  : ")); DebugPort.println(rpm / 10);
  //  DebugPort.print(("Micro: ")); DebugPort.println(micro_stepping);
  freq = round(long((rpm * micro_stepping) / 600.0));
  //  DebugPort.print(("Freq : ")); DebugPort.println(freq);

  // initialize timer1
  noInterrupts(); // disable all interrupts
  //set timer1 interrupt at 1Hz
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1 = 0;  //initialize counter value to 0

  // set compare match register for 1hz increments
  TCCR1B_var = 1;
  OCR1A_var = (16000000.0 / (freq * TCCR1B_var)) - 1; // (must be <65536)
  //  DebugPort.println("1 :");
  //  DebugPort.print(("TCCR : ")); DebugPort.print(TCCR1B_var);
  //  DebugPort.print(("       OCR1 : ")); DebugPort.println(OCR1A_var);
  if (OCR1A_var > 65536 || OCR1A_var <= 0)
  {
    TCCR1B_var = 8;
    OCR1A_var = ((16000000) / (freq * TCCR1B_var)) - 1; // (must be <65536)
  }
  //  DebugPort.println("2 :");
  //  DebugPort.print(("TCCR : ")); DebugPort.print(TCCR1B_var);
  //  DebugPort.print(("       OCR1 : ")); DebugPort.println(OCR1A_var);
  if (OCR1A_var > 65536 || OCR1A_var <= 0)
  {
    TCCR1B_var = 64;
    OCR1A_var = ((16000000) / (freq * TCCR1B_var)) - 1; // (must be <65536)
  }
  //  DebugPort.println("3 :");
  //  DebugPort.print(("TCCR : ")); DebugPort.print(TCCR1B_var);
  //  DebugPort.print(("       OCR1 : ")); DebugPort.println(OCR1A_var);
  if (OCR1A_var > 65536 || OCR1A_var <= 0)
  {
    TCCR1B_var = 256;
    OCR1A_var = ((16000000) / (freq * TCCR1B_var)) - 1; // (must be <65536)
  }
  //  DebugPort.println("4 :");
  //  DebugPort.print(("TCCR : ")); DebugPort.print(TCCR1B_var);
  //  DebugPort.print(("       OCR1 : ")); DebugPort.println(OCR1A_var);
  if (OCR1A_var > 65536 || OCR1A_var <= 0)
  {
    TCCR1B_var = 1024;
    OCR1A_var = ((16000000) / (freq * TCCR1B_var)) - 1; // (must be <65536)
  }
  //  DebugPort.println("5 :");
  //  DebugPort.print(("TCCR : ")); DebugPort.print(TCCR1B_var);
  //  DebugPort.print(("       OCR1 : ")); DebugPort.println(OCR1A_var);

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

#if BOARD_VERSION == VERSION_2_2 

void serialEvent2()

#else

void serialEvent()

#endif
{
  while (DebugPort.available())
  {
    char inChar = (char)DebugPort.read();
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
          DebugPort.print(rxdata);
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
          DebugPort.println(rxdata);
          Prcs_RxData();
        }
      }
    }
  }
}


bool Prcs_RxData (void)  {
    
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


    if (p1 == "VM")   {
        //------------------------------
        if (p2 == STROKE_LENGTH)    {
            // stroke lenght

            
            float     inc_or_dec = payload.toFloat();
            float     abs_value  = abs(inc_or_dec);
            
            // DebugPort.print("Stroke Lenght packet received \"P9\" : ");
            DebugPort.println(payload);
            Stroke_length_forced = payload.toFloat();
            DebugPort.print  ("Stroke_length_forced : ");
            DebugPort.println(Stroke_length_forced);
           

            DebugPort.print("abs_value : ");
            DebugPort.println(abs_value);
            
            // pick_stroke_length ();
            if (abs_value <= 5)  {
                Stroke_length_forced += inc_or_dec;   // increment / decrement by 'inc_or_dec'
                Stroke_length        += inc_or_dec;
                Stroke_length_new    += inc_or_dec;
                f_test_data_updated = 1;               
            }
            
            // temp hack, if 'Stroke_length_forced' == 0, ventilator will work normally
            else if (Stroke_length_forced != 0.0)    {
                DebugPort.print("Stroke_length        : ");
                DebugPort.println(Stroke_length);
                DebugPort.print("Stroke_length_new    : ");
                DebugPort.println(Stroke_length_new);
                DebugPort.print("Stroke_length_forced : ");
                DebugPort.println(Stroke_length_forced);
                Stroke_length_new = Stroke_length_forced;
                Stroke_length = Stroke_length_forced;
                f_test_data_updated = 1;
            }
            else {
                // nop
            }
            
        }
        //------------------------------
        
        if (p2 == "ST")    {
            // stepper motor
            if (payload == "0000")  {
                if (cycle_start == true) {
                    run_motor = false;
                    if (comp_start == true && comp_end == false) {
                        stop_n_return_pulse_count = 0;
                        for (int i = 0; i < motion_profile_count_temp; i++) 
                        {
                            stop_n_return_pulse_count = stop_n_return_pulse_count + compression_step_array[i];
                            // stop_n_return_pulse_count = run_pulse_count_temp + 0;
                        }
                        stop_n_return_pulse_count = stop_n_return_pulse_count + run_pulse_count_temp + 0;
                    }
                    if (exp_start == true && exp_end == false) {
                        stop_n_return_pulse_count = 0;
                        for (int i = motion_profile_count_temp + 1; i < CURVE_EXP_STEPS; i++) 
                        {
                            stop_n_return_pulse_count = stop_n_return_pulse_count + expansion_step_array[i];
                            // stop_n_return_pulse_count = run_pulse_count - run_pulse_count_temp + 0;
                        }
                        stop_n_return_pulse_count = stop_n_return_pulse_count +  run_pulse_count - run_pulse_count_temp + 0;
                    }
                    if ((comp_start == true && comp_end == true) || (exp_start == true && exp_end == true)) {
                        stop_n_return_pulse_count = 0;
                        for (int i = 0; i < CURVE_EXP_STEPS; i++) 
                        {
                            stop_n_return_pulse_count = stop_n_return_pulse_count + compression_step_array[i];
                            // stop_n_return_pulse_count = run_pulse_count - run_pulse_count_temp + 0;
                        }
                    }
                    cycle_start = false;
                    DebugPort.print("ST: stop and home pulses : "); 
                    DebugPort.println(stop_n_return_pulse_count);
                    inti_Stop_n_Home();
                }
            }
        }
        
        else if (p2 == "IN")    {
            if (payload == "0000")  {
                // if (cycle_start == false)
                inti_Home_n_Start();
            }
            if (payload == "0001")   {
                if (cycle_start == true) {
                    run_motor = false;
                    if (comp_start == true) {
                        stop_n_return_pulse_count = 0;
                        for (int i = 0; i < motion_profile_count_temp; i++) 
                        {
                            stop_n_return_pulse_count = stop_n_return_pulse_count + compression_step_array[i];
                            // stop_n_return_pulse_count = run_pulse_count_temp + 0;
                        }
                        stop_n_return_pulse_count = stop_n_return_pulse_count + run_pulse_count_temp + 0;
                    }
                    if (exp_start == true) {
                        stop_n_return_pulse_count = 0;
                        for (int i = motion_profile_count_temp + 1; i < CURVE_EXP_STEPS; i++) 
                        {
                            stop_n_return_pulse_count = stop_n_return_pulse_count + expansion_step_array[i];
                            // stop_n_return_pulse_count = run_pulse_count - run_pulse_count_temp + 0;
                        }
                        stop_n_return_pulse_count = stop_n_return_pulse_count +  run_pulse_count - run_pulse_count_temp + 0;
                    }
                    cycle_start = false;
                    DebugPort.print("IN : stop and home pulses : "); DebugPort.println(stop_n_return_pulse_count);
                    inti_Stop_n_Home();
                }
            }
            
            if (payload == "0003")    {
                if ((cycle_start == true) && (digitalRead(HOME_SENSOR_PIN) == HOME_SENSE_VALUE))
                    breathe_detected_skip_exhale_n_start_inhale();
            }
            if (payload == "0002")    {
                inti_all_Valves();
            }
        }
    
        else if (p2 == "PP")    {
            if (payload == "0000")    {
                Par_editstat = 1;
            }
            if (payload == "1111")   {
                Par_editstat = 0;
            }
        }
        
        else if (p2 == TV_PARAM)    {
            tidal_volume_new = payload.toInt();
            DebugPort.print("TV : ");
            DebugPort.println(tidal_volume_new);
            pick_stroke_length();
            // Stroke_length_new=tidal_volume_new / 10;  //enable this to do calibration using serial cmd to control strok length with xx.x accuracy
            if (flag_Serial_requested == true)    {
                Serial3.print("$VSP20002&");
            }
        }
        
        else if (p2 == RR_PARAM)    {
            BPM_new = payload.toInt();
            DebugPort.print("BPM : ");
            DebugPort.println(BPM_new);
            cycle_time = 60.0 / BPM_new;
            DebugPort.print("cycle time : ");
            DebugPort.println(cycle_time);
            inhale_hold_time = (cycle_time * (inhale_hold_percentage / 100)) * 1000;
            DebugPort.print("Compression hold in mS: ");
            DebugPort.println(inhale_hold_time);
            pick_stroke_length();
            if (flag_Serial_requested == true)     {
                Serial3.print("$VSP50004&");
            }
        }

        else if (p2 == "P3")    {
            FiO2 = payload.toInt();
            DebugPort.print("FiO2 : "); DebugPort.println(FiO2);
        }
    
        else if (p2 == "P4")    {
            PEEP_new = payload.toInt();
            DebugPort.print("PEEP_new : "); DebugPort.println(PEEP_new);
        }
        
        else if (p2 == IER_PARAM)    {
            IER_new = payload.toInt();
            DebugPort.print("IER : "); DebugPort.println(IER_new);
            pick_stroke_length();
            //      IER = 1020;
            //      inhale_ratio = 1.0;
            //      exhale_ratio = 2.0;
            if (flag_Serial_requested == true)      {
                Serial3.print("$VSP60006&");
            }
        }
    
        else if (p2 == PEAK_PARAM)    {
            peak_prsur = payload.toInt();
            DebugPort.print("peak_prsur_new : "); 
            DebugPort.println(peak_prsur);
            if (flag_Serial_requested == true)    {
                Serial3.print("$VSP70011&");
            }
        }
        
        else if (p2 == GP0_PARAM)    {
            CAL_GP0_new = rxdata.substring(5, 13).toFloat() / 100000;
            apply_zerocal_offset_MPX5010(SENSOR_PRESSURE_A1, CAL_GP0_new);
            CAL_GP0 = get_zerocal_offset_MPX5010(SENSOR_PRESSURE_A1);
            DebugPort.print("CAL_GP0 : "); DebugPort.println(CAL_GP0);
            if (flag_Serial_requested == true)   {
                Serial3.print("$VSP80012&");
            }
        }
        
        else if (p2 == GP1_PARAM)    {
            CAL_GP1_new = rxdata.substring(5, 13).toFloat() / 100000;
            apply_zerocal_offset_MPX5010(SENSOR_PRESSURE_A0, CAL_GP1_new);
            CAL_GP1 = get_zerocal_offset_MPX5010(SENSOR_PRESSURE_A0);
            DebugPort.print("CAL_GP1 : "); DebugPort.println(CAL_GP1);
            
            DebugPort.print("IER: 1:");
            DebugPort.print(IER);
            DebugPort.print("  BPM: ");
            DebugPort.print(BPM);
            DebugPort.print("  TV: ");
            DebugPort.print(tidal_volume);
            DebugPort.print("  Stroke: ");
            DebugPort.println(Stroke_length);
            
            DebugPort.print("Peak Pressure: ");
            DebugPort.print(peak_prsur);
            DebugPort.print("  Cali. GP0: ");
            DebugPort.print(CAL_GP0);
            DebugPort.print("  Cali. GP1: ");
            DebugPort.println(CAL_GP1);
            
            
            if (flag_Serial_requested == true)      {
                flag_Serial_requested = false;
                convert_all_set_params_2_machine_values();
                Serial3.print("$VSO20000&");
            }
        }
    
        else if (p2 == "F2")    {
            if (1 == payload.toInt())      {
                DebugPort.println("Pressure flag == true   Milli volt flag == false");
                send_pressure_data = true;
                send_millivolts_data = false;
            }
            else if (0 == payload.toInt())      {
                DebugPort.println("Pressure flag == false");
                send_pressure_data = false;
            }
        }

        else if (p2 == "F3")    {
            if (1 == payload.toInt())      {
                DebugPort.println("Milli volt flag == true   Pressure flag == false");
                send_millivolts_data = true;
                send_pressure_data = false;
            }
            else if (0 == payload.toInt())      {
                DebugPort.println("Milli volt flag == false");
                send_millivolts_data = false;
            }
        }
        
        else if (p2 == "F1")    {
            if (1 == payload.toInt())      {
                send_pressure_data = false;
                delay(1000);
                perform_calib_gp = true;
                calibrate_MPX5010();
                DebugPort.print("sending calibration GP0 : ");
                CAL_GP0 = get_zerocal_offset_MPX5010(SENSOR_PRESSURE_A1);
                DebugPort.println(CAL_GP0 * 100000);
                DebugPort.println(Ctrl_CreateCommand(GP0_PARAM, (long)(get_zerocal_offset_MPX5010(SENSOR_PRESSURE_A1) * 100000), 0));
                Serial3.print(Ctrl_CreateCommand(GP0_PARAM, (long)(get_zerocal_offset_MPX5010(SENSOR_PRESSURE_A1) * 100000), 0));
                
                delay(5000);
                DebugPort.print("sending calibration GP1 : ");
                CAL_GP1 = get_zerocal_offset_MPX5010(SENSOR_PRESSURE_A0);
                DebugPort.println(CAL_GP1 * 100000);
                DebugPort.println(Ctrl_CreateCommand(GP1_PARAM, (long)(get_zerocal_offset_MPX5010(SENSOR_PRESSURE_A0) * 100000), 0));
                Serial3.print(Ctrl_CreateCommand(GP1_PARAM, (long)(get_zerocal_offset_MPX5010(SENSOR_PRESSURE_A0) * 100000), 0));
                delay(5000);
                perform_calib_gp = false;
                // send_pressure_data = true;
            }
            else if (0 == payload.toInt())      {
                perform_calib_gp = false;
            }
        }

        else if (p2 == "SV")    {
            if (p3 == "01")      {
                if (p4 == "00")    {
                    //digitalWrite(INHALE_VLV_PIN, LOW);
                    INHALE_VLV_CLOSE();
                    //Stop motor
                    if ((cycle_start == true) && (comp_start == true) && (comp_end == false))
                    Emergency_motor_stop = true;
                    //relief valve ON
                    //INHALE_RELEASE_VLV_OPEN();
                }
                else if (p4 == "01")        {
                    //digitalWrite(INHALE_VLV_PIN, HIGH);
                    INHALE_VLV_OPEN();
                }
            }
            else if (p3 == "02")      {
                if (p4 == "00")        {
                    // digitalWrite(EXHALE_VLV_PIN, LOW);
                    EXHALE_VLV_CLOSE();
                }
                else if (p4 == "01")        {
                    // digitalWrite(EXHALE_VLV_PIN, HIGH);
                    EXHALE_VLV_OPEN();
                }
            }
            else if (p3 == "03")      {
                if (p4 == "00")   {
                    INHALE_RELEASE_VLV_CLOSE();
                }
                else if (p4 == "01")  {
                    INHALE_RELEASE_VLV_OPEN();
                }   
            }
        }
   
        else if (p2 == "O2")    { //solanoide valve for Oxygen line
            if (p3 == "01")      {
                if (p4 == "00")   {
                    //digitalWrite(O2Cyl_VLV_PIN, LOW);
                    DebugPort.println("2Hln_VLV SELECTED");
                    O2_line_option = 1;
                    if (cycle_start == true)          {
                        O2Cyl_VLV_CLOSE();
                    }
                }
                else if (p4 == "01")   {
                    // digitalWrite(O2Cyl_VLV_PIN, HIGH);
                    DebugPort.println("O2Cyl_VLV SELECTED");
                    O2_line_option = 0;
                    if (cycle_start == true)  {
                        O2Cyl_VLV_OPEN();
                    }
                }
            }
            else if (p3 == "02") {
                if (p4 == "00")   {
                    DebugPort.println("O2Cyl_VLV SELECTED");
                    O2_line_option = 0;
                    if (cycle_start == true)     {
                        O2Cyl_VLV_OPEN();
                    }
                }
                else if (p4 == "01")    {
                    DebugPort.println("2Hln_VLV SELECTED");
                    O2_line_option = 1;
                    if (cycle_start == true)     {
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
    DebugPort.println("O2Cyl_VLV Opened...");
    O2Cyl_VLV_OPEN();
  }
  else
  {
    DebugPort.println("2Hln_VLV Opened...");
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
  DebugPort.println("Skipping Home Cycle : ");
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
  run_pulse_count = stop_n_return_pulse_count; // ((micro_stepping * (100.0 / LEAD_SCREW_PITCH * 1.0)) / 2.0);
  run_pulse_count_temp = 0.0;

  Exhale_timer_timout();
  DebugPort.println("Cycle Stop & goto Home : ");
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

  Serial3.print("$VSSY0000&");  ////cycle stop
   DebugPort.print("$VSSY0000&");  ////cycle stop
  delay(500);//this delay is necessary to avoid sending othr packet during processing of above command
  Serial3.print("$VSSY0000&");  ////cycle stop
  return true;
}

bool inti_Home_n_Start()
{
  Emergency_motor_stop = false;
  motion_profile_count_temp = 0;
  run_pulse_count_temp = 0.0;
  if (digitalRead(HOME_SENSOR_PIN) == !(HOME_SENSE_VALUE))
  {
    Home_attempt_count++;
    DebugPort.println("Home Cycle : ");
    run_pulse_count_temp = 0.0;
    run_pulse_count =  ((micro_stepping * (Start_home_travel / LEAD_SCREW_PITCH * 1.0)) / 2.0);
    DebugPort.print("Homing attempt count before start : "); DebugPort.println(Home_attempt_count);
    DebugPort.print("travel in 'mm' back to home on before start : "); DebugPort.println(Start_home_travel);
    DebugPort.print("Pulses required to travel back to home on before start : "); DebugPort.println(run_pulse_count);
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
    Serial3.print("$VSSY0002&");  ////homing
    DebugPort.print("$VSSY0002&");  ////homing
  }
  else
  {
    inti_Start();
  }
  return true;
}

bool inti_Start()
{
  //this will be done with $VS01........ comp sync signal
//  Serial3.print("$VSSY0001&");  ////cycle start
//  DebugPort.print("$VSSY0001&");  ////cycle start
  Home_attempt_count = 0;
  convert_all_set_params_2_machine_values();
  open_selected_O2_value();
  Emergency_motor_stop = false;
  DebugPort.println("Skipping Home Cycle : ");
  home_cycle = false;
  cycle_start = true;
  comp_start = false;
  comp_end = false;
  exp_start = true;
  exp_end = true;
  //exp_timer_end = true;
  Exhale_timer_timout();
  //run_motor = true
  delay(600); //this delay is necessary to avoid sending othr packet during processing of above command
  return true;
}



#define RR_LOWEST_VALUE     (10)
#define RR_HIGHEST_VALUE    (30)
#define RR_STEP_VALUE       (1)
#define TV_LOWEST_VLAUE     (200)
#define TV_HIGHEST_VLAUE    (700)
#define TV_STEP_VLAUE       (50)

int get_index_from_RR (int   rr) {

    if (rr < RR_LOWEST_VALUE)   {
        return 0;
    }
    else if (rr > RR_HIGHEST_VALUE)   {
        return (RR_HIGHEST_VALUE - RR_LOWEST_VALUE);
    }  
    else {
        return (rr - RR_LOWEST_VALUE);
    }
    
}

int get_index_from_TV (int   tv) {

    int     index = 0;
    
    switch (tv)
    {
        case 200:     index = 0;           break;
        case 250:     index = 1;           break;
        case 300:     index = 2;           break;
        case 350:     index = 3;           break;
        case 400:     index = 4;           break;
        case 450:     index = 5;           break;
        case 500:     index = 6;           break;
        case 550:     index = 7;           break;
        case 600:     index = 8;           break;
        case 650:     index = 9;           break;
        case 700:     index = 10;          break;
        default:      index = 0;           break;
    }

    if (tv < TV_LOWEST_VLAUE)   {
        return 0;
    }
    else if (tv > TV_HIGHEST_VLAUE)   {
        return 10;
    }  
    else {
        return index;
    }
    
}

int get_index_from_IER (int   ier) {

    int ier_index = 0;

    switch (ier)
    {
        default:
        case 1: // IER = 1:1
            ier_index = 0;
            break;
        case 2: // IER = 1:2
            ier_index = 1;
            break;
        case 3: // IER = 1:3
            ier_index = 2;
            break;
        
    }

    return ier_index;
    
}

// temp
#define    NA  (77)



const volatile float IER1_RR_TV_to_SL[3][21][11] = {


    // IER - 1
    {   // Tidal volume 200 to 700 (11 entries in all) --->
       // 200         300         400         500         600         700
        { 47.3, 51.9, 56.5, 61.4, 65.9, 70.1, 74.3, 79.9, 85.6, 93.0,  99.9  }, /*  |  */      /* 10 */	// tested
        { 47.0, 51.0, 56.0, 61.0, 65.7, 70.0, 74.0, 79.6, 85.8, 92.7,  99.9  }, /*  |  */      /* 11 */
        { 47.5, 51.2, 56.0, 61.5, 65.0, 70.5, 74.5, 79.2, 85.3, 93.0,  99.9  }, /*  |  */      /* 12 */
        { 46.5, 50.0, 55.1, 60.5, 65.4, 70.0, 74.0, 79.8, 85.5, 93.2, 100.0  }, /*  V  */      /* 13 */
        { 46.5, 50.5, 55.4, 60.8, 65.8, 70.5, 74.5, 79.4, 85.0, 93.1, 100.0  }, /*     */      /* 14 */
        { 45.3, 50.4, 55.7, 60.5, 65.3, 69.5, 74.3, 79.9, 85.5, 93.0, 100.0  }, /*  R  */      /* 15 */	// tested
        { 45.0, 50.0, 55.6, 60.2, 65.2, 69.8, 74.8, 79.8, 85.0, 93.1, 100.3  }, /*  R  */      /* 16 */
        { 45.0, 50.5, 55.2, 60.8, 65.8, 69.6, 74.6, 79.6, 85.6, 93.1, 100.2  }, /*     */      /* 17 */
        { 44.7, 50.2, 55.8, 60.5, 65.2, 69.4, 74.4, 79.4, 85.3, 93.0, 100.2  }, /*  10 */      /* 18 */
        { 44.5, 50.9, 55.4, 60.2, 65.1, 69.2, 74.2, 79.2, 85.5, 93.5, 100.0  }, /*     */      /* 19 */
        { 44.3, 50.3, 55.3, 60.8, 65.5, 69.9, 74.5, 79.6, 86.5, 93.3, 100.7  }, /*  to */      /* 20 */	// tested 
        { 44.1, 50.3, 55.9, 60.0, 65.6, 69.8, 74.1, 79.8, 86.7, 93.7, 100.0  }, /*     */      /* 21 */
        { 44.9, 50.0, 55.8, 60.9, 65.6, 69.6, 74.2, 79.6, 86.9, 93.9, 100.0  }, /*  30 */      /* 22 */
        { 44.7, 50.7, 55.7, 60.9, 65.2, 69.4, 74.3, 79.4, 86.1, 93.1, 100.0  }, /*     */      /* 23 */
        { 44.5, 50.4, 55.6, 60.9, 65.0, 69.4, 74.4, 79.2, 86.3, 93.3, 100.0  }, /*     */      /* 24 */    
        { 44.2, 50.2, 55.2, 60.8, 65.5, 69.9, 74.5, 81.0, 89.0, 95.5, 104.0  }, /*     */      /* 25 */ // tested
        { 44.3, 50.1, 55.4, 60.5, 65.8, 70.1, 75.8, 82.4, 90.5, 95.9, 102.0  }, /*     */      /* 26 */
        { 44.5, 50.1, 55.3, 60.5, 65.7, 70.3, 76.1, 83.8, 92.0, 96.3, 102.0  }, /*     */      /* 27 */
        { 44.6, 50.1, 56.2, 60.4, 65.7, 70.5, 77.4, 84.2, 93.5, 96.7,    NA  }, /*     */      /* 28 */
        { 44.7, 50.1, 56.1, 61.2, 65.7, 71.1, 78.7, 85.6, 94.0, 97.1,    NA  }, /*     */      /* 29 */
        { 44.7, 50.6, 56.1, 61.2, 65.8, 71.3, 79.9, 87.6, 95.1,   NA,    NA  }, /*     */      /* 30 */	// tested
    },
    
    
    // IER - 2
    {   // Tidal volume 200 to 700 (11 entries in all) --->
       // 200         300         400         500         600         700
      //{ 53.5, 58 0, 63 0, 67.5, 71.4, 75.2, 79.3, 83.5, 88.6, 94.5, 102.0  }, /*  |  */		/* 10 */
        { 59.5, 50.7, 55.5, 60.4, 64.9, 69.1, 74.3, 79.9, 85.6, 93.0,  99.9  }, /*  |  */      /* 10 */

        { 52.5, 57.5, 62.5, 66.8, 71.2, 75.0, 78.9, 83.1, 88.3, 94.3, 101.6  }, /*  |  */       /* 11 */
        { 51.5, 56.6, 62.2, 66.4, 70.8, 74.5, 78.7, 82.8, 88.0, 94.1, 101.2  }, /*  |  */       /* 12 */
        { 51.0, 56.5, 61.9, 66.2, 70.2, 74.2, 78.3, 82.4, 87.7, 93.9, 100.8  }, /*  V  */       /* 13 */
        { 50.5, 56.2, 61.0, 65.8, 70.1, 73.8, 78.0, 82.2, 87.4, 93.7, 100.4  }, /*     */       /* 14 */
      //{ 50.2, 55.9, 60.7, 65.5, 69.8, 73.8, 77.7, 82.2, 87.1, 93.5, 100.4  }, /*  R  */       /* 15 */
        { 53.5, 51.0, 55.7, 60.9, 65.5, 69.5, 74.7, 80.1, 86.5, 93.0, 106.7  }, /*  R  */      /* 15 */
       
        { 50.0, 55.6, 60.4, 65.2, 69.6, 73.6, 77.6, 82.2, 87.3, 93.7, 100.6  }, /*  R  */       /* 16 */
        { 49.8, 55.3, 60.2, 65.1, 69.6, 73.4, 77.6, 82.0, 87.5, 93.9, 100.8  }, /*     */       /* 17 */
        { 49.5, 55.1, 60.2, 65.0, 69.2, 73.3, 77.6, 82.0, 87.7, 94.1, 101.0  }, /*  10 */       /* 18 */
        { 49.5, 55.0, 59.8, 64.9, 69.0, 73.1, 77.6, 82.0, 87.9, 94.3, 101.2  }, /*     */       /* 19 */
        
      //{ 49.1, 54.9, 59.8, 64.9, 68.8, 73.1, 77.6, 82.0, 88.1, 94.5, 101.4  }, /*  to */       /* 20 */
        { 51.3, 50.3, 55.9, 60.8, 65.5, 69.9, 75.5, 80.8, 86.5, 93.3,  99.9  }, /*  to */      /* 20 */

        { 49.0, 54.8, 59.8, 64.9, 68.8, 73.3, 77.9, 82.3, 88.3, 94.7, 101.9  }, /*     */       /* 21 */
        { 48.8, 54.7, 59.8, 64.8, 68.8, 73.5, 78.2, 82.6, 88.5, 94.9, 102.4  }, /*  30 */       /* 22 */
        { 48.8, 54.6, 59.8, 64.5, 68.8, 73.7, 78.5, 82.9, 88.7, 95.1, 102.9  }, /*     */       /* 23 */
        { 48.8, 54.5, 59.8, 64.5, 68.8, 73.9, 78.8, 83.2, 88.9, 95.3, 103.4  }, /*     */       /* 24 */
        { 48.8, 54.5, 59.8, 64.5, 68.8, 74.1, 79.1, 83.5, 89.4, 95.5,    NA  }, /*     */       /* 25 */
        { 49.0, 54.5, 59.8, 64.5, 69.0, 74.3, 79.4, 83.9, 89.9, 96.5,    NA  }, /*     */       /* 26 */
        { 48.8, 54.5, 59.6, 64.5, 69.3, 74.5, 79.7, 84.3, 90.4, 97.0,    NA  }, /*     */       /* 27 */
        { 48.8, 54.5, 59.6, 64.5, 69.6, 74.7, 80.0, 84.7, 90.9,   NA,    NA  }, /*     */       /* 28 */
        { 48.8, 54.6, 59.8, 64.7, 69.9, 74.9, 80.3, 85.1, 91.4,   NA,    NA  }, /*     */       /* 29 */
      //{ 48.8, 54.6, 59.8, 64.7, 70.2, 75.1, 80.6, 85.5, 91.9,   NA,    NA  }, /*     */       /* 30 */
        { 48.8, 50.9, 57.1, 62.5, 67.6, 72.5, 78.1, 83.9, 90.1,   NA,    NA  }, /*     */       /* 30 */
    },
    
    
    // IER - 3
    {   // Tidal volume 200 to 700 (11 entries in all) --->
       // 200         300         400         500         600         700
      //{ 51.0, 56.5, 61.5, 66.2, 70.2, 74.2, 78.0, 82.4, 87.7, 93.5,  99.4  }, /*  |  */		/* 10 */
        { 59.5, 50.1, 55.5, 60.4, 64.9, 69.4, 74.3, 79.9, 85.6, 93.0,  99.9  }, /*  |  */      /* 10 */

        { 50.5, 56.2, 61.0, 65.8, 70.1, 73.8, 77.9, 82.4, 87.4, 93.7,  99.6  }, /*  |  */       /* 11 */
        { 50.2, 55.9, 60.7, 65.5, 69.8, 73.8, 77.8, 82.4, 87.1, 93.9,  99.8  }, /*  |  */       /* 12 */
        { 50.0, 55.6, 60.4, 65.2, 69.6, 73.6, 77.7, 82.4, 87.3, 94.1, 100.0  }, /*  V  */       /* 13 */
        { 49.8, 55.3, 60.2, 65.1, 69.6, 73.4, 77.6, 82.4, 87.5, 94.3, 100.2  }, /*     */       /* 14 */
      //{ 49.5, 55.1, 60.2, 65.0, 69.2, 73.6, 77.5, 82.4, 87.7, 94.6, 100.6  }, /*  R  */       /* 15 */
        { 53.5, 51.0, 55.7, 60.9, 65.5, 69.5, 74.7, 80.1, 86.5, 93.0, 106.7  }, /*  R  */      /* 15 */

        { 49.5, 55.0, 59.8, 65.0, 69.2, 73.8, 77.6, 82.8, 88.2, 95.0, 101.0  }, /*  R  */       /* 16 */
        { 49.1, 54.9, 59.8, 65.0, 69.2, 74.0, 77.9, 83.2, 88.7, 95.4, 101.4  }, /*     */       /* 17 */
        { 49.0, 54.8, 59.8, 65.0, 69.2, 74.2, 78.2, 83.6, 89.2, 95.8, 101.8  }, /*  10 */       /* 18 */
        { 48.8, 54.7, 59.8, 65.0, 69.2, 74.4, 78.5, 84.0, 89.7, 96.2,    NA  }, /*     */       /* 19 */
      //{ 48.8, 54 6, 59 8, 65.0, 69.2, 74.6, 78.8, 84.5, 90.2, 96.6,    NA  }, /*  to */       /* 20 */
        { 51.3, 50.5, 56.1, 61.8, 67.5, 71.9, 76.5, 81.6, 86.5, 93.3, 100.7  }, /*  to */      /* 20 */
  
        { 48.8, 54.5, 59.9, 65.4, 69.4, 74.8, 79.2, 84.9, 90.7, 97.0,    NA  }, /*     */       /* 21 */
        { 48.8, 54.5, 60.0, 65.5, 69.6, 75.0, 79.6, 85.3, 91.2,   NA,    NA  }, /*  30 */       /* 22 */
        { 48.8, 54.5, 60.1, 65.6, 69.8, 75.2, 80.0, 85.7, 91.7,   NA,    NA  }, /*     */       /* 23 */
        { 48.8, 54.5, 60.2, 65.7, 70.0, 75.4, 80.4, 86.1, 92.2,   NA,    NA  }, /*     */       /* 24 */
        { 48.8, 54.5, 60.3, 65.9, 70.4, 75.6, 80.8, 86.5,   NA,   NA,    NA  }, /*     */       /* 25 */
        { 48.8, 54.6, 60.4, 66.1, 70.8, 76.0, 81.3, 87.0,   NA,   NA,    NA  }, /*     */       /* 26 */
        { 48.8, 54.6, 60.5, 66.3, 71.2, 76.4, 81.8, 87.5,   NA,   NA,    NA  }, /*     */       /* 27 */
        { 48.8, 54.5, 60.6, 66.5, 71.6, 76.8, 82.3,   NA,   NA,   NA,    NA  }, /*     */       /* 28 */
        { 48.8, 54.6, 60.7, 66.7, 72.0, 77.2, 82.8,   NA,   NA,   NA,    NA  }, /*     */       /* 29 */
        
      //{ 48.8, 54.6, 60.8, 66.9, 72.4, 77.6, 83.3,   NA,   NA,   NA,    NA  }, /*     */       /* 30 */
        { 48.8, 51.6, 58.1, 63.9, 69.8, 74.6, 80.1,   NA,   NA,   NA,    NA  }, /*     */       /* 30 */
    },
    
};


void pick_stroke_length (void)   {

    int index1, index2, index3;

    index1 = get_index_from_IER (IER_new);
    index2 = get_index_from_RR  (BPM_new);
    index3 = get_index_from_TV  (tidal_volume_new);

    Stroke_length_new = IER1_RR_TV_to_SL[index1][index2][index3];

    // -------------------------------------------------------------

    DebugPort.print("index1 : ");
    DebugPort.print(index1);
    DebugPort.print("  index2 : ");
    DebugPort.print(index2);
    DebugPort.print("  index3 : ");
    DebugPort.print(index3);
    DebugPort.print("  SL_new : ");
    DebugPort.println(Stroke_length_new);
  
}



// ------------------------ EOF ----------------
