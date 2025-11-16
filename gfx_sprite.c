#include "gfx_sprite.h"
#include <string.h>

static Sprite sprites[MAX_SPRITES];

void sprite_init(void) {
    memset(sprites, 0, sizeof(sprites));
}

int sprite_create(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t *bitmap, uint8_t type) {
    for (int i = 0; i < MAX_SPRITES; i++) {
        if (!sprites[i].visible) {
            sprites[i].x = x;
            sprites[i].y = y;
            sprites[i].width = w;
            sprites[i].height = h;
            sprites[i].bitmap = bitmap;
            sprites[i].visible = true;
            sprites[i].dx = 0;
            sprites[i].dy = 0;
            sprites[i].type = type;
            return i;
        }
    }
    return -1; // no free slot
}

Sprite *sprite_get(int id) {
    if (id < 0 || id >= MAX_SPRITES) return NULL;
    return &sprites[id];
}

void sprite_update_all(void) {
    for (int i = 0; i < MAX_SPRITES; i++) {
        if (!sprites[i].visible) continue;
        sprites[i].x += sprites[i].dx;
        sprites[i].y += sprites[i].dy;
    }
}

// Draw sprite directly in framebuffer
void sprite_draw_all(uint16_t *framebuffer, uint16_t screen_width, uint16_t screen_height) {
    for (int i = 0; i < MAX_SPRITES; i++) {
        Sprite *s = &sprites[i];
        if (!s->visible || !s->bitmap) continue;

        for (int yy = 0; yy < s->height; yy++) {
            int y = s->y + yy;
            if (y < 0 || y >= screen_height) continue;

            for (int xx = 0; xx < s->width; xx++) {
                int x = s->x + xx;
                if (x < 0 || x >= screen_width) continue;

                uint16_t color = s->bitmap[yy * s->width + xx];
                if (color != 0xFFFF) // transparency (white)
                    framebuffer[y * screen_width + x] = color;
            }
        }
    }
}

// Simple rectangular collision
bool sprite_collide(const Sprite *a, const Sprite *b) {
    return (a->x < b->x + b->width &&
            a->x + a->width > b->x &&
            a->y < b->y + b->height &&
            a->y + a->height > b->y);
}

// Check collisions between sprites and invoke callback
void sprite_check_collisions(collision_callback_t cb) {
    for (int i = 0; i < MAX_SPRITES; i++) {
        Sprite *a = &sprites[i];
        if (!a->visible) continue;

        for (int j = i + 1; j < MAX_SPRITES; j++) {
            Sprite *b = &sprites[j];
            if (!b->visible) continue;

            if (sprite_collide(a, b)) {
                if (cb) cb(i, j);
            }
        }
    }
}
