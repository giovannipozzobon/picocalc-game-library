#include "lcd_ili9488.h"
#include "gfx_tilemap.h"
#include "tiles.h"
#include "gfx_sprite.h"
#include "sprite.h"
#include "keyboard.h"

#include <stdio.h>

// Global variable required by keyboard.c for user interrupt handling
volatile bool user_interrupt = false;

extern tilemap_t map;
lcd_t lcd;
extern const uint16_t sprite_hero[];
extern const uint16_t sprite_enemy[];

// Cursor keys state
typedef struct {
    bool up;
    bool down;
    bool left;
    bool right;
} key_state_t;

void on_collision(int id1, int id2) {
    Sprite *a = sprite_get(id1);
    Sprite *b = sprite_get(id2);
    if (a && b)
        printf("Collision between type %d and %d!\n", a->type, b->type);
}

void update_key_state(key_state_t *keys) {
    // Reset state
    keys->up = false;
    keys->down = false;
    keys->left = false;
    keys->right = false;

    // Read all available keys in the buffer
    while (keyboard_key_available()) {
        char key = keyboard_get_key();

        if (key == KEY_UP) keys->up = true;
        else if (key == KEY_DOWN) keys->down = true;
        else if (key == KEY_LEFT) keys->left = true;
        else if (key == KEY_RIGHT) keys->right = true;
    }
}

int main() {
    stdio_init_all();
    lcd_init(&lcd);
    keyboard_init();  // Initialize PicoCalc keyboard
    keyboard_set_background_poll(true);  // Enable background polling
    sprite_init();

    int scroll_x = 0, scroll_y = 0;

    // Calculate maximum possible scroll
    int max_scroll_x = (map.width * map.tile_size) - LCD_WIDTH;
    int max_scroll_y = (map.height * map.tile_size) - LCD_HEIGHT;

    int hero = sprite_create(50, 50, 16, 16, sprite_hero, 1);
    int enemy = sprite_create(10, 155, 16, 16, sprite_enemy, 2);

    key_state_t keys;
    Sprite *hero_sprite = sprite_get(hero);

    while (true) {
        // Manual keyboard poll for real-time input
        keyboard_poll();

        // Read key state
        update_key_state(&keys);

        // Move hero sprite based on input
        if (hero_sprite) {
            hero_sprite->dx = 0;
            hero_sprite->dy = 0;

            if (keys.up) hero_sprite->dy = -2;
            if (keys.down) hero_sprite->dy = 2;
            if (keys.left) hero_sprite->dx = 2;
            if (keys.right) hero_sprite->dx = -2;

            // Limit movement to screen borders
            if (hero_sprite->x + hero_sprite->dx < 0)
                hero_sprite->dx = -hero_sprite->x;
            if (hero_sprite->x + hero_sprite->dx + hero_sprite->width > LCD_WIDTH)
                hero_sprite->dx = LCD_WIDTH - hero_sprite->x - hero_sprite->width;
            if (hero_sprite->y + hero_sprite->dy < 0)
                hero_sprite->dy = -hero_sprite->y;
            if (hero_sprite->y + hero_sprite->dy + hero_sprite->height > LCD_HEIGHT)
                hero_sprite->dy = LCD_HEIGHT - hero_sprite->y - hero_sprite->height;
        }

        // Draw tilemap with scroll
        tilemap_draw(&lcd, &map, scroll_x, scroll_y);

        sprite_update_all();
        sprite_check_collisions(on_collision);
        sprite_draw_all(lcd_get_framebuffer(&lcd), LCD_WIDTH, LCD_HEIGHT);
        lcd_update_dma(&lcd);
        sleep_ms(40);

        // Horizontal scroll
        scroll_x += 2;
        if (scroll_x > max_scroll_x)
            scroll_x = 0;
    }

}
