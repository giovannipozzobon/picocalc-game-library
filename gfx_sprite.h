#ifndef GFX_SPRITE_H
#define GFX_SPRITE_H

#include <stdint.h>
#include <stdbool.h>

// Sprite massimo (ad es. 32 sprite gestibili)
#define MAX_SPRITES 32

// Struttura che rappresenta uno sprite
typedef struct {
    int16_t x, y;             // posizione sullo schermo
    int16_t width, height;    // dimensioni sprite
    int16_t dx, dy;           // velocità
    const uint16_t *bitmap;   // puntatore ai dati grafici (RGB565)
    bool visible;             // visibilità
    uint8_t type;             // tipo logico (per collisioni)
} Sprite;

// Inizializza il sistema sprite
void sprite_init(void);

// Crea un nuovo sprite e restituisce il suo ID
int sprite_create(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t *bitmap, uint8_t type);

// Aggiorna la posizione in base alla velocità
void sprite_update_all(void);

// Disegna tutti gli sprite visibili sul framebuffer
void sprite_draw_all(uint16_t *framebuffer, uint16_t screen_width, uint16_t screen_height);

// Controlla collisione tra due sprite
bool sprite_collide(const Sprite *a, const Sprite *b);

// Trova collisioni tra sprite e invoca un callback
typedef void (*collision_callback_t)(int id1, int id2);
void sprite_check_collisions(collision_callback_t cb);

// Accesso diretto
Sprite *sprite_get(int id);

#endif
