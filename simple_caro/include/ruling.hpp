#ifndef __RULING_HPP__
#define __RULING_HPP__

#include "defines.hpp"

namespace Caro {

class Ruling {
public:
    Ruling() = default;
    virtual ~Ruling() = default;

    virtual GAME_CHECK
    check_win(const Board& board_, const Coordinate& latest_move = {-1, -1}) = 0;

    virtual GAME_CHECK
    check_draw(const Board& board_) = 0;
};

class Tic_Tac_Toe_Rule : public Ruling {
public:
    GAME_CHECK
    check_win(const Board& board_, const Coordinate& latest_move_ = {-1, -1}) override;

    GAME_CHECK
    check_draw(const Board& board_) override;
};

class Four_Block_1_Rule : public Ruling {
public:
    GAME_CHECK
    check_win(const Board& board_, const Coordinate& latest_move_ = {-1, -1}) override;

    GAME_CHECK
    check_draw(const Board& board_) override;
};

class Five_Block_2_Rule : public Ruling {
public:
    GAME_CHECK
    check_win(const Board& board_, const Coordinate& latest_move_ = {-1, -1}) override;

    GAME_CHECK
    check_draw(const Board& board_) override;
};

} // namespace Caro

#endif /* __RULING_HPP__ */