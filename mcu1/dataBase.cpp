// 
// 
// 

#include "dataBase.h"


// temp
// #define    NA  (77.7)
#define    NA  (101.77)



// const volatile float ier_bpm_tv_2_strk_len[3][21][11] = {
extern STRK_LEN_U	ier_bpm_tv_2_strk_len[IER_TABLE_COUNT][BPM_ROWS_COUNT][TIDAL_VOLUMES_COUNT] = {

	// IER - 1:1
	{   // Tidal volume 200 to 700 (11 entries in all) --->
		// 200         300         400         500         600         700
		{ 47.30, 57.20, 62.50, 67.40, 71.30, 76.60, 81.50, 87.60, 93.90, 102.20, 110.20  }, /*  |  */      /* 10 */   // tested
		{ 47.00, 56.72, 62.40, 67.10, 71.20, 76.50, 81.40, 87.40, 93.90, 102.10, 109.90  }, /*  |  */      /* 11 */
		{ 47.50, 56.24, 61.80, 66.80, 71.20, 76.40, 81.30, 87.20, 93.90, 101.90, 109.20  }, /*  |  */      /* 12 */
		{ 46.50, 55.76, 61.40, 66.50, 71.20, 76.30, 81.10, 87.10, 94.00, 101.70, 109.00  }, /*  V  */      /* 13 */
		{ 46.50, 55.28, 61.00, 66.20, 71.10, 76.20, 80.90, 87.00, 94.20, 101.60, 108.90  }, /*     */      /* 14 */
		{ 45.30, 54.80, 60.30, 65.90, 71.10, 76.10, 80.70, 86.90, 94.40, 101.60, 108.70  }, /*  B  */      /* 15 */   // tested
		{ 45.00, 54.58, 60.30, 65.90, 71.00, 75.80, 80.70, 86.90, 94.00, 101.40, 108.40  }, /*  P  */      /* 16 */
		{ 45.00, 54.36, 60.30, 65.90, 71.00, 75.60, 80.60, 86.90, 93.90, 101.20, 108.30  }, /*  M  */      /* 17 */
		{ 44.70, 54.35, 60.30, 65.80, 71.00, 75.40, 80.40, 86.80, 93.70, 100.90, 108.10  }, /*     */      /* 18 */
		{ 44.50, 54.28, 60.30, 65.80, 71.00, 75.20, 80.60, 86.80, 93.40, 100.50, 107.70  }, /*  10 */      /* 19 */
		{ 44.30, 54.20, 60.30, 65.80, 71.00, 75.20, 80.50, 86.70, 93.10, 100.60, 107.60  }, /*     */      /* 20 */   // tested
		{ 44.10, 54.30, 60.30, 65.70, 71.00, 75.20, 80.50, 86.70, 86.70, 100.70, 107.90  }, /*  to */      /* 21 */
		{ 44.90, 54.30, 60.30, 65.60, 70.90, 75.20, 80.50, 86.80, 86.90, 100.90, 108.00  }, /*     */      /* 22 */
		{ 44.70, 54.30, 60.30, 65.60, 70.80, 75.20, 80.50, 86.80, 86.10, 101.10, 108.30  }, /*  30 */      /* 23 */
		{ 44.50, 54.40, 60.30, 65.50, 70.70, 75.20, 80.50, 86.80, 86.30, 101.30, 108.70  }, /*     */      /* 24 */
		{ 44.20, 54.40, 60.20, 65.50, 70.70, 75.20, 80.50, 86.80, 94.70, 101.30, 109.00  }, /*     */      /* 25 */   // tested
		{ 44.30, 54.50, 60.20, 60.50, 70.60, 75.30, 80.80, 87.40, 94.50, 102.80, 108.70  }, /*     */      /* 26 */
		{ 44.50, 54.60, 60.30, 65.50, 70.60, 75.30, 81.10, 87.80, 95.00, 103.10, 108.10  }, /*     */      /* 27 */
		{ 44.60, 54.70, 60.30, 65.50, 70.60, 75.40, 81.40, 88.20, 96.50, 103.60, 107.70  }, /*     */      /* 28 */
		{ 44.70, 54.80, 60.40, 65.50, 70.50, 75.50, 81.70, 88.90, 97.00, 104.30, 107.00  }, /*     */      /* 29 */
		{ 44.70, 54.90, 60.40, 65.50, 70.50, 75.60, 81.90, 89.60, 98.50, 104.77, 106.77  }, /*     */      /* 30 */   // tested
	},
	
	
	// IER - 1:2
	{   // Tidal volume 200 to 700 (11 entries in all) --->
		// 200         300         400         500         600           700
		{ 49.90, 54.90, 60.50, 66.10, 70.60, 75.20, 80.50, 86.60, 93.60, 100.80, 109.10  }, /*  |  */       /* 10 */  // tested
		{ 49.80, 54.80, 60.50, 66.10, 70.60, 75.20, 80.50, 86.30, 93.60, 100.50, 108.00  }, /*  |  */       /* 11 */
		{ 49.70, 54.80, 60.40, 66.00, 70.60, 75.20, 80.40, 85.90, 92.90, 100.40, 107.10  }, /*  |  */       /* 12 */
		{ 49.60, 54.70, 60.40, 65.80, 70.60, 75.20, 80.40, 85.80, 92.40, 100.10, 106.10  }, /*  V  */       /* 13 */
		{ 49.50, 54.70, 60.40, 65.80, 70.60, 75.20, 80.30, 85.80, 92.40, 99.90,  105.30  }, /*     */       /* 14 */
		{ 49.30, 54.70, 60.30, 65.50, 70.50, 75.20, 80.30, 85.80, 92.10, 99.10,  105.10  }, /*  B  */       /* 15 */  // tested
		{ 49.00, 54.70, 60.50, 65.50, 70.50, 75.30, 81.20, 85.60, 92.10, 99.10,  108.60  }, /*  P  */       /* 16 */
		{ 49.80, 54.70, 60.50, 65.50, 70.50, 75.40, 81.20, 85.40, 92.10, 99.10,  108.80  }, /*  M  */       /* 17 */
		{ 49.50, 54.70, 60.50, 65.50, 70.50, 75.30, 81.10, 85.40, 92.10, 99.10,  108.00  }, /*     */       /* 18 */
		{ 49.50, 54.70, 60.50, 65.50, 70.50, 75.20, 80.10, 85.20, 92.00, 98.90,  108.20  }, /*  10 */       /* 19 */
		{ 49.30, 54.70, 60.50, 65.50, 70.50, 75.10, 80.10, 85.50, 92.00, 98.70,  106.60  }, /*     */       /* 20 */  // tested
		{ 49.00, 54.80, 60.60, 65.50, 70.50, 75.10, 80.10, 85.80, 92.30, 98.70,  101.90  }, /*  to */       /* 21 */
		{ 49.80, 54.80, 60.70, 65.50, 70.40, 75.10, 80.10, 85.60, 92.50, 98.90,  102.40  }, /*     */       /* 22 */
		{ 49.80, 54.90, 60.70, 65.70, 70.50, 75.10, 80.10, 86.00, 92.70, 99.10,  102.90  }, /*  30 */       /* 23 */
		{ 49.80, 55.00, 60.70, 65.90, 70.40, 75.10, 80.20, 86.00, 92.90, 99.30,  103.40  }, /*     */       /* 24 */
		{ 49.00, 55.00, 60.70, 66.10, 70.50, 75.10, 80.20, 86.10, 93.00, 101.00, 105.77  }, /*     */       /* 25 */  // tested
		{ 49.00, 55.00, 60.80, 66.50, 70.50, 75.30, 80.40, 86.90, 94.00, 101.60, 101.77  }, /*     */       /* 26 */
		{ 49.80, 55.00, 60.80, 66.50, 70.50, 75.50, 80.70, 87.30, 94.40, 101.60, 101.77  }, /*     */       /* 27 */
		{ 48.80, 55.00, 60.80, 65.50, 70.50, 75.70, 81.00, 88.70, 95.40, 101.60, 101.77  }, /*     */       /* 28 */
		{ 48.80, 55.10, 60.80, 65.70, 70.50, 75.90, 81.30, 88.10, 95.60, 101.60, 101.77  }, /*     */       /* 29 */
		{ 48.90, 55.10, 60.90, 65.60, 70.50, 76.10, 81.90, 88.90, 95.60, 101.60, 101.77  }, /*     */       /* 30 */  // tested
	},
	
	
	// IER - 1:3
	{   // Tidal volume 200 to 700 (11 entries in all) --->
		// 200         300         400         500         600         700
		{ 45.10, 54.70, 60.90, 65.60, 70.70, 75.40, 80.90, 86.90,   93.40, 99.80,  107.00  }, /*  |  */       /* 10 */
		{ 45.40, 54.70, 60.70, 65.50, 67.70, 75.40, 80.90, 86.80,   93.70, 99.48,  106.82  }, /*  |  */       /* 11 */
		{ 45.70, 54.70, 60.50, 65.50, 68.00, 75.30, 80.70, 86.60,   92.00, 99.36,  106.24  }, /*  |  */       /* 12 */
		{ 46.00, 54.70, 60.40, 65.50, 68.30, 75.30, 80.50, 86.40,   92.30, 99.25,  106.00  }, /*  V  */       /* 13 */
		{ 46.20, 54.70, 60.30, 65.50, 68.60, 75.20, 80.30, 86.20,   92.60, 99.12,  106.08  }, /*     */       /* 14 */
		{ 46.50, 54.80, 60.20, 65.50, 70.60, 75.10, 80.10, 85.90,   91.90, 99.00,  106.80  }, /*  B  */       /* 15 */
		{ 46.50, 54.80, 60.20, 65.60, 69.20, 75.10, 80.10, 86.10,   91.20, 99.00,  101.00  }, /*  P  */       /* 16 */
		{ 46.50, 54.80, 60.40, 65.60, 69.20, 75.20, 80.20, 86.30,   92.70, 99.40,  101.40  }, /*  M  */       /* 17 */
		{ 46.40, 54.80, 60.50, 65.70, 69.20, 75.30, 80.30, 86.50,   92.20, 99.80,  101.80  }, /*     */       /* 18 */
		{ 46.40, 54.80, 60.60, 65.80, 69.20, 75.20, 80.30, 86.60,   93.70, 100.20, 101.77  }, /*  10 */       /* 19 */
		{ 46.40, 54.80, 60.60, 65.90, 70.40, 75.20, 80.30, 86.60,   93.90, 100.50, 104.10  }, /*     */       /* 20 */
		{ 46.40, 54.90, 60.70, 66.00, 66.50, 75.80, 80.60, 86.80,   93.90, 101.00, 101.77  }, /*  to */       /* 21 */
		{ 46.40, 54.90, 60.80, 62.10, 66.70, 76.10, 81.20, 87.00,   93.90, 101.77, 101.77  }, /*     */       /* 22 */
		{ 46.40, 55.00, 60.80, 66.00, 66.80, 76.50, 81.90, 87.50,   94.50, 101.77, 101.77  }, /*  30 */       /* 23 */
		{ 46.40, 55.10, 60.80, 66.10, 66.80, 76.90, 82.60, 88.40,   94.90, 101.77, 101.77  }, /*     */       /* 24 */
		{ 46.40, 55.20, 60.90, 66.20, 71.40, 77.30, 83.20, 89.80,   95.40, 101.77, 101.77  }, /*     */       /* 25 */
		{ 46.40, 55.10, 61.00, 66.50, 67.10, 77.70, 83.20, 93.70,  101.77, 101.77, 101.77  }, /*     */       /* 26 */
		{ 46.40, 55.10, 61.00, 66.80, 67.30, 77.90, 83.60, 97.90,  101.77, 101.77, 101.77  }, /*     */       /* 27 */
		{ 46.30, 55.20, 61.10, 66.80, 67.50, 78.30, 84.40, 101.77, 101.77, 101.77, 101.77  }, /*     */       /* 28 */
		{ 46.30, 55.30, 61.20, 67.00, 67.70, 78.80, 84.80, 101.77, 101.77, 101.77, 101.77  }, /*     */       /* 29 */
		{ 46.20, 55.10, 61.30, 67.20, 73.20, 79.00, 85.50, 101.77, 101.77, 101.77, 101.77  }, /*     */       /* 30 */
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
			
				
void update_stroke_length_in_eeprom (float length)   {

	int			index1, index2, index3;
	int			eepAddress;
	STRK_LEN_U	save_element;

	index1 = get_index_from_IER (IER_new);
	index2 = get_index_from_RR  (BPM_new);
	index3 = get_index_from_TV  (tidal_volume_new);

	// 	update in system variable
	ier_bpm_tv_2_strk_len[index1][index2][index3].strk_len = length;		
	// update in eeprom as well..
	eepAddress = index1 * IER_TABLE_SIZE + index2 * BPM_ROW_SIZE + index3 * ELEMENT_SIZE;
	save_element.strk_len = length;
	
	char *c_ptr = (char*)&save_element.word;
	for (int i = 0; i < (sizeof(float)); i++)
	{
		EEPROM.update(eepAddress, *c_ptr);
		eepAddress++;
		c_ptr++;
		
	}
	
	// -------------------------------------------------------------

	DebugPort.print("index1 : ");
	DebugPort.print(index1);
	DebugPort.print(", IER_TABLE_SIZE : ");
	DebugPort.print(IER_TABLE_SIZE);
	DebugPort.print(", index1 * IER_TABLE_SIZE : ");
	DebugPort.println(index1 * IER_TABLE_SIZE);

	DebugPort.print("index2 : ");
	DebugPort.print(index2);
	DebugPort.print(", BPM_ROW_SIZE : ");
	DebugPort.print(BPM_ROW_SIZE);
	DebugPort.print(", index2 * BPM_ROW_SIZE : ");
	DebugPort.println(index2 * BPM_ROW_SIZE);
	
	DebugPort.print("index3 : ");
	DebugPort.print(index3);
	DebugPort.print(", ELEMENT_SIZE : ");
	DebugPort.print(ELEMENT_SIZE);
	DebugPort.print(", index3 * ELEMENT_SIZE : ");
	DebugPort.println(index3 * ELEMENT_SIZE);
	
	DebugPort.print("eepAddress : ");
	DebugPort.println(eepAddress);
		
	DebugPort.print("  index2 : ");
	DebugPort.print(index2);
	DebugPort.print("  index3 : ");
	DebugPort.print(index3);
	DebugPort.print("  SL_new : ");
	DebugPort.println(Stroke_length_new);
	DebugPort.print("  eepAddress : ");
	DebugPort.println(eepAddress);


	DebugPort.print("index1 : ");
	DebugPort.print(index1);
	DebugPort.print("  index2 : ");
	DebugPort.print(index2);
	DebugPort.print("  index3 : ");
	DebugPort.print(index3);
	DebugPort.print("  SL_new : ");
	DebugPort.println(Stroke_length_new);
	DebugPort.print("  eepAddress : ");
	DebugPort.println(eepAddress);	
	
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
		for (int j = 0; j < BPM_ROWS_COUNT; j++)
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
			for (int j = 0; j < BPM_ROWS_COUNT; j++)
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
		for (int j = 0; j < BPM_ROWS_COUNT; j++)
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
// 					DebugPort.print ("element["); DebugPort.print (i);
// 					DebugPort.print ("][");DebugPort.print (j);
// 					DebugPort.print ("][");DebugPort.print (k);
// 					DebugPort.print ("] = ");
					DebugPort.print (one_element.strk_len, 2);
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






