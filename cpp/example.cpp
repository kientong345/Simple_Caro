#include "Simple_Caro.h"
#include <iostream>
#include <string>

void print_board(std::shared_ptr<const std::vector<std::vector<Caro::TILE_STATE>>> board) {
    for (int i = 0; i < board->size(); ++i) {
        for (int j = 0; j < board->at(i).size(); ++j) {
            switch (board->at(i)[j]) {
            case Caro::TILE_STATE::EMPTY:
                std::cout << ". ";
                break;
            case Caro::TILE_STATE::PLAYER1:
                std::cout << "X ";
                break;
            case Caro::TILE_STATE::PLAYER2:
                std::cout << "O ";
                break;
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

int main() {
    Caro::Simple_Caro<std::string> game;

    std::string player1_info = "Me";
    std::string player2_info = "Computer";

    game.set_board_size(3, 3);
    
    game.register_player_info(Caro::PARTICIPANT::PLAYER1, player1_info);
    game.register_player_info(Caro::PARTICIPANT::PLAYER2, player2_info);

    game.set_rule(Caro::RULE_TYPE::TIC_TAC_TOE);

    game.start(Caro::GAME_STATE::PLAYER1_TURN);
    
    while (!game.is_over()) {
        Caro::Coordinate move;
        Caro::GAME_STATE game_state = game.get_state();

        switch (game_state) {
        case Caro::GAME_STATE::PLAYER1_TURN:
            std::cout << "Player1 move (x y): ";
            std::cin >> move.x >> move.y;
            if ((game.player_move(Caro::PARTICIPANT::PLAYER1, move) == Caro::MOVE_RESULT::SUCCESS)) {
                game.switch_turn();
            }
            break;
        case Caro::GAME_STATE::PLAYER2_TURN:
            std::cout << "Player2 move (x y): ";
            std::cin >> move.x >> move.y;
            if ((game.player_move(Caro::PARTICIPANT::PLAYER2, move) == Caro::MOVE_RESULT::SUCCESS)) {
                game.switch_turn();
            }
            break;
        default:
            break;
        }
        print_board(game.get_board());
    }

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
    
    return 0;
}