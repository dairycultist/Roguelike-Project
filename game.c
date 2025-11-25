#include "honeyfw.h"

#include <stdlib.h>
#include <time.h>

static render_t tile_sprites;
static render_t player_sprite;

typedef struct {

	int sprites[4]; // can vary between all four
	unsigned char bool_collidable;

} Tile;

typedef struct {

	int w, h;

	int *tiles;
	int start_x, start_y;

} Dungeon;

static const Tile tile_types[] = {
	{ { 0, 0, 0, 0 }, 0 }, // NULL
	{ { 0, 1, 2, 3 }, 0 }, // floor
	{ { 6, 6, 6, 6 }, 1 }, // wall
	{ { 7, 7, 7, 7 }, 1 }, // door
	{ { 4, 4, 4, 4 }, 0 }, // spike floor
};

static int player_x = 2;
static int player_y = 2;

static Dungeon dungeon;

static void draw_tile(int tile, int x, int y) {

	if (tile == 0)
		return;

	draw_render_t(
		tile_sprites,
		(x - player_x + 16) * 8 - 4,
		(y - player_y + 10) * 8,
		0,
		&tile_types[tile].sprites[(((x >> 1) * x + (y >> 1) * y) * (y & 0b01010101) + x * 2 + y) % 4]
	);
}

static void draw_dungeon(Dungeon *dungeon) {

	for (int x = 0; x < dungeon->w; x++) {
		for (int y = 0; y < dungeon->h; y++) {

			if ((y - player_y + 10) * 8 + 8 > 160) // bottom section of screen is for UI
				continue;

			draw_tile(dungeon->tiles[x + y * dungeon->w], x, y);
		}
	}
}

static void generate_corridor(Dungeon *dungeon, int ax, int ay, int bx, int by) {

	int dx = ax < bx ? 1 : -1;
	int dy = ay < by ? 1 : -1;

	int boolean_placed_door = 0;

	while (ax != bx || ay != by) {

		// floors
		if (dungeon->tiles[ax + ay * dungeon->w] == 2) {

			if (boolean_placed_door) {
				dungeon->tiles[ax + ay * dungeon->w] = 1;
			} else {
				dungeon->tiles[ax + ay * dungeon->w] = 3; // door
				boolean_placed_door = 1;
			}

		} else if (dungeon->tiles[ax + ay * dungeon->w] == 0) {
			dungeon->tiles[ax + ay * dungeon->w] = 1;
		}

		// walls
		if (dungeon->tiles[ax + (ay - 1) * dungeon->w] == 0)
			dungeon->tiles[ax + (ay - 1) * dungeon->w] = 2;
		if (dungeon->tiles[ax + (ay + 1) * dungeon->w] == 0)
			dungeon->tiles[ax + (ay + 1) * dungeon->w] = 2;

		// walls
		if (dungeon->tiles[ax - 1 + ay * dungeon->w] == 0)
			dungeon->tiles[ax - 1 + ay * dungeon->w] = 2;
		if (dungeon->tiles[ax + 1 + ay * dungeon->w] == 0)
			dungeon->tiles[ax + 1 + ay * dungeon->w] = 2;

		if (ax != bx) {
			ax += dx;
		} else {
			ay += dy;
		}
	}
}

// must have w, h preset (might change later so this instead spits out a malloc-ed dungeon pointer idc rn)
static void generate_dungeon(Dungeon *dungeon) {

	dungeon->tiles = calloc(dungeon->w * dungeon->h, sizeof(int));

	int room_xs[128];
	int room_ys[128];

	int room_count = dungeon->w * dungeon->h / 800;

	// generate n rectangular rooms
	for (int i = 0; i < room_count; i++) {

		int w = (rand() % 7) + 5;
		int h = (rand() % 7) + 5;

		int x = rand() % (dungeon->w - w);
		int y = rand() % (dungeon->h - h);

		// walls
		for (int ix = x; ix < x + w; ix++) {
		for (int iy = y; iy < y + h; iy++) {
			
			if (dungeon->tiles[ix + iy * dungeon->w] == 0)
				dungeon->tiles[ix + iy * dungeon->w] = 2;
		}}

		// floor + content
		for (int ix = x + 1; ix < x + w - 1; ix++) {
		for (int iy = y + 1; iy < y + h - 1; iy++) {
			
			if ((ix % 8) / 2 == 0 && (iy % 8) / 2 == 0) {
				dungeon->tiles[ix + iy * dungeon->w] = 4;
			} else {
				dungeon->tiles[ix + iy * dungeon->w] = 1;
			}
		}}

		room_xs[i] = x + w / 2;
		room_ys[i] = y + h / 2;
	}

	// for each room, generate a corridor between its center and the center of the following room
	// (may change it up later; just want to make sure all rooms are connected for now)
	for (int i = 0; i < room_count; i++) {
		
		generate_corridor(
			dungeon,
			room_xs[i],
			room_ys[i],
			room_xs[(i + 1) % room_count],
			room_ys[(i + 1) % room_count]
		);
	}

	dungeon->start_x = room_xs[0];
	dungeon->start_y = room_ys[0];
}

void init(int *width, int *height) {

	srand(time(NULL));

	*width = 256;
	*height = 240;
	tile_sprites = load_sprite_sheet("rogue_tileset.png", 8, 8);
	player_sprite = load_sprite("knight.png");

	dungeon.w = 64;
	dungeon.h = 64;
	generate_dungeon(&dungeon);

	player_x = dungeon.start_x;
	player_y = dungeon.start_y;
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

	draw_dungeon(&dungeon);

	draw_render_t(player_sprite, 124, 80, 0, NULL);
}