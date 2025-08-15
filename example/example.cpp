#include <simple_caro/simple_caro.hpp>
#include <iostream>
#include <string>

void print_board(const Caro::Board& board);

int main() {
    Caro::Simple_Caro game;

    game.set_board_size(10, 10);

    game.set_rule(Caro::RULE_TYPE::FIVE_BLOCK_2);

    game.start(Caro::GAME_STATE::PLAYER1_TURN);

    Caro::Board board = game.get_board();

    print_board(board);
    
    while (!game.is_over()) {
        Caro::Coordinate move;
        Caro::GAME_STATE game_state = game.get_state();

        switch (game_state) {
        case Caro::GAME_STATE::PLAYER1_TURN:
            std::cout << "Player1 move (x y): ";
            std::cin >> move.latitude >> move.longitude;
            if ((game.player_move(Caro::PARTICIPANT::PLAYER1, move) == Caro::MOVE_RESULT::SUCCESS)) {
                game.switch_turn();
            }
            break;
        case Caro::GAME_STATE::PLAYER2_TURN:
            std::cout << "Player2 move (x y): ";
            std::cin >> move.latitude >> move.longitude;
            if ((game.player_move(Caro::PARTICIPANT::PLAYER2, move) == Caro::MOVE_RESULT::SUCCESS)) {
                game.switch_turn();
            }
            break;
        default:
            break;
        }

        Caro::Board board = game.get_board();
        print_board(board);
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
    // cli.continue_ui();
    
    return 0;
}

void print_board(const Caro::Board& board) {
    for (int i = 0; i < board.height(); ++i) {
        std::cout << "[";
        for (const auto& tile : board.row(i)) {
            switch (tile) {
            case Caro::TILE_STATE::EMPTY:
                std::cout << " . ";
                break;
            case Caro::TILE_STATE::PLAYER1:
                std::cout << " X ";
                break;
            case Caro::TILE_STATE::PLAYER2:
                std::cout << " O ";
                break;
            }
        }
        std::cout << "]\n";
    }
}
