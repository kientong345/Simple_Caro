// #include "cli_graphic.h"

// int main() {
//     Cli_Graphic cli(COLOR_WHITE, COLOR_BLACK);
//     // cli.display_allscreen();

//     // Example usage of the CLI graphic functions
//     cli.display_board(40, 6, 15, 15, COLOR_BLUE);
//     cli.display_tile(1, 1, COLOR_YELLOW);
//     cli.display_symbol(1, 1, SYMBOL::PLAYER1, COLOR_GREEN);

//     // Wait for user input before exiting
//     cli.get_user_input();

//     return 0;
// }

#include <ncurses.h>

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    mvprintw(0, 0, "Nhấn phím (q để thoát):");
    int ch;
    while ((ch = getch()) != 'q') {
        clear();
        mvprintw(0, 0, "Bạn vừa nhấn: %c (%d)", ch, ch);
        refresh();
    }

    endwin();
    return 0;
}