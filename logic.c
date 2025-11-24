#include "logic.h"
#include "sprite.h"

static SpriteSheet *world_sprites;

void logic_init() {
	world_sprites = load_sprite_sheet("rogue_tileset.png", 8, 8);
}

void logic_process() {
	draw_sprite_from_sheet(world_sprites, 0, 0, 0, 0);
}