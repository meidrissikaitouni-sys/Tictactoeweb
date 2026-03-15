#ifndef AI_H
#define AI_H

#include "game.h"

/* ── Structure représentant un coup ───────────────────────────────────── */

typedef struct {
    int row;
    int col;
} Move;

/* ── Fonctions publiques ──────────────────────────────────────────────── */

/**
 * Retourne le meilleur coup via Minimax + élagage alpha-bêta.
 * @param game       État courant du jeu (non modifié)
 * @param ai_player  La pièce jouée par l'IA (CELL_X ou CELL_O)
 */
Move ai_get_best_move  (const Game *game, Cell ai_player);

/**
 * Retourne un coup aléatoire parmi les cases libres (mode FACILE).
 */
Move ai_get_random_move(const Game *game);

#endif /* AI_H */