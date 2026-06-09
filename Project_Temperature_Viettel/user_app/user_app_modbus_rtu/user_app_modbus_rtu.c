#include "user_app_modbus_rtu.h"
#include "user_modbus_rtu.h"
#include "user_convert_variable.h"
#include "math.h"

uint8_t NONE_Register_CallBack(sData *str, uint16_t Pos);

uint8_t _Cb_R_ModbusRTU_REG_ID_RS485(sData *str, uint16_t Pos);
uint8_t _Cb_W_ModbusRTU_REG_ID_RS485(sData *str, uint16_t Pos);

uint8_t _Cb_R_ModbusRTU_REG_Baudrate_RS485(sData *str, uint16_t Pos);
uint8_t _Cb_W_ModbusRTU_REG_Baudrate_RS485(sData *str, uint16_t Pos);

uint8_t _Cb_R_ModbusRTU_REG_Temp_Int(sData *str, uint16_t Pos);
uint8_t _Cb_R_ModbusRTU_REG_Temp_Decimal(sData *str, uint16_t Pos);
uint8_t _Cb_R_ModbusRTU_REG_Humi_Int(sData *str, uint16_t Pos);
uint8_t _Cb_R_ModbusRTU_REG_Humi_Decimal(sData *str, uint16_t Pos);

uint8_t _Cb_R_ModbusRTU_REG_Temp_Warning(sData *str, uint16_t Pos);
uint8_t _Cb_R_ModbusRTU_REG_Humi_Warning(sData *str, uint16_t Pos);

uint8_t _Cb_R_ModbusRTU_REG_Temp_Offset_Int(sData *str, uint16_t Pos);
uint8_t _Cb_W_ModbusRTU_REG_Temp_Offset_Int(sData *str, uint16_t Pos);\
  
uint8_t _Cb_R_ModbusRTU_REG_Temp_Alarm(sData *str, uint16_t Pos);
uint8_t _Cb_W_ModbusRTU_REG_Temp_Alarm(sData *str, uint16_t Pos);

uint8_t _Cb_R_ModbusRTU_REG_Humi_Alarm(sData *str, uint16_t Pos);
uint8_t _Cb_W_ModbusRTU_REG_Humi_Alarm(sData *str, uint16_t Pos);

uint8_t _Cb_R_ModbusRTU_REG_Humi_Offset_Int(sData *str, uint16_t Pos);
uint8_t _Cb_W_ModbusRTU_REG_Humi_Offset_Int(sData *str, uint16_t Pos);

uint8_t _Cb_R_ModbusRTU_REG_Temp_Lower_Int(sData *str, uint16_t Pos);
uint8_t _Cb_W_ModbusRTU_REG_Temp_Lower_Int(sData *str, uint16_t Pos);

uint8_t _Cb_R_ModbusRTU_REG_Temp_Upper_Int(sData *str, uint16_t Pos);
uint8_t _Cb_W_ModbusRTU_REG_Temp_Upper_Int(sData *str, uint16_t Pos);

uint8_t _Cb_R_ModbusRTU_REG_Humi_Lower_Int(sData *str, uint16_t Pos);
uint8_t _Cb_W_ModbusRTU_REG_Humi_Lower_Int(sData *str, uint16_t Pos);

uint8_t _Cb_R_ModbusRTU_REG_Humi_Upper_Int(sData *str, uint16_t Pos);
uint8_t _Cb_W_ModbusRTU_REG_Humi_Upper_Int(sData *str, uint16_t Pos);

uint8_t _Cb_R_ModbusRTU_REG_Temp_Float(sData *str, uint16_t Pos);
uint8_t _Cb_R_ModbusRTU_REG_Humi_Float(sData *str, uint16_t Pos);

uint8_t _Cb_R_ModbusRTU_REG_Temp_Offset(sData *str, uint16_t Pos);
uint8_t _Cb_W_ModbusRTU_REG_Temp_Offset(sData *str, uint16_t Pos);

uint8_t _Cb_R_ModbusRTU_REG_Humi_Offset(sData *str, uint16_t Pos);
uint8_t _Cb_W_ModbusRTU_REG_Humi_Offset(sData *str, uint16_t Pos);

uint8_t _Cb_R_ModbusRTU_REG_Temp_Lower(sData *str, uint16_t Pos);
uint8_t _Cb_W_ModbusRTU_REG_Temp_Lower(sData *str, uint16_t Pos);

uint8_t _Cb_R_ModbusRTU_REG_Temp_Upper(sData *str, uint16_t Pos);
uint8_t _Cb_W_ModbusRTU_REG_Temp_Upper(sData *str, uint16_t Pos);

uint8_t _Cb_R_ModbusRTU_REG_Humi_Lower(sData *str, uint16_t Pos);
uint8_t _Cb_W_ModbusRTU_REG_Humi_Lower(sData *str, uint16_t Pos);

uint8_t _Cb_R_ModbusRTU_REG_Humi_Upper(sData *str, uint16_t Pos);
uint8_t _Cb_W_ModbusRTU_REG_Humi_Upper(sData *str, uint16_t Pos);

/*============================ Struct var ============================*/
struct_CheckList_Reg_Modbus_RTU sCheckList_Reg_Modbus_RTU[] =
{
//      id ENUM                     id Reg     Length         Cb_Read         Cb_Write
      {_E_REGISTER_BEGIN,           NULL,      NULL,   NONE_Register_CallBack,           NONE_Register_CallBack},

      {_E_REG_ID_RS485,             0x0000,    1,      _Cb_R_ModbusRTU_REG_ID_RS485,        _Cb_W_ModbusRTU_REG_ID_RS485},
      {_E_REG_TEMP_INT,             0x0001,    1,      _Cb_R_ModbusRTU_REG_Temp_Int,        NONE_Register_CallBack},
      {_E_REG_HUMI_INT,             0x0002,    1,      _Cb_R_ModbusRTU_REG_Humi_Int,        NONE_Register_CallBack},
      {_E_REG_BAUDRATE_RS485,       0x0003,    1,      _Cb_R_ModbusRTU_REG_Baudrate_RS485,  _Cb_W_ModbusRTU_REG_Baudrate_RS485},
      {_E_REG_TEMP_DECIMAL,         0x0004,    1,      _Cb_R_ModbusRTU_REG_Temp_Decimal,    NONE_Register_CallBack},
      {_E_REG_HUMI_DECIMAL,         0x0005,    1,      _Cb_R_ModbusRTU_REG_Humi_Decimal,    NONE_Register_CallBack},
      {_E_REG_TEMP_WARNING,         0x0006,    1,      _Cb_R_ModbusRTU_REG_Temp_Warning,    NONE_Register_CallBack},
      {_E_REG_HUMI_WARNING,         0x0007,    1,      _Cb_R_ModbusRTU_REG_Humi_Warning,    NONE_Register_CallBack},
      {_E_REG_TEMP_OFFSET_INT,      0x0008,    1,      _Cb_R_ModbusRTU_REG_Temp_Offset_Int, _Cb_W_ModbusRTU_REG_Temp_Offset_Int},
      {_E_REG_HUMI_OFFSET_INT,      0x0009,    1,      _Cb_R_ModbusRTU_REG_Humi_Offset_Int, _Cb_W_ModbusRTU_REG_Humi_Offset_Int},
      {_E_REG_TEMP_ALARM,           0x000A,    1,      _Cb_R_ModbusRTU_REG_Temp_Alarm,      _Cb_W_ModbusRTU_REG_Temp_Alarm},
      {_E_REG_HUMI_ALARM,           0x000B,    1,      _Cb_R_ModbusRTU_REG_Humi_Alarm,      _Cb_W_ModbusRTU_REG_Humi_Alarm},
      {_E_REG_TEMP_LOWER_INT,       0x000C,    1,      _Cb_R_ModbusRTU_REG_Temp_Lower_Int,  _Cb_W_ModbusRTU_REG_Temp_Lower_Int},
      {_E_REG_TEMP_UPPER_INT,       0x000D,    1,      _Cb_R_ModbusRTU_REG_Temp_Upper_Int,  _Cb_W_ModbusRTU_REG_Temp_Upper_Int},
      {_E_REG_HUMI_LOWER_INT,       0x000E,    1,      _Cb_R_ModbusRTU_REG_Humi_Lower_Int,  _Cb_W_ModbusRTU_REG_Humi_Lower_Int},
      {_E_REG_HUMI_UPPER_INT,       0x000F,    1,      _Cb_R_ModbusRTU_REG_Humi_Upper_Int,  _Cb_W_ModbusRTU_REG_Humi_Upper_Int},
      
      {_E_REG_TEMP_FLOAT,           0x0010,    2,      _Cb_R_ModbusRTU_REG_Temp_Float,      NONE_Register_CallBack},
      {_E_REG_HUMI_FLOAT,           0x0012,    2,      _Cb_R_ModbusRTU_REG_Humi_Float,      NONE_Register_CallBack},
      {_E_REG_TEMP_OFFSET,          0x0014,    2,      _Cb_R_ModbusRTU_REG_Temp_Offset,     _Cb_W_ModbusRTU_REG_Temp_Offset},
      {_E_REG_HUMI_OFFSET,          0x0016,    2,      _Cb_R_ModbusRTU_REG_Humi_Offset,     _Cb_W_ModbusRTU_REG_Humi_Offset},
      {_E_REG_TEMP_LOWER,           0x0018,    2,      _Cb_R_ModbusRTU_REG_Temp_Lower,      _Cb_W_ModbusRTU_REG_Temp_Lower},
      {_E_REG_TEMP_UPPER,           0x001A,    2,      _Cb_R_ModbusRTU_REG_Temp_Upper,      _Cb_W_ModbusRTU_REG_Temp_Upper},
      {_E_REG_HUMI_LOWER,           0x001C,    2,      _Cb_R_ModbusRTU_REG_Humi_Lower,      _Cb_W_ModbusRTU_REG_Humi_Lower},
      {_E_REG_HUMI_UPPER,           0x001E,    2,      _Cb_R_ModbusRTU_REG_Humi_Upper,      _Cb_W_ModbusRTU_REG_Humi_Upper},
};
static uint8_t aDATA_CONFIG[128];

uint32_t aBaudrate_value[11]={1200,2400,4800,9600,14400,19200,28800,38400,56000,57600,115200};
struct_Slave_ModbusRTU sSlave_ModbusRTU = {1,3};

static uint8_t aDATA_LOG_REGISTER[128];
static uint8_t aDATA_LOG_FRAME[128];

sData   sLogData_ModbusRTU = {aDATA_LOG_REGISTER,0};
sData   sFrameData_ModbusRTU = {aDATA_LOG_FRAME,0};

/*===================== Handle Callback Reg Modbus RTU ===================*/
uint8_t NONE_Register_CallBack(sData *str, uint16_t Pos)
{
    return 0;
}

/*----------- _E_REGISTER_ID -----------*/
uint8_t _Cb_R_ModbusRTU_REG_ID_RS485(sData *str, uint16_t Pos)
{
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = 0;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = sSlave_ModbusRTU.ID;
    return 1;
}
uint8_t _Cb_W_ModbusRTU_REG_ID_RS485(sData *str, uint16_t Pos)
{
    uint16_t ConvertData = 0;
    uint8_t pos = 0;
    pos = Pos;
    ConvertData = str->Data_a8[pos] << 8 | str->Data_a8[pos+1]; 
    if(ConvertData < 256 && ConvertData > 0)
    {
        sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos];
        sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos+1];
        
        Save_InforSlaveModbusRTU(_E_RS485_ID, (uint8_t)ConvertData);
        return 1;
    }
    return 0;
}

/*----------- _E_REGISTER_BR -----------*/
uint8_t _Cb_R_ModbusRTU_REG_Baudrate_RS485(sData *str, uint16_t Pos)
{
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = 0;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = sSlave_ModbusRTU.Baudrate;
    return 1;
}
uint8_t _Cb_W_ModbusRTU_REG_Baudrate_RS485(sData *str, uint16_t Pos)
{
    uint16_t ConvertData = 0;
    uint8_t pos = 0;
    pos = Pos;
    ConvertData = str->Data_a8[pos] << 8 | str->Data_a8[pos+1]; 
    if(ConvertData < 11)
    {
        sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos];
        sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos+1];
        Save_InforSlaveModbusRTU(_E_RS485_BR, (uint8_t)ConvertData);
        return 1;
    }
    return 0;
}

/*----------- _E_REGISTER_Measure Int -----------*/
uint8_t _Cb_R_ModbusRTU_REG_Temp_Int(sData *str, uint16_t Pos)
{
    uint16_t Stamp_u16 = 0;
    Stamp_u16 = (uint16_t)(sMeasure.Temp_Filter_f*Calculator_Scale(sParaDisplay.Scale_Temp));
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = Stamp_u16 >> 8;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = Stamp_u16;
    return 1;
}
uint8_t _Cb_R_ModbusRTU_REG_Temp_Decimal(sData *str, uint16_t Pos)
{
    uint16_t Decimal = Calculator_Scale(sParaDisplay.Scale_Temp);
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = Decimal >>8 ;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = Decimal;
    return 1;
}
uint8_t _Cb_R_ModbusRTU_REG_Humi_Int(sData *str, uint16_t Pos)
{
    uint16_t Stamp_u16 = 0;
    Stamp_u16 = (uint16_t)(sMeasure.Humi_Filter_f*Calculator_Scale(sParaDisplay.Scale_Humi));
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = Stamp_u16 >> 8;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = Stamp_u16;
    return 1;
}
uint8_t _Cb_R_ModbusRTU_REG_Humi_Decimal(sData *str, uint16_t Pos)
{
    uint16_t Decimal = Calculator_Scale(sParaDisplay.Scale_Humi);
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = Decimal >> 8;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = Decimal;
    return 1;
}

/*----------- _E_REGISTER_Warning -----------*/
uint8_t _Cb_R_ModbusRTU_REG_Temp_Warning(sData *str, uint16_t Pos)
{
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = 0x00;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = sAlarm.State_Temp_u8;
    return 1;
}
uint8_t _Cb_R_ModbusRTU_REG_Humi_Warning(sData *str, uint16_t Pos)
{
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = 0x00;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = sAlarm.State_Humi_u8;
    return 1;
}

/*----------- _E_REGISTER_Temp_Offset -----------*/
uint8_t _Cb_R_ModbusRTU_REG_Temp_Offset_Int(sData *str, uint16_t Pos)
{
    int16_t hexValue = 0;
    hexValue = (int16_t)(sOffset.Temp_f*Calculator_Scale(sParaDisplay.Scale_Temp));
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue >> 8;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue;
    return 1;
}
uint8_t _Cb_W_ModbusRTU_REG_Temp_Offset_Int(sData *str, uint16_t Pos)
{
    int16_t ConvertData = 0;
    float Convert_F = 0;
    uint8_t pos = 0;
    pos = Pos;
    ConvertData = str->Data_a8[pos]<<8 | str->Data_a8[pos+1];

    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos];
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos+1];
    
    Convert_F = (float)ConvertData/Calculator_Scale(sParaDisplay.Scale_Temp);
    Save_OffsetSensor(Convert_F, sOffset.Humi_f);
    return 1;
}

/*----------- _E_REGISTER_Humi_Offset -----------*/
uint8_t _Cb_R_ModbusRTU_REG_Humi_Offset_Int(sData *str, uint16_t Pos)
{
    int16_t hexValue = 0;
    hexValue = (int16_t)(sOffset.Humi_f*Calculator_Scale(sParaDisplay.Scale_Humi));
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue >> 8;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue;
    return 1;
}
uint8_t _Cb_W_ModbusRTU_REG_Humi_Offset_Int(sData *str, uint16_t Pos)
{
    int16_t ConvertData = 0;
    float Convert_F = 0;
    uint8_t pos = 0;
    pos = Pos;
    ConvertData = str->Data_a8[pos]<<8 | str->Data_a8[pos+1];

    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos];
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos+1];
    
    Convert_F = (float)ConvertData/Calculator_Scale(sParaDisplay.Scale_Humi);
    Save_OffsetSensor(sOffset.Temp_f, Convert_F);
    return 1;
}


/*----------- _E_REGISTER_Temp_Alarm -----------*/
uint8_t _Cb_R_ModbusRTU_REG_Temp_Alarm(sData *str, uint16_t Pos)
{
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = 0;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = sAlarm.Mode_Temp_u8;
    return 1;
}
uint8_t _Cb_W_ModbusRTU_REG_Temp_Alarm(sData *str, uint16_t Pos)
{
    uint16_t ConvertData = 0;
    uint8_t pos = 0;
    pos = Pos;
    ConvertData = str->Data_a8[pos] << 8 | str->Data_a8[pos+1]; 
    if(ConvertData <= 1)
    {
        sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos];
        sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos+1];
        
        Save_AlarmSensor(_E_MODE_TEMP, &ConvertData);
        return 1;
    }
    return 0;
}

/*----------- _E_REGISTER_Humi_Alarm -----------*/
uint8_t _Cb_R_ModbusRTU_REG_Humi_Alarm(sData *str, uint16_t Pos)
{
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = 0;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = sAlarm.Mode_Humi_u8;
    return 1;
}
uint8_t _Cb_W_ModbusRTU_REG_Humi_Alarm(sData *str, uint16_t Pos)
{
    uint16_t ConvertData = 0;
    uint8_t pos = 0;
    pos = Pos;
    ConvertData = str->Data_a8[pos] << 8 | str->Data_a8[pos+1]; 
    if(ConvertData <= 1)
    {
        sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos];
        sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos+1];
        
        Save_AlarmSensor(_E_MODE_HUMI, &ConvertData);
        return 1;
    }
    return 0;
}

/*----------- _E_REGISTER_Temp_Lower -----------*/
uint8_t _Cb_R_ModbusRTU_REG_Temp_Lower_Int(sData *str, uint16_t Pos)
{
    uint16_t hexValue = 0;
    hexValue = (uint16_t)(sAlarm.Lower_Temp_f*Calculator_Scale(sParaDisplay.Scale_Temp));
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue >> 8;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue;
    return 1;
}
uint8_t _Cb_W_ModbusRTU_REG_Temp_Lower_Int(sData *str, uint16_t Pos)
{
    uint16_t ConvertData = 0;
    float Convert_F = 0;
    uint8_t pos = 0;
    pos = Pos;
    ConvertData = str->Data_a8[pos]<<8 | str->Data_a8[pos+1];

    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos];
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos+1];
    
    Convert_F = (float)ConvertData/Calculator_Scale(sParaDisplay.Scale_Temp);
    Save_AlarmSensor(_E_LOWER_TEMP, &Convert_F);
    return 1;
}

/*----------- _E_REGISTER_Temp_Upper -----------*/
uint8_t _Cb_R_ModbusRTU_REG_Temp_Upper_Int(sData *str, uint16_t Pos)
{
    uint16_t hexValue = 0;
    hexValue = (uint16_t)(sAlarm.Upper_Temp_f*Calculator_Scale(sParaDisplay.Scale_Temp));
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue >> 8;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue;
    return 1;
}
uint8_t _Cb_W_ModbusRTU_REG_Temp_Upper_Int(sData *str, uint16_t Pos)
{
    uint16_t ConvertData = 0;
    float Convert_F = 0;
    uint8_t pos = 0;
    pos = Pos;
    ConvertData = str->Data_a8[pos]<<8 | str->Data_a8[pos+1];

    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos];
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos+1];
    
    Convert_F = (float)ConvertData/Calculator_Scale(sParaDisplay.Scale_Temp);
    Save_AlarmSensor(_E_UPPER_TEMP, &Convert_F);
    return 1;
}

/*----------- _E_REGISTER_Humi_Lower -----------*/
uint8_t _Cb_R_ModbusRTU_REG_Humi_Lower_Int(sData *str, uint16_t Pos)
{
    uint16_t hexValue = 0;
    hexValue = (uint16_t)(sAlarm.Lower_Humi_f*Calculator_Scale(sParaDisplay.Scale_Humi));
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue >> 8;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue;
    return 1;
}
uint8_t _Cb_W_ModbusRTU_REG_Humi_Lower_Int(sData *str, uint16_t Pos)
{
    uint16_t ConvertData = 0;
    float Convert_F = 0;
    uint8_t pos = 0;
    pos = Pos;
    ConvertData = str->Data_a8[pos]<<8 | str->Data_a8[pos+1];

    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos];
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos+1];
    
    Convert_F = (float)ConvertData/Calculator_Scale(sParaDisplay.Scale_Humi);
    Save_AlarmSensor(_E_LOWER_HUMI, &Convert_F);
    return 1;
}

/*----------- _E_REGISTER_Humi_Upper -----------*/
uint8_t _Cb_R_ModbusRTU_REG_Humi_Upper_Int(sData *str, uint16_t Pos)
{
    uint16_t hexValue = 0;
    hexValue = (uint16_t)(sAlarm.Upper_Humi_f*Calculator_Scale(sParaDisplay.Scale_Humi));
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue >> 8;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue;
    return 1;
}
uint8_t _Cb_W_ModbusRTU_REG_Humi_Upper_Int(sData *str, uint16_t Pos)
{
    uint16_t ConvertData = 0;
    float Convert_F = 0;
    uint8_t pos = 0;
    pos = Pos;
    ConvertData = str->Data_a8[pos]<<8 | str->Data_a8[pos+1];

    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos];
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos+1];
    
    Convert_F = (float)ConvertData/Calculator_Scale(sParaDisplay.Scale_Humi);
    Save_AlarmSensor(_E_UPPER_HUMI, &Convert_F);
    return 1;
}

/*----------- _E_REGISTER_Measure Float -----------*/
uint8_t _Cb_R_ModbusRTU_REG_Temp_Float(sData *str, uint16_t Pos)
{
    uint32_t hexValue = 0;
    hexValue = Handle_Float_To_hexUint32(sMeasure.Temp_Filter_f);
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue >> 8;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue >> 24;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue >> 16;
    return 1;
}
uint8_t _Cb_R_ModbusRTU_REG_Humi_Float(sData *str, uint16_t Pos)
{
    uint32_t hexValue = 0;
    hexValue = Handle_Float_To_hexUint32(sMeasure.Humi_Filter_f);
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue >> 8;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue >> 24;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue >> 16;
    return 1;
}

/*----------- _E_REGISTER_Temp_Offset -----------*/
uint8_t _Cb_R_ModbusRTU_REG_Temp_Offset(sData *str, uint16_t Pos)
{
    uint32_t hexValue = 0;
    hexValue = Handle_Float_To_hexUint32(sOffset.Temp_f);
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue >> 8;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue >> 24;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue >> 16;
    return 1;
}
uint8_t _Cb_W_ModbusRTU_REG_Temp_Offset(sData *str, uint16_t Pos)
{
    uint32_t ConvertData = 0;
    float Convert_F = 0;
    uint8_t pos = 0;
    pos = Pos;
    ConvertData = str->Data_a8[pos+2]<<8 | str->Data_a8[pos+3];
    ConvertData = (ConvertData << 16) | (str->Data_a8[pos]<<8 | str->Data_a8[pos+1]);
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos];
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos+1];
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos+2];
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos+3];
    
    Convert_uint32Hex_To_Float(ConvertData, &Convert_F);
    Save_OffsetSensor(Convert_F, sOffset.Humi_f);
    return 1;
}

/*----------- _E_REGISTER_Humi_Offset -----------*/
uint8_t _Cb_R_ModbusRTU_REG_Humi_Offset(sData *str, uint16_t Pos)
{
    uint32_t hexValue = 0;
    hexValue = Handle_Float_To_hexUint32(sOffset.Humi_f);
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue >> 8;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue >> 24;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue >> 16;
    return 1;
}
uint8_t _Cb_W_ModbusRTU_REG_Humi_Offset(sData *str, uint16_t Pos)
{
    uint32_t ConvertData = 0;
    float Convert_F = 0;
    uint8_t pos = 0;
    pos = Pos;
    ConvertData = str->Data_a8[pos+2]<<8 | str->Data_a8[pos+3];
    ConvertData = (ConvertData << 16) | (str->Data_a8[pos]<<8 | str->Data_a8[pos+1]);
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos];
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos+1];
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos+2];
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos+3];
    
    Convert_uint32Hex_To_Float(ConvertData, &Convert_F);
    Save_OffsetSensor(sOffset.Temp_f, Convert_F);
    return 1;
}



/*----------- _E_REGISTER_Temp_Lower -----------*/
uint8_t _Cb_R_ModbusRTU_REG_Temp_Lower(sData *str, uint16_t Pos)
{
    uint32_t hexValue = 0;
    hexValue = Handle_Float_To_hexUint32(sAlarm.Lower_Temp_f);
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue >> 8;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue >> 24;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue >> 16;
    return 1;
}
uint8_t _Cb_W_ModbusRTU_REG_Temp_Lower(sData *str, uint16_t Pos)
{
    uint32_t ConvertData = 0;
    float Convert_F = 0;
    uint8_t pos = 0;
    pos = Pos;
    ConvertData = str->Data_a8[pos+2]<<8 | str->Data_a8[pos+3];
    ConvertData = (ConvertData << 16) | (str->Data_a8[pos]<<8 | str->Data_a8[pos+1]);
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos];
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos+1];
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos+2];
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos+3];
    
    Convert_uint32Hex_To_Float(ConvertData, &Convert_F);
    Save_AlarmSensor(_E_LOWER_TEMP, &Convert_F);
    return 1;
}

/*----------- _E_REGISTER_Temp_Upper -----------*/
uint8_t _Cb_R_ModbusRTU_REG_Temp_Upper(sData *str, uint16_t Pos)
{
    uint32_t hexValue = 0;
    hexValue = Handle_Float_To_hexUint32(sAlarm.Upper_Temp_f);
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue >> 8;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue >> 24;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue >> 16;
    return 1;
}
uint8_t _Cb_W_ModbusRTU_REG_Temp_Upper(sData *str, uint16_t Pos)
{
    uint32_t ConvertData = 0;
    float Convert_F = 0;
    uint8_t pos = 0;
    pos = Pos;
    ConvertData = str->Data_a8[pos+2]<<8 | str->Data_a8[pos+3];
    ConvertData = (ConvertData << 16) | (str->Data_a8[pos]<<8 | str->Data_a8[pos+1]);
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos];
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos+1];
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos+2];
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos+3];
    
    Convert_uint32Hex_To_Float(ConvertData, &Convert_F);
    Save_AlarmSensor(_E_UPPER_TEMP, &Convert_F);
    return 1;
}



/*----------- _E_REGISTER_Humi_Lower -----------*/
uint8_t _Cb_R_ModbusRTU_REG_Humi_Lower(sData *str, uint16_t Pos)
{
    uint32_t hexValue = 0;
    hexValue = Handle_Float_To_hexUint32(sAlarm.Lower_Humi_f);
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue >> 8;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue >> 24;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue >> 16;
    return 1;
}
uint8_t _Cb_W_ModbusRTU_REG_Humi_Lower(sData *str, uint16_t Pos)
{
    uint32_t ConvertData = 0;
    float Convert_F = 0;
    uint8_t pos = 0;
    pos = Pos;
    ConvertData = str->Data_a8[pos+2]<<8 | str->Data_a8[pos+3];
    ConvertData = (ConvertData << 16) | (str->Data_a8[pos]<<8 | str->Data_a8[pos+1]);
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos];
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos+1];
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos+2];
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos+3];
    
    Convert_uint32Hex_To_Float(ConvertData, &Convert_F);
    Save_AlarmSensor(_E_LOWER_HUMI, &Convert_F);
    return 1;
}

/*----------- _E_REGISTER_Humi_Upper -----------*/
uint8_t _Cb_R_ModbusRTU_REG_Humi_Upper(sData *str, uint16_t Pos)
{
    uint32_t hexValue = 0;
    hexValue = Handle_Float_To_hexUint32(sAlarm.Upper_Humi_f);
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue >> 8;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue >> 24;
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = hexValue >> 16;
    return 1;
}
uint8_t _Cb_W_ModbusRTU_REG_Humi_Upper(sData *str, uint16_t Pos)
{
    uint32_t ConvertData = 0;
    float Convert_F = 0;
    uint8_t pos = 0;
    pos = Pos;
    ConvertData = str->Data_a8[pos+2]<<8 | str->Data_a8[pos+3];
    ConvertData = (ConvertData << 16) | (str->Data_a8[pos]<<8 | str->Data_a8[pos+1]);
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos];
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos+1];
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos+2];
    sLogData_ModbusRTU.Data_a8[sLogData_ModbusRTU.Length_u16++] = str->Data_a8[pos+3];
    
    Convert_uint32Hex_To_Float(ConvertData, &Convert_F);
    Save_AlarmSensor(_E_UPPER_HUMI, &Convert_F);
    return 1;
}

/*===================== Send Data RS485 ======================*/
/*
    @brief Send 485 sensor
*/

uint32_t count_trans = 0;
void        Send_RS458_Sensor(uint8_t *aData, uint16_t Length_u16) 
{
//    HAL_GPIO_WritePin(RS485_TXDE_S_GPIO_Port, RS485_TXDE_S_Pin, GPIO_PIN_SET);
//    HAL_Delay(5);
//    HAL_UART_Transmit(&uart_485, aData, Length_u16, 1000);
//    
//    UTIL_MEM_set(sUart485.Data_a8 , 0x00, sUart485.Length_u16);
//    sUart485.Length_u16 = 0;
//    CountBufferHandleRecv = 0;
//    
//    HAL_GPIO_WritePin(RS485_TXDE_S_GPIO_Port, RS485_TXDE_S_Pin, GPIO_PIN_RESET);
  
    count_trans++;
    HAL_GPIO_WritePin(DE_GPIO_PORT, DE_GPIO_PIN, GPIO_PIN_SET);
    HAL_Delay(5);
    // Send
//    RS485_Init_Data();
    HAL_UART_Transmit(&uart_rs485, aData , Length_u16, 1000); 
    
    //Dua DE ve Receive
    HAL_GPIO_WritePin(DE_GPIO_PORT, DE_GPIO_PIN, GPIO_PIN_RESET);
    
    sCtrlLed.Led_Tx = Length_u16;
}

/*====================== Handle Function =======================*/
void Reset_sData(sData *str)
{
	uint16_t i;
    
	for (i = 0; i < str->Length_u16; i++)
		*(str->Data_a8 +i ) = 0;
    
	str->Length_u16 = 0;
}
/*======================= Function ID Slave =======================*/
void Save_InforSlaveModbusRTU(uint8_t eKind, uint8_t Data)
{
#ifdef USING_CHECK_MODBUS_RTU
    uint8_t aData[8] = {0};
    uint8_t length = 0;
    switch(eKind)
    {
        case _E_RS485_ID:
            sSlave_ModbusRTU.ID = Data;
            break;
            
        case _E_RS485_BR:
            if(Data < 11)
                sSlave_ModbusRTU.Baudrate = Data;
            break;
            
        default:
            break;
    }
    
    aData[length++] = sSlave_ModbusRTU.ID;
    aData[length++] = sSlave_ModbusRTU.Baudrate;

    Save_Array(ADDR_INFOR_SLAVE_MODBUS_RTU, aData, length);
#endif
}

void Init_InforSlaveModbusRTU(void)
{
#ifdef USING_CHECK_MODBUS_RTU
    if(*(__IO uint8_t*)(ADDR_INFOR_SLAVE_MODBUS_RTU) != FLASH_BYTE_EMPTY)
    {
        sSlave_ModbusRTU.ID         = *(__IO uint8_t*)(ADDR_INFOR_SLAVE_MODBUS_RTU+2);
        sSlave_ModbusRTU.Baudrate   = *(__IO uint8_t*)(ADDR_INFOR_SLAVE_MODBUS_RTU+3);
    }
    else
    {
        sSlave_ModbusRTU.ID         = ID_DEFAULT;
        sSlave_ModbusRTU.Baudrate   = BAUDRATE_DEFAULT;
    }
#endif    
}
    uint8_t aframe[128] = {0};
/*====================== Handle Receive Modbus RTU =======================*/
uint8_t Modem_Check_RTU(sData *StrUartRecei)
{
#ifdef USING_CHECK_MODBUS_RTU
    uint16_t Crc_Check = 0;
    uint16_t Crc_Recv = 0;
    uint16_t Register = 0;
    uint16_t Length = 0;
    
	sData sDataConfig = {aDATA_CONFIG, 0};
    uint16_t var = 0;
    

    sData sFrame={aframe,0};
    
    Reset_Buff(&sLogData_ModbusRTU);
    
    if(StrUartRecei->Length_u16 >= 8)
    {
        sCtrlLed.Led_Rx = StrUartRecei->Length_u16;
        Crc_Recv = (StrUartRecei->Data_a8[StrUartRecei->Length_u16 - 1] << 8) |
                   (StrUartRecei->Data_a8[StrUartRecei->Length_u16 - 2]);
        Crc_Check = ModRTU_CRC(StrUartRecei->Data_a8, StrUartRecei->Length_u16 - 2);
        
        if(Crc_Check == Crc_Recv && StrUartRecei->Data_a8[0] == sSlave_ModbusRTU.ID)
        {
            Register = (StrUartRecei->Data_a8[2] << 8) | StrUartRecei->Data_a8[3];
            
            switch(StrUartRecei->Data_a8[1])
            {
                case 0x03:
                case 0x04:
                  Length = (StrUartRecei->Data_a8[4] << 8) | StrUartRecei->Data_a8[5];
                  for(uint8_t i = _E_REGISTER_BEGIN+1; i < _E_REGISTER_END; i++)
                  {
                    if(sCheckList_Reg_Modbus_RTU[i].idRegister == Register)
                    {
                       if(sCheckList_Reg_Modbus_RTU[i].Length <= Length)
                       {
                           if(sCheckList_Reg_Modbus_RTU[i].CallBack_Read(NULL, NULL) == 1)
                           {
                               Length -= sCheckList_Reg_Modbus_RTU[i].Length;
                               Register += sCheckList_Reg_Modbus_RTU[i].Length;
                           }
                           else
                             break;
                       }
                    }
                  }
                  if(sLogData_ModbusRTU.Length_u16 > 0 && sLogData_ModbusRTU.Length_u16%2 == 0)
                  {
                    ModRTU_Slave_ACK_Read_Frame(&sFrame, StrUartRecei->Data_a8[0], StrUartRecei->Data_a8[1], Register, sLogData_ModbusRTU.Length_u16/2, sLogData_ModbusRTU.Data_a8);
                    Send_RS458_Sensor(sFrame.Data_a8, sFrame.Length_u16);
                  }
                  break;
                  
                case 0x06:
                  for(uint8_t i = 4; i < StrUartRecei->Length_u16-2; i++)
                  {
                    sDataConfig.Data_a8[sDataConfig.Length_u16++] = StrUartRecei->Data_a8[i];
                  }
                  for(uint8_t i = _E_REGISTER_BEGIN+1; i < _E_REGISTER_END; i++)
                  {
                    if(sCheckList_Reg_Modbus_RTU[i].idRegister == Register)
                    {
                      sCheckList_Reg_Modbus_RTU[i].CallBack_Write(&sDataConfig, 0);
                    }
                  }         
                  if(sLogData_ModbusRTU.Length_u16 > 0 && sLogData_ModbusRTU.Length_u16%2 == 0)
                  {
                    ModRTU_Slave_ACK_Write_Frame(&sFrame, StrUartRecei->Data_a8[0], StrUartRecei->Data_a8[1], Register, sLogData_ModbusRTU.Length_u16/2, sLogData_ModbusRTU.Data_a8);
                    Send_RS458_Sensor(sFrame.Data_a8, sFrame.Length_u16);
                  }
                  break;
                  
                case 0x10:
                  var = 0;
                  Length = (StrUartRecei->Data_a8[4] << 8) | StrUartRecei->Data_a8[5];
                  for(uint8_t i = 7; i < StrUartRecei->Length_u16-2; i++)
                  {
                    sDataConfig.Data_a8[sDataConfig.Length_u16++] = StrUartRecei->Data_a8[i];
                  }
                  if((Length*2 == StrUartRecei->Data_a8[6]) && (sDataConfig.Length_u16 == StrUartRecei->Data_a8[6]))
                  {
                      for(uint8_t i = _E_REGISTER_BEGIN+1; i < _E_REGISTER_END; i++)
                      {
                        if(sCheckList_Reg_Modbus_RTU[i].idRegister == Register)
                        {
                           if(sCheckList_Reg_Modbus_RTU[i].Length <= Length)
                           {
                               if(sCheckList_Reg_Modbus_RTU[i].CallBack_Write(&sDataConfig, var) == 1)
                               {
                                   var += (sCheckList_Reg_Modbus_RTU[i].Length)*2;
                                   Length -= sCheckList_Reg_Modbus_RTU[i].Length;
                                   Register += sCheckList_Reg_Modbus_RTU[i].Length;
                               }
                               else 
                                 break;
                           }
                        }
                      }   
                  }
                  if(sLogData_ModbusRTU.Length_u16 > 0 && sLogData_ModbusRTU.Length_u16%2 == 0)
                  {
                    Register = (StrUartRecei->Data_a8[2] << 8) | StrUartRecei->Data_a8[3];
                    ModRTU_Slave_ACK_Write_Frame(&sFrame, StrUartRecei->Data_a8[0], StrUartRecei->Data_a8[1], Register, sLogData_ModbusRTU.Length_u16/2, sLogData_ModbusRTU.Data_a8);
                    Send_RS458_Sensor(sFrame.Data_a8, sFrame.Length_u16);
                  }
                  break;
              
                default:
                  return 0;
                  break;
            }
        }
        else
            return 0;
    }
    else
        return 0;
  
    return 1;
#endif
    return 0;
}


