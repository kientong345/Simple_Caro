#include "../include/player_context.hpp"

namespace Caro {

MOVE_RESULT
Player_Context::move(Coordinate move_) {
    MOVE_RESULT ret = MOVE_RESULT::SUCCESS;
    if ((move_.latitude < 0) || (move_.longtitude < 0)) {
        ret = MOVE_RESULT::OUT_OF_BOUNDS;
    } else if ((moves_set.find(move_)) != 
                (moves_set.end())) {
        ret = MOVE_RESULT::ALREADY_OCCUPIED;
    } else {
        moves_history.push_back(move_);
        moves_set.insert(move_);
        if (!undone_moves.empty()) {
            undone_moves.pop_back();
        }
        ret = MOVE_RESULT::SUCCESS;
    }
    return ret;
}

MOVE_RESULT
Player_Context::undo() {
    if (!moves_history.empty()) {
        Coordinate move_ = moves_history.back();
        moves_history.pop_back();
        undone_moves.push_back(move_);
        moves_set.erase(move_);
        return MOVE_RESULT::SUCCESS;
    } else {
        return MOVE_RESULT::OUT_OF_BOUNDS;
    }
}

MOVE_RESULT
Player_Context::redo() {
    if (!undone_moves.empty()) {
        Coordinate move_ = undone_moves.back();
        undone_moves.pop_back();
        moves_history.push_back(move_);
        moves_set.insert(move_);
        return MOVE_RESULT::SUCCESS;
    } else {
        return MOVE_RESULT::OUT_OF_BOUNDS;
    }
}

void
Player_Context::reset_context() {
    undone_moves.clear();
    moves_history.clear();
    moves_set.clear();
}

const std::vector<Coordinate>
Player_Context::get_moves_history() const {
    return moves_history;
}

const std::vector<Coordinate>
Player_Context::get_undone_moves() const {
    return undone_moves;
}
    
const std::set<Coordinate>
Player_Context::get_moves_set() const {
    return moves_set;
}

} // namespace Caro