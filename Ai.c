#include "ai.h"
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <string.h>

/* ── Utilitaires internes ─────────────────────────────────────────────── */

/**
 * Évalue le plateau pour détecter un gagnant.
 * Retourne  CELL_EMPTY si pas de gagnant, sinon la pièce gagnante.
 */
static Cell board_winner(Cell board[BOARD_SIZE][BOARD_SIZE])
{
    /* Lignes */
    for (int r = 0; r < BOARD_SIZE; r++) {
        if (board[r][0] != CELL_EMPTY &&
            board[r][0] == board[r][1] &&
            board[r][1] == board[r][2])
            return board[r][0];
    }
    /* Colonnes */
    for (int c = 0; c < BOARD_SIZE; c++) {
        if (board[0][c] != CELL_EMPTY &&
            board[0][c] == board[1][c] &&
            board[1][c] == board[2][c])
            return board[0][c];
    }
    /* Diagonales */
    if (board[0][0] != CELL_EMPTY &&
        board[0][0] == board[1][1] &&
        board[1][1] == board[2][2])
        return board[0][0];

    if (board[0][2] != CELL_EMPTY &&
        board[0][2] == board[1][1] &&
        board[1][1] == board[2][0])
        return board[0][2];

    return CELL_EMPTY;
}

static bool board_full(Cell board[BOARD_SIZE][BOARD_SIZE])
{
    for (int r = 0; r < BOARD_SIZE; r++)
        for (int c = 0; c < BOARD_SIZE; c++)
            if (board[r][c] == CELL_EMPTY)
                return false;
    return true;
}

/* ── Algorithme Minimax avec élagage alpha-bêta ───────────────────────── */

/**
 * @param board          Copie de travail du plateau
 * @param is_maximizing  true = tour de l'IA, false = tour humain
 * @param ai_cell        Pièce jouée par l'IA
 * @param human_cell     Pièce jouée par l'humain
 * @param alpha          Meilleur score garanti pour le maximiseur
 * @param beta           Meilleur score garanti pour le minimiseur
 * @param depth          Profondeur de récursion courante
 * @return               Score de l'état évalué
 */
static int minimax(Cell board[BOARD_SIZE][BOARD_SIZE],
                   bool  is_maximizing,
                   Cell  ai_cell,
                   Cell  human_cell,
                   int   alpha,
                   int   beta,
                   int   depth)
{
    Cell w = board_winner(board);
    if (w == ai_cell)    return 10 - depth;   /* IA gagne — préfère gagner vite  */
    if (w == human_cell) return depth - 10;   /* Humain gagne — pire des cas     */
    if (board_full(board)) return 0;           /* Match nul                       */

    if (is_maximizing) {
        int best = INT_MIN;
        for (int r = 0; r < BOARD_SIZE; r++) {
            for (int c = 0; c < BOARD_SIZE; c++) {
                if (board[r][c] != CELL_EMPTY) continue;

                board[r][c] = ai_cell;
                int score = minimax(board, false, ai_cell, human_cell,
                                    alpha, beta, depth + 1);
                board[r][c] = CELL_EMPTY;

                if (score > best)  best  = score;
                if (best > alpha)  alpha = best;
                if (beta <= alpha) return best; /* Coupure bêta */
            }
        }
        return best;
    } else {
        int best = INT_MAX;
        for (int r = 0; r < BOARD_SIZE; r++) {
            for (int c = 0; c < BOARD_SIZE; c++) {
                if (board[r][c] != CELL_EMPTY) continue;

                board[r][c] = human_cell;
                int score = minimax(board, true, ai_cell, human_cell,
                                    alpha, beta, depth + 1);
                board[r][c] = CELL_EMPTY;

                if (score < best)  best = score;
                if (best < beta)   beta = best;
                if (beta <= alpha) return best; /* Coupure alpha */
            }
        }
        return best;
    }
}

/* ── Interface publique ───────────────────────────────────────────────── */

Move ai_get_best_move(const Game *game, Cell ai_cell)
{
    Cell human_cell = (ai_cell == CELL_X) ? CELL_O : CELL_X;
    Move best_move  = { -1, -1 };
    int  best_score = INT_MIN;

    /* Copie du plateau pour ne pas modifier l'état de jeu */
    Cell board[BOARD_SIZE][BOARD_SIZE];
    memcpy(board, game->board, sizeof(board));

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            if (board[r][c] != CELL_EMPTY) continue;

            board[r][c] = ai_cell;
            int score = minimax(board, false, ai_cell, human_cell,
                                INT_MIN, INT_MAX, 0);
            board[r][c] = CELL_EMPTY;

            if (score > best_score) {
                best_score      = score;
                best_move.row   = r;
                best_move.col   = c;
            }
        }
    }

    return best_move;
}

Move ai_get_random_move(const Game *game)
{
    static bool seeded = false;
    if (!seeded) { srand((unsigned)time(NULL)); seeded = true; }

    Move moves[BOARD_SIZE * BOARD_SIZE];
    int  count = 0;

    for (int r = 0; r < BOARD_SIZE; r++)
        for (int c = 0; c < BOARD_SIZE; c++)
            if (game->board[r][c] == CELL_EMPTY) {
                moves[count].row = r;
                moves[count].col = c;   
                count++;
            }

    if (count == 0) return (Move){ -1, -1 };
    return moves[rand() % count];
}