#ifndef GFX_TILEMAP_H
#define GFX_TILEMAP_H

#include "lcd_ili9488.h"

typedef struct {
    uint16_t width;       // numero di tile orizzontali
    uint16_t height;      // numero di tile verticali
    uint8_t tile_size;    // 16 o 32 pixel
    const uint16_t *tileset;   // array di tile RGB565 consecutivi
    const uint8_t *map;        // mappa dei tile (indici nel tileset)
} tilemap_t;

void tilemap_draw(lcd_t *lcd, const tilemap_t *tm, int scroll_x, int scroll_y);
void tilemap_set_pixel_tile(uint16_t *tile, uint8_t x, uint8_t y, uint16_t color);

#endif
