#ifndef SPRITE_H
#define SPRITE_H

typedef struct {

    void *sdl_texture;
    int w;
    int h;

} Sprite;

Sprite *load_sprite(SDL_Renderer *renderer, const char *path);
void draw_sprite(SDL_Renderer *renderer, Sprite *sprite, int x, int y, int flip);
void free_sprite(Sprite *sprite);

typedef struct {

    void *sdl_texture;
    int sprite_w;
    int sprite_h;
    int sprites_per_row;

} SpriteSheet;

SpriteSheet *load_sprite_sheet(SDL_Renderer *renderer, const char *path, int sprite_width, int sprite_height);
void draw_sprite_from_sheet(SDL_Renderer *renderer, SpriteSheet *sprite_sheet, int index, int x, int y, int flip);
void draw_text(SDL_Renderer *renderer, SpriteSheet *sprite_sheet, char *text, int x, int y);
void free_sprite_sheet(SpriteSheet *sprite_sheet);

#endif