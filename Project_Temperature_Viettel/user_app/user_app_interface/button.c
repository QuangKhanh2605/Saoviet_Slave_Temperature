

#include "button.h"
#include "lcd_display.h"
#include "user_define.h"

sButtonStruct sButton;


/*============ Function =============*/

void BUTTON_Process(void)
{
    switch(sButton.Value)
    {
        case _ENTER:  
            BUTTON_Enter_Process();
            break;
        case _UP:  
            BUTTON_Up_Process();
            break;
        case _DOWN:               
            BUTTON_Down_Process();
            break;
        case _ESC:
            BUTTON_ESC_Process();
            break;
        default:
            break;
    }
}

uint8_t BUTTON_scan(void)
{
    uint8_t r_value = 0;
    static uint8_t last_value = 0;
    static uint32_t hold = 0;
    static uint16_t block_time = 0;

    if (block_time > 0) {
        block_time--;
        return 0;
    }

    if (sButton.Status == 0)
    {
        r_value = (!BUTTON_ENTER<<0)|(!BUTTON_UP<<1)|(!BUTTON_DOWN<<2)|(!BUTTON_ESC<<3);

        switch(r_value)
        {
            case _ENTER:
            case _UP:
            case _DOWN:
            case _ESC:

                sButton.Count++;

                if (sButton.Count >= BUT_TIME_DETECT)
                {
                    sButton.Value = r_value;
                    sButton.Status = 1;

                    if (hold < BUT_COUNT_HOLD)
                        block_time = 25;

                    if ( r_value == last_value )
                    {
                        hold++;
                        
                        if (hold < BUT_COUNT_FAST/2) {
                            sButton.Count = 0;
                        }
                        else if (hold < BUT_COUNT_FAST*3) {
                            sButton.Count = BUT_TIME_DETECT - 15; 
                        }
                        else if (hold < BUT_COUNT_FAST*4) {
                            sButton.Count = BUT_TIME_DETECT - 10; 
                        }
                        else {
                            sButton.Count = BUT_TIME_DETECT - 2;                
                            hold = BUT_COUNT_FAST*4;               
                        }
                    }
                    else
                    {
                        sButton.Count = 0;
                        hold = 0;
                    }
                    last_value = r_value;
                }
                break;

            default:
                last_value = 0;
                sButton.Count = BUT_TIME_DETECT/2;
                hold = 0;
                break;
        }
    }

    return sButton.Value;
}

//uint8_t BUTTON_scan(void)
//{
//    uint8_t r_value = 0;
//    static uint8_t last_value = 0;
//    static uint32_t hold = 0;
//    
//	if (sButton.Status == 0)
//	{
//		r_value = (!BUTTON_ENTER<<0)|(!BUTTON_UP<<1)|(!BUTTON_DOWN<<2)|(!BUTTON_ESC<<3);
//        
//		switch(r_value)
//		{
//			case _ENTER:
//            case _UP:
//            case _DOWN:
//            case _ESC:
//                sButton.Count++;
//                if (sButton.Count >= BUT_TIME_DETECT) {
//                    sButton.Value = r_value;
//                    sButton.Status = 1;
//                    
//                    if ( r_value == last_value ) {
//                        hold++;
//                        if ( hold >= BUT_COUNT_FAST ) {
//                            sButton.Count = BUT_TIME_DETECT;
//                            hold = BUT_COUNT_FAST;
//                        } else if (hold >= BUT_COUNT_HOLD) {
//                            sButton.Count = BUT_TIME_DETECT - 2;
//                        } else {
//                            sButton.Count = 0;
//                        }
//                    } else {
//                        sButton.Count = 0;  
//                        hold = 0;
//                    } 
//                                
//                    last_value = r_value;
//                }
//  				break;
//			default:
//                last_value = 0;
//				sButton.Count = BUT_TIME_DETECT/2;
//                hold = 0;
//				break;
//		}
//	}
//
//    return sButton.Value;
//}

static char aTEST[10] = {"1234"};

//static sPressureLinearInter sPressConfig = {0};
//static Struct_Pulse sPulseConfig = {0};
//static int16_t PressOldCalib_i16 = 0;
//static char sConfirm[2][10] = {"No", "Yes"};

float       Old_Value_f = 0;
uint32_t    Old_Value_u32 = 0;
int16_t     Old_Value_i16 = 0;
float       Button_Stamp1_f = 0;
float       Button_Stamp2_f = 0;

float       Button_K_Const_f = 0;
float       Button_B_Const_f = 0;
      
void BUTTON_Enter_Process (void)
{
//    static uint8_t tempu8 = 0;
//    static uint8_t TempScale = 1;
    float stamp_f = 0;
    
    switch (sLCD.sScreenNow.Index_u8)
    {
        case _LCD_SCREEN:
            sIndexLCD.Index_LCD_P  = 1;
            sIndexLCD.Index_LCD_P1 = 1;
            sIndexLCD.Index_LCD_P2 = 1;
            sIndexLCD.Index_LCD_P3 = 1;
            
            sButton.Old_value = 0;
            UTIL_MEM_set(aTEST, 0, sizeof(aTEST));
            aTEST[0] = '0'; aTEST[1] = '0'; aTEST[2] = '0'; aTEST[3] = '0';
            
            Display_Set_Screen(&sLCD.sScreenNow, _LCD_SCR_PASS_WORD, 0,
                               __PASSWORD_1, __PASSWORD_1, __PASSWORD_1,
                               aTEST, (sButton.Old_value << 4) + 0x02);
            break;
            
        case _LCD_SCR_PASS_WORD:
            sButton.Old_value++;
            if (sButton.Old_value > 3)
            { 
                if (Display_Check_Password((uint8_t *) aTEST) == true) {
                    //copy screen now to screenback
                    UTIL_MEM_cpy(&sLCD.sScreenBack, &sLCD.sScreenNow, sizeof(sScreenInformation));
                    
                    Display_Set_Screen(&sLCD.sScreenNow, _LCD_SCR_SETTING_P, 0,
                                       __SETTING_P, __SETTING_P, __SETTING_P,
                                        &sIndexLCD.Index_LCD_P, 0xF2);
                } else {
                    sLCD.sScreenNow.Index_u8 = _LCD_SCREEN;  //thoat
                }
            } else {
                Display_Set_Screen_Flag(&sLCD.sScreenNow, NULL, (sButton.Old_value << 4) + 0x02);
            }
            break;
            
        case _LCD_SCR_SETTING_P:
            switch(sIndexLCD.Index_LCD_P)
            {
                case 1:
                    UTIL_MEM_cpy(&sLCD.sScreenBack, &sLCD.sScreenNow, sizeof(sScreenInformation));
                    Display_Set_Screen(&sLCD.sScreenNow, _LCD_SCR_SETTING_P1, 0,
                                       __SETTING_P1, __SETTING_P1, __SETTING_P1,
                                        &sIndexLCD.Index_LCD_P1, 0xF2);
                    break;
                    
                case 2:
                    UTIL_MEM_cpy(&sLCD.sScreenBack, &sLCD.sScreenNow, sizeof(sScreenInformation));
                    Display_Set_Screen(&sLCD.sScreenNow, _LCD_SCR_SETTING_P2, 0,
                                       __SETTING_P2, __SETTING_P2, __SETTING_P2,
                                        &sIndexLCD.Index_LCD_P2, 0xF2);
                    break;
                    
                case 3:
                    UTIL_MEM_cpy(&sLCD.sScreenBack, &sLCD.sScreenNow, sizeof(sScreenInformation));
                    Display_Set_Screen(&sLCD.sScreenNow, _LCD_SCR_SETTING_P3, 0,
                                       __SETTING_P3, __SETTING_P3, __SETTING_P3,
                                        &sIndexLCD.Index_LCD_P3, 0xF2);
                    break;
                    
                default:
                    break;
            }
            break;
            
        case _LCD_SCR_SETTING_P1:
            switch(sIndexLCD.Index_LCD_P1)
            {
                case 1:
                    UTIL_MEM_cpy(&sLCD.sScreenBack, &sLCD.sScreenNow, sizeof(sScreenInformation));
                    Display_Set_Screen(&sLCD.sScreenNow, _LCD_SCR_SETTING_P1_1, 0,
                                       __SETTING_P1_1_S, __SETTING_P1_1_S, __SETTING_P1_1_S,
                                        &sButton.Old_value, 0xF2);
                    sButton.Old_value = sSlave_ModbusRTU.ID;
                    break;
                    
                case 2:
                    UTIL_MEM_cpy(&sLCD.sScreenBack, &sLCD.sScreenNow, sizeof(sScreenInformation));
                    Display_Set_Screen(&sLCD.sScreenNow, _LCD_SCR_SETTING_P1_2, 0,
                                       __SETTING_P1_2_S, __SETTING_P1_2_S, __SETTING_P1_2_S,
                                        &sButton.Old_value, 0xF2);
                    sButton.Old_value = sSlave_ModbusRTU.Baudrate;
                    break;

                default:
                    break;
            }
            break;
            
        case _LCD_SCR_SETTING_P2:
            switch(sIndexLCD.Index_LCD_P2)
            {
                case 1:
                    UTIL_MEM_cpy(&sLCD.sScreenBack, &sLCD.sScreenNow, sizeof(sScreenInformation));
                    Display_Set_Screen(&sLCD.sScreenNow, _LCD_SCR_SETTING_P2_1, 0,
                                       __SETTING_P2_1_S, __SETTING_P2_1_S, __SETTING_P2_1_S,
                                        &sButton.Old_value, 0xF2);
                    sButton.Old_value = sParaDisplay.Temp_Offset_i32;
                    break;
                    
                case 2:
                    UTIL_MEM_cpy(&sLCD.sScreenBack, &sLCD.sScreenNow, sizeof(sScreenInformation));
                    Display_Set_Screen(&sLCD.sScreenNow, _LCD_SCR_SETTING_P2_2, 0,
                                       __SETTING_P2_2_S, __SETTING_P2_2_S, __SETTING_P2_2_S,
                                        &sButton.Old_value, 0xF2);
                    sButton.Old_value = sParaDisplay.Humi_Offset_i32;
                    break;
                    
                default:
                    break;
            }
            break;
            
        case _LCD_SCR_SETTING_P3:
            switch(sIndexLCD.Index_LCD_P3)
            {
                case 1:
                    UTIL_MEM_cpy(&sLCD.sScreenBack, &sLCD.sScreenNow, sizeof(sScreenInformation));
                    Display_Set_Screen(&sLCD.sScreenNow, _LCD_SCR_SETTING_P3_1, 0,
                                       __SETTING_P3_1_S, __SETTING_P3_1_S, __SETTING_P3_1_S,
                                        &sButton.Old_value, 0xF2);
                    sButton.Old_value = sAlarm.Mode_Temp_u8;
                    break;
                    
                case 2:
                    UTIL_MEM_cpy(&sLCD.sScreenBack, &sLCD.sScreenNow, sizeof(sScreenInformation));
                    Display_Set_Screen(&sLCD.sScreenNow, _LCD_SCR_SETTING_P3_2, 0,
                                       __SETTING_P3_2_S, __SETTING_P3_2_S, __SETTING_P3_2_S,
                                        &sButton.Old_value, 0xF2);
                    sButton.Old_value = sParaDisplay.Temp_Lower_i32;
                    break;
                    
                case 3:
                    UTIL_MEM_cpy(&sLCD.sScreenBack, &sLCD.sScreenNow, sizeof(sScreenInformation));
                    Display_Set_Screen(&sLCD.sScreenNow, _LCD_SCR_SETTING_P3_3, 0,
                                       __SETTING_P3_3_S, __SETTING_P3_3_S, __SETTING_P3_3_S,
                                        &sButton.Old_value, 0xF2);
                    sButton.Old_value = sParaDisplay.Temp_Upper_i32;
                    break;
                    
                case 4:
                    UTIL_MEM_cpy(&sLCD.sScreenBack, &sLCD.sScreenNow, sizeof(sScreenInformation));
                    Display_Set_Screen(&sLCD.sScreenNow, _LCD_SCR_SETTING_P3_4, 0,
                                       __SETTING_P3_4_S, __SETTING_P3_4_S, __SETTING_P3_4_S,
                                        &sButton.Old_value, 0xF2);
                    sButton.Old_value = sAlarm.Mode_Humi_u8;
                    break;
                    
                case 5:
                    UTIL_MEM_cpy(&sLCD.sScreenBack, &sLCD.sScreenNow, sizeof(sScreenInformation));
                    Display_Set_Screen(&sLCD.sScreenNow, _LCD_SCR_SETTING_P3_5, 0,
                                       __SETTING_P3_5_S, __SETTING_P3_5_S, __SETTING_P3_5_S,
                                        &sButton.Old_value, 0xF2);
                    sButton.Old_value = sParaDisplay.Humi_Lower_i32;
                    break;
                    
                case 6:
                    UTIL_MEM_cpy(&sLCD.sScreenBack, &sLCD.sScreenNow, sizeof(sScreenInformation));
                    Display_Set_Screen(&sLCD.sScreenNow, _LCD_SCR_SETTING_P3_6, 0,
                                       __SETTING_P3_6_S, __SETTING_P3_6_S, __SETTING_P3_6_S,
                                        &sButton.Old_value, 0xF2);
                    sButton.Old_value = sParaDisplay.Humi_Upper_i32;
                    break;
                    
                default:
                    break;
            }
            break;
            
        case _LCD_SCR_SETTING_P1_1:
            UTIL_MEM_cpy(&sLCD.sScreenNow, &sLCD.sScreenBack, sizeof(sScreenInformation));
            Save_InforSlaveModbusRTU(_E_RS485_ID, (uint8_t)sButton.Old_value);
            break;
            
        case _LCD_SCR_SETTING_P1_2:
            UTIL_MEM_cpy(&sLCD.sScreenNow, &sLCD.sScreenBack, sizeof(sScreenInformation));
            Save_InforSlaveModbusRTU(_E_RS485_BR, (uint8_t)sButton.Old_value);
            break;
            
        case _LCD_SCR_SETTING_P2_1:
            UTIL_MEM_cpy(&sLCD.sScreenNow, &sLCD.sScreenBack, sizeof(sScreenInformation));
            Save_OffsetSensor(((float)(sButton.Old_value)/Calculator_Scale(sParaDisplay.Scale_Temp)), sOffset.Humi_f);
            break;
            
        case _LCD_SCR_SETTING_P2_2:
            UTIL_MEM_cpy(&sLCD.sScreenNow, &sLCD.sScreenBack, sizeof(sScreenInformation));
            Save_OffsetSensor(sOffset.Temp_f, ((float)(sButton.Old_value)/Calculator_Scale(sParaDisplay.Scale_Humi)));
            break;
            
        case _LCD_SCR_SETTING_P3_1:
            UTIL_MEM_cpy(&sLCD.sScreenNow, &sLCD.sScreenBack, sizeof(sScreenInformation));
            Save_AlarmSensor(_E_MODE_TEMP, &sButton.Old_value);
            break;
            
        case _LCD_SCR_SETTING_P3_2:
            UTIL_MEM_cpy(&sLCD.sScreenNow, &sLCD.sScreenBack, sizeof(sScreenInformation));
            stamp_f = (float)(sButton.Old_value)/Calculator_Scale(sParaDisplay.Scale_Temp);
            Save_AlarmSensor(_E_LOWER_TEMP, &stamp_f);
            break;
            
        case _LCD_SCR_SETTING_P3_3:
            UTIL_MEM_cpy(&sLCD.sScreenNow, &sLCD.sScreenBack, sizeof(sScreenInformation));
            stamp_f = (float)(sButton.Old_value)/Calculator_Scale(sParaDisplay.Scale_Temp);
            Save_AlarmSensor(_E_UPPER_TEMP, &stamp_f);
            break;
            
        case _LCD_SCR_SETTING_P3_4:
            UTIL_MEM_cpy(&sLCD.sScreenNow, &sLCD.sScreenBack, sizeof(sScreenInformation));
            Save_AlarmSensor(_E_MODE_HUMI, &sButton.Old_value);
            break;
            
        case _LCD_SCR_SETTING_P3_5:
            UTIL_MEM_cpy(&sLCD.sScreenNow, &sLCD.sScreenBack, sizeof(sScreenInformation));
            stamp_f = (float)(sButton.Old_value)/Calculator_Scale(sParaDisplay.Scale_Humi);
            Save_AlarmSensor(_E_LOWER_HUMI, &stamp_f);
            break;
            
        case _LCD_SCR_SETTING_P3_6:
            UTIL_MEM_cpy(&sLCD.sScreenNow, &sLCD.sScreenBack, sizeof(sScreenInformation));
            stamp_f = (float)(sButton.Old_value)/Calculator_Scale(sParaDisplay.Scale_Humi);
            Save_AlarmSensor(_E_UPPER_HUMI, &stamp_f);
            break;
      
        default:
            break;
    }
}

void BUTTON_Up_Process (void)
{
    switch (sLCD.sScreenNow.Index_u8)
    {
        case _LCD_SCREEN:
          break;
          
        case _LCD_SCR_PASS_WORD:
            aTEST[sButton.Old_value] ++;
            if (aTEST[sButton.Old_value] > '9')
                aTEST[sButton.Old_value] = '0';
            break;
            
        case _LCD_SCR_SETTING_P:
          if(sIndexLCD.Index_LCD_P < 3)
            sIndexLCD.Index_LCD_P++;
          break;
          
        case _LCD_SCR_SETTING_P1:
          if(sIndexLCD.Index_LCD_P1 < 2)
            sIndexLCD.Index_LCD_P1++;
          break;
          
        case _LCD_SCR_SETTING_P2:
          if(sIndexLCD.Index_LCD_P2 < 2)
            sIndexLCD.Index_LCD_P2++;
          break;
          
        case _LCD_SCR_SETTING_P3:
          if(sIndexLCD.Index_LCD_P3 < 6)
            sIndexLCD.Index_LCD_P3++;
          break;
          
        case _LCD_SCR_SETTING_P1_1:
            if(sButton.Old_value < 255)
                sButton.Old_value++;
            break;
            
        case _LCD_SCR_SETTING_P1_2:
           if(sButton.Old_value < 10)  
                sButton.Old_value++;
            break;
            
        case _LCD_SCR_SETTING_P2_1:
            sButton.Old_value++;
            break;
            
        case _LCD_SCR_SETTING_P2_2:
            sButton.Old_value++;
            break;
            
        case _LCD_SCR_SETTING_P3_1:
            sButton.Old_value = !sButton.Old_value;
            break;
            
        case _LCD_SCR_SETTING_P3_2:
            if(sButton.Old_value + 1 < sParaDisplay.Temp_Upper_i32)
                sButton.Old_value++;
            break;
            
        case _LCD_SCR_SETTING_P3_3: 
            if(sButton.Old_value < TEMP_MAX * Calculator_Scale(sParaDisplay.Scale_Temp))
                sButton.Old_value++;
            break;
            
        case _LCD_SCR_SETTING_P3_4:
            sButton.Old_value = !sButton.Old_value;
            break;
            
        case _LCD_SCR_SETTING_P3_5:
            if(sButton.Old_value + 1 < sParaDisplay.Humi_Upper_i32)
                sButton.Old_value++;
            break;
            
        case _LCD_SCR_SETTING_P3_6:
            if(sButton.Old_value < HUMI_MAX * Calculator_Scale(sParaDisplay.Scale_Humi))
                sButton.Old_value++;
            break;
            
        default:
            break;
    }
}


void BUTTON_Down_Process (void)
{
    switch (sLCD.sScreenNow.Index_u8)
    {
        case _LCD_SCREEN:
          break;
          
        case _LCD_SCR_PASS_WORD:
            aTEST[sButton.Old_value] --;
            if (aTEST[sButton.Old_value] < '0') {
                aTEST[sButton.Old_value] = '9';
            }
            break;
            
        case _LCD_SCR_SETTING_P:
          if(sIndexLCD.Index_LCD_P > 1)
            sIndexLCD.Index_LCD_P--;
          break;
          
        case _LCD_SCR_SETTING_P1:
          if(sIndexLCD.Index_LCD_P1 > 1)
            sIndexLCD.Index_LCD_P1--;
          break;
          
        case _LCD_SCR_SETTING_P2:
          if(sIndexLCD.Index_LCD_P2 > 1)
            sIndexLCD.Index_LCD_P2--;
          break;
          
        case _LCD_SCR_SETTING_P3:
          if(sIndexLCD.Index_LCD_P3 > 1)
            sIndexLCD.Index_LCD_P3--;
          break;
          
        case _LCD_SCR_SETTING_P1_1:
            sButton.Old_value--;
            break;
            
        case _LCD_SCR_SETTING_P1_2:
            sButton.Old_value--;
            break;
            
        case _LCD_SCR_SETTING_P2_1:
            sButton.Old_value--;
            break;
            
        case _LCD_SCR_SETTING_P2_2:
            sButton.Old_value--;
            break;
            
        case _LCD_SCR_SETTING_P3_1:
            sButton.Old_value = !sButton.Old_value;
            break;
            
        case _LCD_SCR_SETTING_P3_2:
            if(sButton.Old_value > TEMP_MIN * Calculator_Scale(sParaDisplay.Scale_Temp))
                sButton.Old_value--;
            break;
            
        case _LCD_SCR_SETTING_P3_3: 
            if(sButton.Old_value - 1 > sParaDisplay.Temp_Lower_i32)
                sButton.Old_value--;
            break;
            
        case _LCD_SCR_SETTING_P3_4:
            sButton.Old_value = !sButton.Old_value;
            break;
            
        case _LCD_SCR_SETTING_P3_5:
            if(sButton.Old_value > HUMI_MIN * Calculator_Scale(sParaDisplay.Scale_Humi))
                sButton.Old_value--;
            break;
            
        case _LCD_SCR_SETTING_P3_6:
            if(sButton.Old_value - 1 > sParaDisplay.Humi_Lower_i32)
                sButton.Old_value--;
            break;
            
        default:
            break;
    }
}

void BUTTON_ESC_Process (void)
{
    switch (sLCD.sScreenNow.Index_u8)
    {
        case _LCD_SCR_PASS_WORD:
        case _LCD_SCR_SETTING_P:
            sLCD.sScreenNow.Index_u8 = _LCD_SCREEN;
            break;
            
        case _LCD_SCR_SETTING_P1:
        case _LCD_SCR_SETTING_P2:
        case _LCD_SCR_SETTING_P3:
            sIndexLCD.Index_LCD_P1 = 1;
            sIndexLCD.Index_LCD_P2 = 1;
            sIndexLCD.Index_LCD_P3 = 1;
            UTIL_MEM_cpy(&sLCD.sScreenBack, &sLCD.sScreenNow, sizeof(sScreenInformation));
            
            Display_Set_Screen(&sLCD.sScreenNow, _LCD_SCR_SETTING_P, 0,
                               __SETTING_P, __SETTING_P, __SETTING_P,
                                &sIndexLCD.Index_LCD_P, 0xF2);
            break;
            
        case _LCD_SCR_SETTING_P1_1:
        case _LCD_SCR_SETTING_P1_2:
        case _LCD_SCR_SETTING_P2_1:
        case _LCD_SCR_SETTING_P2_2:
        case _LCD_SCR_SETTING_P3_1:
        case _LCD_SCR_SETTING_P3_2:
        case _LCD_SCR_SETTING_P3_3:    
        case _LCD_SCR_SETTING_P3_4:
        case _LCD_SCR_SETTING_P3_5:
        case _LCD_SCR_SETTING_P3_6:
            UTIL_MEM_cpy(&sLCD.sScreenNow, &sLCD.sScreenBack, sizeof(sScreenInformation));
            break;
            
        default:
            break;
    }
}



