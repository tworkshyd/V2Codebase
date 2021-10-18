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


#define	IER_TABLE_COUNT			(3)
#define	BPM_ROWS_COUNT			(21)	// 10BPM to 30BPM (inclusive limits)
#define	TIDAL_VOLUMES_COUNT		(11)	// 200 to 700 in steps of 50 (inclusive limits)

#define	ELEMENT_SIZE			(sizeof(float))
#define	BPM_ROW_SIZE			(ELEMENT_SIZE * TIDAL_VOLUMES_COUNT)	
#define IER_TABLE_SIZE			(BPM_ROW_SIZE * BPM_ROWS_COUNT)

// structure declarations
typedef union strk_len_u {
	
	float		strk_len;
	uint32_t	word;		// just to access 4 bytes at ones for blank check
	
} STRK_LEN_U;


	
//extern STRK_LEN_U	ier_bpm_tv_2_strk_len[3][21][11];
extern STRK_LEN_U	ier_bpm_tv_2_strk_len[IER_TABLE_COUNT][BPM_ROWS_COUNT][TIDAL_VOLUMES_COUNT];
	




// variable declarations
//const volatile float ier_bpm_tv_2_strk_len[3][21][11];


// function prototypes..
void pick_stroke_length (void);
void update_stroke_length_in_eeprom (float length);
void print_stride_lenght_tables (void);
void print_stride_lenght_tables_from_eeprom (void);
void update_stride_length (void);



#endif

