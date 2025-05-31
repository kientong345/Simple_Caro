#ifndef __CLI_GRAPHIC_H__
#define __CLI_GRAPHIC_H__

#include "cli_graphic_inc.h"
#include <vector>
#include <string>

class Cli_Graphic {
private:
    int text_color;
    int theme_color;
    coordinate board_base;

public:
    Cli_Graphic(int text_color_ = COLOR_WHITE, int theme_color_ = COLOR_BLACK)
    : board_base{0, 0},
      text_color{text_color_},
      theme_color{theme_color_}
    {
        initscr();
        cbreak();
        noecho();
        keypad(stdscr, TRUE);
        curs_set(0);

        start_color();
        init_pair(COLOR_BLACK, COLOR_BLACK, theme_color);
        init_pair(COLOR_RED, COLOR_RED, theme_color);
        init_pair(COLOR_GREEN, COLOR_GREEN, theme_color);
        init_pair(COLOR_YELLOW, COLOR_YELLOW, theme_color);
        init_pair(COLOR_BLUE, COLOR_BLUE, theme_color);
        init_pair(COLOR_MAGENTA, COLOR_MAGENTA, theme_color);
        init_pair(COLOR_CYAN, COLOR_CYAN, theme_color);
        init_pair(COLOR_WHITE, COLOR_WHITE, theme_color);

        init_pair(8, text_color, theme_color);

        clear();
        bkgd(COLOR_PAIR(8));
        refresh();
    }

    ~Cli_Graphic() {
        curs_set(1);
        clear();
        keypad(stdscr, FALSE);
        echo();
        nocbreak();
        endwin();
    }

    void display_allscreen() {
        background bg_ = {
            COLOR_BLACK,
            BG_ART,
        };
        erase();
        attron(COLOR_PAIR(bg_.bg_color));
        printw("%s", bg_.bg_img);
        attroff(COLOR_PAIR(bg_.bg_color));
        refresh();
    }

    void display_board(int abs_xpos_, int abs_ypos_, int width_,
                        int height_, int board_color_) {
        board_base.xPos = abs_xpos_;
        board_base.yPos = abs_ypos_;
        caro_board board_ = {
            {board_base.xPos, board_base.yPos},
            width_,
            height_,
            board_color_,
            TILE_ART,
        };
        for (int i = 0; i < board_.height; ++i) {
            for (int j = 0; j < board_.width; ++j) {
                display_tile(
                    i,
                    j,
                    board_.board_color
                );
            }
        }
        refresh();
    }

    void display_symbol(int rowpos_, int colpos_, SYMBOL symbol_, 
                        int tile_color_) {
        
        switch (symbol_) {
            case SYMBOL::PLAYER1:
                display_text(board_base.xPos+colpos_*TILE_ART_WIDTH+2,
                            board_base.yPos+rowpos_*TILE_ART_HEIGHT+1,
                            "X", tile_color_);
                break;
            case SYMBOL::PLAYER2:
                display_text(board_base.xPos+colpos_*TILE_ART_WIDTH+2,
                            board_base.yPos+rowpos_*TILE_ART_HEIGHT+1,
                            "O", tile_color_);
                break;
            default:
                break;
        }
    }
    
    void display_tile(int rowpos_, int colpos_,
                    int tile_color_) {
        tile tile_ = {
            {
                board_base.xPos+colpos_*TILE_ART_WIDTH,
                board_base.yPos+rowpos_*TILE_ART_HEIGHT,
            },
            tile_color_,
            TILE_ART,
        };
        std::vector<std::string> line_vec;
        std::string tile_str(tile_.tile_art);
        auto it = tile_str.begin();
        while (it != tile_str.end()) {
            std::string line = "";
            while (*it != '\n' && it != tile_str.end()) {
                line += *it;
                ++it;
            }
            line_vec.push_back(std::move(line));
            if (it != tile_str.end()) ++it;
        }
        uint8_t y_offset = 0;
        attron(COLOR_PAIR(tile_.tile_color));
        for (const auto& line_ : line_vec) {
            mvprintw(tile_.pos.yPos + y_offset,
                    tile_.pos.xPos,
                    "%s",
                    line_.c_str());
            ++y_offset;
        }
        attroff(COLOR_PAIR(tile_.tile_color));
        refresh();
    }

    void display_text(int xpos_, int ypos_, const char* content_, 
                      int text_color_ = COLOR_WHITE) {
        text text_ = {
            {xpos_, ypos_},
            text_color_,
            content_,
        };
        attron(COLOR_PAIR(text_.text_color));
        mvprintw(text_.pos.yPos,
                text_.pos.xPos,
                "%s",
                text_.content);
        attroff(COLOR_PAIR(text_.text_color));
        refresh();
    }

    INPUT_TYPE get_user_input() {
        INPUT_TYPE ret;

        bool invalid_key = true;
        while (invalid_key) {
            int key = getch();
            switch (key) {
            case KEY_UP:
                ret = P1_UP;
                invalid_key = false;
                break;
            case KEY_DOWN:
                ret = P1_DOWN;
                invalid_key = false;
                break;
            case KEY_LEFT:
                ret = P1_LEFT;
                invalid_key = false;
                break;
            case KEY_RIGHT:
                ret = P1_RIGHT;
                invalid_key = false;
                break;
            case 10: // ASCII code of Enter
                ret = P1_SELECT;
                invalid_key = false;
                break;
            case 'w':
            case 'W':
                ret = P2_UP;
                invalid_key = false;
                break;
            case 's':
            case 'S':
                ret = P2_DOWN;
                invalid_key = false;
                break;
            case 'a':
            case 'A':
                ret = P2_LEFT;
                invalid_key = false;
                break;
            case 'd':
            case 'D':
                ret = P2_RIGHT;
                invalid_key = false;
                break;
            case 9: // ASCII code of Tab
                ret = P2_SELECT;
                invalid_key = false;
                break;
            default: 
                break;
            }
        }
        return ret;
    }

    void pause_ui() {
        keypad(stdscr, FALSE);
        echo();
        nocbreak();
        curs_set(1);
    }

    void continue_ui() {
        curs_set(0);
        cbreak();
        noecho();
        keypad(stdscr, TRUE);
    }

};

#endif /* __CLI_GRAPHIC_H__ */