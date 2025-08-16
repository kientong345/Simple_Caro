#ifndef __GAME_JUDGE_HPP__
#define __GAME_JUDGE_HPP__

#include "defines.hpp"
#include "ruling.hpp"

namespace Caro {

class Game_Judge {
private:
    std::unique_ptr<Ruling> mRuler;

public:
    Game_Judge(std::unique_ptr<Ruling> ruler = nullptr);
    ~Game_Judge() = default;

    void
    set_rule(RULE_TYPE rule);

    GAME_EVENT
    judge(const Board& board, const Coordinate& latest_move = {-1, -1});

};

} // namespace Caro

#endif /* __GAME_JUDGE_HPP__ */