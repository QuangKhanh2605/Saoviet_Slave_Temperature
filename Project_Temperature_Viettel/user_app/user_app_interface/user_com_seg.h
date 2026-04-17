#ifndef _USER_COM_SEG_H
#define _USER_COM_SEG_H

#include <stdint.h>
#include "lcd_segment.h"

typedef struct
{
    uint8_t com;
    uint8_t seg;
}LCD_SEG_PIN;

typedef struct
{
    LCD_SEG_PIN A;
    LCD_SEG_PIN B;
    LCD_SEG_PIN C;
    LCD_SEG_PIN D;
    LCD_SEG_PIN E;
    LCD_SEG_PIN F;
    LCD_SEG_PIN G;
}LCD_DIGIT_MAP;

typedef struct
{
    LCD_SEG_PIN DP1;
    LCD_SEG_PIN DP2;
    LCD_SEG_PIN DP3;
    LCD_SEG_PIN DP4;
    LCD_SEG_PIN DP5;
    LCD_SEG_PIN DP6;
}LCD_DP_MAP;

void LCD_InitMap(void);

void LCD_DisplayLine0(char *str);
void LCD_DisplayLine1(char *str);
void LCD_DisplayLine2(char *str);

void LCD_ClearLine(void);

void LCD_BlinkLine(uint8_t line, char *text, uint32_t period);

void LCD_ShowChar(uint8_t pos,char c);
void LCD_SetDP(uint8_t index,uint8_t state);

#endif