#include "service_mode.h"
void loop__ ()
{
     //Diagnostics_Mode();
}  

int Menu_Sel=0;

boolean continue_diag_mode = true;

void print_menu_common( menuIndex menuIdx)
{
  int numOfRowsToWrite=0;
  String strOnLine234 = ">";
  
  VENT_DEBUG_FUNC_START();
  
  VENT_DEBUG_INFO("Menu Index", menuIdx);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(menuItems[menuIdx].menuName);
  
  VENT_DEBUG_INFO("Menu Index", menuItems[menuIdx].menuName);

  numOfRowsToWrite = min(LCD_HEIGHT_CHAR-1, menuItems[menuIdx].menuLength);
  for (int i=0; i < numOfRowsToWrite; i++)//menuItems[menuIdx].menuLength; i++)
  {
    lcd.setCursor(0,i+1);
    if (seletIndicator == i+1)
    {
      strOnLine234 = ">";
    }
    else
    {
      strOnLine234 = " ";
    }
      strOnLine234 += menuItems[menuIdx].menu[scrollIndex + i];
      lcd.print (strOnLine234);
	  
	  VENT_DEBUG_INFO("Menu Index", strOnLine234);
  }
  lcd.setCursor(0,(CursorLine-scrollIndex)+1);
  
  VENT_DEBUG_FUNC_END();
}


void move_up()
{
 
  VENT_DEBUG_FUNC_START();
  
  if (menuItems[currentMenuIdx].menuLength <= LCD_HEIGHT_CHAR-1) //menu length starts with 1
  {
    scrollIndex = 0;
  }
  seletIndicator++;
  /*
   * check wrap around of select indicator
   */
  if ((seletIndicator > menuItems[currentMenuIdx].menuLength)||
      (seletIndicator > LCD_HEIGHT_CHAR-1))
  {
    seletIndicator=min(LCD_HEIGHT_CHAR-1,menuItems[currentMenuIdx].menuLength);
    if (seletIndicator == LCD_HEIGHT_CHAR-1)
    {
      scrollIndex++;
    }
    if ((scrollIndex + seletIndicator) > menuItems[currentMenuIdx].menuLength)
    {
      scrollIndex = menuItems[currentMenuIdx].menuLength - seletIndicator;
    }
  }
  print_menu_common (currentMenuIdx);
  
   VENT_DEBUG_FUNC_END();
}

void move_down()
{
  VENT_DEBUG_FUNC_START();
  
  if (menuItems[currentMenuIdx].menuLength <= LCD_HEIGHT_CHAR-1) //menu length starts with 1
  {
    scrollIndex = 0;
  }
  seletIndicator--;
  /*
   * check wrap around of select indicator
   */
  if (seletIndicator == 0 )
  {
    seletIndicator=1;
    if (scrollIndex>0)
    {
      scrollIndex--;
    }
  }
  print_menu_common (currentMenuIdx);
  
  VENT_DEBUG_FUNC_END();
}

void selection()
{
  VENT_DEBUG_FUNC_START();
  
  lcd.clear();
  lcd.setCursor(0,1);
  lcd.print("You selected:");
  lcd.setCursor(0,2);
  lcd.print(menuItems[currentMenuIdx].menu[seletIndicator + scrollIndex -1 ]);
  delay(2000);
  lcd.clear();

  if ((seletIndicator == 1) &&
      (scrollIndex == 0))
  {
    if (currentMenuIdx != mainMenuE)
    {
      currentMenuLevel = MENU_LEVEL_0; 
      currentMenuIdx = mainMenuE;
    }
    else
    {
      continue_diag_mode = false;
    }
  }
  else
  {
    currentMenuLevel++;
    
    if (currentMenuLevel >= MAX_LEVEL )
    {
      if (menuItems[currentMenuIdx].functionPtr != NULL)
      {
        (menuItems[currentMenuIdx].functionPtr)();
      }
    }
    currentMenuIdx = (menuIndex)(seletIndicator + scrollIndex - 1);
    if (currentMenuLevel >= MAX_LEVEL )
    {
      currentMenuLevel = MENU_LEVEL_0; 
      currentMenuIdx = mainMenuE;
    }
  }
  seletIndicator=1;
  scrollIndex=0;
  print_menu_common(currentMenuIdx);
  VENT_DEBUG_FUNC_END();
}



void Diagnostics_Mode(void)
{
  VENT_DEBUG_FUNC_START();
  
  VENT_DEBUG_INFO("Diagnostic Mode Selected", 0);
  
  while(continue_diag_mode)
  {
    RT_Events_T eRTState = RT_NONE;
    if (!Menu_Sel)
    {
      print_menu_common(mainMenuE);
      Encoder_Scan();
      Menu_Sel++;
    }
    eRTState = Encoder_Scan();
	VENT_DEBUG_INFO("Encoder Scan State", eRTState);
    switch(eRTState)
    {
      case RT_INC:
         move_up();
         break;
      case   RT_DEC:
         move_down();
         break;
      case   RT_BT_PRESS:
         selection();
         break;
	  case RT_NONE:
		break;
		
	  default:
		break;
    }
  }
  VENT_DEBUG_FUNC_END();
}

int initSelect = 0;
void move_up_init()
{
  lcd.setCursor(0,1);
  lcd.print(">>Edit parameters");
  lcd.setCursor(0,2);
  lcd.print("  Enter Diagnostics");
  initSelect = 1;
}

void move_down_init()
{
  lcd.setCursor(0,1);
  lcd.print("  Edit parameters");
  lcd.setCursor(0,2);
  lcd.print(">>Enter Diagnostics");
  initSelect = 2;
}

void selection_init(displayManager &dM)
{
  VENT_DEBUG_FUNC_START();
  
  if  (initSelect == 1)
  {
     dM.displayManagerSetup();
  }
  else if (initSelect == 2)
  {
    Diagnostics_Mode();
  }
  initSelect = 0;
  
  VENT_DEBUG_FUNC_END();
}
void drawSplashScreen(displayManager &dM) {
  boolean continueLoop = true;
  int wait = 1000;
  RT_Events_T eRTState = RT_NONE;
  
  VENT_DEBUG_FUNC_START();
  
 	
	lcd.setCursor(0,0);
	lcd.write(splashScreenTopBottomBuffer);
	lcd.setCursor(0,1);
	lcd.write(splashScreenMiddleBuffer);
	lcd.setCursor(0,2);
	lcd.write(splashScreenMiddleBuffer);
	lcd.setCursor(0,3);
	lcd.write(splashScreenTopBottomBuffer);
	lcd.setCursor(5,1);
	lcd.write("  Tworks  ");
	lcd.setCursor(5,2);
	lcd.write("Ventilator");
 
  delay(1500);

  lcd.setCursor(18,3);
	lcd.write("  ");
	lcd.setCursor(19,2);
	lcd.write(" ");
	
	lcd.setCursor(19,3);
	lcd.write((byte)0x6);

  encoderScanUnblocked();
  encoderScanUnblocked();
  encoderScanUnblocked();

 // delay(3000);
  while( wait>0 )
  {
    eRTState = encoderScanUnblocked();
    if (eRTState != RT_NONE)
    {
      break;
    }
    delay (10);
    wait--;
  }
  if (eRTState != RT_NONE)
  {
    while (continueLoop)
    {
      eRTState = Encoder_Scan();
	  VENT_DEBUG_INFO("Encoder Scan State", eRTState);
      switch(eRTState)
      {
        case RT_INC:
           move_up_init();
           break;
        case   RT_DEC:
           move_down_init();
           break;
        case   RT_BT_PRESS:
           //selection_init(dM);
           drawEditScreen5();      
           continueLoop = false;
           break;
		case RT_NONE:
			break;
		default:
			break;
      }
    }
  }
  
VENT_DEBUG_FUNC_END();
	
	return;
}
void drawEditScreen5(void){
	lcd.clear();
	lcd.setCursor(1,0);
	lcd.write(" O2 Calibration");
	
	lcd.setCursor(1,2);lcd.write("< 0% >");
	//lcd.setCursor(7,1);lcd.write("< 21%>");
	//lcd.setCursor(14,1);lcd.write("<100%>");
	
	//lcd.setCursor(7,1);lcd.write("21%");
	//lcd.setCursor(14,1);lcd.write("100%");
	
	lcd.setCursor(8,2);lcd.write("<Calibrate>");// Calibrate/<Current Value>/Reset
	
	lcd.setCursor(5,3);//pot 2
	lcd.write((byte)(0x3));
	lcd.setCursor(13,3);// pot 3
	lcd.write((byte)(0x3));
	
	lcd.setCursor(19,3);/// SAVE NEW CALIB VALUE
	lcd.write((byte)(0x6));
	
	delay( 2000 );
	lcd.setCursor(8,2);lcd.write("  <11.5mV>  ");// Calibrate/<Current Value>/Reset
	delay( 2000 );
	lcd.setCursor(8,2);lcd.write("  <Reset>   ");// Calibrate/<Current Value>/Reset
	delay( 2000 );

	return ;
	
}
void displayInitialScreen(displayManager &dM)
{
  boolean continueLoop = true;
  int wait = 299;
  RT_Events_T eRTState = RT_NONE;
  
  VENT_DEBUG_FUNC_START();
  
  encoderScanUnblocked();
  encoderScanUnblocked();
  encoderScanUnblocked();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Rotate encoder");
  lcd.setCursor(0,1);
  lcd.print("  Edit parameters");
  lcd.setCursor(0,2);
  lcd.print("  Enter Diagnostics");
  while( wait>0 )
  {
    eRTState = encoderScanUnblocked();
    if (eRTState != RT_NONE)
    {
      break;
    }
    lcd.setCursor(0,3);
    lcd.print(wait/100);
    delay (10);
    wait--;
  }
  if (eRTState != RT_NONE)
  {
    while (continueLoop)
    {
      eRTState = Encoder_Scan();
	  VENT_DEBUG_INFO("Encoder Scan State", eRTState);
      switch(eRTState)
      {
        case RT_INC:
           move_up_init();
           break;
        case   RT_DEC:
           move_down_init();
           break;
        case   RT_BT_PRESS:
           selection_init(dM);
           continueLoop = false;
           break;
		case RT_NONE:
			break;
		default:
			break;
      }
    }
  }
  
   VENT_DEBUG_FUNC_END();
}

