#ifndef __BOARD_CONTEXT_HPP__
#define __BOARD_CONTEXT_HPP__

#include "defines.hpp"

namespace Caro {

class Board_Context {
private:
    std::shared_ptr<std::vector<std::vector<TILE_STATE>>> board;
    long occupied_tiles_counter;

public:
    Board_Context(uint32_t height, uint32_t width);
    ~Board_Context() = default;

    Board get_board() const;
    MOVE_RESULT set_tile(Coordinate pos_, TILE_STATE state);
    MOVE_RESULT unset_tile(Coordinate pos_);
    long occupied_tiles_count() const;
    void reset_context();
};

} // namespace Caro

#endif /* __BOARD_CONTEXT_HPP__ */