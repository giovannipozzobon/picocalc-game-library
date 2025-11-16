#ifndef GFX_SPRITE_H
#define GFX_SPRITE_H

#include <stdint.h>
#include <stdbool.h>

// Maximum sprites (e.g. 32 manageable sprites)
#define MAX_SPRITES 32

// Structure representing a sprite
typedef struct {
    int16_t x, y;             // position on screen
    int16_t width, height;    // sprite dimensions
    int16_t dx, dy;           // velocity
    const uint16_t *bitmap;   // pointer to graphic data (RGB565)
    bool visible;             // visibility
    uint8_t type;             // logical type (for collisions)
} Sprite;

// Initialize sprite system
void sprite_init(void);

// Create a new sprite and return its ID
int sprite_create(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t *bitmap, uint8_t type);

// Update position based on velocity
void sprite_update_all(void);

// Draw all visible sprites on framebuffer
void sprite_draw_all(uint16_t *framebuffer, uint16_t screen_width, uint16_t screen_height);

// Check collision between two sprites
bool sprite_collide(const Sprite *a, const Sprite *b);

// Find collisions between sprites and invoke a callback
typedef void (*collision_callback_t)(int id1, int id2);
void sprite_check_collisions(collision_callback_t cb);

// Direct access
Sprite *sprite_get(int id);

#endif
