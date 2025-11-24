#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "sprite.h"

static SDL_Renderer *renderer;

void set_sprite_renderer(void *sdl_renderer) {

	renderer = (SDL_Renderer *) sdl_renderer;
}

/*
 * single sprites
 */
Sprite *load_sprite(const char *path) {

	Sprite *sprite = malloc(sizeof(Sprite));

	sprite->sdl_texture = IMG_LoadTexture(renderer, path);

	SDL_QueryTexture(sprite->sdl_texture, NULL, NULL, &sprite->w, &sprite->h);

	return sprite;
}

void draw_sprite(Sprite *sprite, int x, int y, int flip) {

	SDL_Rect texture_rect = { x, y, sprite->w, sprite->h };

	SDL_RenderCopyEx(renderer, sprite->sdl_texture, NULL, &texture_rect, 0.0, NULL, flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

void free_sprite(Sprite *sprite) {
	
	SDL_DestroyTexture(sprite->sdl_texture);
	free(sprite);
}

/*
 * sprite sheets
 */
SpriteSheet *load_sprite_sheet(const char *path, int sprite_width, int sprite_height) {

	SpriteSheet *sprite_sheet = malloc(sizeof(SpriteSheet));

	sprite_sheet->sdl_texture = IMG_LoadTexture(renderer, path);

	// get the width to determine the number of sprites per row (rounding down)
	int width;
	SDL_QueryTexture(sprite_sheet->sdl_texture, NULL, NULL, &width, NULL);

	sprite_sheet->sprite_w = sprite_width;
	sprite_sheet->sprite_h = sprite_height;
	sprite_sheet->sprites_per_row = width / sprite_width;

	return sprite_sheet;
}

void draw_sprite_from_sheet(SpriteSheet *sprite_sheet, int index, int x, int y, int flip) {

	SDL_Rect copy_rect = {

		(index % sprite_sheet->sprites_per_row) * sprite_sheet->sprite_w,
		(index / sprite_sheet->sprites_per_row) * sprite_sheet->sprite_h,
		sprite_sheet->sprite_w,
		sprite_sheet->sprite_h
	};

	SDL_Rect paste_rect = { x, y, sprite_sheet->sprite_w, sprite_sheet->sprite_h };

	SDL_RenderCopyEx(renderer, sprite_sheet->sdl_texture, &copy_rect, &paste_rect, 0.0, NULL, flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

void free_sprite_sheet(SpriteSheet *sprite_sheet) {

	SDL_DestroyTexture(sprite_sheet->sdl_texture);
	free(sprite_sheet);
}

/*
 * special sprite sheet for text; follows a specific format
 */
void draw_text(SpriteSheet *sprite_sheet, char *text, int x, int y) {

	int start_x = x;

	while (*text) {

		if (*text >= 'A' && *text <= 'Z') {

			draw_sprite_from_sheet(sprite_sheet, *text - 65, x, y, 0);
			x += sprite_sheet->sprite_w;
		}

		else if (*text == '!') {
			draw_sprite_from_sheet(sprite_sheet, 26, x, y, 0);
			x += sprite_sheet->sprite_w;
		}

		else if (*text == '?') {
			draw_sprite_from_sheet(sprite_sheet, 27, x, y, 0);
			x += sprite_sheet->sprite_w;
		}

		else if (*text >= '\'' && *text <= '/') {

			draw_sprite_from_sheet(sprite_sheet, *text - 11, x, y, 0);
			x += sprite_sheet->sprite_w;
		}

		else if (*text == '\n') {

			x = start_x;
			y += sprite_sheet->sprite_h + 1; // + 1 for line height spacing
		}

		else if (*text == ' ') {

			x += sprite_sheet->sprite_w;
		}

		text++;
	}
}