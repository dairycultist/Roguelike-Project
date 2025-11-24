#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "sprite.h"
#include "logic.h"

#define ASPECT_RATIO (width / (float) height)
static int width = 256;
static int height = 240;

static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *screen_buffer;

static void refit_letterbox(SDL_Rect *letterbox, int window_w, int window_h) {

	#define MIN(a, b) ((a) > (b) ? (b) : (a))

	// dynamically change letterbox based on screen resize
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

	window = SDL_CreateWindow("Roguelike", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_RESIZABLE);

	if (!window) {
		printf("Error creating window:\n%s\n", SDL_GetError());
		return 2;
    }

	SDL_Rect letterbox;
	refit_letterbox(&letterbox, 600, 400);

	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer) {
		printf("Error creating renderer:\n%s\n", SDL_GetError());
		return 3;
	}

	set_sprite_renderer(renderer);

	// initialize game state, including screen width/height
	logic_init(&width, &height);

	// initialize screen
	screen_buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, width, height);

	if (!screen_buffer) {
		printf("Error creating screen buffer:\n%s\n", SDL_GetError());
		return 4;
	}

	// process events until window is closed
	SDL_Event event;
	char running = 1;

	while (running) {

		while (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {

				running = 0;

			} else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {

				refit_letterbox(&letterbox, event.window.data1, event.window.data2);

			} else if ((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) && !event.key.repeat) {

				// event.key.keysym.scancode
				// event.key.state == SDL_PRESSED
				// SDL_SCANCODE_Z

				printf("%d\n", event.key.keysym.scancode);
			}
		}

		SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255); 			// clear window to grey
		SDL_RenderClear(renderer);
		SDL_SetRenderTarget(renderer, screen_buffer); 				// set render target to screen_buffer
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 			// clear screen_buffer to black
		SDL_RenderClear(renderer);

		logic_process(NULL);

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