#include "../include/board_context.hpp"
#include "../include/utils.hpp"

namespace Caro {

Board_Context::Board_Context(uint32_t height, uint32_t width)
        : mBoard(std::make_shared<std::vector<std::vector<TILE_STATE>>>(
            height, std::vector<TILE_STATE>(width, TILE_STATE::EMPTY))),
          mOccupied_tiles_counter(0) {}

Board
Board_Context::get_board() const {
    return Board(mBoard);
}

MOVE_RESULT
Board_Context::set_tile(Coordinate pos, TILE_STATE state) {
    MOVE_RESULT ret = MOVE_RESULT::SUCCESS;
    bool pos_on_board = is_valid_coordinate(Board(mBoard), pos);
    if (!pos_on_board) {
        ret = MOVE_RESULT::OUT_OF_BOUNDS;
    } else if (mBoard->at(pos.latitude)[pos.longitude] != TILE_STATE::EMPTY) {
        ret = MOVE_RESULT::ALREADY_OCCUPIED;
    } else {
        (*mBoard)[pos.latitude][pos.longitude] = state;
        ++mOccupied_tiles_counter;
        ret = MOVE_RESULT::SUCCESS;
    }
    return ret;
}

MOVE_RESULT
Board_Context::unset_tile(Coordinate pos) {
    MOVE_RESULT ret = MOVE_RESULT::SUCCESS;
    bool pos_on_board = is_valid_coordinate(Board(mBoard), pos);
    if (!pos_on_board) {
        ret = MOVE_RESULT::OUT_OF_BOUNDS;
    } else {
        (*mBoard)[pos.latitude][pos.longitude] = TILE_STATE::EMPTY;
        --mOccupied_tiles_counter;
        ret = MOVE_RESULT::SUCCESS;
    }
    return ret;
}

long
Board_Context::occupied_tiles_count() const {
    return mOccupied_tiles_counter;
}

void
Board_Context::reset_context() {
    mBoard->clear();
    mOccupied_tiles_counter = 0;
}

} // namespace Caro