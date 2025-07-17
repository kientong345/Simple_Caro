#include "../include/ruling.hpp"
#include "../include/utils.hpp"

namespace Caro {

GAME_CHECK
Tic_Tac_Toe_Rule::check_win(const Board& board_, const Coordinate& latest_move_) {
    if (is_valid_coordinate(board_, latest_move_)) {
        Check_Tiles_Sequence win_checker_(board_, 3);
        return win_checker_(latest_move_, 0);
    } else {
        for (uint32_t latitude_ = 0; latitude_ < board_.height(); ++latitude_) {
            for (uint32_t longtitude_ = 0; longtitude_ < board_.width(); ++longtitude_) {
                Check_Tiles_Sequence tile_checker_(board_, 3);
                GAME_CHECK result_ = tile_checker_({latitude_, longtitude_}, 0);
                if (result_ != GAME_CHECK::ONGOING) {
                    return result_;
                }
            }
        }
        return GAME_CHECK::ONGOING;
    }
}

GAME_CHECK
Tic_Tac_Toe_Rule::check_draw(const Board& board_) {
    // Implement the logic for checking the draw condition
    return GAME_CHECK::ONGOING;
}

GAME_CHECK
Four_Block_1_Rule::check_win(const Board& board_, const Coordinate& latest_move_) {
    if (is_valid_coordinate(board_, latest_move_)) {
        Check_Tiles_Sequence win_checker_(board_, 4);
        return win_checker_(latest_move_, 1);
    } else {
        for (uint32_t latitude_ = 0; latitude_ < board_.height(); ++latitude_) {
            for (uint32_t longtitude_ = 0; longtitude_ < board_.width(); ++longtitude_) {
                Check_Tiles_Sequence tile_checker_(board_, 4);
                GAME_CHECK result_ = tile_checker_({latitude_, longtitude_}, 1);
                if (result_ != GAME_CHECK::ONGOING) {
                    return result_;
                }
            }
        }
        return GAME_CHECK::ONGOING;
    }
}

GAME_CHECK
Four_Block_1_Rule::check_draw(const Board& board_) {
    // Implement the logic for checking the draw condition
    return GAME_CHECK::ONGOING;
}

GAME_CHECK
Five_Block_2_Rule::check_win(const Board& board_, const Coordinate& latest_move_) {
    if (is_valid_coordinate(board_, latest_move_)) {
        Check_Tiles_Sequence win_checker_(board_, 5);
        return win_checker_(latest_move_, 2);
    } else {
        for (uint32_t latitude_ = 0; latitude_ < board_.height(); ++latitude_) {
            for (uint32_t longtitude_ = 0; longtitude_ < board_.width(); ++longtitude_) {
                Check_Tiles_Sequence tile_checker_(board_, 5);
                GAME_CHECK result_ = tile_checker_({latitude_, longtitude_}, 2);
                if (result_ != GAME_CHECK::ONGOING) {
                    return result_;
                }
            }
        }
        return GAME_CHECK::ONGOING;
    }
}

GAME_CHECK
Five_Block_2_Rule::check_draw(const Board& board_) {
    // Implement the logic for checking the draw condition
    return GAME_CHECK::ONGOING;
}

} // namespace Caro