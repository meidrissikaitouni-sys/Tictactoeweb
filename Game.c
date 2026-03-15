#include "game.h"
#include <string.h>

/* ── Initialisation complète (scores inclus) ───────────────────────────── */

void game_init(Game *game)
{
    game->score_x      = 0;
    game->score_o      = 0;
    game->score_draws  = 0;
    game->difficulty   = DIFFICULTY_HARD;
    game->player_x_type = PLAYER_HUMAN;
    game->player_o_type = PLAYER_AI;
    game->state        = STATE_MENU;
    game_reset_board(game);
}

/* ── Réinitialise uniquement le plateau ────────────────────────────────── */

void game_reset_board(Game *game)
{
    memset(game->board, CELL_EMPTY, sizeof(game->board));
    game->current_player  = CELL_X;
    game->winner          = CELL_EMPTY;
    game->is_draw         = false;
    game->has_winning_line = false;
    memset(game->winning_cells, 0, sizeof(game->winning_cells));
}

/* ── Joue un coup et met à jour l'état ─────────────────────────────────── */

bool game_make_move(Game *game, int row, int col)
{
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE)
        return false;
    if (game->board[row][col] != CELL_EMPTY)
        return false;
    if (game->state != STATE_PLAYING)
        return false;

    game->board[row][col] = game->current_player;

    Cell w = game_check_winner(game);
    if (w != CELL_EMPTY) {
        game->winner = w;
        game->state  = STATE_GAME_OVER;
        game_update_scores(game);
    } else if (game_is_board_full(game)) {
        game->is_draw = true;
        game->state   = STATE_GAME_OVER;
        game_update_scores(game);
    } else {
        /* Passe la main à l'autre joueur */
        game->current_player = (game->current_player == CELL_X) ? CELL_O : CELL_X;
    }

    return true;
}

/* ── Vérifie s'il y a un gagnant et mémorise la ligne ──────────────────── */

Cell game_check_winner(Game *game)
{
    /* Lignes */
    for (int r = 0; r < BOARD_SIZE; r++) {
        if (game->board[r][0] != CELL_EMPTY &&
            game->board[r][0] == game->board[r][1] &&
            game->board[r][1] == game->board[r][2])
        {
            game->has_winning_line = true;
            game->winning_cells[0][0] = r; game->winning_cells[0][1] = 0;
            game->winning_cells[1][0] = r; game->winning_cells[1][1] = 1;
            game->winning_cells[2][0] = r; game->winning_cells[2][1] = 2;
            return game->board[r][0];
        }
    }

    /* Colonnes */
    for (int c = 0; c < BOARD_SIZE; c++) {
        if (game->board[0][c] != CELL_EMPTY &&
            game->board[0][c] == game->board[1][c] &&
            game->board[1][c] == game->board[2][c])
        {
            game->has_winning_line = true;
            game->winning_cells[0][0] = 0; game->winning_cells[0][1] = c;
            game->winning_cells[1][0] = 1; game->winning_cells[1][1] = c;
            game->winning_cells[2][0] = 2; game->winning_cells[2][1] = c;
            return game->board[0][c];
        }
    }

    /* Diagonale principale */
    if (game->board[0][0] != CELL_EMPTY &&
        game->board[0][0] == game->board[1][1] &&
        game->board[1][1] == game->board[2][2])
    {
        game->has_winning_line = true;
        game->winning_cells[0][0] = 0; game->winning_cells[0][1] = 0;
        game->winning_cells[1][0] = 1; game->winning_cells[1][1] = 1;
        game->winning_cells[2][0] = 2; game->winning_cells[2][1] = 2;
        return game->board[0][0];
    }

    /* Diagonale anti */
    if (game->board[0][2] != CELL_EMPTY &&
        game->board[0][2] == game->board[1][1] &&
        game->board[1][1] == game->board[2][0])
    {
        game->has_winning_line = true;
        game->winning_cells[0][0] = 0; game->winning_cells[0][1] = 2;
        game->winning_cells[1][0] = 1; game->winning_cells[1][1] = 1;
        game->winning_cells[2][0] = 2; game->winning_cells[2][1] = 0;
        return game->board[0][2];
    }

    return CELL_EMPTY;
}

/* ── Vérifie si toutes les cases sont occupées ─────────────────────────── */

bool game_is_board_full(const Game *game)
{
    for (int r = 0; r < BOARD_SIZE; r++)
        for (int c = 0; c < BOARD_SIZE; c++)
            if (game->board[r][c] == CELL_EMPTY)
                return false;
    return true;
}

/* ── Met à jour les scores selon le résultat ───────────────────────────── */

void game_update_scores(Game *game)
{
    if      (game->winner == CELL_X) game->score_x++;
    else if (game->winner == CELL_O) game->score_o++;
    else                             game->score_draws++;
}