#ifndef __LCD_SEG_H__
#define __LCD_SEG_H__

#include "stm32l4xx_hal.h"

#define LCD_COM_MAX    4

// SEG mapping
#define LCD_SEG_1       25
#define LCD_SEG_2       26
#define LCD_SEG_3       27
#define LCD_SEG_4       17
#define LCD_SEG_5       40
#define LCD_SEG_6       41
#define LCD_SEG_7       42
#define LCD_SEG_8       43
#define LCD_SEG_9       7
#define LCD_SEG_10      8


#define LCD_SEG_13      9
#define LCD_SEG_14      21
#define LCD_SEG_15      16
#define LCD_SEG_16      18
#define LCD_SEG_17      19
#define LCD_SEG_18      20
#define LCD_SEG_19      0

#define LCD_SEG_21      3
#define LCD_SEG_22      4
#define LCD_SEG_23      22

#define LCD_SEG_25      23
#define LCD_SEG_26      5
#define LCD_SEG_27      6
#define LCD_SEG_28      10
#define LCD_SEG_29      11
#define LCD_SEG_30      12
#define LCD_SEG_31      15
#define LCD_SEG_32      24

// COM index (1 ? 4)
#define COM0   0
#define COM1   8
#define COM2   16
#define COM3   24

#define LCD_PIN_1           LCD_SEG_1
#define LCD_PIN_2           LCD_SEG_2
#define LCD_PIN_3           LCD_SEG_3
#define LCD_PIN_4           LCD_SEG_4
#define LCD_PIN_5           LCD_SEG_5
#define LCD_PIN_6           LCD_SEG_6
#define LCD_PIN_7           LCD_SEG_7
#define LCD_PIN_8           LCD_SEG_8
#define LCD_PIN_9           LCD_SEG_9
#define LCD_PIN_10          LCD_SEG_10
#define LCD_PIN_11          LCD_SEG_19
#define LCD_PIN_12          LCD_SEG_19
#define LCD_PIN_13          LCD_SEG_13
#define LCD_PIN_14          LCD_SEG_14
#define LCD_PIN_15          LCD_SEG_15
#define LCD_PIN_16          LCD_SEG_16
#define LCD_PIN_17          LCD_SEG_17
#define LCD_PIN_18          LCD_SEG_18
#define LCD_PIN_19          LCD_SEG_19

#define LCD_PIN_21          LCD_SEG_21
#define LCD_PIN_22          LCD_SEG_22
#define LCD_PIN_23          LCD_SEG_23

#define LCD_PIN_25          LCD_SEG_25
#define LCD_PIN_26          LCD_SEG_26
#define LCD_PIN_27          LCD_SEG_27
#define LCD_PIN_28          LCD_SEG_28
#define LCD_PIN_29          LCD_SEG_29
#define LCD_PIN_30          LCD_SEG_30
#define LCD_PIN_31          LCD_SEG_31
#define LCD_PIN_32          LCD_SEG_32

#define LCD_COM_0           COM0
#define LCD_COM_1           COM1
#define LCD_COM_2           COM2
#define LCD_COM_3           COM3

// API
void LCD_SEG_Init(LCD_HandleTypeDef *hlcd);

void LCD_SegOn(uint8_t com, uint8_t seg);
void LCD_SegOff(uint8_t com, uint8_t seg);
void LCD_SegToggle(uint8_t com, uint8_t seg);

void LCD_SEG_Write(uint8_t com, uint8_t seg, uint8_t state);

void LCD_ClearAll(void);

#endif