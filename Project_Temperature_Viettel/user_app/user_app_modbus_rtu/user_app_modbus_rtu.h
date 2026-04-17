#ifndef __USER_APP_MODBUS_RTU_
#define __USER_APP_MODBUS_RTU_

#include "user_define.h"
#define USING_CHECK_MODBUS_RTU

#define ID_DEFAULT          1
#define BAUDRATE_DEFAULT    3       //9600

typedef enum
{
    _E_REGISTER_BEGIN,

    _E_REG_ID_RS485,
    _E_REG_BAUDRATE_RS485,
    _E_REG_TEMP_INT,
    _E_REG_TEMP_DECIMAL,
    _E_REG_HUMI_INT,
    _E_REG_HUMI_DECIMAL,
    _E_REG_TEMP_WARNING,
    _E_REG_HUMI_WARNING,
    _E_REG_TEMP_OFFSET_INT,
    _E_REG_HUMI_OFFSET_INT,
    _E_REG_TEMP_ALARM,
    _E_REG_HUMI_ALARM,
    _E_REG_TEMP_LOWER_INT,
    _E_REG_TEMP_UPPER_INT,
    _E_REG_HUMI_LOWER_INT,
    _E_REG_HUMI_UPPER_INT,
    
    _E_REG_TEMP_FLOAT,
    _E_REG_HUMI_FLOAT,
    _E_REG_TEMP_OFFSET,
    _E_REG_HUMI_OFFSET,
    _E_REG_TEMP_LOWER,
    _E_REG_TEMP_UPPER,
    _E_REG_HUMI_LOWER,
    _E_REG_HUMI_UPPER,
    
    _E_REGISTER_END,
}eRegister_ModbusRTU;

typedef enum
{
    _E_RS485_ID,
    _E_RS485_BR,
}eKind_RS485;

typedef uint8_t (*_func_callback_handle)(sData *str_Receiv, uint16_t Pos);

typedef struct {
	int 			    idStep;
    int                 idRegister;
    int                 Length;
	_func_callback_handle	CallBack_Read;
    _func_callback_handle	CallBack_Write;
}struct_CheckList_Reg_Modbus_RTU;

typedef struct {
    uint8_t ID;
    uint8_t Baudrate;
}struct_Slave_ModbusRTU;

extern struct_CheckList_Reg_Modbus_RTU sCheckList_Reg_Modbus_RTU[];
extern sData   sLogData_ModbusRTU;
extern sData   sFrameData_ModbusRTU;
extern struct_Slave_ModbusRTU sSlave_ModbusRTU;
extern uint32_t aBaudrate_value[11];
/*====================Function Handle====================*/
void    Save_InforSlaveModbusRTU(uint8_t ID, uint8_t Baudrate);
void    Init_InforSlaveModbusRTU(void);

void    Reset_sData(sData *str);
uint8_t Modem_Check_RTU(sData *StrUartRecei);
void    Send_RS458_Sensor(uint8_t *aData, uint16_t Length_u16);

#endif
