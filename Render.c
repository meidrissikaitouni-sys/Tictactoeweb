#include "render.h"
#include <stdio.h>
#include <math.h>

/* ═══════════════════════════════════════════════════════════════════════ *
 *  UTILITAIRES INTERNES
 * ═══════════════════════════════════════════════════════════════════════ */

/* Centre un texte horizontalement dans [x, x+w] */
static void draw_text_centered(const char *text, int cx, int y, int size, Color color)
{
    int tw = MeasureText(text, size);
    DrawText(text, cx - tw / 2, y, size, color);
}

/* Dessine un X dans une case */
static void draw_x(int px, int py, int size, Color color, float alpha_factor)
{
    color.a = (unsigned char)(255 * alpha_factor);
    int pad = size / 5;
    int thick = size / 10;
    DrawLineEx(
        (Vector2){ (float)(px + pad), (float)(py + pad) },
        (Vector2){ (float)(px + size - pad), (float)(py + size - pad) },
        (float)thick, color);
    DrawLineEx(
        (Vector2){ (float)(px + size - pad), (float)(py + pad) },
        (Vector2){ (float)(px + pad), (float)(py + size - pad) },
        (float)thick, color);
}

/* Dessine un O dans une case */
static void draw_o(int px, int py, int size, Color color, float alpha_factor)
{
    color.a = (unsigned char)(255 * alpha_factor);
    int cx    = px + size / 2;
    int cy    = py + size / 2;
    int outer = size / 2 - size / 8;
    int thick = size / 12;
    DrawRing(
        (Vector2){ (float)cx, (float)cy },
        (float)(outer - thick),
        (float)outer,
        0.0f, 360.0f, 36, color);
}

/* ── Bouton avec survol ──────────────────────────────────────────────── */

bool render_button(int x, int y, int w, int h, const char *label, int font_size)
{
    Vector2 mouse = GetMousePosition();
    bool hovered  = (mouse.x >= x && mouse.x <= x + w &&
                     mouse.y >= y && mouse.y <= y + h);

    Color bg = hovered ? CLR_BTN_HOVER : CLR_BTN;
    DrawRectangleRounded((Rectangle){ (float)x, (float)y,
                                      (float)w, (float)h }, 0.3f, 8, bg);
    DrawRectangleRoundedLinesEx((Rectangle){ (float)x, (float)y,
                                             (float)w, (float)h },
                                0.3f, 8, 2.0f,
                                hovered ? CLR_WIN_FLASH : CLR_GRID);
    draw_text_centered(label, x + w / 2, y + (h - font_size) / 2,
                       font_size, CLR_TEXT);
    return hovered;
}

/* ── Conversion souris → case ─────────────────────────────────────────── */

void render_get_cell_from_mouse(int mx, int my, int *row, int *col)
{
    int bx = mx - BOARD_OFFSET_X;
    int by = my - BOARD_OFFSET_Y;
    int board_px = CELL_SIZE * BOARD_SIZE;

    if (bx < 0 || bx >= board_px || by < 0 || by >= board_px) {
        *row = -1; *col = -1;
        return;
    }
    *row = by / CELL_SIZE;
    *col = bx / CELL_SIZE;
}

/* ═══════════════════════════════════════════════════════════════════════ *
 *  ÉCRAN MENU
 * ═══════════════════════════════════════════════════════════════════════ */

void render_menu(const Game *game)
{
    (void)game; /* non utilisé ici, mais cohérent avec l'interface */

    /* Titre */
    draw_text_centered("TIC TAC TOE", SCREEN_WIDTH / 2, 80, 52, CLR_TEXT);

    /* Sous-titre coloré */
    const char *x_str = "X";
    const char *o_str = "O";
    const char *vs    = " vs ";
    int total_w = MeasureText(x_str, 32) + MeasureText(vs, 32) + MeasureText(o_str, 32);
    int start_x = SCREEN_WIDTH / 2 - total_w / 2;
    DrawText(x_str, start_x, 145, 32, CLR_X);
    DrawText(vs,    start_x + MeasureText(x_str, 32), 145, 32, CLR_TEXT);
    DrawText(o_str, start_x + MeasureText(x_str, 32) + MeasureText(vs, 32), 145, 32, CLR_O);

    /* Modes de jeu */
    draw_text_centered("Choisissez un mode de jeu", SCREEN_WIDTH / 2, 220, 22, CLR_GRID);

    render_button(150, 270, 300, 55, "Joueur vs IA", 24);
    render_button(150, 345, 300, 55, "IA vs Joueur", 24);
    render_button(150, 420, 300, 55, "Joueur vs Joueur", 22);

    /* Difficulté */
    draw_text_centered("Difficulte :", SCREEN_WIDTH / 2, 500, 20, CLR_GRID);
    render_button(115, 530, 160, 45, "FACILE", 20);
    render_button(325, 530, 160, 45, "DIFFICILE", 20);

    /* Indicateur difficulté */
    draw_text_centered("(IA Minimax inebranlable)",
                       SCREEN_WIDTH / 2, 585, 16, CLR_GRID);
}

/* ═══════════════════════════════════════════════════════════════════════ *
 *  GRILLE + SYMBOLES
 * ═══════════════════════════════════════════════════════════════════════ */

static void draw_board(const Game *game)
{
    double t = GetTime();

    /* ── Case survolée ── */
    int hrow, hcol;
    Vector2 mouse = GetMousePosition();
    render_get_cell_from_mouse((int)mouse.x, (int)mouse.y, &hrow, &hcol);

    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
            int px = BOARD_OFFSET_X + c * CELL_SIZE;
            int py = BOARD_OFFSET_Y + r * CELL_SIZE;

            /* Fond de case */
            if (r == hrow && c == hcol &&
                game->board[r][c] == CELL_EMPTY &&
                game->state == STATE_PLAYING)
            {
                DrawRectangle(px, py, CELL_SIZE, CELL_SIZE, CLR_CELL_HOVER);
            }

            /* Flash de la case gagnante */
            bool is_winner_cell = false;
            if (game->has_winning_line) {
                for (int k = 0; k < 3; k++) {
                    if (game->winning_cells[k][0] == r &&
                        game->winning_cells[k][1] == c)
                    {
                        is_winner_cell = true;
                        break;
                    }
                }
            }

            float alpha = 1.0f;
            if (is_winner_cell) {
                /* Clignotement doux */
                alpha = 0.55f + 0.45f * (float)fabs(sin(t * 4.0));
            }

            /* Symboles */
            if (game->board[r][c] == CELL_X) {
                Color c_x = is_winner_cell ? CLR_WIN_FLASH : CLR_X;
                draw_x(px, py, CELL_SIZE, c_x, alpha);
            } else if (game->board[r][c] == CELL_O) {
                Color c_o = is_winner_cell ? CLR_WIN_FLASH : CLR_O;
                draw_o(px, py, CELL_SIZE, c_o, alpha);
            }
        }
    }

    /* ── Lignes de la grille ── */
    int board_px = CELL_SIZE * BOARD_SIZE;

    for (int i = 1; i < BOARD_SIZE; i++) {
        /* Verticales */
        DrawRectangle(BOARD_OFFSET_X + i * CELL_SIZE - LINE_THICKNESS / 2,
                      BOARD_OFFSET_Y,
                      LINE_THICKNESS, board_px, CLR_GRID);
        /* Horizontales */
        DrawRectangle(BOARD_OFFSET_X,
                      BOARD_OFFSET_Y + i * CELL_SIZE - LINE_THICKNESS / 2,
                      board_px, LINE_THICKNESS, CLR_GRID);
    }
}

/* ── Score ─────────────────────────────────────────────────────────────── */

static void draw_score(const Game *game)
{
    char buf[64];

    /* X */
    const char *x_label = (game->player_x_type == PLAYER_AI) ? "IA (X)" : "Vous (X)";
    draw_text_centered(x_label, BOARD_OFFSET_X + CELL_SIZE / 2, 50, 18, CLR_X);
    snprintf(buf, sizeof(buf), "%d", game->score_x);
    draw_text_centered(buf, BOARD_OFFSET_X + CELL_SIZE / 2, 75, 28, CLR_X);

    /* Nuls */
    draw_text_centered("Nuls", SCREEN_WIDTH / 2, 50, 18, CLR_GRID);
    snprintf(buf, sizeof(buf), "%d", game->score_draws);
    draw_text_centered(buf, SCREEN_WIDTH / 2, 75, 28, CLR_TEXT);

    /* O */
    const char *o_label = (game->player_o_type == PLAYER_AI) ? "IA (O)" : "Vous (O)";
    int ox = BOARD_OFFSET_X + 2 * CELL_SIZE + CELL_SIZE / 2;
    draw_text_centered(o_label, ox, 50, 18, CLR_O);
    snprintf(buf, sizeof(buf), "%d", game->score_o);
    draw_text_centered(buf, ox, 75, 28, CLR_O);
}

/* ── Indicateur de tour ─────────────────────────────────────────────── */

static void draw_turn_indicator(const Game *game)
{
    const char *msg;
    Color       col;

    if (game->current_player == CELL_X) {
        bool ai_turn = (game->player_x_type == PLAYER_AI);
        msg = ai_turn ? "IA (X) reflechit..." : "A vous de jouer (X)";
        col = CLR_X;
    } else {
        bool ai_turn = (game->player_o_type == PLAYER_AI);
        msg = ai_turn ? "IA (O) reflechit..." : "A vous de jouer (O)";
        col = CLR_O;
    }

    draw_text_centered(msg, SCREEN_WIDTH / 2, 118, 20, col);
}

/* ═══════════════════════════════════════════════════════════════════════ *
 *  ÉCRAN DE JEU
 * ═══════════════════════════════════════════════════════════════════════ */

void render_game(const Game *game)
{
    draw_score(game);
    draw_turn_indicator(game);
    draw_board(game);

    /* Bouton quitter */
    render_button(SCREEN_WIDTH / 2 - 75, SCREEN_HEIGHT - 55, 150, 42,
                  "Menu", 20);
}

/* ═══════════════════════════════════════════════════════════════════════ *
 *  ÉCRAN FIN DE PARTIE
 * ═══════════════════════════════════════════════════════════════════════ */

void render_game_over(const Game *game)
{
    draw_score(game);
    draw_board(game);

    /* Bandeau résultat */
    const char *result_msg;
    Color       result_col;

    if (game->is_draw) {
        result_msg = "MATCH NUL !";
        result_col = CLR_TEXT;
    } else if (game->winner == CELL_X) {
        bool ai_won = (game->player_x_type == PLAYER_AI);
        result_msg  = ai_won ? "L'IA (X) gagne !" : "Joueur X gagne !";
        result_col  = CLR_X;
    } else {
        bool ai_won = (game->player_o_type == PLAYER_AI);
        result_msg  = ai_won ? "L'IA (O) gagne !" : "Joueur O gagne !";
        result_col  = CLR_O;
    }

    /* Fond semi-transparent */
    DrawRectangle(0, BOARD_OFFSET_Y + BOARD_SIZE * CELL_SIZE + 5,
                  SCREEN_WIDTH, 100,
                  CLITERAL(Color){ 15, 15, 28, 200 });

    draw_text_centered(result_msg, SCREEN_WIDTH / 2,
                       BOARD_OFFSET_Y + BOARD_SIZE * CELL_SIZE + 15,
                       32, result_col);

    render_button(100, BOARD_OFFSET_Y + BOARD_SIZE * CELL_SIZE + 58, 160, 40,
                  "Rejouer", 20);
    render_button(340, BOARD_OFFSET_Y + BOARD_SIZE * CELL_SIZE + 58, 160, 40,
                  "Menu", 20);
}

/* ═══════════════════════════════════════════════════════════════════════ *
 *  DISPATCHER PRINCIPAL
 * ═══════════════════════════════════════════════════════════════════════ */

void render_frame(const Game *game)
{
    BeginDrawing();
    ClearBackground(CLR_BG);

    switch (game->state) {
        case STATE_MENU:      render_menu(game);      break;
        case STATE_PLAYING:   render_game(game);      break;
        case STATE_GAME_OVER: render_game_over(game); break;
    }

    EndDrawing();
}