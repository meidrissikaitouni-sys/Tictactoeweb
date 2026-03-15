#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#define BOARD_SIZE 3

/* ── Types ─────────────────────────────────────────────────────────────── */

typedef enum {
    CELL_EMPTY = 0,
    CELL_X,
    CELL_O
} Cell;

typedef enum {
    STATE_MENU,
    STATE_PLAYING,
    STATE_GAME_OVER
} GameState;

typedef enum {
    PLAYER_HUMAN,
    PLAYER_AI
} PlayerType;

typedef enum {
    DIFFICULTY_EASY,   /* IA joue au hasard             */
    DIFFICULTY_HARD    /* IA utilise Minimax + élagage  */
} Difficulty;

/* ── Structure principale ──────────────────────────────────────────────── */

typedef struct {
    Cell        board[BOARD_SIZE][BOARD_SIZE];
    Cell        current_player;     /* CELL_X ou CELL_O             */
    Cell        winner;             /* CELL_EMPTY si pas de gagnant */
    bool        is_draw;

    GameState   state;
    PlayerType  player_x_type;
    PlayerType  player_o_type;
    Difficulty  difficulty;

    int         score_x;
    int         score_o;
    int         score_draws;

    /* Cellules de la ligne gagnante (pour l'animation) */
    int         winning_cells[3][2];
    bool        has_winning_line;
} Game;

/* ── Fonctions publiques ───────────────────────────────────────────────── */

void game_init        (Game *game);
void game_reset_board (Game *game);
bool game_make_move   (Game *game, int row, int col);
Cell game_check_winner(Game *game);
bool game_is_board_full(const Game *game);
void game_update_scores(Game *game);

#endif /* GAME_H */