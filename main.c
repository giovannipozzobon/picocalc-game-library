#include "lcd_ili9488.h"
#include "gfx_tilemap.h"
#include "tiles.h"

extern tilemap_t map;
lcd_t lcd;

int main() {
    stdio_init_all();
    lcd_init(&lcd);

    // Test con schermo rosso per verificare che funzioni
    uint16_t red = RGB565(31, 0, 0);    // rosso pieno
    uint16_t green = RGB565(0, 63, 0);  // verde pieno
    uint16_t blue = RGB565(0, 0, 31);   // blu pieno
    uint16_t white = RGB565(31, 63, 31); // bianco

    int test_phase = 0;
    int scroll_x = 0, scroll_y = 0;

    while (true) {
        if (test_phase < 4) {
            // Test colori pieni per 1 secondo ciascuno
            uint16_t test_color;
            switch(test_phase) {
                case 0: test_color = red; break;
                case 1: test_color = green; break;
                case 2: test_color = blue; break;
                case 3: test_color = white; break;
            }
            lcd_fill_rect(&lcd, 0, 0, LCD_WIDTH, LCD_HEIGHT, test_color);
            lcd_update_dma(&lcd);
            sleep_ms(1000);
            test_phase++;
        } else {
            // Poi passa alla tilemap con scroll
            tilemap_draw(&lcd, &map, scroll_x, scroll_y);
            lcd_update_dma(&lcd);
            sleep_ms(40);

            scroll_x += 2; // scroll orizzontale
            if (scroll_x >= map.width * map.tile_size - LCD_WIDTH)
                scroll_x = 0;
        }
    }
}
