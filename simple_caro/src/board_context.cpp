#include "../include/board_context.hpp"
#include "../include/utils.hpp"

namespace Caro {

Board_Context::Board_Context(uint32_t height, uint32_t width)
        : board(std::make_shared<std::vector<std::vector<TILE_STATE>>>(
            height, std::vector<TILE_STATE>(width, TILE_STATE::EMPTY))),
          occupied_tiles_counter(0) {}

Board
Board_Context::get_board() const {
    return Board(board);
}

MOVE_RESULT
Board_Context::set_tile(Coordinate pos_, TILE_STATE state) {
    MOVE_RESULT ret = MOVE_RESULT::SUCCESS;
    bool pos_on_board = is_valid_coordinate(Board(board), pos_);
    if ( !pos_on_board ) {
        ret = MOVE_RESULT::OUT_OF_BOUNDS;
    } else if (board->at(pos_.latitude)[pos_.longtitude] != TILE_STATE::EMPTY) {
        ret = MOVE_RESULT::ALREADY_OCCUPIED;
    } else {
        (*board)[pos_.latitude][pos_.longtitude] = state;
        ++occupied_tiles_counter;
        ret = MOVE_RESULT::SUCCESS;
    }
    return ret;
}

MOVE_RESULT
Board_Context::unset_tile(Coordinate pos_) {
    MOVE_RESULT ret = MOVE_RESULT::SUCCESS;
    bool pos_on_board = is_valid_coordinate(Board(board), pos_);
    if ( !pos_on_board ) {
        ret = MOVE_RESULT::OUT_OF_BOUNDS;
    } else {
        (*board)[pos_.latitude][pos_.longtitude] = TILE_STATE::EMPTY;
        --occupied_tiles_counter;
        ret = MOVE_RESULT::SUCCESS;
    }
    return ret;
}

long
Board_Context::occupied_tiles_count() const {
    return occupied_tiles_counter;
}

void
Board_Context::reset_context() {
    board->clear();
    occupied_tiles_counter = 0;
}

} // namespace Caro