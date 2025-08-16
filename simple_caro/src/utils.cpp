#include "../include/utils.hpp"

namespace Caro {

bool
Sequence_Detector::sequence_detected (
    const Coordinate& coor,
    const size_t dx, const size_t dy,
    bool& blocked_start, bool& blocked_end
) const {
    unsigned int move_counter = 0;
    const TILE_STATE tile_state = mBoard.tile(coor.latitude, coor.longitude);
    TILE_STATE opposite_tile_state = TILE_STATE::EMPTY;

    switch (tile_state) {
    case TILE_STATE::PLAYER1:
        opposite_tile_state = TILE_STATE::PLAYER2;
        break;
    case TILE_STATE::PLAYER2:
        opposite_tile_state = TILE_STATE::PLAYER1;
        break;
    default:
        return false;
    }

    // negative moving
    Coordinate cur_coor = {
        coor.latitude,
        coor.longitude,
    };
    while (
        is_valid_coordinate(mBoard, cur_coor) &&
        (mBoard.tile(cur_coor.latitude, cur_coor.longitude) == tile_state)
    ) {
        cur_coor.latitude -= dx;
        cur_coor.longitude -= dy;
        ++move_counter;
    }
    blocked_start = (
        is_valid_coordinate(mBoard, cur_coor) &&
        (mBoard.tile(cur_coor.latitude, cur_coor.longitude) == opposite_tile_state)
    ) ? true : false;
    
    // positive moving
    cur_coor.latitude = coor.latitude;
    cur_coor.longitude = coor.longitude;
    while (
        is_valid_coordinate(mBoard, cur_coor) &&
        (mBoard.tile(cur_coor.latitude, cur_coor.longitude) == tile_state)
    ) {
        cur_coor.latitude += dx;
        cur_coor.longitude += dy;
        ++move_counter;
    }
    blocked_end = (
        is_valid_coordinate(mBoard, cur_coor) &&
        (mBoard.tile(cur_coor.latitude, cur_coor.longitude) == opposite_tile_state)
    ) ? true : false;

    return (move_counter-1 >= mSeq_length) ? true : false;
}

Sequence_Detector::Sequence_Detector(
    const Board& board,
    unsigned int seq_length)
: mBoard(std::move(board)), mSeq_length(seq_length) {}

GAME_EVENT
Sequence_Detector::operator()(
    const Coordinate& coor, unsigned char disallowed_head_blocks
) const {
    if (!is_valid_coordinate(mBoard, coor)) {
        return GAME_EVENT::NO_AVAILABLE_RULE;
    }
    if (mBoard.tile(coor.latitude, coor.longitude) == TILE_STATE::EMPTY) {
        return GAME_EVENT::NO_EVENT;
    }
    if (disallowed_head_blocks > 2) {
        return GAME_EVENT::NO_AVAILABLE_RULE;
    }

    const std::vector<std::pair<size_t, size_t>> direction_units = {
        {0, 1},     // unit of movement to the right
        {1, 0},     // unit of movement upward
        {1, 1},     // unit of movement upward in forward diagonal
        {1, -1},    // unit of movement upward in backward diagonal
    };

    bool blocked_start = false, blocked_end = false;
    for (const auto& [dx, dy] : direction_units) {
        bool winning_sequence = sequence_detected(coor, dx, dy, blocked_start, blocked_end);

        bool nonblocked_winning = winning_sequence;
        bool blocked1_winning = (
            winning_sequence &&
            !blocked_start &&
            !blocked_end
        );
        bool blocked2_winning = (
            winning_sequence &&
            !(blocked_start && blocked_end)
        );

        if (
            ( (disallowed_head_blocks == 0) && (nonblocked_winning) ) ||
            ( (disallowed_head_blocks == 1) && (blocked1_winning) ) ||
            ( (disallowed_head_blocks == 2) && (blocked2_winning) )
        ) {
            switch (mBoard.tile(coor.latitude, coor.longitude)) {
            case TILE_STATE::PLAYER1:
                return GAME_EVENT::PLAYER1_FULL_SEQUENCED;
            case TILE_STATE::PLAYER2:
                return GAME_EVENT::PLAYER2_FULL_SEQUENCED;
            case TILE_STATE::EMPTY:
            default:
                break;
            }
        }
    }

    return GAME_EVENT::NO_EVENT;
}

} // namespace Caro