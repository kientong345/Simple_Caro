#ifndef __COMMON_H__
#define __COMMON_H__

#include <cstdint>

struct Coordinate {
    int64_t x;
    int64_t y;
    // for std::set<Coordinate>
    bool operator<(const Coordinate& other) const {
        return (x < other.x) || (x == other.x && y < other.y);
    }
};

enum class TILE_STATE {
    EMPTY,
    PLAYER1,
    PLAYER2,
};

enum class MOVE_RESULT {
    SUCCESS,
    ALREADY_OCCUPIED,
    WRONG_TURN,
    OUT_OF_BOUNDS,
};

enum class GAME_STATE {
    PLAYER1_TURN,
    PLAYER2_TURN,
    PLAYER1_WIN,
    PLAYER2_WIN,
    DRAW,
    RULE_NOT_FOUND,
    NOT_INPROGRESS,
};

enum class RULE_TYPE {
    TIC_TAC_TOE,
    FOUR_BLOCK_1,
    FIVE_BLOCK_2,
};

enum class PARTICIPANT {
    PLAYER1,
    PLAYER2,
};

#endif /* __COMMON_H__ */