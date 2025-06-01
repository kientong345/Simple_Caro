#include "../lib/simple_caro/Simple_Caro.h"
// #include "../single_include/Simple_Caro.h"
#include "../lib/caro_cli/cli_graphic.h"
#include <iostream>
#include <string>
#include <atomic>

void print_match();
void handle_player_input();

Cli_Graphic cli(COLOR_WHITE, COLOR_BLACK);
Caro::Simple_Caro game;
struct Pos {
    int rowpos;
    int colpos;
};

std::atomic<Pos> pl1_pos;
std::atomic<Pos> pl2_pos;

std::shared_ptr<const std::vector<std::vector<Caro::TILE_STATE>>> board_cache;

int main() {
    // cli.display_allscreen();

    std::string player1_info = "Me";
    std::string player2_info = "Computer";

    game.set_board_size(21, 34);
    
    game.register_player_info<std::string>(Caro::PARTICIPANT::PLAYER1, player1_info);
    game.register_player_info<std::string>(Caro::PARTICIPANT::PLAYER2, player2_info);

    game.set_rule(Caro::RULE_TYPE::FIVE_BLOCK_2);

    game.start(Caro::GAME_STATE::PLAYER1_TURN);
    board_cache = game.get_board();
    print_match();

    while (!game.is_over()) {
        handle_player_input();
        // std::cout << "done\n";
        board_cache = game.get_board();
        print_match();
    }
    
    // while (!game.is_over()) {
    //     Caro::Coordinate move;
    //     Caro::GAME_STATE game_state = game.get_state();

    //     switch (game_state) {
    //     case Caro::GAME_STATE::PLAYER1_TURN:
    //         std::cout << "Player1 move (x y): ";
    //         std::cin >> move.x >> move.y;
    //         if ((game.player_move(Caro::PARTICIPANT::PLAYER1, move) == Caro::MOVE_RESULT::SUCCESS)) {
    //             game.switch_turn();
    //         }
    //         break;
    //     case Caro::GAME_STATE::PLAYER2_TURN:
    //         std::cout << "Player2 move (x y): ";
    //         std::cin >> move.x >> move.y;
    //         if ((game.player_move(Caro::PARTICIPANT::PLAYER2, move) == Caro::MOVE_RESULT::SUCCESS)) {
    //             game.switch_turn();
    //         }
    //         break;
    //     default:
    //         break;
    //     }
    //     board_cache = game.get_board();
    //     print_match();
    // }
    // cli.pause_ui();
    switch (game.get_state()) {
    case Caro::GAME_STATE::PLAYER1_WON:
        std::cout << "Player 1 wins!" << std::endl;
        break;
    case Caro::GAME_STATE::PLAYER2_WON:
        std::cout << "Player 2 wins!" << std::endl;
        break;
    case Caro::GAME_STATE::DREW:
        std::cout << "It's a draw!" << std::endl;
        break;
    default:
        std::cout << "Game is still ongoing" << std::endl;
        break;
    }
    // cli.continue_ui();
    
    return 0;
}

void print_match() {
    cli.display_board(1, 1, board_cache->size(), board_cache->at(0).size(), COLOR_BLUE);
    cli.display_tile(pl1_pos.load().rowpos, pl1_pos.load().colpos, COLOR_GREEN);
    cli.display_tile(pl2_pos.load().rowpos, pl2_pos.load().colpos, COLOR_RED);
    for (int i = 0; i < board_cache->size(); ++i) {
        for (int j = 0; j < board_cache->at(i).size(); ++j) {
            switch (board_cache->at(i)[j]) {
            case Caro::TILE_STATE::PLAYER1:
                cli.display_symbol(i, j, SYMBOL::PLAYER1, COLOR_GREEN);
                break;
            case Caro::TILE_STATE::PLAYER2:
                cli.display_symbol(i, j, SYMBOL::PLAYER2, COLOR_RED);
                break;
            }
        }
    }
}

void handle_player_input() {
    int input = cli.get_user_input();
    switch (input) {
    case INPUT_TYPE::P1_UP:
    {
        int new_row = std::max(pl1_pos.load().rowpos - 1, 0);
        int new_col = pl1_pos.load().colpos;
        pl1_pos.store({new_row, new_col});
    }
        break;
    case INPUT_TYPE::P1_DOWN:
    {
        int new_row = std::min(pl1_pos.load().rowpos + 1, (int)board_cache->at(0).size() - 1);
        int new_col = pl1_pos.load().colpos;
        pl1_pos.store({new_row, new_col});
    }
        break;
    case INPUT_TYPE::P1_LEFT:
    {
        int new_row = pl1_pos.load().rowpos;
        int new_col = std::max(pl1_pos.load().colpos - 1, 0);
        pl1_pos.store({new_row, new_col});
    }
        break;
    case INPUT_TYPE::P1_RIGHT:
    {
        int new_row = pl1_pos.load().rowpos;
        int new_col = std::min(pl1_pos.load().colpos + 1, (int)board_cache->size() - 1);
        pl1_pos.store({new_row, new_col});
    }
        break;
    case INPUT_TYPE::P1_SELECT:
    {
        int row = pl1_pos.load().rowpos;
        int col = pl1_pos.load().colpos;
        if ((game.get_state() == Caro::GAME_STATE::PLAYER1_TURN) &&
            ((game.player_move(Caro::PARTICIPANT::PLAYER1, {row, col}) == Caro::MOVE_RESULT::SUCCESS))) {
            game.switch_turn();
        }
    }
        break;
    case INPUT_TYPE::P2_UP:
    {
        int new_row = std::max(pl2_pos.load().rowpos - 1, 0);
        int new_col = pl2_pos.load().colpos;
        pl2_pos.store({new_row, new_col});
    }
        break;
    case INPUT_TYPE::P2_DOWN:
    {
        int new_row = std::min(pl2_pos.load().rowpos + 1, (int)board_cache->at(0).size() - 1);
        int new_col = pl2_pos.load().colpos;
        pl2_pos.store({new_row, new_col});
    }
        break;
    case INPUT_TYPE::P2_LEFT:
    {
        int new_row = pl2_pos.load().rowpos;
        int new_col = std::max(pl2_pos.load().colpos - 1, 0);
        pl2_pos.store({new_row, new_col});
    }
        break;
    case INPUT_TYPE::P2_RIGHT:
    {
        int new_row = pl2_pos.load().rowpos;
        int new_col = std::min(pl2_pos.load().colpos + 1, (int)board_cache->size() - 1);
        pl2_pos.store({new_row, new_col});
    }
        break;
    case INPUT_TYPE::P2_SELECT:
    {
        int row = pl2_pos.load().rowpos;
        int col = pl2_pos.load().colpos;
        if ((game.get_state() == Caro::GAME_STATE::PLAYER2_TURN) &&
            ((game.player_move(Caro::PARTICIPANT::PLAYER2, {row, col}) == Caro::MOVE_RESULT::SUCCESS))) {
            game.switch_turn();
        }
    }
        break;
    default:
        break;
    }
}