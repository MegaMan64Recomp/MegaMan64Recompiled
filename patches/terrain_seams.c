#include "patches.h"

// Terrain seams: every terrain tile is placed by its own PSX style matrix (4.12 rotation, integer translation), so
// the edges of neighboring tiles land up to a unit apart. That's invisible at 320x240 but shows up as thin lines
// between tiles at higher resolutions. The terrain draw marks its tiles so they get scaled up very slightly, making
// each tile overlap its neighbors enough to cover the gap.

typedef struct {
    s16 m[3][3];
    s16 pad;
    s32 t[3];
} TerrainMatrix; // size = 0x20

void func_80031E10_D210(TerrainMatrix* in, TerrainMatrix* out); // TransposeMatrix
void func_8002C420_7820(TerrainMatrix* in, Mtx* out); // PSX matrix to Mtx

// Extra size given to terrain tiles, in 1/4096ths (~3%).
#define TERRAIN_TILE_OVERLAP 128 // Distant tiles need ~100 to close their seams.

static bool sTerrainTile = FALSE;

// Called by the us.rev1.toml hooks around the terrain draw's matrix conversions.
void terrain_tile_matrix_begin(void) {
    sTerrainTile = TRUE;
}

void terrain_tile_matrix_end(void) {
    sTerrainTile = FALSE;
}

// psx_matrix_to_mtx
RECOMP_PATCH Mtx* func_8002C5A4_79A4(TerrainMatrix* in, Mtx* out) {
    TerrainMatrix tmp;

    memcpy(&tmp, in, sizeof(TerrainMatrix));
    func_80031E10_D210(in, &tmp);
    tmp.t[0] <<= 16;
    tmp.t[1] <<= 16;
    tmp.t[2] <<= 16;

    //@recomp Grow terrain tiles slightly so they overlap and hide the seams between them.
    if (sTerrainTile) {
        s32 scale = 4096 + TERRAIN_TILE_OVERLAP;
        s32 r, c;
        for (r = 0; r < 3; r++) {
            for (c = 0; c < 3; c++) {
                tmp.m[r][c] = (tmp.m[r][c] * scale) >> 12;
            }
        }
    }

    func_8002C420_7820(&tmp, out);
    return out;
}
