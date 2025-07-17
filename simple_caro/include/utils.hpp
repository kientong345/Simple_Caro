#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include "defines.hpp"

namespace Caro {

inline bool is_valid_coordinate(const Board& board_, const Coordinate& coor_) {
    return  board_.height() > 0 &&
            board_.width() > 0 &&
            coor_.latitude >= 0 && coor_.latitude < board_.height() &&
            coor_.longtitude >= 0 && coor_.longtitude < board_.width();
}

class Check_Tiles_Sequence {
private:
    Board board;
    const unsigned int seq_count;

    bool
    is_winning_sequence (
        const Coordinate& coor_,
        const size_t dx_, const size_t dy_,
        bool& blocked_start_, bool& blocked_end_
    ) const;

public:
    Check_Tiles_Sequence(
        const Board& board_,
        unsigned int seq_count_);

    GAME_CHECK
    operator()(
        const Coordinate& coor_, unsigned char block_num_
    ) const;

};

} // namespace Caro

#endif /* __UTILS_HPP__ */