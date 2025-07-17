#ifndef __PLAYER_CONTEXT_HPP__
#define __PLAYER_CONTEXT_HPP__

#include "defines.hpp"
#include <set>

namespace Caro {

class Player_Context {
private:
    std::vector<Coordinate> moves_history;
    std::vector<Coordinate> undone_moves;
    std::set<Coordinate> moves_set;

public:
    Player_Context() = default;
    ~Player_Context() = default;
    
    MOVE_RESULT move(Coordinate move_);
    MOVE_RESULT undo();
    MOVE_RESULT redo();
    void reset_context();
    const std::vector<Coordinate> get_moves_history() const;
    const std::vector<Coordinate> get_undone_moves() const;
    const std::set<Coordinate> get_moves_set() const;
};

} // namespace Caro

#endif /* __PLAYER_CONTEXT_HPP__ */