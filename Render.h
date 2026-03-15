#ifndef RENDER_H
#define RENDER_H

#include "game.h"
#include "raylib.h"

/* ── Dimensions de la fenêtre ─────────────────────────────────────────── */
#define SCREEN_WIDTH   600
#define SCREEN_HEIGHT  700

/* ── Dimensions du plateau ────────────────────────────────────────────── */
#define BOARD_OFFSET_X   75    /* Pixel X de la case (0,0)   */
#define BOARD_OFFSET_Y  150    /* Pixel Y de la case (0,0)   */
#define CELL_SIZE       150    /* Largeur/hauteur d'une case  */
#define LINE_THICKNESS    5    /* Épaisseur des lignes de grille */

/* ── Palette de couleurs ──────────────────────────────────────────────── */
#define CLR_BG         CLITERAL(Color){ 15,  15,  28, 255 }
#define CLR_GRID       CLITERAL(Color){ 70,  70, 110, 255 }
#define CLR_X          CLITERAL(Color){255,  75,  75, 255 }
#define CLR_O          CLITERAL(Color){ 75, 175, 255, 255 }
#define CLR_WIN_FLASH  CLITERAL(Color){255, 220,  50, 255 }
#define CLR_CELL_HOVER CLITERAL(Color){ 35,  35,  55, 255 }
#define CLR_BTN        CLITERAL(Color){ 50,  50,  90, 255 }
#define CLR_BTN_HOVER  CLITERAL(Color){ 80,  80, 140, 255 }
#define CLR_TEXT       CLITERAL(Color){220, 220, 240, 255 }
#define CLR_TITLE_X    CLITERAL(Color){255, 100, 100, 255 }
#define CLR_TITLE_O    CLITERAL(Color){100, 200, 255, 255 }

/* ── Fonctions publiques ──────────────────────────────────────────────── */

void render_menu      (const Game *game);
void render_game      (const Game *game);
void render_game_over (const Game *game);
void render_frame     (const Game *game);

/**
 * Convertit des coordonnées souris en (row, col) du plateau.
 * Écrit -1,-1 si hors plateau.
 */
void render_get_cell_from_mouse(int mouse_x, int mouse_y, int *row, int *col);

/**
 * Dessine un bouton et retourne true si la souris est dessus.
 */
bool render_button(int x, int y, int w, int h, const char *label, int font_size);

#endif /* RENDER_H */