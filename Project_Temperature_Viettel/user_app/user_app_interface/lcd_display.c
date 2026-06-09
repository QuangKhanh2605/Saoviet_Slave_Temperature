#include "lcd_display.h"
#include "string.h"
#include "user_com_seg.h"
#include "user_define.h"
#include "lcd.h"
#include "user_app_modbus_rtu.h"


static uint8_t _Cb_Display_Init (uint8_t event);
static uint8_t _Cb_Display_Show (uint8_t event);
static uint8_t _Cb_button_scan (uint8_t event);
static uint8_t _Cb_button_detect (uint8_t event);
static uint8_t _Cb_Display_Auto_SW (uint8_t event);
static uint8_t _Cb_Display_Logo (uint8_t event);
static uint8_t _Cb_Off_Speaker(uint8_t event);
extern sData   sFirmVersion;


sEvent_struct sEventDisplay [] =
{
    { _EVENT_DISP_INIT, 		    1, 5, 1500, 	_Cb_Display_Init }, 
    { _EVENT_DISP_LOGO, 		    0, 0, 2000, 	_Cb_Display_Logo}, 
    { _EVENT_DISP_SHOW, 		    0, 0, 200,      _Cb_Display_Show }, 
    { _EVENT_DISP_AUTO_SW, 		    0, 0, 5000,     _Cb_Display_Auto_SW }, 
    
    { _EVENT_BUTTON_SCAN, 		    0, 0, 2,    	_Cb_button_scan  },
    { _EVENT_BUTTON_DETECTTED, 	    0, 0, 10, 		_Cb_button_detect },
    { _EVENT_OFF_SPEAKER,           0, 5, 5,        _Cb_Off_Speaker},
};

sLCDinformation      sLCD;

sParameter_Display   sParaDisplay = {
    .Scale_Temp = 0xFE,
    .Scale_Humi = 0xFF,
};

Struct_Index_LCD     sIndexLCD = {
    .Index_LCD_P = 1,
    .Index_LCD_P1 = 1,
    .Index_LCD_P2 = 1,
    .Index_LCD_P3 = 1,
};

sData   sModelVersion = {(uint8_t *) "SV_ENVI_WM_NH4", 14}; 

uint8_t aPASSWORD[4] = {"1111"};

uint8_t aSTT_SETTING_FREE[14]   = {"              "};
uint8_t aSTT_SETTING_ENTER[14]  = {"Enter to Setup"};
uint8_t aSTT_SETTING_WAIT[14]   = {"    Waiting   "};
uint8_t aSTT_SETTING_DONE[14]   = {"     Done    "};
uint8_t aSTT_SETTING_ERROR[14]  = {"     Error   "};

uint8_t *aModeSelectONOFF[2] = {"OFF", "ON"};
char aDisconnect[4] = {"    "};

sOjectInformation  sLCDObject[] = 
{
//          para          name                  value      dtype         scale   unit      row  col      screen
    {   __SC_TEMP,        NULL,             NULL,   _DTYPE_I16,          0xFE,    NULL,    0,  0,   0x00,    _LCD_SCREEN},
    {   __SC_HUMI,        NULL,             NULL,   _DTYPE_I16,          0xFF,    NULL,    1,  0,   0x00,    _LCD_SCREEN},
    
    {   __PASSWORD_TITLE, "PASS",           NULL,   _DTYPE_STRING,       0,       NULL,    0,  0,   0x00,    _LCD_SCR_PASS_WORD},
    {   __PASSWORD_1,     NULL,             NULL,   _DTYPE_STRING,       0,       NULL,    1,  0,   0x00,    _LCD_SCR_PASS_WORD},
    
    {   __SETTING_P,      "P",              NULL,   _DTYPE_U8,          0,       NULL,    0,  0,   0x00,    _LCD_SCR_SETTING_P},
    
    {   __SETTING_P1,     "P1 ",            NULL,   _DTYPE_U8,          0,       NULL,    0,  0,   0x00,    _LCD_SCR_SETTING_P1},
    {   __SETTING_P2,     "P2 ",            NULL,   _DTYPE_U8,          0,       NULL,    0,  0,   0x00,    _LCD_SCR_SETTING_P2},
    {   __SETTING_P3,     "P3 ",            NULL,   _DTYPE_U8,          0,       NULL,    0,  0,   0x00,    _LCD_SCR_SETTING_P3},
    
    {   __SETTING_P1_1_T,   "P1 1",         NULL,   _DTYPE_STRING,       0,       NULL,    0,  0,   0x00,    _LCD_SCR_SETTING_P1_1},
    {   __SETTING_P1_1_S,   NULL,           NULL,   _DTYPE_U16,          0,       NULL,    1,  0,   0x00,    _LCD_SCR_SETTING_P1_1},
    
    {   __SETTING_P1_2_T,   "P1 2",         NULL,   _DTYPE_STRING,       0,       NULL,    0,  0,   0x00,    _LCD_SCR_SETTING_P1_2},
    {   __SETTING_P1_2_S,   NULL,           NULL,   _DTYPE_U16,          0,       NULL,    1,  0,   0x00,    _LCD_SCR_SETTING_P1_2},

    {   __SETTING_P2_1_T,   "P2 1",         NULL,   _DTYPE_STRING,       0,       NULL,    0,  0,   0x00,    _LCD_SCR_SETTING_P2_1},
    {   __SETTING_P2_1_S,   NULL,           NULL,   _DTYPE_I16,          0,       NULL,    1,  0,   0x00,    _LCD_SCR_SETTING_P2_1},
    
    {   __SETTING_P2_2_T,   "P2 2",         NULL,   _DTYPE_STRING,       0,       NULL,    0,  0,   0x00,    _LCD_SCR_SETTING_P2_2},
    {   __SETTING_P2_2_S,   NULL,           NULL,   _DTYPE_I16,          0,       NULL,    1,  0,   0x00,    _LCD_SCR_SETTING_P2_2},
    
    {   __SETTING_P3_1_T,   "P3 1",         NULL,   _DTYPE_STRING,       0,       NULL,    0,  0,   0x00,    _LCD_SCR_SETTING_P3_1},
    {   __SETTING_P3_1_S,   NULL,           NULL,   _DTYPE_U16,          0,       NULL,    1,  0,   0x00,    _LCD_SCR_SETTING_P3_1},
    
    {   __SETTING_P3_2_T,   "P3 2",         NULL,   _DTYPE_STRING,       0,       NULL,    0,  0,   0x00,    _LCD_SCR_SETTING_P3_2},
    {   __SETTING_P3_2_S,   NULL,           NULL,   _DTYPE_U16,          0,       NULL,    1,  0,   0x00,    _LCD_SCR_SETTING_P3_2},
    
    {   __SETTING_P3_3_T,   "P3 3",         NULL,   _DTYPE_STRING,       0,       NULL,    0,  0,   0x00,    _LCD_SCR_SETTING_P3_3},
    {   __SETTING_P3_3_S,   NULL,           NULL,   _DTYPE_U16,          0,       NULL,    1,  0,   0x00,    _LCD_SCR_SETTING_P3_3},
    
    {   __SETTING_P3_4_T,   "P3 4",         NULL,   _DTYPE_STRING,       0,       NULL,    0,  0,   0x00,    _LCD_SCR_SETTING_P3_4},
    {   __SETTING_P3_4_S,   NULL,           NULL,   _DTYPE_U16,          0,       NULL,    1,  0,   0x00,    _LCD_SCR_SETTING_P3_4},
    
    {   __SETTING_P3_5_T,   "P3 5",         NULL,   _DTYPE_STRING,       0,       NULL,    0,  0,   0x00,    _LCD_SCR_SETTING_P3_5},
    {   __SETTING_P3_5_S,   NULL,           NULL,   _DTYPE_U16,          0,       NULL,    1,  0,   0x00,    _LCD_SCR_SETTING_P3_5},
    
    {   __SETTING_P3_6_T,   "P3 6",         NULL,   _DTYPE_STRING,       0,       NULL,    0,  0,   0x00,    _LCD_SCR_SETTING_P3_6},
    {   __SETTING_P3_6_S,   NULL,           NULL,   _DTYPE_U16,          0,       NULL,    1,  0,   0x00,    _LCD_SCR_SETTING_P3_6},
    
//    {   __SC1_TITLE,      "NH4+.",          NULL,   _DTYPE_STRING,        0,    NULL,      0,  0,  0x00,      _LCD_SCREEN_1  },
//    {   __SC1_PH_ATC,     NULL,             NULL,   _DTYPE_I16,      0,    "mg/L",      4,  0,  0x00,      _LCD_SCREEN_1  },
//    {   __SC1_TEMP,       "Temp  : ",       NULL,   _DTYPE_I16,      0,    " ‰C",      7,  0,  0x00,      _LCD_SCREEN_1  },
//  
//    {   __PASS_WORD_TITLE,  "Loggin",         NULL,   _DTYPE_STRING,   0,      NULL,     0,   0, 0x00,      _LCD_SCR_PASS    },
//    {   __PASS_WORD_1,    "Enter Password",   NULL,   _DTYPE_STRING,   0,      NULL,      2,  24, 0x00,      _LCD_SCR_PASS    },
//    {   __PASS_WORD_2,    NULL,               NULL,   _DTYPE_STRING,   0,      NULL,      3,  48, 0x00,      _LCD_SCR_PASS    },
//    
//    {   __SCR_SET_TITLE,  "SETTING",          NULL,   _DTYPE_STRING,   0,      NULL,      0,   0, 0x00,      _LCD_SCR_SETTING },
//    {   __SCR_SET_MODBUS, "1.Modbus RTU",     NULL,   _DTYPE_STRING,   0,      NULL,      2,   18, 0x00,      _LCD_SCR_SETTING },
//    {   __SCR_SET_CALIB,  "2.Calib",          NULL,   _DTYPE_STRING,   0,      NULL,      3,   18, 0x00,      _LCD_SCR_SETTING },
//    {   __SCR_SET_OFFSET, "3.Offset",         NULL,   _DTYPE_STRING,   0,      NULL,      4,   18, 0x00,      _LCD_SCR_SETTING },
//    {   __SCR_SET_ALARM,  "4.Warning",        NULL,   _DTYPE_STRING,   0,      NULL,      5,   18, 0x00,      _LCD_SCR_SETTING },
//    {   __SCR_SET_RANGE,  "5.Range",          NULL,   _DTYPE_STRING,   0,      NULL,      6,   18, 0x00,      _LCD_SCR_SETTING },
//    {   __SCR_SET_INFOR,  "6.Information",    NULL,   _DTYPE_STRING,   0,      NULL,      7,   18, 0x00,      _LCD_SCR_SETTING },
//    
//    {   __SET_MODBUS_TITLE,     "SET MODBUS RTU", NULL,   _DTYPE_STRING,   0,      NULL,      0,   0, 0x00,   _LCD_SCR_SET_MODBUS },
//    {   __SET_MODBUS_ID,        "1.ID      : ",   NULL,   _DTYPE_U8,       0x00,   NULL,      2,   4, 0x00,   _LCD_SCR_SET_MODBUS },
//    {   __SET_MODBUS_BR,        "2.Baudrate: ",   NULL,   _DTYPE_U32,      0x00,   NULL,      3,   4, 0x00,   _LCD_SCR_SET_MODBUS },
//    
//    {   __SET_MODE_CALIB_TITLE, "CALIB SENSOR",     NULL,   _DTYPE_STRING,   0x00,  NULL,       0,  0,  0x00,    _LCD_SCR_SET_MODE_CALIB},
//    {   __SET_MODE_CALIB_NH4,   "1.Calib NH4+",     NULL,   _DTYPE_STRING,   0x00,  NULL,       2,  0,  0x00,    _LCD_SCR_SET_MODE_CALIB},
//    {   __SET_MODE_CALIB_PH,    "2.Calib pH",       NULL,   _DTYPE_STRING,   0x00,  NULL,       3,  0,  0x00,    _LCD_SCR_SET_MODE_CALIB},
//    {   __SET_MODE_CALIB_K,     "3.Calib K+",       NULL,   _DTYPE_STRING,   0x00,  NULL,       4,  0,  0x00,    _LCD_SCR_SET_MODE_CALIB},
//    
//    {   __SET_CALIB_NH4_TITLE,  "CALIB NH4+",      NULL,   _DTYPE_STRING,   0x00,  NULL,       0,  0,  0x00,    _LCD_SCR_CALIB_NH4},
//    {   __SET_CALIB_NH4_VALUE,  "NH4: ",           NULL,   _DTYPE_I32,      0x00,  NULL,       2,  0,  0x00,    _LCD_SCR_CALIB_NH4},
//    {   __SET_CALIB_NH4_CONST_K,"K: ",             NULL,   _DTYPE_I32,      0x00,  NULL,       3,  0,  0x00,    _LCD_SCR_CALIB_NH4},
//    {   __SET_CALIB_NH4_CONST_B,"B: ",             NULL,   _DTYPE_I32,      0x00,  NULL,       3,  70, 0x00,    _LCD_SCR_CALIB_NH4},
//    {   __SET_CALIB_NH4_RESET,  "1.Reset",         NULL,   _DTYPE_STRING,   0x00,  NULL,       4,  0,  0x00,    _LCD_SCR_CALIB_NH4},
//    {   __SET_CALIB_NH4_P1,     "2.NH4+ P1: ",     NULL,   _DTYPE_I32,      0x00,  " mg/L",    5,  0,  0x00,    _LCD_SCR_CALIB_NH4},
//    {   __SET_CALIB_NH4_P2,     "3.NH4+ P2: ",     NULL,   _DTYPE_I32,      0x00,  " mg/L",    6,  0,  0x00,    _LCD_SCR_CALIB_NH4},
//    {   __SET_CALIB_NH4_CONFIRM,"4.Confirm",       NULL,   _DTYPE_STRING,   0x00,  NULL,       7,  0,  0x00,    _LCD_SCR_CALIB_NH4},
//    
//    {   __SET_CALIB_PH_TITLE,   "CALIB PH",        NULL,   _DTYPE_STRING,   0x00,  NULL,       0,  0,  0x00,    _LCD_SCR_CALIB_PH},
//    {   __SET_CALIB_PH_VALUE,   "pH: ",            NULL,   _DTYPE_I32,      0x00,  NULL,       2,  0,  0x00,    _LCD_SCR_CALIB_PH},
//    {   __SET_CALIB_PH_CONST_K, "K: ",             NULL,   _DTYPE_I32,      0x00,  NULL,       3,  0,  0x00,    _LCD_SCR_CALIB_PH},
//    {   __SET_CALIB_PH_CONST_B, "B: ",             NULL,   _DTYPE_I32,      0x00,  NULL,       3,  70, 0x00,    _LCD_SCR_CALIB_PH},
//    {   __SET_CALIB_PH_RESET,   "1.Reset",         NULL,   _DTYPE_STRING,   0x00,  NULL,       4,  0,  0x00,    _LCD_SCR_CALIB_PH},
//    {   __SET_CALIB_PH_P1,      "2.PH P1: ",       NULL,   _DTYPE_I32,      0x00,  NULL,       5,  0,  0x00,    _LCD_SCR_CALIB_PH},
//    {   __SET_CALIB_PH_P2,      "3.PH P2: ",       NULL,   _DTYPE_I32,      0x00,  NULL,       6,  0,  0x00,    _LCD_SCR_CALIB_PH},
//    {   __SET_CALIB_PH_CONFIRM, "4.Confirm",       NULL,   _DTYPE_STRING,   0x00,  NULL,       7,  0,  0x00,    _LCD_SCR_CALIB_PH},
//    
//    {   __SET_CALIB_K_TITLE,   "CALIB K+",        NULL,   _DTYPE_STRING,   0x00,  NULL,       0,  0,  0x00,    _LCD_SCR_CALIB_K},
//    {   __SET_CALIB_K_VALUE,   "K+: ",            NULL,   _DTYPE_I32,      0x00,  NULL,       2,  0,  0x00,    _LCD_SCR_CALIB_K},
//    {   __SET_CALIB_K_CONST_K, "K: ",             NULL,   _DTYPE_I32,      0x00,  NULL,       3,  0,  0x00,    _LCD_SCR_CALIB_K},
//    {   __SET_CALIB_K_CONST_B, "B: ",             NULL,   _DTYPE_I32,      0x00,  NULL,       3,  70, 0x00,    _LCD_SCR_CALIB_K},
//    {   __SET_CALIB_K_RESET,   "1.Reset",         NULL,   _DTYPE_STRING,   0x00,  NULL,       4,  0,  0x00,    _LCD_SCR_CALIB_K},
//    {   __SET_CALIB_K_P1,      "2.K+ P1: ",       NULL,   _DTYPE_I32,      0x00,  " mg/L",    5,  0,  0x00,    _LCD_SCR_CALIB_K},
//    {   __SET_CALIB_K_P2,      "3.K+ P2: ",       NULL,   _DTYPE_I32,      0x00,  " mg/L",    6,  0,  0x00,    _LCD_SCR_CALIB_K},
//    {   __SET_CALIB_K_CONFIRM, "4.Confirm",       NULL,   _DTYPE_STRING,   0x00,  NULL,       7,  0,  0x00,    _LCD_SCR_CALIB_K},
//    
//    {   __SET_OFFSET_TITLE, "OFFSET",           NULL,   _DTYPE_STRING,  0,      NULL,       0,  0, 0x00,     _LCD_SCR_SET_OFFSET},
//    {   __SET_OFFSET_PH,    "1.pH   : ",       NULL,   _DTYPE_I32,     0,   " pH",       2,  0, 0x00,    _LCD_SCR_SET_OFFSET},
//    {   __SET_OFFSET_TEMP,  "2.Temp : ",       NULL,   _DTYPE_I32,     0,   " ‰C",       3,  0, 0x00,    _LCD_SCR_SET_OFFSET},
//    
//    {   __SET_ALARM_TITLE,      "WARNING PH",      NULL,    _DTYPE_STRING,  0,      NULL,        0,  0, 0x00,    _LCD_SCR_SET_ALARM},
//    {   __SET_ALARM_STATE,      "1.Mode : ",       NULL,    _DTYPE_STRING,  0,      NULL,        2,  0, 0x00,    _LCD_SCR_SET_ALARM},
//    {   __SET_ALARM_UPPER,      "2.Upper: ",       NULL,    _DTYPE_I32,     0,      " pH",    3,  0, 0x00,    _LCD_SCR_SET_ALARM},
//    {   __SET_ALARM_LOWER,      "3.Lower: ",       NULL,    _DTYPE_I32,     0,      " pH",    4,  0, 0x00,    _LCD_SCR_SET_ALARM},
//    
//    {   __SET_RANGE_TITLE,      "RANGE SETTING",    NULL,   _DTYPE_STRING,  0,      NULL,        0,  0, 0x00,       _LCD_SCR_SET_RANGE},
//    {   __SET_RANGE_U_KEY,        "1.U_pH  : ",      NULL,   _DTYPE_I32,  0,      NULL,        2,  0, 0x00,    _LCD_SCR_SET_RANGE},
//    {   __SET_RANGE_L_KEY,        "2.L_pH  : ",      NULL,   _DTYPE_I32,  0,      NULL,        3,  0, 0x00,    _LCD_SCR_SET_RANGE},
//    {   __SET_RANGE_U_TEMP,       "3.U_Temp: ",     NULL,   _DTYPE_I32,  0,      " ‰C",        4,  0, 0x00,    _LCD_SCR_SET_RANGE},
//    {   __SET_RANGE_L_TEMP,       "4.L_Temp: ",     NULL,   _DTYPE_I32,  0,      " ‰C",        5,  0, 0x00,    _LCD_SCR_SET_RANGE},
//    
//    {   __SCR_INFOR_TITLE,          "Infor.",   NULL,   _DTYPE_STRING,   0,      NULL,      0,   0, 0x00,    _LCD_SCR_SET_INFORMATION },
//    {   __SCR_INFOR_FW_VERSION_1,   "*Version", NULL,   _DTYPE_STRING,   0,      NULL,      2,   28, 0x00,   _LCD_SCR_SET_INFORMATION },
//    {   __SCR_INFOR_FW_VERSION_2,   NULL,       NULL,   _DTYPE_STRING,   0,      NULL,      3,    4, 0x00,   _LCD_SCR_SET_INFORMATION },
//    
//    {   __SCR_INFOR_MODEL_1,          "*Model",         NULL,   _DTYPE_STRING,   0,      NULL,      4,   28, 0x00,      _LCD_SCR_SET_INFORMATION },
//    {   __SCR_INFOR_MODEL_2,                NULL,       NULL,   _DTYPE_STRING,   0,      NULL,      5,    4, 0x00,       _LCD_SCR_SET_INFORMATION },
//    
//    {   __CHECK_STATE_SETTING,        NULL,             NULL,   _DTYPE_STRING,   0,      NULL,      3,  24, 0x00,     _LCD_SCR_CHECK_SETTING},
//    
//    {   __SET_RESTORE_1,    "Successfully!",    NULL,   _DTYPE_STRING,   0,      NULL,      3,  24, 0x02,    _LCD_SCR_SET_RESTORE},
};

static char charNotDetectPress = '-';

/*=========================Function=========================*/
void Display_Init (void)
{
  
    sLCDObject[__SC_TEMP].pData       = &sParaDisplay.Temp_Filter_i32;   
    sLCDObject[__SC_TEMP].Scale_u8    = sParaDisplay.Scale_Temp;
    sLCDObject[__SC_HUMI].pData       = &sParaDisplay.Humi_Filter_i32;    
    sLCDObject[__SC_HUMI].Scale_u8    = sParaDisplay.Scale_Humi;
    
    sLCDObject[__SETTING_P].pData  = &sIndexLCD.Index_LCD_P;
    
    sLCDObject[__SETTING_P1].pData  = &sIndexLCD.Index_LCD_P1;
    
    sLCDObject[__SETTING_P2].pData  = &sIndexLCD.Index_LCD_P2;
    
    sLCDObject[__SETTING_P3].pData  = &sIndexLCD.Index_LCD_P3;
    
    sLCDObject[__SETTING_P1_1_S].pData  = &sSlave_ModbusRTU.ID;
    sLCDObject[__SETTING_P1_2_S].pData  = &sSlave_ModbusRTU.Baudrate;
    
    sLCDObject[__SETTING_P2_1_S].pData       = &sParaDisplay.Temp_Offset_i32;   
    sLCDObject[__SETTING_P2_1_S].Scale_u8    = sParaDisplay.Scale_Temp;
    sLCDObject[__SETTING_P2_2_S].pData       = &sParaDisplay.Humi_Offset_i32;    
    sLCDObject[__SETTING_P2_2_S].Scale_u8    = sParaDisplay.Scale_Humi;
    
    sLCDObject[__SETTING_P3_1_S].pData       = &sAlarm.Mode_Temp_u8;
    sLCDObject[__SETTING_P3_2_S].pData       = &sParaDisplay.Temp_Lower_i32;   
    sLCDObject[__SETTING_P3_2_S].Scale_u8    = sParaDisplay.Scale_Temp;
    sLCDObject[__SETTING_P3_3_S].pData       = &sParaDisplay.Temp_Upper_i32;    
    sLCDObject[__SETTING_P3_3_S].Scale_u8    = sParaDisplay.Scale_Temp;
    
    sLCDObject[__SETTING_P3_4_S].pData       = &sAlarm.Mode_Humi_u8;
    sLCDObject[__SETTING_P3_5_S].pData       = &sParaDisplay.Humi_Lower_i32;   
    sLCDObject[__SETTING_P3_5_S].Scale_u8    = sParaDisplay.Scale_Humi;
    sLCDObject[__SETTING_P3_6_S].pData       = &sParaDisplay.Humi_Upper_i32;    
    sLCDObject[__SETTING_P3_6_S].Scale_u8    = sParaDisplay.Scale_Humi;
  
//  
////    HAL_GPIO_WritePin (LCD_ON_OFF_GPIO_Port, LCD_ON_OFF_Pin, GPIO_PIN_SET);   
//    HAL_GPIO_WritePin (LCD_RW_GPIO_Port, LCD_RW_Pin, GPIO_PIN_RESET);
//    HAL_GPIO_WritePin (LCD_C86_GPIO_Port, LCD_C86_Pin, GPIO_PIN_RESET); 
//
//    glcd_init();
////    glcd_clear_buffer();
////    glcd_write();	
////    Deinit_LCD12864();
//    
//    sParaDisplay.aAlarm_State_u8 = &sTempAlarm.State;
//    
//    sLCDObject[__SET_MODBUS_ID].pData  = &sParaDisplay.ID_u8; 
//    sLCDObject[__SET_MODBUS_BR].pData  = &sParaDisplay.Baudrate_u32; 
//    
//    sLCDObject[__SC1_PH_ATC].pData     = &sParaDisplay.NH4_Filter_i32;   
//    sLCDObject[__SC1_PH_ATC].Scale_u8  = sParaDisplay.Scale_NH4;
//    sLCDObject[__SC1_TEMP].pData       = &sParaDisplay.Temp_Filter_i32;    
//    sLCDObject[__SC1_TEMP].Scale_u8    = sParaDisplay.Scale_Temp;
//    
//    sLCDObject[__SET_OFFSET_PH].pData       = &sParaDisplay.pH_Offset_i32; 
//    sLCDObject[__SET_OFFSET_PH].Scale_u8    = sParaDisplay.Scale_NH4;
//    sLCDObject[__SET_OFFSET_TEMP].pData     = &sParaDisplay.temp_Offset_i32; 
//    sLCDObject[__SET_OFFSET_TEMP].Scale_u8  = sParaDisplay.Scale_Temp;
//    
////    sLCDObject[__SET_ALARM_STATE].pData         = &sTempAlarm.State;
//    sLCDObject[__SET_ALARM_UPPER].pData         = &sParaDisplay.Alarm_Upper_i32;
//    sLCDObject[__SET_ALARM_UPPER].Scale_u8      = sParaDisplay.Scale_Alarm; 
//    sLCDObject[__SET_ALARM_LOWER].pData         = &sParaDisplay.Alarm_Lower_i32;
//    sLCDObject[__SET_ALARM_LOWER].Scale_u8      = sParaDisplay.Scale_Alarm; 
//    
//    sLCDObject[__SET_RANGE_U_KEY].pData         = &sParaDisplay.Upper_Key_i32;
//    sLCDObject[__SET_RANGE_U_KEY].Scale_u8      = sParaDisplay.Scale_Range; 
//    sLCDObject[__SET_RANGE_L_KEY].pData         = &sParaDisplay.Lower_Key_i32;
//    sLCDObject[__SET_RANGE_L_KEY].Scale_u8      = sParaDisplay.Scale_Range; 
//    sLCDObject[__SET_RANGE_U_TEMP].pData        = &sParaDisplay.Upper_Temp_i32;
//    sLCDObject[__SET_RANGE_U_TEMP].Scale_u8     = sParaDisplay.Scale_Range; 
//    sLCDObject[__SET_RANGE_L_TEMP].pData        = &sParaDisplay.Lower_Temp_i32;
//    sLCDObject[__SET_RANGE_L_TEMP].Scale_u8     = sParaDisplay.Scale_Range; 
//   
//    sLCDObject[__SCR_INFOR_FW_VERSION_2].pData   = sFirmVersion.Data_a8;
//    sLCDObject[__SCR_INFOR_MODEL_2].pData   = sModelVersion.Data_a8;
//    
//    sLCDObject[__SET_CALIB_NH4_VALUE].pData       = &sParaDisplay.NH4_Raw_i32; 
//    sLCDObject[__SET_CALIB_NH4_VALUE].Scale_u8    = sParaDisplay.Scale_NH4; 
//    sLCDObject[__SET_CALIB_NH4_CONST_K].pData     = &sParaDisplay.NH4_K_i32; 
//    sLCDObject[__SET_CALIB_NH4_CONST_K].Scale_u8  = sParaDisplay.Scale_NH4; 
//    sLCDObject[__SET_CALIB_NH4_CONST_B].pData     = &sParaDisplay.NH4_B_i32; 
//    sLCDObject[__SET_CALIB_NH4_CONST_B].Scale_u8  = sParaDisplay.Scale_NH4; 
//    sLCDObject[__SET_CALIB_NH4_P1].pData          = &sParaDisplay.NH4_P1_i32; 
//    sLCDObject[__SET_CALIB_NH4_P1].Scale_u8       = sParaDisplay.Scale_NH4; 
//    sLCDObject[__SET_CALIB_NH4_P2].pData          = &sParaDisplay.NH4_P2_i32; 
//    sLCDObject[__SET_CALIB_NH4_P2].Scale_u8       = sParaDisplay.Scale_NH4; 
//    
//    sLCDObject[__SET_CALIB_PH_VALUE].pData       = &sParaDisplay.PH_Raw_i32; 
//    sLCDObject[__SET_CALIB_PH_VALUE].Scale_u8    = sParaDisplay.Scale_NH4; 
//    sLCDObject[__SET_CALIB_PH_CONST_K].pData     = &sParaDisplay.PH_K_i32; 
//    sLCDObject[__SET_CALIB_PH_CONST_K].Scale_u8  = sParaDisplay.Scale_NH4; 
//    sLCDObject[__SET_CALIB_PH_CONST_B].pData     = &sParaDisplay.PH_B_i32; 
//    sLCDObject[__SET_CALIB_PH_CONST_B].Scale_u8  = sParaDisplay.Scale_NH4; 
//    sLCDObject[__SET_CALIB_PH_P1].pData          = &sParaDisplay.PH_P1_i32; 
//    sLCDObject[__SET_CALIB_PH_P1].Scale_u8       = sParaDisplay.Scale_NH4; 
//    sLCDObject[__SET_CALIB_PH_P2].pData          = &sParaDisplay.PH_P2_i32; 
//    sLCDObject[__SET_CALIB_PH_P2].Scale_u8       = sParaDisplay.Scale_NH4; 
//    
//    sLCDObject[__SET_CALIB_K_VALUE].pData       = &sParaDisplay.K_Raw_i32; 
//    sLCDObject[__SET_CALIB_K_VALUE].Scale_u8    = sParaDisplay.Scale_NH4; 
//    sLCDObject[__SET_CALIB_K_CONST_K].pData     = &sParaDisplay.K_K_i32; 
//    sLCDObject[__SET_CALIB_K_CONST_K].Scale_u8  = sParaDisplay.Scale_NH4; 
//    sLCDObject[__SET_CALIB_K_CONST_B].pData     = &sParaDisplay.K_B_i32; 
//    sLCDObject[__SET_CALIB_K_CONST_B].Scale_u8  = sParaDisplay.Scale_NH4; 
//    sLCDObject[__SET_CALIB_K_P1].pData          = &sParaDisplay.K_P1_i32; 
//    sLCDObject[__SET_CALIB_K_P1].Scale_u8       = sParaDisplay.Scale_NH4; 
//    sLCDObject[__SET_CALIB_K_P2].pData          = &sParaDisplay.K_P2_i32; 
//    sLCDObject[__SET_CALIB_K_P2].Scale_u8       = sParaDisplay.Scale_NH4; 
}

uint8_t Display_Task(void)
{
	uint8_t i = 0;

	for (i = 0; i < _EVENT_END_DISPLAY; i++)
	{
		if (sEventDisplay[i].e_status == 1)
		{
			if ((sEventDisplay[i].e_systick == 0) ||
					((HAL_GetTick() - sEventDisplay[i].e_systick)  >=  sEventDisplay[i].e_period))
			{
                sEventDisplay[i].e_status = 0;
				sEventDisplay[i].e_systick = HAL_GetTick();
				sEventDisplay[i].e_function_handler(i);
			}
		}
	}
    
	return 0;
}

/*=======================Func Callback=====================*/
static uint8_t _Cb_Display_Init (uint8_t event)
{
//    if(sLCD.Ready_u8 == false)
//    {
////        UTIL_Printf_Str(DBLEVEL_M, "u_lcd: init...\r\n");
//        
////        HAL_GPIO_WritePin (LCD_ON_OFF_GPIO_Port, LCD_ON_OFF_Pin, GPIO_PIN_SET);   
//        HAL_GPIO_WritePin (LCD_RW_GPIO_Port, LCD_RW_Pin, GPIO_PIN_RESET);
//        HAL_GPIO_WritePin (LCD_C86_GPIO_Port, LCD_C86_Pin, GPIO_PIN_RESET); 
//        
//        glcd_init();
////        glcd_clear_buffer();
////        glcd_write();	
////        Deinit_LCD12864();
//    
//        glcd_tiny_set_font(Font5x7, 5, 7, 32, 127 + 10);
//        
//        sLCD.Ready_u8 = true;
//            
    fevent_active(sEventDisplay, _EVENT_DISP_SHOW); 
    fevent_active(sEventDisplay, _EVENT_DISP_AUTO_SW); 
    fevent_active(sEventDisplay, _EVENT_BUTTON_SCAN); 
//    }
//    LCD_SEG_SetDuty(4);
//    LCD_InitMap();
    HAL_Delay(5);
//    LCD_SegOn(COM1, LCD_SEG_1);
//    LCD_SegOn(COM1, LCD_SEG_2);

//    hlcd.Instance->RAM[0] = 0xFFFFFFFF;
//    hlcd.Instance->RAM[1] = 0xFFFFFFFF;
//    hlcd.Instance->RAM[2] = 0xFFFFFFFF;
//    hlcd.Instance->RAM[3] = 0xFFFFFFFF;
//    hlcd.Instance->RAM[4] = 0xFFFFFFFF;
//    hlcd.Instance->RAM[5] = 0xFFFFFFFF;
//    hlcd.Instance->RAM[6] = 0xFFFFFFFF;
//    hlcd.Instance->RAM[1] = 0x00000200;
//
//
//    LCD_SegOn(LCD_COM_0,LCD_PIN_21);
//    HAL_LCD_UpdateDisplayRequest(&hlcd); // r?t quan tr?ng
    
    LCD_InitMap();
//    LCD_ShowChar(6, 'P');
//    LCD_SetDP(5, 1);
//    LCD_DisplayLine1("2655");
//    LCD_DisplayLine2("0025");
//    HAL_LCD_UpdateDisplayRequest(&hlcd);
    return 1;
}

static uint8_t _Cb_Display_Logo (uint8_t event)
{
//    static uint8_t step_u8 = 0;
//    
//    switch (step_u8)
//    {
//        case 0:
//            glcd_test_bitmap_128x64();
//            break;
//            
//        case 1:
//            glcd_clear_buffer();
////            glcd_set_font(Liberation_Sans17x17_Alpha, 17, 17, 65, 90);
////            
////            glcd_draw_string_xy(30, 13, "SV");
////            
////            glcd_set_pixel(60, 25, BLACK);
////            glcd_set_pixel(60, 26, BLACK);
////            glcd_set_pixel(61, 26, BLACK);
////            glcd_set_pixel(61, 25, BLACK);
////            
////            glcd_draw_string_xy(65, 13, "JSC");
//            
//              glcd_tiny_set_font(Font5x7, 5, 7, 32, 127 + 10);
////            glcd_tiny_draw_string(34+4, 1, "CHAT LUONG");
////            glcd_tiny_draw_string(31+4, 2, "LA NIEM TIN");
////            https://saovietgroup.com.vn/
//              glcd_tiny_draw_string(44, 3, "Website:");
//              glcd_tiny_draw_string(8, 4, "saovietgroup.com.vn");
//            
////            glcd_draw_rect(0, 0, 128, 64,BLACK);
//            
////            glcd_set_font(Liberation_Sans11x14_Numbers,11,14,46,57);
////            glcd_draw_string_xy(4,0, ".963");
////            
////            glcd_set_font(Liberation_Sans11x14_Numbers,11,14,46,57);
////            glcd_draw_string_xy(4,16, ".852");
//            
//            glcd_write();	
//            sEventDisplay[_EVENT_DISP_LOGO].e_period = 5000;
//          break;
//          
//        case 2:
//            sLCD.Ready_u8 = true;
//            glcd_tiny_set_font(Font5x7, 5, 7, 32, 127 + 10);
            fevent_active(sEventDisplay, _EVENT_DISP_SHOW); 
            fevent_active(sEventDisplay, _EVENT_DISP_AUTO_SW); 
            fevent_active(sEventDisplay, _EVENT_BUTTON_SCAN); 
////            HAL_TIM_Base_Start_IT(&htim2);
//          break;
//          
//        default:
//          break;
//    }
//    step_u8++;
//    fevent_enable(sEventDisplay, event);
    
    return 1;
}

static uint8_t _Cb_Display_Show (uint8_t event)
{
    
//    static uint8_t ScreenLast = 0;
//    static uint32_t LandMarkChange_u32 = 0;

    Update_ParaDisplay();
//    //update cac bien moi cai dat
//    Display_Update();
    //hien thi man hinh: index
//    if (sLCD.Ready_u8 == true) {
//        //neu index 9: modbus disp: gan lai cac bien va subindex
////        if (sLCD.sScreenNow.Index_u8 == _LCD_SCREEN_7b) {
////            Display_Setup_SCREEN_Modb();
////        }
//        
//        //hien thá»‹
//        Display_Show_Screen(sLCD.sScreenNow.Index_u8);
//    } else {
//        fevent_active(sEventDisplay, _EVENT_DISP_INIT); 
//    }
    Display_Show_Screen(sLCD.sScreenNow.Index_u8);
    //ghi moc thoi gian man hinh dc chuyen: cho su kien auto next
//    if (sLCD.sScreenNow.Index_u8 != ScreenLast) {
//        ScreenLast = sLCD.sScreenNow.Index_u8;
//        LandMarkChange_u32 = RtCountSystick_u32;
//    }
    
    //hien thi man hinh cho "OK" sau do quay lai man hinh truoc do
//    if ( (sLCD.sScreenNow.Index_u8 == _LCD_SCR_SET_RESTORE)
//        && (Check_Time_Out(LandMarkChange_u32, 2000) == true ) ) {
//        LandMarkChange_u32 = RtCountSystick_u32;
//        //doi ve man hinh truoc do
//        UTIL_MEM_cpy(&sLCD.sScreenNow, &sLCD.sScreenBack, sizeof(sScreenInformation));
//    }
    
    fevent_enable(sEventDisplay, event); 

	return 1;
}

static uint8_t _Cb_Display_Auto_SW (uint8_t event)
{
//    static uint16_t cNext = 0;
    static uint8_t MarkButtPressed = false;
    
    if (Check_Time_Out(sButton.LandMarkPressButton_u32, 60000) == true) {
        if (MarkButtPressed == true) {
            MarkButtPressed = false;
            sLCD.sScreenNow.Index_u8 = _LCD_SCREEN;
        }}
//        
//        cNext++;
//        if (cNext >= (DURATION_DISPALY/sEventDisplay[event].e_period)) {
//            cNext = 0;
//            
////            if (sLCD.sScreenNow.Index_u8 == _LCD_SCREEN_7b) {                
////                sLCD.sScreenNow.SubIndex_u8++;
////                if (sLCD.sScreenNow.SubIndex_u8 >= sWmDigVar.nModbus_u8) {
////                    sLCD.sScreenNow.Index_u8++;
////                }
////            } else {
////                if (sLCD.sScreenNow.Index_u8 == _LCD_SCREEN_7) {
////                    sLCD.sScreenNow.SubIndex_u8 = 0;
////                }
////                
////                sLCD.sScreenNow.Index_u8++;
////            }
//            
//            if (sLCD.sScreenNow.Index_u8 > _LCD_SCREEN_1) {
//                sLCD.sScreenNow.Index_u8 = _LCD_SCREEN_1;
//            }
//        }
//    } else {
//        MarkButtPressed = true;
//    }
//    
    fevent_enable(sEventDisplay, event); 
    
	return 1;
}

static uint8_t _Cb_button_scan (uint8_t event)
{
    BUTTON_scan();

    if (sButton.Status == 1) {
        fevent_active(sEventDisplay, _EVENT_BUTTON_DETECTTED);
        sButton.LandMarkPressButton_u32 = RtCountSystick_u32;
    }

    fevent_enable(sEventDisplay, event);
    
	return 1;
}

static uint8_t _Cb_button_detect (uint8_t event)
{
    BUTTON_Process();
    
    sButton.Value = 0;
    sButton.Status = 0;
    
	return 1;
}

static uint8_t _Cb_Off_Speaker(uint8_t event)
{
//    HAL_GPIO_WritePin(SPEAKERS_GPIO_Port, SPEAKERS_Pin, GPIO_PIN_RESET);
    return 1;
}
/*==========================Function LCD=======================*/
void On_Speaker(uint16_t TimeOn)
{
//    HAL_GPIO_WritePin(SPEAKERS_GPIO_Port, SPEAKERS_Pin, GPIO_PIN_SET);
//    sEventDisplay[_EVENT_OFF_SPEAKER].e_period = TimeOn;
//    fevent_enable(sEventDisplay, _EVENT_OFF_SPEAKER);
}

void Update_ParaDisplay(void)
{
//    sLCDObject[__SET_ALARM_STATE].pData         = aModeSelectONOFF[*sParaDisplay.aAlarm_State_u8];
//    
//    sParaDisplay.ID_u8 = sSlave_ModbusRTU.ID;
//    sParaDisplay.Baudrate_u32 = aBaudrate_value[sSlave_ModbusRTU.Baudrate];
//    
//    sParaDisplay.pH_Value_i32 = (int32_t)(sSensor_NH4.pH_Value_f * Calculator_Scale(sParaDisplay.Scale_NH4));
//    sParaDisplay.Temp_Value_i32 = (int32_t)(sSensor_NH4.temp_Value_f * Calculator_Scale(sParaDisplay.Scale_Temp));
//    
//    sParaDisplay.NH4_Filter_i32 = (int32_t)(sSensor_NH4.NH4_Filter_f*Calculator_Scale(sParaDisplay.Scale_NH4));
//    sParaDisplay.pH_Filter_i32 = (int32_t)(sSensor_NH4.pH_Filter_f*Calculator_Scale(sParaDisplay.Scale_NH4));
//    sParaDisplay.Temp_Filter_i32 = (int32_t)(sSensor_NH4.temp_Filter_f * Calculator_Scale(sParaDisplay.Scale_Temp));
//    
//    sParaDisplay.pH_Offset_i32   = (int32_t)(sSensor_NH4.pH_Offset_f*Calculator_Scale(sParaDisplay.Scale_NH4));
//    sParaDisplay.temp_Offset_i32 = (int32_t)(sSensor_NH4.temp_Offset_f * Calculator_Scale(sParaDisplay.Scale_Temp));
//    
//    sParaDisplay.Alarm_Upper_i32 = (int32_t)(sTempAlarm.Alarm_Upper * Calculator_Scale(sParaDisplay.Scale_Alarm));
//    sParaDisplay.Alarm_Lower_i32 = (int32_t)(sTempAlarm.Alarm_Lower * Calculator_Scale(sParaDisplay.Scale_Alarm));
//    
//    sParaDisplay.Upper_Key_i32  = (int32_t)(sMeasureRange.Upper_Key  * Calculator_Scale(sParaDisplay.Scale_Range));
//    sParaDisplay.Lower_Key_i32  = (int32_t)(sMeasureRange.Lower_Key  * Calculator_Scale(sParaDisplay.Scale_Range));
//    sParaDisplay.Upper_Temp_i32 = (int32_t)(sMeasureRange.Upper_Temp * Calculator_Scale(sParaDisplay.Scale_Range));
//    sParaDisplay.Lower_Temp_i32 = (int32_t)(sMeasureRange.Lower_Temp * Calculator_Scale(sParaDisplay.Scale_Range));
//
//    sParaDisplay.NH4_Raw_i32 = (int32_t)(sSensor_NH4.NH4_Value_f * Calculator_Scale(sParaDisplay.Scale_NH4));
//    sParaDisplay.NH4_K_i32  = (int32_t)(sSensor_NH4.Const_K_NH4_f * Calculator_Scale(sParaDisplay.Scale_NH4));
//    sParaDisplay.NH4_B_i32  = (int32_t)(sSensor_NH4.Const_B_NH4_f * Calculator_Scale(sParaDisplay.Scale_NH4));
//    sParaDisplay.NH4_P1_i32 = (int32_t)(sNH4Calib.Standard_NH4_P1 * Calculator_Scale(sParaDisplay.Scale_NH4));
//    sParaDisplay.NH4_P2_i32 = (int32_t)(sNH4Calib.Standard_NH4_P2 * Calculator_Scale(sParaDisplay.Scale_NH4));
//    
//    sParaDisplay.PH_Raw_i32 = (int32_t)(sSensor_NH4.pH_Value_f * Calculator_Scale(sParaDisplay.Scale_NH4));
//    sParaDisplay.PH_K_i32   = (int32_t)(sSensor_NH4.Const_K_pH_f * Calculator_Scale(sParaDisplay.Scale_NH4));
//    sParaDisplay.PH_B_i32   = (int32_t)(sSensor_NH4.Const_B_pH_f * Calculator_Scale(sParaDisplay.Scale_NH4));
//    sParaDisplay.PH_P1_i32  = (int32_t)(sNH4Calib.Standard_pH_P1 * Calculator_Scale(sParaDisplay.Scale_NH4));
//    sParaDisplay.PH_P2_i32  = (int32_t)(sNH4Calib.Standard_pH_P2 * Calculator_Scale(sParaDisplay.Scale_NH4));
//    
//    sParaDisplay.K_Raw_i32 = (int32_t)(sSensor_NH4.K_Value_f * Calculator_Scale(sParaDisplay.Scale_NH4));
//    sParaDisplay.K_K_i32   = (int32_t)(sSensor_NH4.Const_K_K_f * Calculator_Scale(sParaDisplay.Scale_NH4));
//    sParaDisplay.K_B_i32   = (int32_t)(sSensor_NH4.Const_B_K_f * Calculator_Scale(sParaDisplay.Scale_NH4));
//    sParaDisplay.K_P1_i32  = (int32_t)(sNH4Calib.Standard_K_P1 * Calculator_Scale(sParaDisplay.Scale_NH4));
//    sParaDisplay.K_P2_i32  = (int32_t)(sNH4Calib.Standard_K_P2 * Calculator_Scale(sParaDisplay.Scale_NH4));

    if(sMeasure.sConnect_u8 == 0)
    {
        sLCDObject[__SC_TEMP].sName = aDisconnect;
        sLCDObject[__SC_HUMI].sName = aDisconnect;
    }
    else
    {
        sLCDObject[__SC_TEMP].sName = NULL;
        sLCDObject[__SC_HUMI].sName = NULL;
    }
  
    sParaDisplay.Temp_Value_i32 = (int32_t)(sMeasure.Temp_Value_f * Calculator_Scale(sParaDisplay.Scale_Temp));
    sParaDisplay.Humi_Value_i32 = (int32_t)(sMeasure.Humi_Value_f * Calculator_Scale(sParaDisplay.Scale_Humi));
    
    sParaDisplay.Temp_Filter_i32 = (int32_t)(sMeasure.Temp_Filter_f * Calculator_Scale(sParaDisplay.Scale_Temp));
    sParaDisplay.Humi_Filter_i32 = (int32_t)(sMeasure.Humi_Filter_f * Calculator_Scale(sParaDisplay.Scale_Humi));
    
    sParaDisplay.Temp_Offset_i32 = (int32_t)(sOffset.Temp_f * Calculator_Scale(sParaDisplay.Scale_Temp));
    sParaDisplay.Humi_Offset_i32 = (int32_t)(sOffset.Humi_f * Calculator_Scale(sParaDisplay.Scale_Humi));
    
    sParaDisplay.Temp_Lower_i32 = (int32_t)(sAlarm.Lower_Temp_f * Calculator_Scale(sParaDisplay.Scale_Temp));
    sParaDisplay.Temp_Upper_i32 = (int32_t)(sAlarm.Upper_Temp_f * Calculator_Scale(sParaDisplay.Scale_Temp));
    
    sParaDisplay.Humi_Lower_i32 = (int32_t)(sAlarm.Lower_Humi_f * Calculator_Scale(sParaDisplay.Scale_Humi));
    sParaDisplay.Humi_Upper_i32 = (int32_t)(sAlarm.Upper_Humi_f * Calculator_Scale(sParaDisplay.Scale_Humi));
}

void Display_Show_Oject (uint8_t object)
{
    char aTEMP[32]  = {0};
    char aName[16]  = {0};
    char aValue[16] = {0};

    int64_t TempVal = 0;
    uint8_t type = 0, temp = 0;
    uint8_t Result = 0;

    /*==================== NAME ====================*/
    if (sLCDObject[object].sName != NULL)
    {
        strncpy(aName, sLCDObject[object].sName, sizeof(aName) - 1);

        if (Display_Check_Toggle(object, 0x01) == true)
        {
            memset(aName, ' ', strlen(aName));
        }
        Result = 1;
    }

    /*==================== VALUE ====================*/
    if (sLCDObject[object].pData != NULL)
    {
        switch (sLCDObject[object].dType_u8)
        {
            case _DTYPE_U8:  TempVal = *((uint8_t *)sLCDObject[object].pData); break;
            case _DTYPE_I8:  TempVal = *((int8_t *)sLCDObject[object].pData); break;
            case _DTYPE_U16: TempVal = *((uint16_t *)sLCDObject[object].pData); break;
            case _DTYPE_I16: TempVal = *((int16_t *)sLCDObject[object].pData); break;
            case _DTYPE_U32: TempVal = *((uint32_t *)sLCDObject[object].pData); break;
            case _DTYPE_I32: TempVal = *((int32_t *)sLCDObject[object].pData); break;
            case _DTYPE_STRING: type = 1; break;
            case _DTYPE_CHAR:   type = 2; break;
        }

        if (type == 0)
        {
            UtilIntToStringWithScale(TempVal, aValue, 0xFF - sLCDObject[object].Scale_u8 + 1);
        }
        else if (type == 1)
        {
            strncpy(aValue, (char *)sLCDObject[object].pData, sizeof(aValue) - 1);
        }
        else
        {
            aValue[0] = *((char *)sLCDObject[object].pData);
            aValue[1] = '\0';
        }

        /*==================== BLINK VALUE ====================*/
        if (Display_Check_Toggle(object, 0x02) == true)
        {
            temp = (sLCDObject[object].Mode_u8 >> 4) & 0x0F;

            if (temp != 0x0F)
            {
                if (temp < strlen(aValue))
                    aValue[temp] = ' ';
            }
            else
            {
                memset(aValue, ' ', strlen(aValue));
            }
        }

        Result = 1;
    }

    /*==================== MERGE ====================*/
    if(Result == 1)
    {
        snprintf(aTEMP, sizeof(aTEMP), "%s%s", aName, aValue);

        if (sLCDObject[object].Row_u8 == 0)
            LCD_DisplayLine1(aTEMP);
        else
            LCD_DisplayLine2(aTEMP);
    }
}

/*
    Func: check toggle object
        + creat toggle effect
*/

uint8_t Display_Check_Toggle (uint8_t object, uint8_t Flag)
{
    static uint32_t LandMarkToggle_u32[3][__OJECT_END] = {0};
    static uint8_t Hide[3][__OJECT_END] = {0};

    if ((sLCDObject[object].Mode_u8 & Flag) == Flag) {
        if (Check_Time_Out(LandMarkToggle_u32[Flag][object], TIME_TOGGLE) == true) {
            LandMarkToggle_u32[Flag][object] = RtCountSystick_u32;
            Hide[Flag][object] =  1- Hide[Flag][object];
        }
    } else {
        Hide[Flag][object] = false;
    }
    
    return Hide[Flag][object];
}

void Display_Show_Screen (uint8_t screen)
{
    uint16_t  i = 0; 

    //Clear buff lcd data
//    glcd_clear_buffer();
//    LCD_Clear();
//    //Show static param: stime, icon internet,...
    LCD_ClearLine();
    Display_Show_Static_Param();
//    
//    //Show state connect sensor
//    Display_Show_State_Sensor_Network(screen);
//    Display_Show_State_Setting(screen);
     Display_Show_ParaIcon(screen);
    
//    //Show name of screen
//    if (screen >= _LCD_SCR_CAL_CHANN_1) {
//        glcd_tiny_draw_string(0, 0, "Cal.");
//    } else if (screen >= _LCD_SCR_SETTING) {
//        glcd_tiny_draw_string(0, 0, "Set.");
//    }
    
    for (i = 0; i < __OJECT_END; i++) {
        if (sLCDObject[i].Screen_u8 == screen) {
            Display_Show_Oject(i);
        }
    }
    
//    glcd_write();	
    HAL_LCD_UpdateDisplayRequest(&hlcd);
}

void Display_Set_Screen_Flag (sScreenInformation *screen, void *pData, uint8_t flag)
{
    screen->Flag_u8 = flag;
    //set ting mode    
    for (uint8_t i = screen->ParaMin_u8; i <= screen->ParaMax_u8; i++) {
        sLCDObject[i].Mode_u8 = 0xF0;
    }
    
    sLCDObject[screen->Para_u8].Mode_u8 = screen->Flag_u8;
    if (pData != NULL) {
        sLCDObject[screen->Para_u8].pData = pData;
    }
}

/*
    Func: set screen next
        + index:
        + param: curr, min, max
        + pdata: option
        + flag: toggle: name | pdata | index of pdata
*/

void Display_Set_Screen (sScreenInformation *screen, uint8_t index, uint8_t subindex,
                         uint8_t para, uint8_t paramin, uint8_t paramax,
                         void *pData, uint8_t flag)
{
    screen->Index_u8 = index;
    screen->SubIndex_u8 = subindex;
    screen->Para_u8 = para;
    screen->ParaMin_u8 = paramin;
    screen->ParaMax_u8 = paramax;

    Display_Set_Screen_Flag(screen, pData, flag);
}

/*
    Func: check pass to setting
*/

uint8_t Display_Check_Password (uint8_t pPass[])
{
    for (uint8_t i = 0; i < sizeof(aPASSWORD); i++) {
        if (pPass[i] != aPASSWORD[i])
            return false;
    }
    
    return true;
}

/*
    Func: show static param
        + stime
        + icon: internet, baterry, "____"
*/
void Display_Show_Static_Param (void)
{
//    static uint8_t cCharge = 0, batlevel = 0;
//    uint8_t TempPos = 0;
//    char aData[32] = {0};
//    static uint32_t LandMarkNextScreen = 0;
//    
//    //Hien thi sac pin
//    if (Display_Show_Charge_Bat(&batlevel) == 1) {
//        if (Check_Time_Out(LandMarkNextScreen, 500) == true) {
//            LandMarkNextScreen = RtCountSystick_u32;
//            cCharge++;
//        } 
//        
//        if(cCharge > 3) {
//            cCharge = batlevel;
//        }
//    } else {
//        LandMarkNextScreen = RtCountSystick_u32;
//        cCharge = batlevel;
//    }
//        
//    glcd_tiny_draw_char(120, 0, PIN_ZERO + cCharge);
//        
//    //Hien thi cot song sim
//    if (sSimCommVar.State_u8 == _SIM_CONN_MQTT) {
//        glcd_tiny_draw_char(114, 0, CONNECT_DISPLAY);
//        TempPos = 6;
//    } else {
//        glcd_tiny_draw_char(114, 0, 0x20U);    
//    }
//    
//    //hien thi cot ethernet
//    if (sAppEthVar.Status_u8 == _ETH_MQTT_CONNECTED) {
//        glcd_tiny_draw_char(114 - TempPos, 0, FONT_ETHERNET);    
//    } else {
//        glcd_tiny_draw_char(114 - TempPos, 0, 0x20U);    
//    }
//    
//    //Hien thi stime
//    sprintf(aData, "%02d:%02d:%02d", sRTC.hour, sRTC.min, sRTC.sec);
//    glcd_tiny_draw_string(48, 0, (char *) aData);
    
    //Hien thi gáº¡ch duoi
//    glcd_draw_line(0, 8, 127, 8, BLACK); 
}

/*
    Func: show static param
        + stime
        + icon: internet, baterry, "____"
*/
void Display_Show_State_Sensor_Network (uint8_t screen)
{
//    if(screen == _LCD_SCREEN_1)
//    {
//        if(sSensor_NH4.State_Connect == _SENSOR_DISCONNECT)
//        {
////            glcd_tiny_draw_string(120, sLCDObject[__SC1_PH_ATC].Row_u8, " ");
//            glcd_tiny_draw_string(120, sLCDObject[__SC1_TEMP].Row_u8, " ");
//        }
//        else
//        {
////            glcd_tiny_draw_string(120, sLCDObject[__SC1_PH_ATC].Row_u8, "N");
//            glcd_tiny_draw_string(120, sLCDObject[__SC1_TEMP].Row_u8, "N");
//        }
//    }
}

/*
    Func: show static param
        + stime
        + icon: internet, baterry, "____"
*/
void Display_Show_State_Setting (uint8_t screen)
{
//    if(screen == _LCD_SCR_CHECK_SETTING)
//    {
//        switch(sParaDisplay.State_Setting)
//        {
//            case _STATE_SETTING_FREE:
//              sLCDObject[__CHECK_STATE_SETTING].pData = aSTT_SETTING_FREE;
//              break;
//              
//            case _STATE_SETTING_ENTER:
//              sLCDObject[__CHECK_STATE_SETTING].pData = aSTT_SETTING_ENTER;
//              break;
//
//            case _STATE_SETTING_WAIT:
//              sLCDObject[__CHECK_STATE_SETTING].pData = aSTT_SETTING_WAIT;
//              break;
//              
//            case _STATE_SETTING_DONE:
//              sLCDObject[__CHECK_STATE_SETTING].pData = aSTT_SETTING_DONE;
//              break;
//              
//            case _STATE_SETTING_ERROR:
//              sLCDObject[__CHECK_STATE_SETTING].pData = aSTT_SETTING_ERROR;
//              break;
//              
//            default:
//              break;
//        }
//    }
//    else
//    {
//        sLCDObject[__CHECK_STATE_SETTING].pData = aSTT_SETTING_FREE;
//    }
}

void Display_Show_ParaIcon(uint8_t sScreen)
{
    char aValue[4] = {0};
    
    if(sScreen == _LCD_SCREEN)
    {
        LCD_SegOn(LCD_COM_1, LCD_PIN_23);    //%Temp C
        LCD_SegOn(LCD_COM_3, LCD_PIN_23);    //%RH
    }
    else
    {
        LCD_SegOff(LCD_COM_1, LCD_PIN_23);    //%Temp C
        LCD_SegOff(LCD_COM_3, LCD_PIN_23);    //%RH
    }
    
    UtilIntToStringWithScale((int)(sSlave_ModbusRTU.ID), aValue, 0x00);
    LCD_DisplayLine0(aValue);
    
}

void Deinit_LCD12864(void)
{
//    glcd_command(0xa0); /* ADC select in normal mode */
//	glcd_command(0xae); /* Display OFF */
//	glcd_command(0xc0); /* Common output mode select: reverse direction (last 3 bits are ignored) */
//	glcd_command(0xa1); 
//    glcd_command(0xa2); /* LCD bias set at 1/9 */
//	glcd_command(0x2f); /* Power control set to operating mode: 7 */
//	glcd_command(0x21); /* Internal resistor ratio, set to: 1 */
//	glcd_set_contrast(40); /* Set contrast, value experimentally determined, can set to 6-bit value, 0 to 63 */
//	glcd_command(0xaf); /* Display on */
}