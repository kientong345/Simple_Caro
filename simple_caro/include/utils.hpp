#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include "defines.hpp"

namespace Caro {

inline bool is_valid_coordinate(const Board& board, const Coordinate& coor) {
    return (
        (board.height() > 0) && (board.width() > 0) &&
        (coor.latitude >= 0) && (coor.latitude < board.height()) &&
        (coor.longitude >= 0) && (coor.longitude < board.width())
    );
}

class Sequence_Detector {
private:
    Board mBoard;
    const unsigned int mSeq_length;

    bool
    sequence_detected (
        const Coordinate& coor,
        const size_t dx, const size_t dy,
        bool& blocked_start, bool& blocked_end
    ) const;

public:
    Sequence_Detector(
        const Board& board,
        unsigned int seq_length
    );

    GAME_EVENT
    operator()(
        const Coordinate& coor, unsigned char disallowed_head_blocks
    ) const;

};

} // namespace Caro

#endif /* __UTILS_HPP__ */