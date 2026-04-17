#include "user_com_seg.h"

static LCD_DIGIT_MAP *digit_segment;
static LCD_DP_MAP *dp_segment;

LCD_DIGIT_MAP lcd_digit_map[12]=
{
/*1*/ {{LCD_COM_0,LCD_PIN_4},{LCD_COM_1,LCD_PIN_4},{LCD_COM_2,LCD_PIN_4},{LCD_COM_3,LCD_PIN_3},{LCD_COM_2,LCD_PIN_3},{LCD_COM_0,LCD_PIN_3},{LCD_COM_1,LCD_PIN_3}},
/*2*/ {{LCD_COM_0,LCD_PIN_6},{LCD_COM_1,LCD_PIN_6},{LCD_COM_2,LCD_PIN_6},{LCD_COM_3,LCD_PIN_5},{LCD_COM_2,LCD_PIN_5},{LCD_COM_0,LCD_PIN_5},{LCD_COM_1,LCD_PIN_5}},
/*3*/ {{LCD_COM_0,LCD_PIN_8},{LCD_COM_1,LCD_PIN_8},{LCD_COM_2,LCD_PIN_8},{LCD_COM_3,LCD_PIN_7},{LCD_COM_2,LCD_PIN_7},{LCD_COM_0,LCD_PIN_7},{LCD_COM_1,LCD_PIN_7}},
/*4*/ {{LCD_COM_0,LCD_PIN_10},{LCD_COM_1,LCD_PIN_10},{LCD_COM_2,LCD_PIN_10},{LCD_COM_3,LCD_PIN_9},{LCD_COM_2,LCD_PIN_9},{LCD_COM_0,LCD_PIN_9},{LCD_COM_1,LCD_PIN_9}},
  
/*5*/ {{LCD_COM_0,LCD_PIN_14},{LCD_COM_1,LCD_PIN_14},{LCD_COM_2,LCD_PIN_14},{LCD_COM_3,LCD_PIN_13},{LCD_COM_2,LCD_PIN_13},{LCD_COM_0,LCD_PIN_13},{LCD_COM_1,LCD_PIN_13}},
/*6*/ {{LCD_COM_0,LCD_PIN_16},{LCD_COM_1,LCD_PIN_16},{LCD_COM_2,LCD_PIN_16},{LCD_COM_3,LCD_PIN_15},{LCD_COM_2,LCD_PIN_15},{LCD_COM_0,LCD_PIN_15},{LCD_COM_1,LCD_PIN_15}},
/*7*/ {{LCD_COM_0,LCD_PIN_18},{LCD_COM_1,LCD_PIN_18},{LCD_COM_2,LCD_PIN_18},{LCD_COM_3,LCD_PIN_17},{LCD_COM_2,LCD_PIN_17},{LCD_COM_0,LCD_PIN_17},{LCD_COM_1,LCD_PIN_17}},
/*8*/ {{LCD_COM_0,LCD_PIN_22},{LCD_COM_1,LCD_PIN_22},{LCD_COM_2,LCD_PIN_22},{LCD_COM_3,LCD_PIN_21},{LCD_COM_2,LCD_PIN_21},{LCD_COM_0,LCD_PIN_21},{LCD_COM_1,LCD_PIN_21}},

/*9*/ {{LCD_COM_0,LCD_PIN_26},{LCD_COM_1,LCD_PIN_26},{LCD_COM_2,LCD_PIN_26},{LCD_COM_3,LCD_PIN_25},{LCD_COM_2,LCD_PIN_25},{LCD_COM_0,LCD_PIN_25},{LCD_COM_1,LCD_PIN_25}},
/*10*/{{LCD_COM_0,LCD_PIN_28},{LCD_COM_1,LCD_PIN_28},{LCD_COM_2,LCD_PIN_28},{LCD_COM_3,LCD_PIN_27},{LCD_COM_2,LCD_PIN_27},{LCD_COM_0,LCD_PIN_27},{LCD_COM_1,LCD_PIN_27}},
/*11*/{{LCD_COM_0,LCD_PIN_30},{LCD_COM_1,LCD_PIN_30},{LCD_COM_2,LCD_PIN_30},{LCD_COM_3,LCD_PIN_29},{LCD_COM_2,LCD_PIN_29},{LCD_COM_0,LCD_PIN_29},{LCD_COM_1,LCD_PIN_29}},
/*12*/{{LCD_COM_0,LCD_PIN_32},{LCD_COM_1,LCD_PIN_32},{LCD_COM_2,LCD_PIN_32},{LCD_COM_3,LCD_PIN_31},{LCD_COM_2,LCD_PIN_31},{LCD_COM_0,LCD_PIN_31},{LCD_COM_1,LCD_PIN_31}}
};

LCD_DP_MAP lcd_dp_map =
{
  {LCD_COM_3, LCD_PIN_14}, // DP1
  {LCD_COM_3, LCD_PIN_16}, // DP2
  {LCD_COM_3, LCD_PIN_18}, // DP3
  {LCD_COM_3, LCD_PIN_26}, // DP4
  {LCD_COM_3, LCD_PIN_28}, // DP5
  {LCD_COM_3, LCD_PIN_30}  // DP6
};

static const uint8_t font7seg[36] =
{
    0x3F,0x06,0x5B,0x4F,0x66,
    0x6D,0x7D,0x07,0x7F,0x6F,

    0x77,0x7C,0x39,0x5E,0x79,
    0x71,0x3D,0x76,0x30,0x1E,
    0x76,0x38,0x37,0x54,0x3F,
    0x73,0x67,0x50,0x6D,0x78,
    0x3E,0x1C,0x2A,0x76,0x6E,0x5B
};

static uint32_t blink_tick_line1 = 0;
static uint32_t blink_tick_line2 = 0;

static uint8_t blink_state_line1 = 1;
static uint8_t blink_state_line2 = 1;


void LCD_InitMap(void)
{
    digit_segment = lcd_digit_map;
    dp_segment = &lcd_dp_map;
}

void LCD_ShowChar(uint8_t pos, char c)
{
    uint8_t pattern = 0;

    if(c == ' ')
        pattern = 0;

    else if(c == '-')           // Hien thi dau '-'
        pattern = 0x40;         // segment G

    else if(c >= '0' && c <= '9')
        pattern = font7seg[c - '0'];

    else if(c >= 'A' && c <= 'Z')
        pattern = font7seg[10 + c - 'A'];

    else if(c >= 'a' && c <= 'z')
        pattern = font7seg[10 + c - 'a'];

    LCD_DIGIT_MAP *d = &digit_segment[pos];

    LCD_SEG_Write(d->A.com, d->A.seg, (pattern >> 0) & 1);
    LCD_SEG_Write(d->B.com, d->B.seg, (pattern >> 1) & 1);
    LCD_SEG_Write(d->C.com, d->C.seg, (pattern >> 2) & 1);
    LCD_SEG_Write(d->D.com, d->D.seg, (pattern >> 3) & 1);
    LCD_SEG_Write(d->E.com, d->E.seg, (pattern >> 4) & 1);
    LCD_SEG_Write(d->F.com, d->F.seg, (pattern >> 5) & 1);
    LCD_SEG_Write(d->G.com, d->G.seg, (pattern >> 6) & 1);
}

void LCD_SetDP(uint8_t index,uint8_t state)
{
    LCD_SEG_PIN *p=&((LCD_SEG_PIN*)dp_segment)[index];
    LCD_SEG_Write(p->com,p->seg,state);
}

void LCD_ClearLine0(void)
{
    for(int pos = 0; pos <= 3; pos++)
    {
        LCD_ShowChar(pos, ' ');   
    }
}

void LCD_DisplayLine0(char *str)
{
    int len = 0;
    while(str[len]) len++;

    int pos = 3; 

    for(int i = len-1; i >= 0 && pos >= 0; i--)
    {
        if(str[i] == '.')
        {
//            if(pos < 11) LCD_SetDP(pos - 1 - 4, 1);
        }
        else
        {
            LCD_ShowChar(pos--, str[i]);
        }
    }
}

void LCD_ClearLine1(void)
{
    for(int pos = 4; pos <= 7; pos++)
    {
        LCD_ShowChar(pos, ' ');   
        LCD_SetDP(pos - 4, 0);    
    }
}

void LCD_DisplayLine1(char *str)
{
    int len = 0;
    while(str[len]) len++;

    int pos = 7; 

    for(int i = len-1; i >= 0 && pos >= 4; i--)
    {
        if(str[i] == '.')
        {
            if(pos < 7) LCD_SetDP(pos - 4, 1);
        }
        else
        {
            LCD_ShowChar(pos--, str[i]);
        }
    }
}

void LCD_ClearLine2(void)
{
    for(int pos = 8; pos <= 11; pos++)
    {
        LCD_ShowChar(pos, ' ');   
        LCD_SetDP(pos - 4 - 1, 0);    
    }
}

void LCD_DisplayLine2(char *str)
{
    int len = 0;
    while(str[len]) len++;

    int pos = 11; 

    for(int i = len-1; i >= 0 && pos >= 8; i--)
    {
        if(str[i] == '.')
        {
            if(pos < 11) LCD_SetDP(pos - 1 - 4, 1);
        }
        else
        {
            LCD_ShowChar(pos--, str[i]);
        }
    }
}

void LCD_ClearLine(void)
{
    LCD_ClearLine0();
    LCD_ClearLine1();
    LCD_ClearLine2();
}

void LCD_BlinkLine(uint8_t line, char *text, uint32_t period)
{
    if(line == 1)
    {
        if(HAL_GetTick() - blink_tick_line1 >= period)
        {
            blink_tick_line1 = HAL_GetTick();
            blink_state_line1 ^= 1;

            if(blink_state_line1)
                LCD_DisplayLine1(text);
            else
                LCD_DisplayLine1("    ");
        }
    }

    if(line == 2)
    {
        if(HAL_GetTick() - blink_tick_line2 >= period)
        {
            blink_tick_line2 = HAL_GetTick();
            blink_state_line2 ^= 1;

            if(blink_state_line2)
                LCD_DisplayLine2(text);
            else
                LCD_DisplayLine2("    ");
        }
    }
}

