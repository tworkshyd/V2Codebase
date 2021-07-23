#ifndef __BOARD_INCLUDES_H__
#define __BOARD_INCLUDES_H__



#define VERSION_2_0		0

#define VERSION_2_1		1

#define VERSION_2_2		2


/// Define the Board Here
#define BOARD_VERSION VERSION_2_2

#if BOARD_VERSION == VERSION_2_2

#pragma message("Board Version 2.2")

//#define DebugPort Serial2
#define LoggPort  Serial2
#define DebugPort Serial3

#define OXYGEN_SENSOR_GAIN GAIN_EIGHT

#else

#pragma message("Board Version 2.1/2.0")

#define DebugPort Serial

#define OXYGEN_SENSOR_GAIN GAIN_ONE


#endif


#define ENABLE_BUZZER 0
//#define DEBUG_RECEIVED_DATA 1
#define PRINT_PROCESSING_TIME 0

#define ENABLE_O2_SUPPLY 0


#endif //  __BOARD_INCLUDES_H__
