#include "honeyfw.h"

#include <stdio.h>

static render_t tile_sprites;

typedef unsigned char tile_id;

typedef struct {

	int sprites[4]; // can vary between all four
	unsigned char bool_collidable;

} Tile;

static const Tile tiles[] = {
	{ { 0, 1, 2, 3 }, 0 }
};

static int player_x = 0;
static int player_y = 0;

static void draw_tile(const Tile *tile, int x, int y) {
	draw_render_t(
		tile_sprites,
		(x - player_x) * 8,
		(y - player_y) * 8,
		0,
		&tile->sprites[(((x >> 1) * x + (y >> 1) * y) * (y & 0b01010101) + x * 2 + y) % 4]
	);
}

static void generate_dungeon(tile_id **dungeon, int w, int h) {

	// generate rectangular rooms
	// one by one, connect a non-connected room to its closest room until all rooms are connected
}

void init(int *width, int *height) {

	*width = 256;
	*height = 240;
	tile_sprites = load_sprite_sheet("rogue_tileset.png", 8, 8);
}

void process(Input *input) {

	if (input->up && input->up_justchanged) {
		player_y--;
	}

	if (input->down && input->down_justchanged) {
		player_y++;
	}

	if (input->left && input->left_justchanged) {
		player_x--;
	}

	if (input->right && input->right_justchanged) {
		player_x++;
	}

	for (int x = 0; x < 20; x++) {
		for (int y = 0; y < 20; y++) {
			draw_tile(&tiles[0], x, y);
		}
	}
}