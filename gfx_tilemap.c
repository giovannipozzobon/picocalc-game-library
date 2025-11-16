#include "gfx_tilemap.h"
#include <string.h>

// --- Draw entire tilemap in framebuffer with scroll offset ---
void tilemap_draw(lcd_t *lcd, const tilemap_t *tm, int scroll_x, int scroll_y) {
    uint16_t tile_size = tm->tile_size;
    uint16_t visible_tiles_x = (LCD_WIDTH / tile_size) + 2;
    uint16_t visible_tiles_y = (LCD_HEIGHT / tile_size) + 2;

    int start_tile_x = scroll_x / tile_size;
    int start_tile_y = scroll_y / tile_size;
    int offset_x = -(scroll_x % tile_size);
    int offset_y = -(scroll_y % tile_size);

    for (int ty = 0; ty < visible_tiles_y; ty++) {
        for (int tx = 0; tx < visible_tiles_x; tx++) {
            int map_x = start_tile_x + tx;
            int map_y = start_tile_y + ty;

            if (map_x < 0 || map_x >= tm->width || map_y < 0 || map_y >= tm->height)
                continue;

            uint8_t tile_index = tm->map[map_y * tm->width + map_x];
            const uint16_t *tile = &tm->tileset[tile_index * tile_size * tile_size];

            int dst_x = offset_x + tx * tile_size;
            int dst_y = offset_y + ty * tile_size;

            for (int y = 0; y < tile_size; y++) {
                int sy = dst_y + y;
                if (sy < 0 || sy >= LCD_HEIGHT) continue;

                for (int x = 0; x < tile_size; x++) {
                    int sx = dst_x + x;
                    if (sx < 0 || sx >= LCD_WIDTH) continue;
                    uint16_t color = tile[y * tile_size + x];
                    lcd->framebuffer[sy * LCD_WIDTH + sx] = color;
                }
            }
        }
    }
}

// --- Utility to create a single tile at runtime ---
void tilemap_set_pixel_tile(uint16_t *tile, uint8_t x, uint8_t y, uint16_t color) {
    tile[y * 16 + x] = color; // assuming 16×16 tile
}
