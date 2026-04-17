#include "lcd_segment.h"
#include "lcd.h"

static LCD_HandleTypeDef *lcd = NULL;

// ===== INTERNAL (DUY NH?T 1 mapping) =====
void LCD_SetSegment(uint8_t com, uint8_t seg, uint8_t state)
{
    
//    uint32_t ram = seg / 4;
//    uint32_t bit = (seg % 4) * 8 + com;
//    uint32_t mask = 1U << bit;
    uint32_t ram = seg / 32 + (com/8)*2;
    uint32_t bit = seg % 32;
    uint32_t mask = 1U << bit;

    uint32_t current = hlcd.Instance->RAM[ram];  

    if(state)
    {
        current |= mask;   
    }
    else
    {
        current &= ~mask;  
    }

    hlcd.Instance->RAM[ram] = current;
}

// ===== PUBLIC =====

void LCD_SEG_Init(LCD_HandleTypeDef *hlcd)
{
    lcd = hlcd;
}

void LCD_SEG_Write(uint8_t com, uint8_t seg, uint8_t state)
{
    LCD_SetSegment(com, seg, state);
}

void LCD_SegOn(uint8_t com, uint8_t seg)
{
    LCD_SEG_Write(com, seg, 1);
}

void LCD_SegOff(uint8_t com, uint8_t seg)
{
    LCD_SEG_Write(com, seg, 0);
}

void LCD_SegToggle(uint8_t com, uint8_t seg)
{
    uint32_t ram;
    uint32_t bit_pos;
    uint32_t mask;
    uint32_t current;

    if(lcd == NULL) return;

    ram = seg / 4;
    bit_pos = (seg % 4) * 8 + (com - 1);

    mask = (1UL << bit_pos);

    current = lcd->Instance->RAM[ram];

    if(current & mask)
        HAL_LCD_Write(lcd, ram, mask, 0x00);
    else
        HAL_LCD_Write(lcd, ram, mask, mask);

    HAL_LCD_UpdateDisplayRequest(lcd);
}

void LCD_ClearAll(void)
{
    HAL_LCD_Clear(lcd);
    HAL_LCD_UpdateDisplayRequest(lcd);
}