/*
 * Copyright (c) Lorenzo Veronese 2018
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdlib.h>

#include <SDL2/SDL.h>
#include "game.h"


font *make_font(char *image_path, int w, int h) {
  font *f = malloc(sizeof(font));
  if (!f) ERROR(("font malloc error!"));
  f->w = w;
  f->h = h;
  f->image = SDL_LoadBMP(image_path);
  if (!f->image) ERROR(("Cannot load font! (%s)", image_path));
  return f;
}

void font_draw_char(font *f, SDL_Surface *dst, char ch, int x, int y, float scale) {
  SDL_Rect srect, drect;
  srect.x = (ch % 32) * f->w;
  srect.y = (ch / 32) * f->h;
  srect.w = f->w; srect.h = f->h;
  drect.x = x; drect.y = y; drect.w = f->w *scale; drect.h = f->h *scale;
  SDL_BlitScaled(f->image, &srect, dst, &drect);
}

void font_draw_string(font *f, SDL_Surface *dst, char *str, int x, int y, float scale) {
  while (*str) {
    font_draw_char(f, dst, *str, x,y,scale);
    x+=f->w*scale;
    str++;
  }
}

void font_free(font *f) {
  SDL_FreeSurface(f->image);
  free(f);
}

animation *make_animation(SDL_Rect *frames, int frames_len, double frame_duration) {
  int i;
  animation *a = malloc(sizeof(animation));
  if (!a) ERROR(("animation malloc error!"));
  a->frame_duration = frame_duration;
  a->current_time = 0;
  a->total_time = 0;
  a->frames_len = frames_len;
  a->frames = malloc(sizeof(SDL_Rect)*frames_len);
  if (!a->frames) ERROR(("animation frames malloc error!"));
  for (i=0; i<frames_len; i++)
    a->frames[i] = frames[i];
  return a;
}

void animation_step(animation *a, double delta_time) {
  a->total_time += delta_time;
  a->current_time = fmod(a->total_time, a->frames_len*a->frame_duration); 
}

SDL_Rect animation_current_frame(animation *a) {
  return a->frames[(int)(a->current_time / a->frame_duration)];
}

void animation_free(animation *a) {
  free(a->frames);
  free(a);
}

sprite *make_sprite(char *image_path, SDL_Rect rect, animation *a) {
  sprite *s = malloc(sizeof(sprite));
  if (!s) ERROR(("sprite malloc error!"));
  s->rect = rect;
  s->sfc = SDL_LoadBMP(image_path);
  if (!s->sfc) ERROR(("Cannot load image! (%s)", image_path));
  s->animation = a;
  return s;
}

void sprite_draw(sprite *s, SDL_Surface *dst, int x, int y) {
  SDL_Rect srect = s->rect, drect = s->rect;
  if (s->animation) {
    drect = animation_current_frame(s->animation);
    srect = drect;
  }
  drect.x = x; drect.y = y;
  SDL_BlitSurface(s->sfc, &srect, dst, &drect);
}

void sprite_free(sprite *s){
  SDL_FreeSurface(s->sfc);
  if (s->animation) animation_free(s->animation);
  free(s);
}
