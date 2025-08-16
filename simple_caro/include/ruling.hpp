#ifndef __RULING_HPP__
#define __RULING_HPP__

#include "defines.hpp"

namespace Caro {

class Ruling {
public:
    Ruling() = default;
    virtual ~Ruling() = default;

    virtual GAME_EVENT
    examine(const Board& board, const Coordinate& latest_move = {-1, -1}) = 0;

};

class Tic_Tac_Toe_Rule : public Ruling {
public:
    GAME_EVENT
    examine(const Board& board, const Coordinate& latest_move = {-1, -1}) override;

};

class Four_Block_1_Rule : public Ruling {
public:
    GAME_EVENT
    examine(const Board& board, const Coordinate& latest_move = {-1, -1}) override;

};

class Five_Block_2_Rule : public Ruling {
public:
    GAME_EVENT
    examine(const Board& board, const Coordinate& latest_move = {-1, -1}) override;

};

} // namespace Caro

#endif /* __RULING_HPP__ */