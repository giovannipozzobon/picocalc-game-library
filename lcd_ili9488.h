#ifndef LCD_ILI9488_H
#define LCD_ILI9488_H

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/dma.h"

#define LCD_WIDTH   320
#define LCD_HEIGHT  320

// SPI configuration
#define LCD_SPI_PORT spi1
#define LCD_PIN_SCK  10  // SCL
#define LCD_PIN_MOSI 11  // SDI
#define LCD_PIN_MISO 12  // SDO
#define LCD_PIN_CS   13  // CSX
#define LCD_PIN_DC   14  // DCX
#define LCD_PIN_RST  15  // RESET

// 16-bit RGB color macros
#define RGB565(r,g,b) ((((r)&0x1F)<<11) | (((g)&0x3F)<<5) | ((b)&0x1F))

typedef struct {
    uint16_t width;
    uint16_t height;
    uint16_t *framebuffer;
} lcd_t;

void lcd_init(lcd_t *lcd);
void lcd_set_address_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
void lcd_draw_pixel(lcd_t *lcd, uint16_t x, uint16_t y, uint16_t color);
void lcd_fill_rect(lcd_t *lcd, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void lcd_update_dma(lcd_t *lcd);

#endif
