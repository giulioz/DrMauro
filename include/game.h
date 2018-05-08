#ifndef GAME_H
#define GAME_H

#define ERROR(args) { printf("<!> Error: "); printf args; puts(""); exit(1); }

typedef struct {
    SDL_Surface *image;
    int w;
    int h;
} font;

font *make_font(char *image_path, int w, int h);
void font_draw_char(font *f, SDL_Surface *dst, char ch, int x, int y, float scale);
void font_draw_string(font *f, SDL_Surface *dst, char *str, int x, int y, float scale);
void font_free(font *f);

typedef struct {
    SDL_Rect *frames;
    int frames_len;
    double frame_duration;
    double current_time;
    double total_time;
} animation;

animation *make_animation(SDL_Rect *frames, int frames_len, double frame_duration);
void animation_step(animation *a, double delta_time);
SDL_Rect animation_current_frame(animation *a);
void animation_free(animation *a);

typedef struct {
    SDL_Surface *sfc;
    SDL_Rect rect;
    animation *animation;
} sprite;

sprite *make_sprite(char *image_path, SDL_Rect rect, animation *a);
void sprite_draw(sprite *s, SDL_Surface *dst, int x, int y);
void sprite_free(sprite *s);

#endif
