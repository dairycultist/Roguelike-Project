#ifndef LOGIC_H
#define LOGIC_H

typedef struct {

} Input;

void logic_init(int *width, int *height);
void logic_process(Input *input);

/*
 * rendering stuff
 */
typedef int render_t;

render_t load_sprite(const char *path);
render_t load_sprite_sheet(const char *path, int sprite_width, int sprite_height);
// render_t load_font(const char *path, int char_width, int char_height);

void draw_render_t(render_t r, int x, int y, int flip, const void *extra);
// extra's value:
// sprite 		= NULL
// sprite sheet = int *index
// font 		= char *text

void free_render_t(render_t r);

#endif