// dataBase.h




#ifndef _DATABASE_h
#define _DATABASE_h


#include "Arduino.h"
#include "BoardDefines.h"
#include "EEPROM.h"


#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif


#define	IER_TABLE_COUNT		(3)
#define	BPM_COUNT			(21)	// 10BPM to 30BPM (inclusive limits)
#define	TIDAL_VOLUMES_COUNT	(11)	// 200 to 700 in steps of 50 (inclusive limits)


// structure declarations
typedef union strk_len_u {
	
	float		strk_len;
	uint32_t	word;		// just to access 4 bytes at ones for blank check
	
} STRK_LEN_U;


	
//extern STRK_LEN_U	ier_bpm_tv_2_strk_len[3][21][11];
extern STRK_LEN_U	ier_bpm_tv_2_strk_len[IER_TABLE_COUNT][BPM_COUNT][TIDAL_VOLUMES_COUNT];
	




// variable declarations
//const volatile float ier_bpm_tv_2_strk_len[3][21][11];


// function prototypes..
void pick_stroke_length (void);
void print_stride_lenght_tables (void);
void print_stride_lenght_tables_from_eeprom (void);
void update_stride_length (void);



#endif

