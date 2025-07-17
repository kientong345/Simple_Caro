#ifndef __GAME_JUDGE_HPP__
#define __GAME_JUDGE_HPP__

#include "defines.hpp"
#include "ruling.hpp"

namespace Caro {

class Game_Judge {
private:
    std::unique_ptr<Ruling> ruler;

public:
    Game_Judge(std::unique_ptr<Ruling> ruler_ = nullptr);
    ~Game_Judge() = default;

    void
    set_rule(RULE_TYPE rule_);

    GAME_CHECK
    check_end_condition(const Board& board_, const Coordinate& latest_move_ = {-1, -1});

};

} // namespace Caro

#endif /* __GAME_JUDGE_HPP__ */