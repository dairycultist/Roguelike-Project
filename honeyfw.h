#ifndef HONEYFW_H
#define HONEYFW_H

/*
 * logic stuff
 */
typedef unsigned char istate;

#define PRESSED(i) ((i) & 0x00000001)
#define JUST_PRESSED(i) ((i) & 0x00000011)
#define RELEASED(i) (!((i) & 0x00000001))
#define JUST_RELEASED(i) (!((i) & 0x00000001) && ((i) & 0x00000010))

typedef struct {

	istate up, down, left, right;

} Input;

void init(int *width, int *height);
void process(Input *input);

/*
 * rendering stuff
 */
typedef int render_t;

render_t load_sprite(const char *path);
render_t load_sprite_sheet(const char *path, int sprite_width, int sprite_height);
// render_t load_font(const char *path, int char_width, int char_height);

/* draw_render_t
 *
 * extra's value depends on the type of render_t:
 * sprite 		= NULL
 * sprite sheet = int *index
 * font 		= char *text
 */
void draw_render_t(render_t r, int x, int y, int flip, const void *extra);

void free_render_t(render_t r);

#endif