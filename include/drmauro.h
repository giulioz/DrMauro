#ifndef DRMAURO_H
#define DRMAURO_H

#define RIGHE 16
#define COLONNE 8

enum contenuto { VUOTO, MOSTRO, PASTIGLIA };
enum colore { ROSSO, GIALLO, BLU };
enum mossa { NONE, DESTRA, SINISTRA, GIU, ROT_DX, ROT_SX };
enum stato { IN_CORSO, VITTORIA, SCONFITTA };

struct cella {
  enum contenuto tipo;
  enum colore colore;
  /* ---- */
  int id;
};

struct gioco {
  struct cella campo[RIGHE][COLONNE];
  int punti;
  /* ---- */
  int active_id;
};


void carica_campo(struct gioco *gioco, char *percorso);
void riempi_campo(struct gioco *gioco, int difficolta);
void step(struct gioco *gioco, enum mossa comando);
enum stato vittoria(struct gioco *gioco);

#endif
