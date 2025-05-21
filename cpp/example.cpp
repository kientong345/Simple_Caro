#include "Simple_Caro.h"
#include <iostream>
#include <string>

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
        std::cout << "Enter your move (x y): ";
        std::cin >> move.x >> move.y;

        GAME_STATE game_state = game.get_state();

        switch (game_state) {
        case GAME_STATE::PLAYER1_TURN:
            if ((game.player_move(PARTICIPANT::PLAYER1, move) == MOVE_RESULT::SUCCESS)) {
                game.switch_turn();
            }
            break;
        case GAME_STATE::PLAYER2_TURN:
            if ((game.player_move(PARTICIPANT::PLAYER2, move) == MOVE_RESULT::SUCCESS)) {
                game.switch_turn();
            }
            break;
        default:
            break;
        }
    }

    switch (game.get_state()) {
    case GAME_STATE::PLAYER1_WIN:
        std::cout << "Player 1 wins!" << std::endl;
        break;
    case GAME_STATE::PLAYER2_WIN:
        std::cout << "Player 2 wins!" << std::endl;
        break;
    case GAME_STATE::DRAW:
        std::cout << "It's a draw!" << std::endl;
        break;
    default:
        std::cout << "Game is still ongoing" << std::endl;
        break;
    }
    
    return 0;
}