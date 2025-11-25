#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "honeyfw.h"

#define ASPECT_RATIO (width / (float) height)
static int width = 128;
static int height = 128;

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *screen_buffer;

static inline void refit_letterbox(SDL_Rect *letterbox, int window_w, int window_h) {

	#define MIN(a, b) ((a) > (b) ? (b) : (a))

	// dynamically change letterbox based on window resize
	letterbox->w = MIN(window_w, window_h * ASPECT_RATIO);
	letterbox->h = MIN(window_h, window_w / ASPECT_RATIO);

	letterbox->x = (window_w - letterbox->w) / 2;
	letterbox->y = (window_h - letterbox->h) / 2;
}

int main() {

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Error initializing SDL:\n%s\n", SDL_GetError());
		return 1;
	}

	// initialize window
	window = SDL_CreateWindow("Roguelike", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_RESIZABLE);

	if (!window) {
		printf("Error creating window:\n%s\n", SDL_GetError());
		return 2;
    }

	// initialize window renderer
	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer) {
		printf("Error creating renderer:\n%s\n", SDL_GetError());
		return 3;
	}

	// initialize game state (which requires renderer), including screen width/height
	init(&width, &height);

	// initialize screen
	screen_buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, width, height);

	if (!screen_buffer) {
		printf("Error creating screen buffer:\n%s\n", SDL_GetError());
		return 4;
	}

	SDL_Rect letterbox;
	refit_letterbox(&letterbox, 600, 400);

	// process events until window is closed
	SDL_Event event;
	Input input = {};
	char running = 1;

	while (running) {

		input.up_justchanged    = 0;
		input.down_justchanged  = 0;
		input.left_justchanged  = 0;
		input.right_justchanged = 0;

		while (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {

				running = 0;

			} else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {

				refit_letterbox(&letterbox, event.window.data1, event.window.data2);

			} else if ((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) && !event.key.repeat) {

				switch (event.key.keysym.scancode) {

					case SDL_SCANCODE_UP:
						input.up = event.key.state == SDL_PRESSED;
						input.up_justchanged = 1;
						break;

					case SDL_SCANCODE_DOWN:
						input.down = event.key.state == SDL_PRESSED;
						input.down_justchanged = 1;
						break;

					case SDL_SCANCODE_LEFT:
						input.left = event.key.state == SDL_PRESSED;
						input.left_justchanged = 1;
						break;

					case SDL_SCANCODE_RIGHT:
						input.right = event.key.state == SDL_PRESSED;
						input.right_justchanged = 1;
						break;
					
					default:
						break;
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255); 			// clear window to grey
		SDL_RenderClear(renderer);
		SDL_SetRenderTarget(renderer, screen_buffer); 				// set render target to screen_buffer
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 			// clear screen_buffer to black
		SDL_RenderClear(renderer);

		process(&input);

		SDL_SetRenderTarget(renderer, NULL); 						// reset render target back to window
		SDL_RenderCopy(renderer, screen_buffer, NULL, &letterbox); 	// render screen_buffer
		SDL_RenderPresent(renderer); 								// present rendered content to screen

		SDL_Delay(1000 / 60);
	}

	SDL_DestroyTexture(screen_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

/*
 * rendering implementation
 */

#define TYPE_SPRITE 0
#define TYPE_SPRITE_SHEET 1
#define TYPE_FONT 2

typedef struct {

	unsigned char type;
    void *sdl_texture;
    int w;
    int h;

} Sprite;

typedef struct {

	unsigned char type;
    void *sdl_texture;
    int sprite_w;
    int sprite_h;
    int sprites_per_row;

} SpriteSheet;

typedef struct {

	unsigned char type;
	render_t sprite_sheet;

} Font;

static void *render_ts[256] = {};

render_t load_sprite(const char *path) {

	Sprite *sprite = malloc(sizeof(Sprite));

	sprite->type = TYPE_SPRITE;
	sprite->sdl_texture = IMG_LoadTexture(renderer, path);

	SDL_QueryTexture(sprite->sdl_texture, NULL, NULL, &sprite->w, &sprite->h);

	//
	for (render_t i = 0; i < sizeof(render_ts); i++) {

		if (render_ts[i] == NULL) {

			render_ts[i] = sprite;
			return i;
		}
	}

	return -1;
}

render_t load_sprite_sheet(const char *path, int sprite_width, int sprite_height) {

	SpriteSheet *sprite_sheet = malloc(sizeof(SpriteSheet));

	sprite_sheet->type = TYPE_SPRITE_SHEET;
	sprite_sheet->sdl_texture = IMG_LoadTexture(renderer, path);

	// get the width to determine the number of sprites per row (rounding down)
	int width;
	SDL_QueryTexture(sprite_sheet->sdl_texture, NULL, NULL, &width, NULL);

	sprite_sheet->sprite_w = sprite_width;
	sprite_sheet->sprite_h = sprite_height;
	sprite_sheet->sprites_per_row = width / sprite_width;

	//
	for (render_t i = 0; i < sizeof(render_ts); i++) {

		if (render_ts[i] == NULL) {

			render_ts[i] = sprite_sheet;
			return i;
		}
	}

	return -1;
}

render_t load_font(const char *path, int char_width, int char_height) {

	Font *font = malloc(sizeof(Font));

	font->type = TYPE_FONT;
	font->sprite_sheet = load_sprite_sheet(path, char_width, char_height);

	//
	for (render_t i = 0; i < sizeof(render_ts); i++) {

		if (render_ts[i] == NULL) {

			render_ts[i] = font;
			return i;
		}
	}

	return -1;
}

void draw_render_t(render_t r, int x, int y, int flip, const void *extra) {

	switch (*(unsigned char *) render_ts[r]) {

		case TYPE_SPRITE: {
			Sprite *sprite = (Sprite *) render_ts[r];
			SDL_Rect texture_rect = { x, y, sprite->w, sprite->h };
			SDL_RenderCopyEx(renderer, sprite->sdl_texture, NULL, &texture_rect, 0.0, NULL, flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
			break;
		}

		case TYPE_SPRITE_SHEET: {
			SpriteSheet *sprite_sheet = (SpriteSheet *) render_ts[r];
			SDL_Rect copy_rect = {
				((*(int *) extra) % sprite_sheet->sprites_per_row) * sprite_sheet->sprite_w,
				((*(int *) extra) / sprite_sheet->sprites_per_row) * sprite_sheet->sprite_h,
				sprite_sheet->sprite_w,
				sprite_sheet->sprite_h
			};
			SDL_Rect paste_rect = { x, y, sprite_sheet->sprite_w, sprite_sheet->sprite_h };
			SDL_RenderCopyEx(renderer, sprite_sheet->sdl_texture, &copy_rect, &paste_rect, 0.0, NULL, flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
			break;
		}

		case TYPE_FONT: { // special sprite sheet for text; follows a specific format

			render_t sprite_sheet = ((Font *) render_ts[r])->sprite_sheet;
			SpriteSheet *sprite_sheet_obj = (SpriteSheet *) (render_ts[sprite_sheet]);

			const char *text = extra;
			int start_x = x;

			int sprite_i;

			while (*text) {

				if (*text == '\n') {

					x = start_x;
					y += sprite_sheet_obj->sprite_h + 1; // + 1 for line height spacing
					
				} else if (*text == ' ') {

					x += sprite_sheet_obj->sprite_w;

				} else {

					if (*text >= 'A' && *text <= 'Z') {
						sprite_i = *text - 65;
					} else if (*text >= '0' && *text <= '9') {
						sprite_i = *text - 10;
					} else if (*text == ':') {
						sprite_i = 37;
					} else if (*text == '!') {
						sprite_i = 26;
					} else if (*text >= '\'' && *text <= '/') {
						sprite_i = *text - 11;
					} else {
						sprite_i = 27; // all unrecognized characters map to ? (including actual ?)
					}

					draw_render_t(sprite_sheet, x, y, 0, &sprite_i);
					x += sprite_sheet_obj->sprite_w;
				}

				text++;
			}
			break;
		}
		
		default:
			break;
	}
}

void free_render_t(render_t r) {
	
	switch (*(unsigned char *) render_ts[r]) {

		case TYPE_SPRITE:
			SDL_DestroyTexture(((Sprite *) render_ts[r])->sdl_texture);
			break;

		case TYPE_SPRITE_SHEET:
		case TYPE_FONT:
			SDL_DestroyTexture(((SpriteSheet *) render_ts[r])->sdl_texture);
			break;
		
		default:
			break;
	}

	free(render_ts[r]);
	render_ts[r] = NULL;
}