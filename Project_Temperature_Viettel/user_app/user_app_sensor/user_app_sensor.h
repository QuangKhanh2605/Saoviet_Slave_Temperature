#ifndef USER_APP_SENSOR_H__
#define USER_APP_SENSOR_H__

#define USING_APP_SENSOR

#include "user_util.h"
#include "event_driven.h"

#define MB7040_I2C_ADDR  (0x70 << 1)  // 8-bit d?a ch? (HAL d?ng d?ng n?y)

#define MLX90614_ADDR   (0x5A << 1) // HAL dùng 8-bit address
#define MLX90614_TA     0x06
#define MLX90614_TOBJ1  0x07

#define ALARM_ON     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
#define ALARM_OFF    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);

#define CURR_OUT_MIN            4
#define CURR_OUT_MAX            20

#define DAC_MIN                 0
#define DAC_MAX                 4095

#define TEMP_MAX                99.99
#define TEMP_MIN                0

#define HUMI_MAX                100
#define HUMI_MIN                0

typedef enum
{
    _EVENT_SENSOR_ENTRY,
    _EVENT_SENSOR_HANDLE,
    _EVENT_SENSOR_DAC,
    _EVENT_SENSOR_RESET,
    
    _EVENT_TEMP_ALARM,
    _EVENT_REFRESH_IWDG,
    
    _EVENT_LCD_BACKLIGHT,
    _EVENT_LCD_WARNING,
    
    _EVENT_LDC_LED_RS485_TX,
    _EVENT_LDC_LED_RS485_RX,
    
    _EVENT_SENSOR_END,
}eKindEventSENSOR;

typedef enum
{
    _KIND_CALIB_OFFSET,
    _KIND_CALIB_POINT_1,
    _KIND_CALIB_POINT_2,
}eKindCalibTemp;

typedef enum
{
    _E_MODE_TEMP,
    _E_LOWER_TEMP,
    _E_UPPER_TEMP,
    _E_MODE_HUMI,
    _E_LOWER_HUMI,
    _E_UPPER_HUMI,
}eKindAlarm;

typedef struct 
{
    float CurrOutValue_f;
    float TempObject_f;
    float TempAmbient_f;
    float Calib_Offset;
    
    float CalibPoint1_x_f;
    float CalibPoint1_y_f;
    
    float CalibPoint2_x_f;
    float CalibPoint2_y_f;
}struct_SensorTemp;

typedef struct
{
    uint8_t sConnect_u8;
    float Temp_Value_f;
    float Humi_Value_f;
    
    float Temp_Filter_f;
    float Humi_Filter_f;
}struct_Measure;

typedef struct
{
    float Temp_f;
    float Humi_f;
}struct_Offset;

typedef struct
{
    uint8_t Mode_Temp_u8;
    float   Lower_Temp_f;
    float   Upper_Temp_f;
    
    uint8_t Mode_Humi_u8;
    float   Lower_Humi_f;
    float   Upper_Humi_f;
    
    uint8_t State_Temp_u8;
    uint8_t State_Humi_u8;
}struct_Alarm;

typedef struct
{
    uint8_t State;
    float Alarm_Lower;
    float Alarm_Upper;
}struct_TempAlarm;

typedef struct
{
    uint8_t stateDAC_u8;
    uint16_t DAC_Min_u16;
    uint16_t DAC_Max_u16;
    uint16_t DAC_ATcmd_u16;
}struct_CalibDAC;

typedef struct
{
    uint8_t BackLight;
    uint8_t Led_Rx;
    uint8_t Led_Tx;
    
    uint8_t LCD_Off;
}sStructCtrlLed;

extern sEvent_struct        sEventAppSensor[];
extern struct_SensorTemp    sSensorTemp;
extern struct_TempAlarm     sTempAlarm;
extern struct_Measure       sMeasure;
extern struct_Offset        sOffset;
extern struct_Alarm         sAlarm;
extern sStructCtrlLed       sCtrlLed;
/*====================Function Handle====================*/

uint8_t    AppSensor_Task(void);
void       Init_AppSensor(void);

void       Save_CalibTemperature(uint8_t eKind, float var);
void       Init_CalibTemperature(void);

void       Save_TempAlarm(uint8_t State, float AlarmLower, float AlarmUpper);
void       Init_TempAlarm(void);

void       Save_CalibDAC(uint16_t DAC_Min, uint16_t DAC_Max);
void       Init_CalibDAC(void);

void       Save_OffsetSensor(float Temp_Offset_f, float Humi_Offset_f);
void       Init_OffsetSensor(void);

void       Save_AlarmSensor(uint8_t eKind, void *Value_p);
void       Init_AlarmSensor(void);
    
void       Save_StateBackLight(uint8_t State);
void       Init_StateBackLight(void);

float Filter_pH(float var);
float ConvertTemperature_Calib(float var);


#endif
