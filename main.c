#include "lcd_ili9488.h"
#include "gfx_tilemap.h"
#include "tiles.h"

extern tilemap_t map;
lcd_t lcd;

int main() {
    stdio_init_all();
    lcd_init(&lcd);

    int scroll_x = 0, scroll_y = 0;

    // Calcola il massimo scroll possibile
    int max_scroll_x = (map.width * map.tile_size) - LCD_WIDTH;
    int max_scroll_y = (map.height * map.tile_size) - LCD_HEIGHT;

    while (true) {
        // Disegna la tilemap con scroll
        tilemap_draw(&lcd, &map, scroll_x, scroll_y);
        lcd_update_dma(&lcd);
        sleep_ms(40);

        // Scroll orizzontale
        scroll_x += 2;
        if (scroll_x > max_scroll_x)
            scroll_x = 0;
    }
}
