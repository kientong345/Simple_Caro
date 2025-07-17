#include "../include/utils.hpp"

namespace Caro {

bool
Check_Tiles_Sequence::is_winning_sequence (
    const Coordinate& coor_,
    const size_t dx_, const size_t dy_,
    bool& blocked_start_, bool& blocked_end_
) const {
    unsigned int move_counter_ = 0;
    const TILE_STATE tile_state_ = board.tile(coor_.latitude, coor_.longtitude);
    TILE_STATE opposite_tile_state_ = TILE_STATE::EMPTY;

    switch (tile_state_) {
    case TILE_STATE::PLAYER1:
        opposite_tile_state_ = TILE_STATE::PLAYER2;
        break;
    case TILE_STATE::PLAYER2:
        opposite_tile_state_ = TILE_STATE::PLAYER1;
        break;
    default:
        return false;
    }

    // negative moving
    Coordinate cur_coor_ = {
        coor_.latitude,
        coor_.longtitude,
    };
    while ( ( is_valid_coordinate(board, cur_coor_) ) &&
            ( board.tile(cur_coor_.latitude, cur_coor_.longtitude) == tile_state_) ) {
        cur_coor_.latitude -= dx_;
        cur_coor_.longtitude -= dy_;
        ++move_counter_;
    }
    blocked_start_ = (is_valid_coordinate(board, cur_coor_)) &&
        ((board.tile(cur_coor_.latitude, cur_coor_.longtitude) == opposite_tile_state_)) ?
        true : false;
    
    // positive moving
    cur_coor_.latitude = coor_.latitude;
    cur_coor_.longtitude = coor_.longtitude;
    while ( ( is_valid_coordinate(board, cur_coor_) ) &&
            ( board.tile(cur_coor_.latitude, cur_coor_.longtitude) == tile_state_) ) {
        cur_coor_.latitude += dx_;
        cur_coor_.longtitude += dy_;
        ++move_counter_;
    }
    blocked_end_ = (is_valid_coordinate(board, cur_coor_)) &&
        ((board.tile(cur_coor_.latitude, cur_coor_.longtitude) == opposite_tile_state_)) ?
        true : false;

    return (move_counter_-1 >= seq_count) ? true : false;
}

Check_Tiles_Sequence::Check_Tiles_Sequence(
    const Board& board_,
    unsigned int seq_count_)
: board(std::move(board_)), seq_count(seq_count_) {}

GAME_CHECK
Check_Tiles_Sequence::operator()(
    const Coordinate& coor_, unsigned char block_num_
) const {
    if (!is_valid_coordinate(board, coor_)) {
        return GAME_CHECK::RULE_NOT_FOUND;
    }
    if ( board.tile(coor_.latitude, coor_.longtitude) == TILE_STATE::EMPTY ) {
        return GAME_CHECK::ONGOING;
    }
    if ( block_num_ > 2 ) {
        return GAME_CHECK::RULE_NOT_FOUND;
    }

    const std::vector<std::pair<size_t, size_t>> direction_units_ = {
        {0, 1},     // unit of movement to the right
        {1, 0},     // unit of movement upward
        {1, 1},     // unit of movement upward in forward diagonal
        {1, -1},    // unit of movement upward in backward diagonal
    };

    bool blocked_start_ = false, blocked_end_ = false;
    for ( const auto& [dx_, dy_] : direction_units_ ) {
        bool winning_sequence_ = is_winning_sequence(coor_, dx_, dy_,
                                                    blocked_start_, blocked_end_);

        bool nonblocked_winning_ = winning_sequence_;
        bool blocked1_winning_ = winning_sequence_ &&
                                !blocked_start_ &&
                                !blocked_end_;
        bool blocked2_winning_ = winning_sequence_ &&
                                !(blocked_start_ && blocked_end_);

        if ( ( ( block_num_ == 0 ) && ( nonblocked_winning_ ) ) ||
            ( ( block_num_ == 1 ) && ( blocked1_winning_ ) ) ||
            ( ( block_num_ == 2 ) && ( blocked2_winning_ ) ) ) {
            switch (board.tile(coor_.latitude, coor_.longtitude)) {
            case TILE_STATE::PLAYER1:
                return GAME_CHECK::PLAYER1_WIN;
            case TILE_STATE::PLAYER2:
                return GAME_CHECK::PLAYER2_WIN;
            case TILE_STATE::EMPTY:
            default:
                break;
            }
        }
    }

    return GAME_CHECK::ONGOING;
}

} // namespace Caro