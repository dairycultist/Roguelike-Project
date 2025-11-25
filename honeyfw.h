#ifndef HONEYFW_H
#define HONEYFW_H

/*
 * input stuff
 */
typedef struct {

    int up, down, left, right;
    int up_justchanged, down_justchanged, left_justchanged, right_justchanged;

} Input;

/*
 * logic stuff
 */
void init(int *width, int *height);
void process(Input *input);

/*
 * rendering stuff
 */
typedef int render_t;

render_t load_sprite(const char *path);
render_t load_sprite_sheet(const char *path, int sprite_width, int sprite_height);
render_t load_font(const char *path, int char_width, int char_height);

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