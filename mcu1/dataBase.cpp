// 
// 
// 

#include "dataBase.h"


// temp
// #define    NA  (77.7)
#define    NA  (101.77)



// const volatile float ier_bpm_tv_2_strk_len[3][21][11] = {
extern STRK_LEN_U	ier_bpm_tv_2_strk_len[IER_TABLE_COUNT][BPM_COUNT][TIDAL_VOLUMES_COUNT] = {

	// IER - 1
	{   // Tidal volume 200 to 700 (11 entries in all) --->
		// 200         300         400         500         600         700
		{ 47.3, 51.9, 56.5, 61.4, 65.9, 70.1, 74.3, 79.9, 85.6, 93.0,  99.9  }, /*  |  */      /* 10 */   // tested
		{ 47.0, 51.0, 56.0, 61.0, 65.7, 70.0, 74.0, 79.6, 85.8, 92.7,  99.9  }, /*  |  */      /* 11 */
		{ 47.5, 51.2, 56.0, 61.5, 65.0, 70.5, 74.5, 79.2, 85.3, 93.0,  99.9  }, /*  |  */      /* 12 */
		{ 46.5, 50.0, 55.1, 60.5, 65.4, 70.0, 74.0, 79.8, 85.5, 93.2, 100.0  }, /*  V  */      /* 13 */
		{ 46.5, 50.5, 55.4, 60.8, 65.8, 70.5, 74.5, 79.4, 85.0, 93.1, 100.0  }, /*     */      /* 14 */
		{ 45.3, 50.4, 55.7, 60.5, 65.3, 69.5, 74.3, 79.9, 85.5, 93.0, 100.0  }, /*  B  */      /* 15 */   // tested
		{ 45.0, 50.0, 55.6, 60.2, 65.2, 69.8, 74.8, 79.8, 85.0, 93.1, 100.3  }, /*  P  */      /* 16 */
		{ 45.0, 50.5, 55.2, 60.8, 65.8, 69.6, 74.6, 79.6, 85.6, 93.1, 100.2  }, /*  M  */      /* 17 */
		{ 44.7, 50.2, 55.8, 60.5, 65.2, 69.4, 74.4, 79.4, 85.3, 93.0, 100.2  }, /*     */      /* 18 */
		{ 44.5, 50.9, 55.4, 60.2, 65.1, 69.2, 74.2, 79.2, 85.5, 93.5, 100.0  }, /*  10 */      /* 19 */
		{ 44.3, 50.3, 55.3, 60.8, 65.5, 69.9, 74.5, 79.6, 86.5, 93.3, 100.7  }, /*     */      /* 20 */   // tested
		{ 44.1, 50.3, 55.9, 60.0, 65.6, 69.8, 74.1, 79.8, 86.7, 93.7, 100.0  }, /*  to */      /* 21 */
		{ 44.9, 50.0, 55.8, 60.9, 65.6, 69.6, 74.2, 79.6, 86.9, 93.9, 100.0  }, /*     */      /* 22 */
		{ 44.7, 50.7, 55.7, 60.9, 65.2, 69.4, 74.3, 79.4, 86.1, 93.1, 100.0  }, /*  30 */      /* 23 */
		{ 44.5, 50.4, 55.6, 60.9, 65.0, 69.4, 74.4, 79.2, 86.3, 93.3, 100.0  }, /*     */      /* 24 */
		{ 44.2, 50.2, 55.2, 60.8, 65.5, 69.9, 74.5, 81.0, 89.0, 95.5, 104.0  }, /*     */      /* 25 */   // tested
		{ 44.3, 50.1, 55.4, 60.5, 65.8, 70.1, 75.8, 82.4, 90.5, 95.9, 102.0  }, /*     */      /* 26 */
		{ 44.5, 50.1, 55.3, 60.5, 65.7, 70.3, 76.1, 83.8, 92.0, 96.3, 102.0  }, /*     */      /* 27 */
		{ 44.6, 50.1, 56.2, 60.4, 65.7, 70.5, 77.4, 84.2, 93.5, 96.7,    NA  }, /*     */      /* 28 */
		{ 44.7, 50.1, 56.1, 61.2, 65.7, 71.1, 78.7, 85.6, 94.0, 97.1,    NA  }, /*     */      /* 29 */
		{ 44.7, 50.6, 56.1, 61.2, 65.8, 71.3, 79.9, 87.6, 95.1,   NA,    NA  }, /*     */      /* 30 */   // tested
	},
	
	
	// IER - 2
	{   // Tidal volume 200 to 700 (11 entries in all) --->
		// 200         300         400         500         600           700
		{ 49.9, 55.5, 62.0, 66.3, 71.4, 76.6, 82.9, 89.2, 96.5, 103.7,  99.9  }, /*  |  */       /* 10 */  // tested
		{ 49.8, 55.5, 62.5, 66.8, 71.9, 76.0, 82.5, 89.7, 96.6, 103.0, 101.0  }, /*  |  */       /* 11 */
		{ 49.7, 55.6, 62.2, 66.4, 71.1, 76.5, 82.3, 89.6, 95.5, 102.1, 102.1  }, /*  |  */       /* 12 */
		{ 49.6, 55.5, 61.9, 66.2, 71.2, 76.2, 82.1, 88.5, 95.4, 102.1, 103.1  }, /*  V  */       /* 13 */
		{ 49.5, 55.2, 61.0, 66.8, 71.3, 75.8, 81.9, 88.5, 85.4, 102.9, 104.3  }, /*     */       /* 14 */
		{ 49.3, 55.7, 61.3, 66.5, 71.1, 75.7, 81.3, 87.5, 94.4, 101.1, 108.1  }, /*  B  */       /* 15 */  // tested
		{ 49.0, 55.6, 61.4, 66.2, 71.6, 75.6, 81.6, 86.2, 94.3, 101.7, 108.6  }, /*  P  */       /* 16 */
		{ 49.8, 55.3, 61.2, 66.1, 71.6, 75.4, 81.6, 85.0, 94.5, 101.9, 108.8  }, /*  M  */       /* 17 */
		{ 49.5, 55.1, 61.2, 65.0, 70.2, 75.3, 81.6, 86.0, 94.7, 101.1, 108.0  }, /*     */       /* 18 */
		{ 49.5, 55.0, 60.8, 65.9, 70.0, 75.6, 80.6, 86.0, 94.9, 101.3, 108.2  }, /*  10 */       /* 19 */
		{ 49.3, 55.0, 60.8, 65.5, 70.1, 75.3, 80.0, 86.9, 94.0, 101.3, 108.3  }, /*     */       /* 20 */  // tested
		{ 49.0, 55.8, 60.8, 65.9, 70.8, 75.3, 80.9, 86.3, 94.3, 101.7, 101.9  }, /*  to */       /* 21 */
		{ 49.8, 55.7, 60.8, 65.8, 70.8, 75.5, 80.2, 86.6, 94.5, 101.9, 102.4  }, /*     */       /* 22 */
		{ 49.8, 55.6, 60.8, 65.5, 70.8, 75.7, 80.5, 86.9, 94.7, 101.1, 102.9  }, /*  30 */       /* 23 */
		{ 49.8, 55.5, 60.8, 66.5, 70.8, 75.9, 80.8, 86.2, 94.9, 101.3, 103.4  }, /*     */       /* 24 */
		{ 49.0, 55.0, 60.7, 66.1, 70.5, 75.1, 80.2, 86.7, 94.4, 101.6,    NA  }, /*     */       /* 25 */  // tested
		{ 49.0, 55.5, 60.8, 66.5, 70.0, 75.3, 80.4, 86.9, 94.9, 101.6,    NA  }, /*     */       /* 26 */
		{ 49.8, 55.5, 60.6, 66.5, 70.3, 75.5, 80.7, 87.3, 94.4, 101.6,    NA  }, /*     */       /* 27 */
		{ 48.8, 55.5, 61.6, 65.5, 70.6, 75.7, 80.0, 88.7, 95.9, 101.6,    NA  }, /*     */       /* 28 */
		{ 48.8, 55.6, 61.8, 65.7, 70.9, 75.9, 80.3, 88.1, 95.4, 101.6,    NA  }, /*     */       /* 29 */
		{ 48.9, 55.1, 61.2, 65.6, 70.8, 76.1, 81.9, 89.1, 95.5, 101.6,    NA  }, /*     */       /* 30 */  // tested
	},
	
	
	// IER - 3
	{   // Tidal volume 200 to 700 (11 entries in all) --->
		// 200         300         400         500         600         700
		{ 45.1, 50.1, 57.9, 62.6, 67.4, 71.4, 75.3, 79.9, 84.4, 88.60,  94.40  }, /*  |  */       /* 10 */
		{ 45.4, 50.5, 58.4, 63.0, 67.7, 71.7, 75.5, 80.1, 84.7, 89.48,  94.82  }, /*  |  */       /* 11 */
		{ 45.7, 50.9, 58.9, 63.4, 68.0, 72.0, 75.8, 80.3, 85.0, 90.36,  95.24  }, /*  |  */       /* 12 */
		{ 46.0, 51.3, 59.4, 63.8, 68.3, 72.3, 76.1, 80.5, 85.3, 91.25,  95.66  }, /*  V  */       /* 13 */
		{ 46.2, 51.7, 59.9, 64.2, 68.6, 72.5, 76.4, 80.7, 85.6, 92.12,  96.08  }, /*     */       /* 14 */
		{ 46.5, 51.8, 60.2, 64.5, 68.9, 72.8, 76.7, 80.9, 85.9,  93.0,  96.50  }, /*  B  */       /* 15 */
		{ 46.5, 51.7, 59.8, 65.0, 69.2, 73.8, 77.6, 82.8, 88.2,  95.0, 101.0   }, /*  P  */       /* 16 */
		{ 46.5, 51.6, 59.8, 65.0, 69.2, 74.0, 77.9, 83.2, 88.7,  95.4, 101.4   }, /*  M  */       /* 17 */
		{ 46.4, 51.5, 59.8, 65.0, 69.2, 74.2, 78.2, 83.6, 89.2,  95.8, 101.8   }, /*     */       /* 18 */
		{ 46.4, 51.4, 59.8, 65.0, 69.2, 74.4, 78.5, 84.0, 89.7,  96.2,    NA   }, /*  10 */       /* 19 */
		{ 46.4, 51.4, 56.6, 61.5, 66.3, 70.9, 75.3, 80.6, 85.6,  92.0,  98.1   }, /*     */       /* 20 */
		{ 46.4, 51.5, 56.7, 61.8, 66.5, 71.1, 75.6, 80.8, 85.9,  94.0,    NA   }, /*  to */       /* 21 */
		{ 46.4, 51.6, 56.8, 62.1, 66.7, 71.3, 75.9, 81.0, 86.2,    NA,    NA   }, /*     */       /* 22 */
		{ 46.4, 51.7, 56.8, 62.1, 66.8, 71.5, 76.2, 81.2, 86.5,    NA,    NA   }, /*  30 */       /* 23 */
		{ 46.4, 51.8, 56.8, 62.2, 66.8, 71.7, 76.4, 81.4, 86.9,    NA,    NA   }, /*     */       /* 24 */
		{ 46.4, 51.9, 56.9, 62.2, 66.9, 71.9, 76.8, 81.5, 87.4,    NA,    NA   }, /*     */       /* 25 */
		{ 46.4, 52.0, 56.9, 62.5, 67.1, 76.1, 77.1, 81.7,   NA,    NA,    NA   }, /*     */       /* 26 */
		{ 46.4, 52.1, 56.9, 62.8, 67.3, 76.3, 77.4, 81.9,   NA,    NA,    NA   }, /*     */       /* 27 */
		{ 46.3, 52.2, 57.0, 62.8, 67.5, 76.5, 77.7,   NA,   NA,    NA,    NA   }, /*     */       /* 28 */
		{ 46.3, 52.3, 57.0, 62.9, 67.7, 76.7, 78.1,   NA,   NA,    NA,    NA   }, /*     */       /* 29 */
		{ 46.2, 52.3, 57.1, 62.9, 68.2, 73.0, 78.6,   NA,   NA,    NA,    NA   }, /*     */       /* 30 */
	},
	
};


// exported variables...
extern volatile float Stroke_length_new; /*!< Auto calculated Stroke length based on Tidal Volume lookup table created based on our calibration.*/
extern volatile float tidal_volume_new;  /*!< User settable Tidal Volume setting to calculate equivalent Stroke length to compress */
extern volatile float BPM_new;           /*!< User settable BPM setting to calculate cycle time */
extern volatile float IER_new;           /*!< User settable inhale:exhale ratio */



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


void pick_stroke_length (void)   {

	int index1, index2, index3;

	index1 = get_index_from_IER (IER_new);
	index2 = get_index_from_RR  (BPM_new);
	index3 = get_index_from_TV  (tidal_volume_new);

// 	Stroke_length_new = ier_bpm_tv_2_strk_len[index1][index2][index3];
	Stroke_length_new = ier_bpm_tv_2_strk_len[index1][index2][index3].strk_len;

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


void print_stride_lenght_tables (void)	{
	

// 	float f = 0.00f;   //Variable to store data read from EEPROM.
// 	int eeAddress = 0; //EEPROM address to start reading from
// 	
// 	//Get the float data from the EEPROM at position 'eeAddress'
// 	EEPROM.get(eeAddress, f);
// 	Serial.println(f, 3);    //This may print 'ovf, nan' if the data inside the EEPROM is not a valid float.
 	
	DebugPort.println ("\r\nStride length table..\r\n");
	for (int i = 0; i <IER_TABLE_COUNT; i++)
	{
		DebugPort.print ("\t// IER 1:");
		DebugPort.println (i + 1);
		DebugPort.print ("\t{\t// 200          300           400           500           600           700\r\n");
		for (int j = 0; j < BPM_COUNT; j++)
		{
			DebugPort.print ("\t\t{ ");
			for (int k = 0; k < TIDAL_VOLUMES_COUNT; k++)
			{
				DebugPort.print (ier_bpm_tv_2_strk_len[i][j][k].strk_len, 2);
				if (k < (TIDAL_VOLUMES_COUNT - 1))	{
					DebugPort.print (", ");
				}
			}
			DebugPort.print (" }, // ");
			DebugPort.println (j + 10);
		}
		DebugPort.println ("\t},");
		DebugPort.println ();
	}	
		
}

void print_stride_lenght_tables_from_eeprom (void)	{
	

	float f = 0.00f;   //Variable to store data read from EEPROM.
	int eeAddress = 0; //EEPROM address to start reading from

// 	//Get the float data from the EEPROM at position 'eeAddress'
// 	EEPROM.get(eeAddress, f);
// 	Serial.println(f, 3);    //This may print 'ovf, nan' if the data inside the EEPROM is not a valid float.
	
	eeAddress = 0;
	DebugPort.println ("\r\nStride length table.. from eeprom\r\n");
	for (int i = 0; i <IER_TABLE_COUNT; i++)
	{
		DebugPort.print ("\t// IER 1:");
		DebugPort.println (i + 1);
		DebugPort.print ("\t{\t// 200          300           400           500           600           700\r\n");
			for (int j = 0; j < BPM_COUNT; j++)
			{
				DebugPort.print ("\t\t{ ");
					for (int k = 0; k < TIDAL_VOLUMES_COUNT; k++)
					{
						EEPROM.get(eeAddress, f);
						eeAddress += sizeof(float);
						// DebugPort.print (ier_bpm_tv_2_strk_len[i][j][k].strk_len, 2);
						DebugPort.print (f, 2);
						
						if (k < (TIDAL_VOLUMES_COUNT - 1))	{
							DebugPort.print (", ");
						}
					}
				DebugPort.print (" }, // ");
				DebugPort.println (j + 10);
			}
		DebugPort.println ("\t},");
		DebugPort.println ();
	}
	
}


void update_stride_length (void)	{
	
	int eeAddress = 0; //EEPROM address to start reading from
	STRK_LEN_U	one_element;

	// 	//Get the float data from the EEPROM at position 'eeAddress'
	// 	EEPROM.get(eeAddress, f);
	// 	Serial.println(f, 3);    //This may print 'ovf, nan' if the data inside the EEPROM is not a valid float.
	
	eeAddress = 0;
	DebugPort.println ("\r\nupdating.. from eeprom\r\n");
	for (int i = 0; i <IER_TABLE_COUNT; i++)
	{
		DebugPort.print ("\t// IER 1:");
		DebugPort.println (i + 1);
		DebugPort.print ("\t{\t// 200          300           400           500           600           700\r\n");
			for (int j = 0; j < BPM_COUNT; j++)
			{
				DebugPort.print ("\t\t{ ");
					for (int k = 0; k < TIDAL_VOLUMES_COUNT; k++)
					{
						EEPROM.get(eeAddress, one_element.strk_len);
						eeAddress += sizeof(float);
						// DebugPort.print (ier_bpm_tv_2_strk_len[i][j][k].strk_len, 2);
						// DebugPort.print (one_element.strk_len, 2);
						
						if (one_element.word != 0xFFFFFFFF)	{
							// update stride length table for this element
							ier_bpm_tv_2_strk_len[i][j][k].strk_len = one_element.strk_len;
							DebugPort.print ("element["); DebugPort.print (i);
							DebugPort.print ("][");DebugPort.print (j);
							DebugPort.print ("][");DebugPort.print (k);
							DebugPort.print ("] = ");
							DebugPort.println (one_element.strk_len, 2);
						}
						else {
							// skip 	
						}
						
						if (k < (TIDAL_VOLUMES_COUNT - 1))	{
							DebugPort.print (", ");
						}
					}
				DebugPort.print (" }, // ");
				DebugPort.println (j + 10);
			}
		DebugPort.println ("\t},");
		DebugPort.println ();
	}	
}

// --------------------------------- EOF ------------------------------






