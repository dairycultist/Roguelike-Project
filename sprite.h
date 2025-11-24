#ifndef SPRITE_H
#define SPRITE_H

void set_sprite_renderer(void *sdl_renderer);

typedef struct {

    void *sdl_texture;
    int w;
    int h;

} Sprite;

Sprite *load_sprite(const char *path);
void draw_sprite(Sprite *sprite, int x, int y, int flip);
void free_sprite(Sprite *sprite);

typedef struct {

    void *sdl_texture;
    int sprite_w;
    int sprite_h;
    int sprites_per_row;

} SpriteSheet;

SpriteSheet *load_sprite_sheet(const char *path, int sprite_width, int sprite_height);
void draw_sprite_from_sheet(SpriteSheet *sprite_sheet, int index, int x, int y, int flip);
void draw_text(SpriteSheet *sprite_sheet, char *text, int x, int y);
void free_sprite_sheet(SpriteSheet *sprite_sheet);

#endif