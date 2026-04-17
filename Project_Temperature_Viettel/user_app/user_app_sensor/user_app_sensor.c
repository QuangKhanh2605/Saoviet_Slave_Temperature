#include "user_app_sensor.h"
#include "user_define.h"
#include "i2c.h"
//#include "dac.h"
#include "user_convert_variable.h"
#include "user_app_modbus_rtu.h"
#include "iwdg.h"
#include "HS300x.h"

/*=========================Fucntion Static=========================*/
static uint8_t fevent_sensor_entry(uint8_t event);
static uint8_t fevent_sensor_handle(uint8_t event);
static uint8_t fevent_sensor_dac(uint8_t event);
static uint8_t fevent_sensor_reset(uint8_t event);

static uint8_t fevent_temp_alarm(uint8_t event);
static uint8_t fevent_refresh_iwdg(uint8_t event);

static uint8_t fevent_lcd_backlight(uint8_t event);
static uint8_t fevent_lcd_warning(uint8_t event);

static uint8_t fevent_lcd_led_rs485_tx(uint8_t event);
static uint8_t fevent_lcd_led_rs485_rx(uint8_t event);
/*==============================Struct=============================*/
sEvent_struct               sEventAppSensor[]=
{
  {_EVENT_SENSOR_ENTRY,              1, 5, 2,                fevent_sensor_entry},            //Doi slave khoi dong moi truyen opera
  {_EVENT_SENSOR_HANDLE,             1, 5, 2000,             fevent_sensor_handle}, 
  {_EVENT_SENSOR_DAC,                1, 5, 500,              fevent_sensor_dac},
  {_EVENT_SENSOR_RESET,              1, 5, 10000,            fevent_sensor_reset},
  
  {_EVENT_TEMP_ALARM,                1, 5, 2000,             fevent_temp_alarm},
  {_EVENT_REFRESH_IWDG,              1, 5, 500,              fevent_refresh_iwdg},
  
  {_EVENT_LCD_BACKLIGHT,             1, 5, 2,                fevent_lcd_backlight},
  {_EVENT_LCD_WARNING,               1, 5, 750,              fevent_lcd_warning},
  
  {_EVENT_LDC_LED_RS485_TX,          1, 5, 80,               fevent_lcd_led_rs485_tx},
  {_EVENT_LDC_LED_RS485_RX,          1, 5, 80,               fevent_lcd_led_rs485_rx},
};

struct_SensorTemp   sSensorTemp = {0};
struct_TempAlarm    sTempAlarm = {0};
struct_CalibDAC     sCalibDAC = {0};

struct_Measure      sMeasure = {0};
struct_Offset       sOffset = {0};
struct_Alarm        sAlarm = {0};

sStructCtrlLed      sCtrlLed = {0};
/*========================Function Handle========================*/
static uint8_t fevent_sensor_entry(uint8_t event)
{
//    HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
    HAL_GPIO_WritePin(ON_SEN_GPIO_Port, ON_SEN_Pin, GPIO_PIN_RESET);
    return 1;
}


    
static uint8_t fevent_sensor_handle(uint8_t event)
{
//    uint8_t data[2];
//    uint16_t raw;
    int16_t tem=0;
    int16_t humi=0;
    static uint8_t Count_Disconnect = 0;
    uint8_t State_Recv = 0;
    uint8_t buf[10] = {0};
    float Stamp = 0;
    
    uint8_t state_sensor = 0;
    
    // Ð?c Ambient Temperature (TA)
    HAL_GPIO_WritePin(ON_SEN_GPIO_Port, ON_SEN_Pin, GPIO_PIN_RESET);
    
//    if(HAL_I2C_Mem_Read(&hi2c1, MLX90614_ADDR, MLX90614_TA, I2C_MEMADD_SIZE_8BIT, data, 2, HAL_MAX_DELAY) == HAL_OK)
//    {
//        raw = ((uint16_t)data[1] << 8) | data[0];
//        sSensorTemp.TempAmbient_f = (raw * 0.02) - 273.15;
//    }
//    else
//    {
//        sSensorTemp.TempAmbient_f = 0;
//        MX_I2C1_Init();
//    }
//
//    // Ð?c Object Temperature (TO)
//    if(HAL_I2C_Mem_Read(&hi2c1, MLX90614_ADDR, MLX90614_TOBJ1, I2C_MEMADD_SIZE_8BIT, data, 2, HAL_MAX_DELAY) == HAL_OK)
//    {
//        raw = ((uint16_t)data[1] << 8) | data[0];
//        sSensorTemp.TempObject_f = (raw * 0.02) - 273.15;
//    }
//    else
//    {
//        sSensorTemp.TempObject_f = 0;
//        MX_I2C1_Init();
//    }
    
    if (HAL_I2C_IsDeviceReady(&hi2c2, HS300X_SENSON_ADDR << 1 , 2, 100) == HAL_OK)
    {
        state_sensor++;
    }
    if (HAL_I2C_Master_Receive(&hi2c2, HS300X_SENSON_ADDR << 1, buf, 4, 100) == HAL_OK)
    {
        state_sensor++;
    }

    if(HS300X_Start_Measurement(&hi2c2, (int16_t*)&tem, (int16_t*)&humi)==1 || state_sensor < 2)
    {
        tem=0; 
        humi=0;
        Count_Disconnect++;
        State_Recv = 0;
    }
    else
    {
        fevent_enable(sEventAppSensor, _EVENT_SENSOR_RESET);
        Count_Disconnect = 0;
        State_Recv = 1;
    }
    
    if(State_Recv == 1)
    {
        sMeasure.sConnect_u8 = 1;
        sMeasure.Temp_Value_f = (float)tem/100;
        sMeasure.Humi_Value_f = (float)humi;

        Stamp = sMeasure.Temp_Value_f + sOffset.Temp_f;
        sMeasure.Temp_Filter_f = ((Stamp) > TEMP_MAX) ? TEMP_MAX : ((Stamp) < TEMP_MIN ? TEMP_MIN : (Stamp));
        
        Stamp = sMeasure.Humi_Value_f + sOffset.Humi_f;
        sMeasure.Humi_Filter_f = ((Stamp) > HUMI_MAX) ? HUMI_MAX : ((Stamp) < HUMI_MIN ? HUMI_MIN : (Stamp));
        
        sMeasure.Temp_Filter_f = Filter_pH(sMeasure.Temp_Filter_f);
        sMeasure.Humi_Filter_f = sMeasure.Humi_Filter_f;
    }
    
    if(Count_Disconnect >= 3)
    {
        Count_Disconnect = 3;
        sMeasure.sConnect_u8 = 0;
        sMeasure.Temp_Filter_f = 0;
        sMeasure.Humi_Filter_f = 0;
    }
    
    fevent_enable(sEventAppSensor, event);
    return 1; 
}
        
static uint8_t fevent_sensor_dac(uint8_t event)
{
//    uint32_t Data = 0;
//    float stamp = 0;
//    float stamp_2 = 0;
//    
//    if(sCalibDAC.stateDAC_u8 == 0)
//    {
//        if(sSensorTemp.TempObject_f < TEMPERATURE_LOWER)
//        {
//            Data = 0;
//        }
//        else
//        {
//            stamp = (sSensorTemp.TempObject_f-TEMPERATURE_LOWER)/(TEMPERATURE_UPPER-TEMPERATURE_LOWER);
//            stamp = stamp * (sCalibDAC.DAC_Max_u16 - sCalibDAC.DAC_Min_u16) + sCalibDAC.DAC_Min_u16;
//            Data = (uint32_t)(stamp);
//        }
//    }
//    else
//    {
//        if(sCalibDAC.DAC_ATcmd_u16 < sCalibDAC.DAC_Min_u16)
//            Data = sCalibDAC.DAC_Min_u16;
//        else if (sCalibDAC.DAC_ATcmd_u16 > sCalibDAC.DAC_Max_u16)
//            Data = sCalibDAC.DAC_Max_u16;
//        else
//            Data = sCalibDAC.DAC_ATcmd_u16;
//    }
//    
//    HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, Data);
//    
//    stamp_2 = (float)(Data - sCalibDAC.DAC_Min_u16)/(sCalibDAC.DAC_Max_u16 - sCalibDAC.DAC_Min_u16);
//    sSensorTemp.CurrOutValue_f = (stamp_2*(CURR_OUT_MAX - CURR_OUT_MIN)) + CURR_OUT_MIN;
//    
    fevent_enable(sEventAppSensor, event);
    return 1; 
}

static uint8_t fevent_sensor_reset(uint8_t event)
{
    MX_I2C2_Init();
    HAL_GPIO_WritePin(ON_SEN_GPIO_Port, ON_SEN_Pin, GPIO_PIN_SET);
    fevent_enable(sEventAppSensor, _EVENT_SENSOR_HANDLE);
    
    fevent_enable(sEventAppSensor, event);
    return 1; 
}

static uint8_t fevent_temp_alarm(uint8_t event)
{
    if(sTempAlarm.State == 1)
    {
        if((sSensorTemp.TempObject_f > sTempAlarm.Alarm_Upper) || 
           (sSensorTemp.TempObject_f < sTempAlarm.Alarm_Lower))
        {
            ALARM_ON;
        }
        else
            ALARM_OFF;
    }
    else 
        ALARM_OFF;
    
    fevent_enable(sEventAppSensor, event);
    return 1;
}

static uint8_t fevent_refresh_iwdg(uint8_t event)
{
    HAL_IWDG_Refresh(&hiwdg);
    fevent_enable(sEventAppSensor, event);
    return 1;
}

static uint8_t fevent_lcd_backlight(uint8_t event)
{
    static uint8_t state_button = 0;
    static uint8_t last_state = 1;
    static uint32_t last_time = 0;

    uint8_t current_state = HAL_GPIO_ReadPin(SW_SET_GPIO_Port, SW_SET_Pin);

    if(current_state != last_state)
    {
        if(HAL_GetTick() - last_time > 20) 
        {
            last_time = HAL_GetTick();

            if(last_state == 1 && current_state == 0)
            {
                last_state = current_state;
                state_button = 1;
            }

            last_state = current_state;
        }
    }
    
    
    if(state_button == 1)
    {
        Save_StateBackLight(!sCtrlLed.BackLight);
        state_button = 0;
    }

    fevent_enable(sEventAppSensor, event);
    return 1;
}

static uint8_t fevent_lcd_warning(uint8_t event)
{
    uint8_t Result = 0;
    sAlarm.State_Temp_u8 = 0;
    sAlarm.State_Humi_u8 = 0;
    if(sAlarm.Mode_Temp_u8 == 1)
    {
        if(sMeasure.Temp_Filter_f < sAlarm.Lower_Temp_f || sMeasure.Temp_Filter_f > sAlarm.Upper_Temp_f)
        {
            Result = true;
            sAlarm.State_Temp_u8 = 1;
        }
    }
    
    if(sAlarm.Mode_Humi_u8 == 1)
    {
        if(sMeasure.Humi_Filter_f < sAlarm.Lower_Humi_f || sMeasure.Humi_Filter_f > sAlarm.Upper_Humi_f)
        {
            Result = true;
            sAlarm.State_Humi_u8 = 1;
        }
    }
  
    if(Result == true)
    {
        HAL_GPIO_TogglePin(LED_BL_GPIO_Port, LED_BL_Pin);
    }
    else
    {
        if(sCtrlLed.BackLight == 1)
            HAL_GPIO_WritePin(LED_BL_GPIO_Port, LED_BL_Pin, GPIO_PIN_RESET);
        else
            HAL_GPIO_WritePin(LED_BL_GPIO_Port, LED_BL_Pin, GPIO_PIN_SET);
    }
    
    fevent_enable(sEventAppSensor, event);
    return 1;
}

static uint8_t fevent_lcd_led_rs485_tx(uint8_t event)
{
    if(sCtrlLed.Led_Tx > 0)
    {
        if(sEventAppSensor[_EVENT_LDC_LED_RS485_TX].e_period == 60)
        {
            HAL_GPIO_WritePin(LED_TX485_GPIO_Port, LED_TX485_Pin, GPIO_PIN_SET);
            sEventAppSensor[_EVENT_LDC_LED_RS485_TX].e_period = 30;
        }
        else
        {
            HAL_GPIO_WritePin(LED_TX485_GPIO_Port, LED_TX485_Pin, GPIO_PIN_RESET);
            sEventAppSensor[_EVENT_LDC_LED_RS485_TX].e_period = 60;
            sCtrlLed.Led_Tx--;
        }
    }
    fevent_enable(sEventAppSensor, event);
    return 1;
}

static uint8_t fevent_lcd_led_rs485_rx(uint8_t event)
{
    if(sCtrlLed.Led_Rx > 0)
    {
        if(sEventAppSensor[_EVENT_LDC_LED_RS485_RX].e_period == 60)
        {
            HAL_GPIO_WritePin(LED_RX485_GPIO_Port, LED_RX485_Pin, GPIO_PIN_SET);
            sEventAppSensor[_EVENT_LDC_LED_RS485_RX].e_period = 30;
        }
        else
        {
            HAL_GPIO_WritePin(LED_RX485_GPIO_Port, LED_RX485_Pin, GPIO_PIN_RESET);
            sEventAppSensor[_EVENT_LDC_LED_RS485_RX].e_period = 60;
            sCtrlLed.Led_Rx--;
        }
    }
    fevent_enable(sEventAppSensor, event);
    return 1;
}
/*=====================Filter Temperature====================*/
float Filter_pH(float var)
{
    //Kalman Filter
    static float x_est = 0.0;   // Uoc luong ban dau
    static float P = 1.0;       // Hiep phuong sai ban dau
    
    static float x_est_last = 0;
  
//    float Q = 0.000000001;  // Nhieu mo hinh
//    float R = 0.000001;   // Nhieu cam bien
    float Q = 0.000000001;  // Nhieu mo hinh
    float R = 0.000001;   // Nhieu cam bien

    float x_pred, P_pred, K;
    
    float varFloat = 0;
//    int32_t varInt32 = 0;
    
    if(var != 0)
    {
//        varFloat = Handle_int32_To_Float_Scale(var, scale);
        varFloat = var;
        
        //Thay doi nhanh du lieu
        if(x_est_last - varFloat > 0.5 || varFloat - x_est_last > 0.5)
        {
           Q *=1000; 
        }
        
        // Buoc du doan
        x_pred = x_est;
        P_pred = P + Q;

        // Tinh he so kalman
        K = P_pred / (P_pred + R);

        // Cap nhat gia tri
        x_est = x_pred + K * (varFloat - x_pred);
        P = (1 - K) * P_pred;
        
//        varInt32 = Hanlde_Float_To_Int32_Scale_Round(x_est, scale);
    }
    else
    {
        P = 1;
        x_est = 0;
    }
    x_est_last = x_est;
//    return varInt32;
    return x_est;
}

float ConvertTemperature_Calib(float var)
{
    float result = 0;
    // Tinh he so a và b cua phuong trinh y = a*pH + b
    float a = 0;
    float b = 0;
    
    if((sSensorTemp.CalibPoint1_x_f != sSensorTemp.CalibPoint2_x_f) && (sSensorTemp.CalibPoint1_y_f != sSensorTemp.CalibPoint2_y_f))
    {
        a = (sSensorTemp.CalibPoint2_y_f  - sSensorTemp.CalibPoint1_y_f) / (sSensorTemp.CalibPoint2_x_f - sSensorTemp.CalibPoint1_x_f);
        b = sSensorTemp.CalibPoint1_y_f - a * sSensorTemp.CalibPoint1_x_f;
        result = a * var + b;
    }
    else
    {
        result = var;
    }
    
    result = result + sSensorTemp.Calib_Offset;
    
    if(result < TEMP_MIN)
      result = TEMP_MIN;
    
    if(result > TEMP_MAX)
      result = TEMP_MAX;
    
    return result;
}

/*==================Save and Init Alarm===================*/
void       Save_AlarmSensor(uint8_t eKind, void *Value_p)
{
    uint8_t aData[200] = {0};
    uint8_t length = 0;
  
    uint32_t hexUint_E_LOWER_TEMP  = 0;
    uint32_t hexUint_E_UPPER_TEMP  = 0;
    uint32_t hexUint_E_LOWER_HUMI  = 0;
    uint32_t hexUint_E_UPPER_HUMI  = 0;

    switch(eKind)
    {
        case _E_MODE_TEMP:
            sAlarm.Mode_Temp_u8 = *((uint8_t *)Value_p);
            break;
        case _E_LOWER_TEMP:
            sAlarm.Lower_Temp_f = *((float *)Value_p);
            break;
        case _E_UPPER_TEMP:
            sAlarm.Upper_Temp_f = *((float *)Value_p);
            break;
        case _E_MODE_HUMI:
            sAlarm.Mode_Humi_u8 = *((uint8_t *)Value_p);
            break;
        case _E_LOWER_HUMI:
            sAlarm.Lower_Humi_f = *((float *)Value_p);
            break;
        case _E_UPPER_HUMI:
            sAlarm.Upper_Humi_f = *((float *)Value_p);
            break;
          
        default:
          break;
    }

    hexUint_E_LOWER_TEMP  = Handle_Float_To_hexUint32(sAlarm.Lower_Temp_f);
    hexUint_E_UPPER_TEMP  = Handle_Float_To_hexUint32(sAlarm.Upper_Temp_f);
    hexUint_E_LOWER_HUMI  = Handle_Float_To_hexUint32(sAlarm.Lower_Humi_f);
    hexUint_E_UPPER_HUMI  = Handle_Float_To_hexUint32(sAlarm.Upper_Humi_f);
    
    aData[length++] = sAlarm.Mode_Temp_u8;
    
    aData[length++] = hexUint_E_LOWER_TEMP >> 24;
    aData[length++] = hexUint_E_LOWER_TEMP >> 16;
    aData[length++] = hexUint_E_LOWER_TEMP >> 8;
    aData[length++] = hexUint_E_LOWER_TEMP ;
    
    aData[length++] = hexUint_E_UPPER_TEMP >> 24;
    aData[length++] = hexUint_E_UPPER_TEMP >> 16;
    aData[length++] = hexUint_E_UPPER_TEMP >> 8;
    aData[length++] = hexUint_E_UPPER_TEMP ;
    
    aData[length++] = sAlarm.Mode_Humi_u8;
    
    aData[length++] = hexUint_E_LOWER_HUMI >> 24;
    aData[length++] = hexUint_E_LOWER_HUMI >> 16;
    aData[length++] = hexUint_E_LOWER_HUMI >> 8;
    aData[length++] = hexUint_E_LOWER_HUMI ;
    
    aData[length++] = hexUint_E_UPPER_HUMI >> 24;
    aData[length++] = hexUint_E_UPPER_HUMI >> 16;
    aData[length++] = hexUint_E_UPPER_HUMI >> 8;
    aData[length++] = hexUint_E_UPPER_HUMI ;

    Save_Array(ADDR_ALARM_SENSOR, aData, length);
}

void       Init_AlarmSensor(void)
{
#ifdef USING_APP_SENSOR
    uint32_t hexUint_E_LOWER_TEMP  = 0;
    uint32_t hexUint_E_UPPER_TEMP  = 0;
    uint32_t hexUint_E_LOWER_HUMI  = 0;
    uint32_t hexUint_E_UPPER_HUMI  = 0;
  
    if(*(__IO uint8_t*)(ADDR_ALARM_SENSOR) != FLASH_BYTE_EMPTY)
    {
        sAlarm.Mode_Temp_u8    = *(__IO uint8_t*)(ADDR_ALARM_SENSOR+2);
        
        hexUint_E_LOWER_TEMP  = *(__IO uint8_t*)(ADDR_ALARM_SENSOR+3) << 24;
        hexUint_E_LOWER_TEMP  |= *(__IO uint8_t*)(ADDR_ALARM_SENSOR+4)<< 16;
        hexUint_E_LOWER_TEMP  |= *(__IO uint8_t*)(ADDR_ALARM_SENSOR+5)<< 8;
        hexUint_E_LOWER_TEMP  |= *(__IO uint8_t*)(ADDR_ALARM_SENSOR+6);
        
        hexUint_E_UPPER_TEMP  = *(__IO uint8_t*)(ADDR_ALARM_SENSOR+7) << 24;
        hexUint_E_UPPER_TEMP  |= *(__IO uint8_t*)(ADDR_ALARM_SENSOR+8)<< 16;
        hexUint_E_UPPER_TEMP  |= *(__IO uint8_t*)(ADDR_ALARM_SENSOR+9)<< 8;
        hexUint_E_UPPER_TEMP  |= *(__IO uint8_t*)(ADDR_ALARM_SENSOR+10);
        
        sAlarm.Mode_Humi_u8    = *(__IO uint8_t*)(ADDR_ALARM_SENSOR+11);
        
        hexUint_E_LOWER_HUMI  = *(__IO uint8_t*)(ADDR_ALARM_SENSOR+12) << 24;
        hexUint_E_LOWER_HUMI  |= *(__IO uint8_t*)(ADDR_ALARM_SENSOR+13)<< 16;
        hexUint_E_LOWER_HUMI  |= *(__IO uint8_t*)(ADDR_ALARM_SENSOR+14)<< 8;
        hexUint_E_LOWER_HUMI  |= *(__IO uint8_t*)(ADDR_ALARM_SENSOR+15);
        
        hexUint_E_UPPER_HUMI  = *(__IO uint8_t*)(ADDR_ALARM_SENSOR+16) << 24;
        hexUint_E_UPPER_HUMI  |= *(__IO uint8_t*)(ADDR_ALARM_SENSOR+17)<< 16;
        hexUint_E_UPPER_HUMI  |= *(__IO uint8_t*)(ADDR_ALARM_SENSOR+18)<< 8;
        hexUint_E_UPPER_HUMI  |= *(__IO uint8_t*)(ADDR_ALARM_SENSOR+19);
        
        Convert_uint32Hex_To_Float(hexUint_E_LOWER_TEMP, &sAlarm.Lower_Temp_f);
        Convert_uint32Hex_To_Float(hexUint_E_UPPER_TEMP, &sAlarm.Upper_Temp_f);
        Convert_uint32Hex_To_Float(hexUint_E_LOWER_HUMI, &sAlarm.Lower_Humi_f);
        Convert_uint32Hex_To_Float(hexUint_E_UPPER_HUMI, &sAlarm.Upper_Humi_f);
    }
    else
    {
        sAlarm.Mode_Temp_u8 = 0;
        sAlarm.Lower_Temp_f = TEMP_MIN;
        sAlarm.Upper_Temp_f = TEMP_MAX;
        
        sAlarm.Mode_Humi_u8 = 0;
        sAlarm.Lower_Humi_f = HUMI_MIN;
        sAlarm.Upper_Humi_f = HUMI_MAX;
    }
#endif   
}   
/*===================Save and Init Calib====================*/
void Save_OffsetSensor(float Temp_Offset_f, float Humi_Offset_f)
{
#ifdef USING_APP_SENSOR
    uint8_t aData[50] = {0};
    uint8_t length = 0;
  
    uint32_t hexUint_Compensation_Temp = 0;
    uint32_t hexUint_Compensation_Humi = 0;
    
    sOffset.Temp_f = Temp_Offset_f;
    sOffset.Humi_f = Humi_Offset_f;
    
    hexUint_Compensation_Temp  = Handle_Float_To_hexUint32(sOffset.Temp_f);
    hexUint_Compensation_Humi  = Handle_Float_To_hexUint32(sOffset.Humi_f);
    
    aData[length++] = hexUint_Compensation_Temp >> 24;
    aData[length++] = hexUint_Compensation_Temp >> 16;
    aData[length++] = hexUint_Compensation_Temp >> 8;
    aData[length++] = hexUint_Compensation_Temp ;
    
    aData[length++] = hexUint_Compensation_Humi >> 24;
    aData[length++] = hexUint_Compensation_Humi >> 16;
    aData[length++] = hexUint_Compensation_Humi >> 8;
    aData[length++] = hexUint_Compensation_Humi ;

    Save_Array(ADDR_OFFSET_SENSOR, aData, length);
#endif   
}

void Init_OffsetSensor(void)
{
#ifdef USING_APP_SENSOR
    uint32_t hexUint_Compensation_Temp = 0;
    uint32_t hexUint_Compensation_Humi = 0;
  
    if(*(__IO uint8_t*)(ADDR_OFFSET_SENSOR) != FLASH_BYTE_EMPTY)
    {
        hexUint_Compensation_Temp  = *(__IO uint8_t*)(ADDR_OFFSET_SENSOR+2) << 24;
        hexUint_Compensation_Temp  |= *(__IO uint8_t*)(ADDR_OFFSET_SENSOR+3)<< 16;
        hexUint_Compensation_Temp  |= *(__IO uint8_t*)(ADDR_OFFSET_SENSOR+4)<< 8;
        hexUint_Compensation_Temp  |= *(__IO uint8_t*)(ADDR_OFFSET_SENSOR+5);
        
        hexUint_Compensation_Humi  = *(__IO uint8_t*)(ADDR_OFFSET_SENSOR+6) << 24;
        hexUint_Compensation_Humi  |= *(__IO uint8_t*)(ADDR_OFFSET_SENSOR+7)<< 16;
        hexUint_Compensation_Humi  |= *(__IO uint8_t*)(ADDR_OFFSET_SENSOR+8)<< 8;
        hexUint_Compensation_Humi  |= *(__IO uint8_t*)(ADDR_OFFSET_SENSOR+9);
        
        Convert_uint32Hex_To_Float(hexUint_Compensation_Temp, &sOffset.Temp_f);
        Convert_uint32Hex_To_Float(hexUint_Compensation_Humi, &sOffset.Humi_f);
    }
    else
    {
        sOffset.Temp_f = 0;
        sOffset.Humi_f = 0;
    }
#endif   
}
/*===================Save and Init Calib====================*/
void Save_CalibTemperature(uint8_t eKind, float var)
{
#ifdef USING_APP_SENSOR
    uint8_t aData[50] = {0};
    uint8_t length = 0;
  
    uint32_t hexUint_Compensation   = 0;
    uint32_t hexUint_CalibPoint1_x  = 0;
    uint32_t hexUint_CalibPoint1_y  = 0;
    uint32_t hexUint_CalibPoint2_x  = 0;
    uint32_t hexUint_CalibPoint2_y  = 0;
    
    switch(eKind)
    {
        case _KIND_CALIB_OFFSET:
          sSensorTemp.Calib_Offset = var;
          break;
          
        case _KIND_CALIB_POINT_1:
          sSensorTemp.CalibPoint1_x_f = sSensorTemp.TempObject_f;
          sSensorTemp.CalibPoint1_y_f = var;
          break;
          
        case _KIND_CALIB_POINT_2:
          sSensorTemp.CalibPoint2_x_f = sSensorTemp.TempObject_f;
          sSensorTemp.CalibPoint2_y_f = var;
          break;
        
        default:
          break;
    }
    
    hexUint_Compensation   = Handle_Float_To_hexUint32(sSensorTemp.Calib_Offset);
    hexUint_CalibPoint1_x  = Handle_Float_To_hexUint32(sSensorTemp.CalibPoint1_x_f);
    hexUint_CalibPoint1_y  = Handle_Float_To_hexUint32(sSensorTemp.CalibPoint1_y_f);
    hexUint_CalibPoint2_x  = Handle_Float_To_hexUint32(sSensorTemp.CalibPoint2_x_f);
    hexUint_CalibPoint2_y  = Handle_Float_To_hexUint32(sSensorTemp.CalibPoint2_y_f);
    
    aData[length++] = hexUint_Compensation >> 24;
    aData[length++] = hexUint_Compensation >> 16;
    aData[length++] = hexUint_Compensation >> 8;
    aData[length++] = hexUint_Compensation ;
    
    aData[length++] = hexUint_CalibPoint1_x >> 24;
    aData[length++] = hexUint_CalibPoint1_x >> 16;
    aData[length++] = hexUint_CalibPoint1_x >> 8;
    aData[length++] = hexUint_CalibPoint1_x ;
    
    aData[length++] = hexUint_CalibPoint1_y >> 24;
    aData[length++] = hexUint_CalibPoint1_y >> 16;
    aData[length++] = hexUint_CalibPoint1_y >> 8;
    aData[length++] = hexUint_CalibPoint1_y ;
    
    aData[length++] = hexUint_CalibPoint2_x >> 24;
    aData[length++] = hexUint_CalibPoint2_x >> 16;
    aData[length++] = hexUint_CalibPoint2_x >> 8;
    aData[length++] = hexUint_CalibPoint2_x ;
    
    aData[length++] = hexUint_CalibPoint2_y >> 24;
    aData[length++] = hexUint_CalibPoint2_y >> 16;
    aData[length++] = hexUint_CalibPoint2_y >> 8;
    aData[length++] = hexUint_CalibPoint2_y ;

    Save_Array(ADDR_CALIB_TEMPERATURE, aData, length);
#endif   
}

void Init_CalibTemperature(void)
{
#ifdef USING_APP_SENSOR
    uint32_t hexUint_Compensation   = 0;
    uint32_t hexUint_CalibPoint1_x  = 0;
    uint32_t hexUint_CalibPoint1_y  = 0;
    uint32_t hexUint_CalibPoint2_x  = 0;
    uint32_t hexUint_CalibPoint2_y  = 0;
  
    if(*(__IO uint8_t*)(ADDR_CALIB_TEMPERATURE) != FLASH_BYTE_EMPTY)
    {
        hexUint_Compensation  = *(__IO uint8_t*)(ADDR_CALIB_TEMPERATURE+2) << 24;
        hexUint_Compensation  |= *(__IO uint8_t*)(ADDR_CALIB_TEMPERATURE+3)<< 16;
        hexUint_Compensation  |= *(__IO uint8_t*)(ADDR_CALIB_TEMPERATURE+4)<< 8;
        hexUint_Compensation  |= *(__IO uint8_t*)(ADDR_CALIB_TEMPERATURE+5);
        
        hexUint_CalibPoint1_x  = *(__IO uint8_t*)(ADDR_CALIB_TEMPERATURE+6) << 24;
        hexUint_CalibPoint1_x  |= *(__IO uint8_t*)(ADDR_CALIB_TEMPERATURE+7)<< 16;
        hexUint_CalibPoint1_x  |= *(__IO uint8_t*)(ADDR_CALIB_TEMPERATURE+8)<< 8;
        hexUint_CalibPoint1_x  |= *(__IO uint8_t*)(ADDR_CALIB_TEMPERATURE+9);
        
        hexUint_CalibPoint1_y  = *(__IO uint8_t*)(ADDR_CALIB_TEMPERATURE+10) << 24;
        hexUint_CalibPoint1_y  |= *(__IO uint8_t*)(ADDR_CALIB_TEMPERATURE+11)<< 16;
        hexUint_CalibPoint1_y  |= *(__IO uint8_t*)(ADDR_CALIB_TEMPERATURE+12)<< 8;
        hexUint_CalibPoint1_y  |= *(__IO uint8_t*)(ADDR_CALIB_TEMPERATURE+13);
        
        hexUint_CalibPoint2_x  = *(__IO uint8_t*)(ADDR_CALIB_TEMPERATURE+14) << 24;
        hexUint_CalibPoint2_x  |= *(__IO uint8_t*)(ADDR_CALIB_TEMPERATURE+15)<< 16;
        hexUint_CalibPoint2_x  |= *(__IO uint8_t*)(ADDR_CALIB_TEMPERATURE+16)<< 8;
        hexUint_CalibPoint2_x  |= *(__IO uint8_t*)(ADDR_CALIB_TEMPERATURE+17);
        
        hexUint_CalibPoint2_y  = *(__IO uint8_t*)(ADDR_CALIB_TEMPERATURE+18) << 24;
        hexUint_CalibPoint2_y  |= *(__IO uint8_t*)(ADDR_CALIB_TEMPERATURE+19)<< 16;
        hexUint_CalibPoint2_y  |= *(__IO uint8_t*)(ADDR_CALIB_TEMPERATURE+20)<< 8;
        hexUint_CalibPoint2_y  |= *(__IO uint8_t*)(ADDR_CALIB_TEMPERATURE+21);
        
        Convert_uint32Hex_To_Float(hexUint_Compensation,  &sSensorTemp.Calib_Offset );
        Convert_uint32Hex_To_Float(hexUint_CalibPoint1_x, &sSensorTemp.CalibPoint1_x_f);
        Convert_uint32Hex_To_Float(hexUint_CalibPoint1_y, &sSensorTemp.CalibPoint1_y_f);
        Convert_uint32Hex_To_Float(hexUint_CalibPoint2_x, &sSensorTemp.CalibPoint2_x_f);
        Convert_uint32Hex_To_Float(hexUint_CalibPoint2_y, &sSensorTemp.CalibPoint2_y_f);
    }
    else
    {
        sSensorTemp.Calib_Offset = 0;
        sSensorTemp.CalibPoint1_x_f = TEMP_MIN;
        sSensorTemp.CalibPoint1_y_f = TEMP_MIN;
        sSensorTemp.CalibPoint2_x_f = TEMP_MAX;
        sSensorTemp.CalibPoint2_y_f = TEMP_MAX;
    }
#endif   
}

void Save_TempAlarm(uint8_t State, float AlarmLower, float AlarmUpper)
{
#ifdef USING_APP_SENSOR
    uint8_t aData[50] = {0};
    uint8_t length = 0;
  
    uint32_t hexUint_AlarmUpper  = 0;
    uint32_t hexUint_AlarmLower  = 0;
    
    sTempAlarm.State = State;
    sTempAlarm.Alarm_Upper = AlarmUpper;
    sTempAlarm.Alarm_Lower = AlarmLower;
    
    hexUint_AlarmUpper  = Handle_Float_To_hexUint32(sTempAlarm.Alarm_Upper);
    hexUint_AlarmLower  = Handle_Float_To_hexUint32(sTempAlarm.Alarm_Lower);
    
    aData[length++] = sTempAlarm.State;
    
    aData[length++] = hexUint_AlarmUpper >> 24;
    aData[length++] = hexUint_AlarmUpper >> 16;
    aData[length++] = hexUint_AlarmUpper >> 8;
    aData[length++] = hexUint_AlarmUpper ;
    
    aData[length++] = hexUint_AlarmLower >> 24;
    aData[length++] = hexUint_AlarmLower >> 16;
    aData[length++] = hexUint_AlarmLower >> 8;
    aData[length++] = hexUint_AlarmLower ;

    Save_Array(ADDR_TEMPERATURE_ALARM, aData, length);
#endif   
}

void Init_TempAlarm(void)
{
#ifdef USING_APP_SENSOR
  
    uint32_t hexUint_AlarmUpper  = 0;
    uint32_t hexUint_AlarmLower  = 0;
  
    if(*(__IO uint8_t*)(ADDR_TEMPERATURE_ALARM) != FLASH_BYTE_EMPTY)
    {
        sTempAlarm.State  = *(__IO uint8_t*)(ADDR_TEMPERATURE_ALARM+2);
      
        hexUint_AlarmUpper  = *(__IO uint8_t*)(ADDR_TEMPERATURE_ALARM+3) << 24;
        hexUint_AlarmUpper  |= *(__IO uint8_t*)(ADDR_TEMPERATURE_ALARM+4)<< 16;
        hexUint_AlarmUpper  |= *(__IO uint8_t*)(ADDR_TEMPERATURE_ALARM+5)<< 8;
        hexUint_AlarmUpper  |= *(__IO uint8_t*)(ADDR_TEMPERATURE_ALARM+6);
        
        hexUint_AlarmLower  = *(__IO uint8_t*)(ADDR_TEMPERATURE_ALARM+7) << 24;
        hexUint_AlarmLower  |= *(__IO uint8_t*)(ADDR_TEMPERATURE_ALARM+8)<< 16;
        hexUint_AlarmLower  |= *(__IO uint8_t*)(ADDR_TEMPERATURE_ALARM+9)<< 8;
        hexUint_AlarmLower  |= *(__IO uint8_t*)(ADDR_TEMPERATURE_ALARM+10);
        
        Convert_uint32Hex_To_Float(hexUint_AlarmUpper, &sTempAlarm.Alarm_Upper);
        Convert_uint32Hex_To_Float(hexUint_AlarmLower, &sTempAlarm.Alarm_Lower);
    }
    else
    {
        sTempAlarm.State = 0;
        sTempAlarm.Alarm_Upper = TEMP_MAX;
        sTempAlarm.Alarm_Lower = TEMP_MIN;
    }
#endif   
}

void Save_CalibDAC(uint16_t DAC_Min, uint16_t DAC_Max)
{
#ifdef USING_APP_SENSOR
    uint8_t aData[50] = {0};
    uint8_t length = 0;
  
    if(DAC_Min < DAC_Max)
    {
        sCalibDAC.DAC_Min_u16 = DAC_Min;
        sCalibDAC.DAC_Max_u16 = DAC_Max;
        
        aData[length++] = sCalibDAC.DAC_Min_u16 >> 8;
        aData[length++] = sCalibDAC.DAC_Min_u16 ;
        
        aData[length++] = sCalibDAC.DAC_Max_u16 >> 8;
        aData[length++] = sCalibDAC.DAC_Max_u16 ;

        Save_Array(ADDR_CALIB_DAC, aData, length);
    }
#endif   
}

void Init_CalibDAC(void)
{
#ifdef USING_APP_SENSOR
  
    if(*(__IO uint8_t*)(ADDR_CALIB_DAC) != FLASH_BYTE_EMPTY)
    {
        sCalibDAC.DAC_Min_u16  |= *(__IO uint8_t*)(ADDR_CALIB_DAC+2)<< 8;
        sCalibDAC.DAC_Min_u16  |= *(__IO uint8_t*)(ADDR_CALIB_DAC+3);
        
        sCalibDAC.DAC_Max_u16  |= *(__IO uint8_t*)(ADDR_CALIB_DAC+4)<< 8;
        sCalibDAC.DAC_Max_u16  |= *(__IO uint8_t*)(ADDR_CALIB_DAC+5);
    }
    else
    {
        sCalibDAC.DAC_Min_u16 = DAC_MIN;
        sCalibDAC.DAC_Max_u16 = DAC_MAX;
    }
#endif   
}

void Save_StateBackLight(uint8_t State)
{
#ifdef USING_APP_SENSOR
    uint8_t aData[50] = {0};
    uint8_t length = 0;

    sCtrlLed.BackLight = State;
    
    aData[length++] = sCtrlLed.BackLight;
    
    Save_Array(ADDR_STATE_BACKLIGHT, aData, length);
    
    if(sCtrlLed.BackLight == 1)
        HAL_GPIO_WritePin(LED_BL_GPIO_Port, LED_BL_Pin, GPIO_PIN_RESET);
    else
        HAL_GPIO_WritePin(LED_BL_GPIO_Port, LED_BL_Pin, GPIO_PIN_SET);
#endif   
}

void Init_StateBackLight(void)
{
#ifdef USING_APP_SENSOR
  
    if(*(__IO uint8_t*)(ADDR_STATE_BACKLIGHT) != FLASH_BYTE_EMPTY)
    {
        sCtrlLed.BackLight = *(__IO uint8_t*)(ADDR_STATE_BACKLIGHT+2);
    }
    else
    {
        sCtrlLed.BackLight = 1;
    }
#endif   
    if(sCtrlLed.BackLight == 1)
        HAL_GPIO_WritePin(LED_BL_GPIO_Port, LED_BL_Pin, GPIO_PIN_RESET);
    else
        HAL_GPIO_WritePin(LED_BL_GPIO_Port, LED_BL_Pin, GPIO_PIN_SET);
}
/*==================Handle Define AT command=================*/
#ifdef USING_AT_CONFIG
void AT_CMD_Reset_Slave(sData *str, uint16_t Pos)
{
    uint8_t aTemp[60] = {0};   
    uint16_t length = 0;
    Save_InforSlaveModbusRTU(ID_DEFAULT, BAUDRATE_DEFAULT);
      
    Insert_String_To_String(aTemp, &length, (uint8_t*)"Reset OK!\r\n",0 , 11);
//	Modem_Respond(PortConfig, aTemp, length, 0);
//    HAL_UART_Transmit(&uart_debug, aTemp,length, 1000);
}

void AT_CMD_Restore_Slave(sData *str, uint16_t Pos)
{
    uint8_t aTemp[60] = {0};   
    uint16_t length = 0;
    OnchipFlashPageErase(ADDR_CALIB_TEMPERATURE);
    sSensorTemp.Calib_Offset = 0;
    sSensorTemp.CalibPoint1_x_f = TEMP_MIN;
    sSensorTemp.CalibPoint1_y_f = TEMP_MIN;
    sSensorTemp.CalibPoint2_x_f = TEMP_MAX;
    sSensorTemp.CalibPoint2_y_f = TEMP_MAX;
    
    OnchipFlashPageErase(ADDR_TEMPERATURE_ALARM);
    sTempAlarm.State = 0;
    sTempAlarm.Alarm_Upper = TEMP_MAX;
    sTempAlarm.Alarm_Lower = TEMP_MIN;
    
    
    Insert_String_To_String(aTemp, &length, (uint8_t*)"Restore OK!\r\n",0 , 13);
//	Modem_Respond(PortConfig, aTemp, length, 0);
//    HAL_UART_Transmit(&uart_debug, aTemp,length, 1000);
}

void AT_CMD_Get_ID_Slave (sData *str, uint16_t Pos)
{
    uint8_t aTemp[50] = "ID Slave: ";   //13 ki tu dau tien
    sData StrResp = {&aTemp[0], 12}; 

    Convert_Point_Int_To_String_Scale (aTemp, &StrResp.Length_u16, (int)(sSlave_ModbusRTU.ID), 0x00);
    Insert_String_To_String(aTemp, &StrResp.Length_u16, (uint8_t*)"\r\n",0 , 2);

//    HAL_UART_Transmit(&uart_debug, StrResp.Data_a8, StrResp.Length_u16, 1000);
}

void AT_CMD_Set_ID_Slave (sData *str_Receiv, uint16_t Pos)
{
    uint32_t TempU32 = 0;
    if( str_Receiv->Data_a8[0] >= '0' && str_Receiv->Data_a8[0] <= '9')
    {
        uint8_t length = 0;
        for(uint8_t i = 0; i < str_Receiv->Length_u16; i++)
        {
            if( str_Receiv->Data_a8[i] < '0' || str_Receiv->Data_a8[i]>'9') break;
            else length++;
        }
        TempU32 = Convert_String_To_Dec(str_Receiv->Data_a8 , length);
        if(TempU32 <= 255 )
        {
            Save_InforSlaveModbusRTU(TempU32, sSlave_ModbusRTU.Baudrate);
//            HAL_UART_Transmit(&uart_debug, (uint8_t*)"OK", 2, 1000);
        }
        else
        {
//            Modem_Respond(PortConfig, (uint8_t*)"ERROR", 5, 0);
//            HAL_UART_Transmit(&uart_debug, (uint8_t*)"ERROR", 5, 1000);
        }
    }
    else
    {
//        HAL_UART_Transmit(&uart_debug, (uint8_t*)"ERROR", 5, 1000);
    }
}

void AT_CMD_Get_BR_Slave (sData *str, uint16_t Pos)
{
    uint8_t aTemp[50] = "BR Slave: ";   //13 ki tu dau tien
    sData StrResp = {&aTemp[0], 12}; 

    Convert_Point_Int_To_String_Scale (aTemp, &StrResp.Length_u16, (int)(sSlave_ModbusRTU.Baudrate), 0x00);
    Insert_String_To_String(aTemp, &StrResp.Length_u16, (uint8_t*)"\r\n",0 , 2);

//    HAL_UART_Transmit(&uart_debug, StrResp.Data_a8, StrResp.Length_u16, 1000);
}

void AT_CMD_Set_BR_Slave (sData *str_Receiv, uint16_t Pos)
{
    uint32_t TempU32 = 0;
    if( str_Receiv->Data_a8[0] >= '0' && str_Receiv->Data_a8[0] <= '9')
    {
        uint8_t length = 0;
        for(uint8_t i = 0; i < str_Receiv->Length_u16; i++)
        {
            if( str_Receiv->Data_a8[i] < '0' || str_Receiv->Data_a8[i]>'9') break;
            else length++;
        }
        TempU32 = Convert_String_To_Dec(str_Receiv->Data_a8 , length);
        if(TempU32 <= 11 )
        {
            Save_InforSlaveModbusRTU(sSlave_ModbusRTU.ID, TempU32);
//            HAL_UART_Transmit(&uart_debug, (uint8_t*)"OK", 2, 1000);
        }
        else
        {
//            Modem_Respond(PortConfig, (uint8_t*)"ERROR", 5, 0);
//            HAL_UART_Transmit(&uart_debug, (uint8_t*)"ERROR", 5, 1000);
        }
    }
    else
    {
//        HAL_UART_Transmit(&uart_debug, (uint8_t*)"ERROR", 5, 1000);
    }
}

void AT_CMD_Get_State_Clb_DAC (sData *str, uint16_t Pos)
{
    uint8_t aTemp[50] = "State DAC: ";   //13 ki tu dau tien
    sData StrResp = {&aTemp[0], 11}; 

    Convert_Point_Int_To_String_Scale (aTemp, &StrResp.Length_u16, (int)(sCalibDAC.stateDAC_u8), 0x00);
    Insert_String_To_String(aTemp, &StrResp.Length_u16, (uint8_t*)"\r\n",0 , 2);

//    HAL_UART_Transmit(&uart_debug, StrResp.Data_a8, StrResp.Length_u16, 1000);
}

void AT_CMD_Set_State_Clb_DAC (sData *str_Receiv, uint16_t Pos)
{
    uint32_t TempU32 = 0;
    if( str_Receiv->Data_a8[0] >= '0' && str_Receiv->Data_a8[0] <= '9')
    {
        uint8_t length = 0;
        for(uint8_t i = 0; i < str_Receiv->Length_u16; i++)
        {
            if( str_Receiv->Data_a8[i] < '0' || str_Receiv->Data_a8[i]>'9') break;
            else length++;
        }
        TempU32 = Convert_String_To_Dec(str_Receiv->Data_a8 , length);
        if(TempU32 <= 1 )
        {
            sCalibDAC.stateDAC_u8 = TempU32;
//            HAL_UART_Transmit(&uart_debug, (uint8_t*)"OK", 2, 1000);
        }
        else
        {
//            HAL_UART_Transmit(&uart_debug, (uint8_t*)"ERROR", 5, 1000);
        }
    }
    else
    {
//        HAL_UART_Transmit(&uart_debug, (uint8_t*)"ERROR", 5, 1000);
    }
}

void AT_CMD_Get_Value_Clb_DAC (sData *str, uint16_t Pos)
{
    uint8_t aTemp[50] = "Value DAC: ";   //13 ki tu dau tien
    sData StrResp = {&aTemp[0], 11}; 

    Convert_Point_Int_To_String_Scale (aTemp, &StrResp.Length_u16, (int)(sCalibDAC.DAC_ATcmd_u16), 0x00);
    Insert_String_To_String(aTemp, &StrResp.Length_u16, (uint8_t*)"\r\n",0 , 2);

//    HAL_UART_Transmit(&uart_debug, StrResp.Data_a8, StrResp.Length_u16, 1000);
}

void AT_CMD_Set_Value_Clb_DAC (sData *str_Receiv, uint16_t Pos)
{
    uint32_t TempU32 = 0;
    if( str_Receiv->Data_a8[0] >= '0' && str_Receiv->Data_a8[0] <= '9')
    {
        uint8_t length = 0;
        for(uint8_t i = 0; i < str_Receiv->Length_u16; i++)
        {
            if( str_Receiv->Data_a8[i] < '0' || str_Receiv->Data_a8[i]>'9') break;
            else length++;
        }
        TempU32 = Convert_String_To_Dec(str_Receiv->Data_a8 , length);
        if(TempU32 <= 4095 )
        {
            sCalibDAC.DAC_ATcmd_u16 = TempU32;
//            HAL_UART_Transmit(&uart_debug, (uint8_t*)"OK", 2, 1000);
        }
        else
        {
//            HAL_UART_Transmit(&uart_debug, (uint8_t*)"ERROR", 5, 1000);
        }
    }
    else
    {
//        HAL_UART_Transmit(&uart_debug, (uint8_t*)"ERROR", 5, 1000);
    }
}

void AT_CMD_Get_Value_DAC_4mA (sData *str, uint16_t Pos)
{
    uint8_t aTemp[50] = "DAC 4mA: ";   //13 ki tu dau tien
    sData StrResp = {&aTemp[0], 9}; 

    Convert_Point_Int_To_String_Scale (aTemp, &StrResp.Length_u16, (int)(sCalibDAC.DAC_Min_u16), 0x00);
    Insert_String_To_String(aTemp, &StrResp.Length_u16, (uint8_t*)"\r\n",0 , 2);

//    HAL_UART_Transmit(&uart_debug, StrResp.Data_a8, StrResp.Length_u16, 1000);
}

void AT_CMD_Set_Value_DAC_4mA (sData *str_Receiv, uint16_t Pos)
{
    uint32_t TempU32 = 0;
    if( str_Receiv->Data_a8[0] >= '0' && str_Receiv->Data_a8[0] <= '9')
    {
        uint8_t length = 0;
        for(uint8_t i = 0; i < str_Receiv->Length_u16; i++)
        {
            if( str_Receiv->Data_a8[i] < '0' || str_Receiv->Data_a8[i]>'9') break;
            else length++;
        }
        TempU32 = Convert_String_To_Dec(str_Receiv->Data_a8 , length);
        if(TempU32 <= 4095 )
        {
            Save_CalibDAC((uint16_t)(TempU32), sCalibDAC.DAC_Max_u16);
//            HAL_UART_Transmit(&uart_debug, (uint8_t*)"OK", 2, 1000);
        }
        else
        {
//            HAL_UART_Transmit(&uart_debug, (uint8_t*)"ERROR", 5, 1000);
        }
    }
    else
    {
//        HAL_UART_Transmit(&uart_debug, (uint8_t*)"ERROR", 5, 1000);
    }
}

void AT_CMD_Get_Value_DAC_20mA (sData *str, uint16_t Pos)
{
    uint8_t aTemp[50] = "DAC 20mA: ";   //13 ki tu dau tien
    sData StrResp = {&aTemp[0], 10}; 

    Convert_Point_Int_To_String_Scale (aTemp, &StrResp.Length_u16, (int)(sCalibDAC.DAC_Max_u16), 0x00);
    Insert_String_To_String(aTemp, &StrResp.Length_u16, (uint8_t*)"\r\n",0 , 2);

//    HAL_UART_Transmit(&uart_debug, StrResp.Data_a8, StrResp.Length_u16, 1000);
}

void AT_CMD_Set_Value_DAC_20mA (sData *str_Receiv, uint16_t Pos)
{
    uint32_t TempU32 = 0;
    if( str_Receiv->Data_a8[0] >= '0' && str_Receiv->Data_a8[0] <= '9')
    {
        uint8_t length = 0;
        for(uint8_t i = 0; i < str_Receiv->Length_u16; i++)
        {
            if( str_Receiv->Data_a8[i] < '0' || str_Receiv->Data_a8[i]>'9') break;
            else length++;
        }
        TempU32 = Convert_String_To_Dec(str_Receiv->Data_a8 , length);
        if(TempU32 <= 4095 )
        {
            Save_CalibDAC(sCalibDAC.DAC_Min_u16, (uint16_t)(TempU32));
//            HAL_UART_Transmit(&uart_debug, (uint8_t*)"OK", 2, 1000);
        }
        else
        {
//            HAL_UART_Transmit(&uart_debug, (uint8_t*)"ERROR", 5, 1000);
        }
    }
    else
    {
//        HAL_UART_Transmit(&uart_debug, (uint8_t*)"ERROR", 5, 1000);
    }
}

void AT_CMD_Get_Current_Out (sData *str, uint16_t Pos)
{
    uint8_t aTemp[50] = "Current: ";   //13 ki tu dau tien
    sData StrResp = {&aTemp[0], 9}; 

    Convert_Point_Int_To_String_Scale (aTemp, &StrResp.Length_u16, (int)(sSensorTemp.CurrOutValue_f*100), 0xFE);
    Insert_String_To_String(aTemp, &StrResp.Length_u16, (uint8_t*)" mA\r\n",0 , 5);

//    HAL_UART_Transmit(&uart_debug, StrResp.Data_a8, StrResp.Length_u16, 1000);
}
#endif
/*==================Handle Task and Init app=================*/
void       Init_AppSensor(void)
{
//    Init_CalibTemperature();
//    Init_TempAlarm();
//    Init_CalibDAC();
    Init_OffsetSensor();
    Init_AlarmSensor();
    Init_StateBackLight();
#ifdef USING_AT_CONFIG
    /* regis cb serial */
    CheckList_AT_CONFIG[_RESET_SLAVE].CallBack = AT_CMD_Reset_Slave;
    CheckList_AT_CONFIG[_RESTORE_SLAVE].CallBack = AT_CMD_Restore_Slave;
    
    CheckList_AT_CONFIG[_GET_ID_SLAVE].CallBack = AT_CMD_Get_ID_Slave;
    CheckList_AT_CONFIG[_SET_ID_SLAVE].CallBack = AT_CMD_Set_ID_Slave;
    CheckList_AT_CONFIG[_GET_BR_SLAVE].CallBack = AT_CMD_Get_BR_Slave;
    CheckList_AT_CONFIG[_SET_BR_SLAVE].CallBack = AT_CMD_Set_BR_Slave;
    
    CheckList_AT_CONFIG[_GET_STATE_CLB_DAC].CallBack = AT_CMD_Get_State_Clb_DAC;
    CheckList_AT_CONFIG[_SET_STATE_CLB_DAC].CallBack = AT_CMD_Set_State_Clb_DAC;
    
    CheckList_AT_CONFIG[_GET_VALUE_CLB_DAC].CallBack = AT_CMD_Get_Value_Clb_DAC;
    CheckList_AT_CONFIG[_SET_VALUE_CLB_DAC].CallBack = AT_CMD_Set_Value_Clb_DAC;
    
    CheckList_AT_CONFIG[_GET_VALUE_DAC_4MA].CallBack = AT_CMD_Get_Value_DAC_4mA;
    CheckList_AT_CONFIG[_SET_VALUE_DAC_4MA].CallBack = AT_CMD_Set_Value_DAC_4mA;
    
    CheckList_AT_CONFIG[_GET_VALUE_DAC_20MA].CallBack = AT_CMD_Get_Value_DAC_20mA;
    CheckList_AT_CONFIG[_SET_VALUE_DAC_20MA].CallBack = AT_CMD_Set_Value_DAC_20mA;
    
    CheckList_AT_CONFIG[_GET_CURRENT_OUT].CallBack = AT_CMD_Get_Current_Out;
#endif
}

uint8_t        AppSensor_Task(void)
{
    uint8_t i = 0;
    uint8_t Result =  false;
    
    for(i = 0; i < _EVENT_SENSOR_END; i++)
    {
        if(sEventAppSensor[i].e_status == 1)
        {
            Result = true; 
            
            if((sEventAppSensor[i].e_systick == 0) ||
               ((HAL_GetTick() - sEventAppSensor[i].e_systick) >= sEventAppSensor[i].e_period))
            {
                sEventAppSensor[i].e_status = 0; //Disable event
                sEventAppSensor[i].e_systick= HAL_GetTick();
                sEventAppSensor[i].e_function_handler(i);
            }
        }
    }
    
    return Result;
}
