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

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include <SDL2/SDL.h>
#include "game.h"
#include "drmauro.h"

#define TITLE "DR. MAURO"
#define WIDTH 480
#define HEIGHT 320

#define BOARD_W (COLONNE * (CELL_SIZE +2))
#define BOARD_H (RIGHE * (CELL_SIZE +2))
#define BOARD_X ((WIDTH - BOARD_W)/2)-98
#define BOARD_Y ((HEIGHT - BOARD_H)/2)
#define CELL_SIZE 14

/******************************************************************************/
/* GLOBALS                                                                    */

font *dr_font;
sprite *logo;
sprite *score_bg;

sprite **enemies;
int enemies_len = 3;

sprite **pills;
int pills_len = 3;

/******************************************************************************/
/* SPRITES                                                                    */

void load_spites() {
  int i;
  SDL_Rect rect = {0};

  dr_font = make_font("img/font.bmp", 8, 16);
  rect.w = 196; rect.h = 44;

  logo = make_sprite("img/drmauro_logo.bmp", rect, NULL);
  rect.w = 128; rect.h = 164;
  score_bg = make_sprite("img/score_bg.bmp", rect, NULL);

  enemies = malloc(sizeof(sprite*) * enemies_len);
  pills = malloc(sizeof(sprite*) * pills_len);
  if (!(enemies && pills)) ERROR(("enemies array malloc error!"));
  for (i=0; i<enemies_len; i++) {
    SDL_Rect rect = {0, 0, CELL_SIZE, CELL_SIZE};
    char buf[512];
    SDL_Rect frames[] = {
      {0, 0, CELL_SIZE, CELL_SIZE},
      {CELL_SIZE, 0, CELL_SIZE, CELL_SIZE}
    };
    int frames_len = sizeof(frames)/sizeof(frames[0]);
    animation *an = make_animation(frames, frames_len, 0.2*(i+1));
    sprintf(buf, "img/enemy_%d.bmp", i);
    enemies[i] = make_sprite(buf, rect, an);
  }
  for (i=0; i<pills_len; i++) {
    SDL_Rect rect = {0, 0, CELL_SIZE, CELL_SIZE};
    char buf[512];
    sprintf(buf, "img/pill_%d.bmp", i);
    pills[i] = make_sprite(buf, rect, NULL);
  }
}

void update_sprites(double delta_time) {
  int i;
  for (i=0; i<enemies_len; i++)
    animation_step(enemies[i]->animation, delta_time);
}

void free_sprites() {
  int i;
  for (i=0; i<enemies_len; i++) sprite_free(enemies[i]);
  for (i=0; i<pills_len; i++) sprite_free(pills[i]);
  free(enemies);
  free(pills);
  sprite_free(logo);
  sprite_free(score_bg);
  font_free(dr_font);
}

/******************************************************************************/
/* DRAW                                                                       */

void draw_background(SDL_Surface *screen,  struct gioco *game_state) {
  int i, j;
  SDL_Rect rect;
  char scores[200];
  /* Draw background pattern */
  for (i=0; i<WIDTH; i += 16)
    for (j=0; j<HEIGHT; j += 16) {
      SDL_Rect rect;
      rect.x = i; rect.y = j; rect.w = 16; rect.h = 16;
      SDL_FillRect(screen, &rect, ((i / 16 + j / 16) %2 == 0)
                   ? SDL_MapRGB(screen->format, 0, 0, 0)
                   : SDL_MapRGB(screen->format, 0x44, 0, 0x9c));
    }
  /* Draw Logo */
  sprite_draw(logo, screen, WIDTH-10-logo->rect.w, 10);
  /* Draw Scores */
  {
    int y, x, virus = 0;
    x = WIDTH-score_bg->rect.w-46;
    y = ((HEIGHT-score_bg->rect.h)/2)+16;
    sprite_draw(score_bg, screen, x, y);
    font_draw_string(dr_font, screen, "SCORE",x+20,y+32, 1);
    sprintf(scores, "%06d", game_state->punti);
    font_draw_string(dr_font, screen, scores,x+20,y+48, 1);
    for (i=0; i<RIGHE; i++)
      for (j=0; j<COLONNE; j++)
        if (game_state->campo[i][j].tipo == MOSTRO) virus++;
    font_draw_string(dr_font, screen, "VIRUS", x+20, y+64,1);
    sprintf(scores, "%06d", virus);
    font_draw_string(dr_font, screen, scores, x+20, y+80, 1);
  }
  /* Clear board */
  rect.x = BOARD_X-5; rect.y = BOARD_Y-5; rect.w = BOARD_W+10; rect.h = BOARD_H+10;
  SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, 0,0xe8,0xd8));
  rect.x = BOARD_X; rect.y = BOARD_Y; rect.w = BOARD_W; rect.h = BOARD_H;
  SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, 0,0,0));
  rect.x = BOARD_X + BOARD_W/2 - CELL_SIZE*1.5; rect.y = BOARD_Y -5; rect.w = 3*CELL_SIZE; rect.h = 10;
  SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, 0,0,0));
}

void draw_board(SDL_Surface *screen,  struct gioco *game_state) {
  int i, j;
  for (i=0; i<RIGHE; i++)
    for (j=0; j<COLONNE; j++) {
      struct cella *cella = &game_state->campo[i][j];
      switch (cella->tipo) {
      case MOSTRO:
      case PASTIGLIA:
        sprite_draw(((cella->tipo == MOSTRO)?enemies:pills)[cella->colore],
                    screen, BOARD_X + j*16, BOARD_Y + i*16);
        break;
      default: /* do nothing */ break;
      }
    }
}

/******************************************************************************/
/* MAIN                                                                       */

void usage() {
  fprintf(stderr, "DR.MAURO - dr.Mario Clone                        \n"
          "Usage: drmauro [-f FILE | -d DIFFICULTY] [-s SPEED] [-h] \n"
          "                                                         \n"
          "OPTIONS:                                                 \n"
          "  -f FILE         Load board from FILE                   \n"
          "  -d DIFFICULTY   Generate random board (default 5)      \n"
          "  -s SPEED        Game speed (default 0.3 sec)           \n"
          "  -h              Show this help message                 \n"
          );
  exit(1);
}

int main(int argc, char **argv) {

  SDL_Window *window;
  SDL_Surface *screen;

  int running = 1;
  enum mossa command = NONE;
  int prev_time;
  double acc_time;

  struct gioco *game_state;
  char *board_file = NULL;
  int difficulty = 5;
  double speed = 0.3;

  extern char *optarg;
  extern int optind;
  char c;
  /* Parse command line arguments */
  while ((c = getopt(argc, argv, "f:d:s:h")) != -1) {
    switch (c) {
    case 'f': board_file = optarg;       break;
    case 'd': difficulty = atoi(optarg); break;
    case 's': speed = atof(optarg);      break;
    case 'h': usage();                   break;
    default:  usage();
    }
  }
  argc -= optind;
  argv += optind;
  if (argc) usage();


  /* Initialize the game */
  game_state = calloc(1,sizeof(struct gioco));
  if (!game_state) ERROR(("malloc error"));

  /* Initialize SDL */
  if (SDL_Init(SDL_INIT_VIDEO) <0) ERROR(("SDL_INIT failed!"));
  window = SDL_CreateWindow(TITLE,
                            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                            WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
  if (!window) ERROR(("SDL_CreateWindow failed!"));
  screen = SDL_GetWindowSurface(window);

  /* Load Sprites and images */
  load_spites();

  if (board_file)
    carica_campo(game_state, board_file);
  else
    riempi_campo(game_state, difficulty);

  prev_time = SDL_GetTicks();
  acc_time = 0;
  while (running) {
    SDL_Event e;
    int cur_time = SDL_GetTicks();
    double delta_time = (cur_time - prev_time) / (double)1000;
    prev_time = cur_time;
    acc_time += delta_time;

    /* Update animations */
    update_sprites(delta_time);

    /* Events */
    while (SDL_PollEvent(&e)) {
      switch (e.type) {
      case SDL_QUIT: running = 0; break;
      case SDL_KEYUP:
        switch (e.key.keysym.sym) {
        case SDLK_LEFT:  command = SINISTRA; break;
        case SDLK_RIGHT: command = DESTRA;   break;
        case SDLK_x:     command = ROT_DX;   break;
        case SDLK_z:     command = ROT_SX;   break;
        case SDLK_DOWN:  command = GIU;      break;
        case SDLK_q:     running = 0;        break;
        }
        break;
      }
    }

    /* Execute commands every speed seconds */
    if (acc_time > speed) {
      /* Check Victory */
      switch(vittoria(game_state)) {
      case SCONFITTA:
        running = 0;
        printf("Hai Perso. \nPunteggio: %d\n", game_state->punti);
        break;
      case VITTORIA:
        running = 0;
        printf("Vittoria! \nPunteggio: %d\n", game_state->punti);
        break;
      default:
        break;
      }
      /* Update State */
      step(game_state, command);
      /* Reset Commands */
      command = NONE;
      acc_time = 0;
    }
    /* Draw the game state */
    draw_background(screen, game_state);
    draw_board(screen, game_state);

    SDL_UpdateWindowSurface(window);
    SDL_Delay(1);
  }

  free_sprites();
  free(game_state);

  SDL_DestroyWindow(window);
  SDL_Quit();

  return EXIT_SUCCESS;
}
