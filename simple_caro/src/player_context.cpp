#include "../include/player_context.hpp"

namespace Caro {

MOVE_RESULT
Player_Context::move(Coordinate move) {
    MOVE_RESULT ret = MOVE_RESULT::SUCCESS;
    if ( (move.latitude < 0) || (move.longitude < 0) ) {
        ret = MOVE_RESULT::OUT_OF_BOUNDS;
    } else if (mMoves_set.find(move) != mMoves_set.end()) {
        ret = MOVE_RESULT::ALREADY_OCCUPIED;
    } else {
        mMoves_history.push_back(move);
        mMoves_set.insert(move);
        if (!mUndone_moves.empty()) {
            mUndone_moves.pop_back();
        }
        ret = MOVE_RESULT::SUCCESS;
    }
    return ret;
}

MOVE_RESULT
Player_Context::undo() {
    if (!mMoves_history.empty()) {
        Coordinate move = mMoves_history.back();
        mMoves_history.pop_back();
        mUndone_moves.push_back(move);
        mMoves_set.erase(move);
        return MOVE_RESULT::SUCCESS;
    } else {
        return MOVE_RESULT::OUT_OF_BOUNDS;
    }
}

MOVE_RESULT
Player_Context::redo() {
    if (!mUndone_moves.empty()) {
        Coordinate move = mUndone_moves.back();
        mUndone_moves.pop_back();
        mMoves_history.push_back(move);
        mMoves_set.insert(move);
        return MOVE_RESULT::SUCCESS;
    } else {
        return MOVE_RESULT::OUT_OF_BOUNDS;
    }
}

void
Player_Context::reset_context() {
    mUndone_moves.clear();
    mMoves_history.clear();
    mMoves_set.clear();
}

const std::vector<Coordinate>
Player_Context::get_moves_history() const {
    return mMoves_history;
}

const std::vector<Coordinate>
Player_Context::get_undone_moves() const {
    return mUndone_moves;
}
    
const std::set<Coordinate>
Player_Context::get_moves_set() const {
    return mMoves_set;
}

} // namespace Caro