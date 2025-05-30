#ifndef __CLI_GRAPHIC_INC_H__
#define __CLI_GRAPHIC_INC_H__

#include <cstdint>
#include <ncurses.h>

struct color {
    uint8_t red, green, blue;
};

#define WHITE         COLOR_WHITE
#define BLACK         COLOR_BLACK
#define RED           COLOR_RED
#define GREEN         COLOR_GREEN
#define BLUE          COLOR_BLUE
#define CYAN          COLOR_CYAN
#define MAGENTA       COLOR_MAGENTA
#define YELLOW        COLOR_YELLOW

enum INPUT_TYPE {
    P1_LEFT,
    P1_RIGHT,
    P1_UP,
    P1_DOWN,
    P1_SELECT,

    P2_LEFT,
    P2_RIGHT,
    P2_UP,
    P2_DOWN,
    P2_SELECT,
};

#define BG_ART \
"=============================================\n" \
"|                                           |\n" \
"|                                           |\n" \
"|                                           |\n" \
"|                                           |\n" \
"|                                           |\n" \
"|                                           |\n" \
"|                                           |\n" \
"|                                           |\n" \
"|                                           |\n" \
"|                                           |\n" \
"|                                           |\n" \
"|                                           |\n" \
"|                                           |\n" \
"|                                           |\n" \
"|                                           |\n" \
"|                                           |\n" \
"|                                           |\n" \
"|                                           |\n" \
"|                                           |\n" \
"|                                           |\n" \
"|                                           |\n" \
"=============================================\n"

#define TILE_ART  \
"+---+\n" \
"|   |\n" \
"+---+\n"

#define TILE_ART_WIDTH      4
#define TILE_ART_HEIGHT     2

enum class SYMBOL {
    PLAYER1,
    PLAYER2
};

struct background {
    int bg_color;
    const char* bg_img;
};

struct coordinate {
    int xPos;
    int yPos;
};

struct caro_board {
    coordinate pos;
    int width;          // number of tiles horizontally
    int height;         // number of tiles vertically
    int board_color;
    const char* tile_art;
};

struct tile {
    coordinate pos;
    int tile_color;
    const char* tile_art;
};

struct text {
    coordinate pos;
    int text_color;
    const char* content;
};

/*
═ ║ ╝ ╚ ╗ ╔ ╠ ╣ ╦ ╩ ╬
*/

#endif /* __CLI_GRAPHIC_INC_H__ */