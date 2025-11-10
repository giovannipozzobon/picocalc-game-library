#include "lcd_ili9488.h"
#include "gfx_tilemap.h"

extern tilemap_t map;
lcd_t lcd;

int main() {
    stdio_init_all();
    lcd_init(&lcd);

    int scroll_x = 0, scroll_y = 0;

    while (true) {
        tilemap_draw(&lcd, &map, scroll_x, scroll_y);
        lcd_update_dma(&lcd);
        sleep_ms(40);

        scroll_x += 2; // scroll orizzontale
        if (scroll_x >= map.width * map.tile_size - LCD_WIDTH)
            scroll_x = 0;
    }
}
