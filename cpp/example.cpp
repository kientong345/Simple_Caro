#include "Simple_Caro.h"
#include <iostream>
#include <string>

void print_board(const std::vector<std::vector<TILE_STATE>>& board) {
    for (const auto& row : board) {
        for (const auto& tile : row) {
            switch (tile) {
            case TILE_STATE::EMPTY:
                std::cout << ". ";
                break;
            case TILE_STATE::PLAYER1:
                std::cout << "X ";
                break;
            case TILE_STATE::PLAYER2:
                std::cout << "O ";
                break;
            }
        }
        std::cout << std::endl;
    }
}

int main() {
    Simple_Caro<std::string> game;

    std::string player1_info = "Me";
    std::string player2_info = "Computer";

    game.set_board_size(3, 3);
    
    game.register_player_info(PARTICIPANT::PLAYER1, player1_info);
    game.register_player_info(PARTICIPANT::PLAYER2, player2_info);

    game.set_rule(RULE_TYPE::TIC_TAC_TOE);

    game.start(GAME_STATE::PLAYER1_TURN);
    
    while (!game.is_over()) {
        Coordinate move;
        GAME_STATE game_state = game.get_state();

        switch (game_state) {
        case GAME_STATE::PLAYER1_TURN:
            std::cout << "Player1 move (x y): ";
            std::cin >> move.x >> move.y;
            if ((game.player_move(PARTICIPANT::PLAYER1, move) == MOVE_RESULT::SUCCESS)) {
                game.switch_turn();
            }
            break;
        case GAME_STATE::PLAYER2_TURN:
            std::cout << "Player2 move (x y): ";
            std::cin >> move.x >> move.y;
            if ((game.player_move(PARTICIPANT::PLAYER2, move) == MOVE_RESULT::SUCCESS)) {
                game.switch_turn();
            }
            break;
        default:
            break;
        }
        print_board(*game.get_board());
    }

    switch (game.get_state()) {
    case GAME_STATE::PLAYER1_WON:
        std::cout << "Player 1 wins!" << std::endl;
        break;
    case GAME_STATE::PLAYER2_WON:
        std::cout << "Player 2 wins!" << std::endl;
        break;
    case GAME_STATE::DREW:
        std::cout << "It's a draw!" << std::endl;
        break;
    default:
        std::cout << "Game is still ongoing" << std::endl;
        break;
    }
    
    return 0;
}