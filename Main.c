/*
 * main.c — Tic Tac Toe avec IA Minimax
 * Bibliothèque graphique : Raylib
 * Compilé avec : C99
 */

#include "game.h"
#include "ai.h"
#include "Render.h"
#include "raylib.h"

/* Délai en secondes avant que l'IA joue (pour simuler la réflexion) */
#define AI_THINK_DELAY 0.45f

/* ── Prototypes internes ─────────────────────────────────────────────── */
static void handle_menu_input  (Game *game);
static void handle_game_input  (Game *game, double *ai_timer, bool *ai_waiting);
static void handle_game_over_input(Game *game);
static void trigger_ai_move    (Game *game);

/* ═══════════════════════════════════════════════════════════════════════ *
 *  POINT D'ENTRÉE
 * ═══════════════════════════════════════════════════════════════════════ */

int main(void)
{
    /* ── Fenêtre Raylib ── */
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Tic Tac Toe – IA Minimax");
    SetTargetFPS(60);

    /* ── État du jeu ── */
    Game   game;
    game_init(&game);

    /* ── Minuterie pour le délai de l'IA ── */
    double ai_timer   = 0.0;
    bool   ai_waiting = false;

    /* ══════════════════════ BOUCLE PRINCIPALE ══════════════════════════ */
    while (!WindowShouldClose()) {

        switch (game.state) {
            case STATE_MENU:
                handle_menu_input(&game);
                break;

            case STATE_PLAYING:
                handle_game_input(&game, &ai_timer, &ai_waiting);
                break;

            case STATE_GAME_OVER:
                handle_game_over_input(&game);
                break;
        }

        render_frame(&game);
    }

    CloseWindow();
    return 0;
}

/* ═══════════════════════════════════════════════════════════════════════ *
 *  GESTION DES ENTRÉES — MENU
 * ═══════════════════════════════════════════════════════════════════════ */

static void handle_menu_input(Game *game)
{
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return;

    Vector2 m = GetMousePosition();
    int mx = (int)m.x, my = (int)m.y;

    /* ── Modes de jeu ── */
    /* Joueur (X) vs IA (O) */
    if (mx >= 150 && mx <= 450 && my >= 270 && my <= 325) {
        game->player_x_type = PLAYER_HUMAN;
        game->player_o_type = PLAYER_AI;
        game_reset_board(game);
        game->state = STATE_PLAYING;
    }
    /* IA (X) vs Joueur (O) */
    else if (mx >= 150 && mx <= 450 && my >= 345 && my <= 400) {
        game->player_x_type = PLAYER_AI;
        game->player_o_type = PLAYER_HUMAN;
        game_reset_board(game);
        game->state = STATE_PLAYING;
    }
    /* Joueur vs Joueur */
    else if (mx >= 150 && mx <= 450 && my >= 420 && my <= 475) {
        game->player_x_type = PLAYER_HUMAN;
        game->player_o_type = PLAYER_HUMAN;
        game_reset_board(game);
        game->state = STATE_PLAYING;
    }
    /* ── Difficulté ── */
    else if (mx >= 115 && mx <= 275 && my >= 530 && my <= 575) {
        game->difficulty = DIFFICULTY_EASY;
    }
    else if (mx >= 325 && mx <= 485 && my >= 530 && my <= 575) {
        game->difficulty = DIFFICULTY_HARD;
    }
}

/* ═══════════════════════════════════════════════════════════════════════ *
 *  GESTION DES ENTRÉES — JEU
 * ═══════════════════════════════════════════════════════════════════════ */

static bool is_ai_turn(const Game *game)
{
    if (game->current_player == CELL_X)
        return (game->player_x_type == PLAYER_AI);
    return (game->player_o_type == PLAYER_AI);
}

static void handle_game_input(Game *game, double *ai_timer, bool *ai_waiting)
{
    /* ── Bouton Menu ── */
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 m = GetMousePosition();
        int mx = (int)m.x, my = (int)m.y;
        int btn_x = SCREEN_WIDTH / 2 - 75;
        int btn_y = SCREEN_HEIGHT - 55;
        if (mx >= btn_x && mx <= btn_x + 150 &&
            my >= btn_y && my <= btn_y + 42) {
            game->state = STATE_MENU;
            game_reset_board(game);
            *ai_waiting = false;
            return;
        }
    }

    /* ── Tour IA ── */
    if (is_ai_turn(game)) {
        if (!(*ai_waiting)) {
            /* Démarre le minuteur */
            *ai_timer   = GetTime();
            *ai_waiting = true;
        }
        if (GetTime() - *ai_timer >= AI_THINK_DELAY) {
            trigger_ai_move(game);
            *ai_waiting = false;
        }
        return;
    }

    /* ── Tour Humain ── */
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 m = GetMousePosition();
        int row, col;
        render_get_cell_from_mouse((int)m.x, (int)m.y, &row, &col);
        if (row != -1 && col != -1)
            game_make_move(game, row, col);
    }
}

/* ── Exécute le coup IA ─────────────────────────────────────────────── */

static void trigger_ai_move(Game *game)
{
    Move mv;
    if (game->difficulty == DIFFICULTY_HARD)
        mv = ai_get_best_move(game, game->current_player);
    else
        mv = ai_get_random_move(game);

    if (mv.row != -1)
        game_make_move(game, mv.row, mv.col);
}

/* ═══════════════════════════════════════════════════════════════════════ *
 *  GESTION DES ENTRÉES — FIN DE PARTIE
 * ═══════════════════════════════════════════════════════════════════════ */

static void handle_game_over_input(Game *game)
{
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) return;

    Vector2 m = GetMousePosition();
    int mx = (int)m.x, my = (int)m.y;

    int base_y = BOARD_OFFSET_Y + BOARD_SIZE * CELL_SIZE + 58;

    /* Rejouer */
    if (mx >= 100 && mx <= 260 && my >= base_y && my <= base_y + 40) {
        game_reset_board(game);
        game->state = STATE_PLAYING;
    }
    /* Menu */
    else if (mx >= 340 && mx <= 500 && my >= base_y && my <= base_y + 40) {
        game->state = STATE_MENU;
        game_reset_board(game);
    }
}