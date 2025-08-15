#include "../include/ruling.hpp"
#include "../include/utils.hpp"

namespace Caro {

GAME_CHECK
Tic_Tac_Toe_Rule::check_win(const Board& board, const Coordinate& latest_move) {
    if (is_valid_coordinate(board, latest_move)) {
        Sequence_Detector detect_from(board, 3);
        return detect_from(latest_move, 0);
    } else {
        for (uint32_t latitude = 0; latitude < board.height(); ++latitude) {
            for (uint32_t longitude = 0; longitude < board.width(); ++longitude) {
                Sequence_Detector detect_from(board, 3);
                GAME_CHECK result = detect_from({latitude, longitude}, 0);
                if (result != GAME_CHECK::ONGOING) {
                    return result;
                }
            }
        }
        return GAME_CHECK::ONGOING;
    }
}

GAME_CHECK
Tic_Tac_Toe_Rule::check_draw(const Board& board) {
    // Implement the logic for checking the draw condition
    return GAME_CHECK::ONGOING;
}

GAME_CHECK
Four_Block_1_Rule::check_win(const Board& board, const Coordinate& latest_move) {
    if (is_valid_coordinate(board, latest_move)) {
        Sequence_Detector detect_from(board, 4);
        return detect_from(latest_move, 1);
    } else {
        for (uint32_t latitude = 0; latitude < board.height(); ++latitude) {
            for (uint32_t longitude = 0; longitude < board.width(); ++longitude) {
                Sequence_Detector detect_from(board, 4);
                GAME_CHECK result = detect_from({latitude, longitude}, 1);
                if (result != GAME_CHECK::ONGOING) {
                    return result;
                }
            }
        }
        return GAME_CHECK::ONGOING;
    }
}

GAME_CHECK
Four_Block_1_Rule::check_draw(const Board& board) {
    // Implement the logic for checking the draw condition
    return GAME_CHECK::ONGOING;
}

GAME_CHECK
Five_Block_2_Rule::check_win(const Board& board, const Coordinate& latest_move) {
    if (is_valid_coordinate(board, latest_move)) {
        Sequence_Detector detect_from(board, 5);
        return detect_from(latest_move, 2);
    } else {
        for (uint32_t latitude = 0; latitude < board.height(); ++latitude) {
            for (uint32_t longitude = 0; longitude < board.width(); ++longitude) {
                Sequence_Detector detect_from(board, 5);
                GAME_CHECK result = detect_from({latitude, longitude}, 2);
                if (result != GAME_CHECK::ONGOING) {
                    return result;
                }
            }
        }
        return GAME_CHECK::ONGOING;
    }
}

GAME_CHECK
Five_Block_2_Rule::check_draw(const Board& board) {
    // Implement the logic for checking the draw condition
    return GAME_CHECK::ONGOING;
}

} // namespace Caro