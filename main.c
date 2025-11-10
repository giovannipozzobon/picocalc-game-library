#include "lcd_ili9488.h"
#include "gfx_tilemap.h"
#include "tiles.h"
#include "gfx_sprite.h"
#include "sprite.h"


#include <stdio.h>

extern tilemap_t map;
lcd_t lcd;
extern const uint16_t sprite_hero[];
extern const uint16_t sprite_enemy[];

void on_collision(int id1, int id2) {
    Sprite *a = sprite_get(id1);
    Sprite *b = sprite_get(id2);
    if (a && b)
        printf("Collisione tra tipo %d e %d!\n", a->type, b->type);
}

int main() {
    stdio_init_all();
    lcd_init(&lcd);

    sprite_init();

    int scroll_x = 0, scroll_y = 0;

    // Calcola il massimo scroll possibile
    int max_scroll_x = (map.width * map.tile_size) - LCD_WIDTH;
    int max_scroll_y = (map.height * map.tile_size) - LCD_HEIGHT;

    int hero = sprite_create(50, 50, 16, 16, sprite_hero, 1);
    int enemy = sprite_create(10, 155, 16, 16, sprite_enemy, 2);

    while (true) {
        // Disegna la tilemap con scroll
        tilemap_draw(&lcd, &map, scroll_x, scroll_y);

        sprite_update_all();
        sprite_check_collisions(on_collision);
        sprite_draw_all(lcd_get_framebuffer(&lcd), LCD_WIDTH, LCD_HEIGHT);
        lcd_update_dma(&lcd);
        sleep_ms(40);

        // Scroll orizzontale
        scroll_x += 2;
        if (scroll_x > max_scroll_x)
            scroll_x = 0;
    }

}
