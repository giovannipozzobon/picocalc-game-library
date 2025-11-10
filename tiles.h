#include "gfx_tilemap.h"

#define TILE_SIZE 16

// 2 tile di esempio: blu e verde
static const uint16_t tileset[] = {
    // Tile 0 (blu)
    [0 ... TILE_SIZE*TILE_SIZE-1] = RGB565(0, 0, 31),
    // Tile 1 (verde)
    [TILE_SIZE*TILE_SIZE ... TILE_SIZE*2*TILE_SIZE-1] = RGB565(0, 63, 0)
};

// Mappa 10x10
static const uint8_t tilemap_data[10*10] = {
    0,1,0,1,0,1,0,1,0,1,
    1,0,1,0,1,0,1,0,1,0,
    0,1,0,1,0,1,0,1,0,1,
    1,0,1,0,1,0,1,0,1,0,
    0,1,0,1,0,1,0,1,0,1,
    1,0,1,0,1,0,1,0,1,0,
    0,1,0,1,0,1,0,1,0,1,
    1,0,1,0,1,0,1,0,1,0,
    0,1,0,1,0,1,0,1,0,1,
    1,0,1,0,1,0,1,0,1,0
};

tilemap_t map = {
    .width = 10,
    .height = 10,
    .tile_size = TILE_SIZE,
    .tileset = tileset,
    .map = tilemap_data
};
