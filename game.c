#include "honeyfw.h"

#include <stdio.h>

static render_t tile_sprites;
static render_t player_sprite;

typedef struct {

	int sprites[4]; // can vary between all four
	unsigned char bool_collidable;

} Tile;

static const Tile tile_types[] = {
	{ { 0, 1, 2, 3 }, 0 }, // floor
	{ { 6, 6, 6, 6 }, 1 }, // wall
};

static int player_x = 2;
static int player_y = 2;

static void draw_tile(int tile, int x, int y) {

	draw_render_t(
		tile_sprites,
		(x - player_x + 16) * 8 - 4,
		(y - player_y + 10) * 8,
		0,
		&tile_types[tile].sprites[(((x >> 1) * x + (y >> 1) * y) * (y & 0b01010101) + x * 2 + y) % 4]
	);
}

static void draw_dungeon(int *tiles, int w, int h) {

	for (int x = 0; x < w; x++) {
		for (int y = 0; y < h; y++) {

			if ((y - player_y + 10) * 8 + 8 > 160) // bottom section of screen is for UI
				continue;

			draw_tile(tiles[x + y * w], x, y);
		}
	}
}

static void generate_dungeon(int *tiles, int w, int h) {

	// generate rectangular rooms
	// one by one, connect a non-connected room to its closest room until all rooms are connected
}

void init(int *width, int *height) {

	*width = 256;
	*height = 240;
	tile_sprites = load_sprite_sheet("rogue_tileset.png", 8, 8);
	player_sprite = load_sprite("knight.png");
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

	int tiles[25] = {
		1, 1, 1, 1, 1,
		1, 0, 0, 0, 1,
		1, 0, 0, 0, 1,
		1, 1, 0, 1, 1,
		0, 1, 0, 1, 0
	};

	draw_dungeon(tiles, 5, 5);

	draw_render_t(player_sprite, 124, 80, 0, NULL);
}