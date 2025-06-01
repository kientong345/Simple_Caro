#ifndef __GAME_CONTEXT_H__
#define __GAME_CONTEXT_H__

#include "common.h"

#include <vector>
#include <set>
#include <memory>
#if __cplusplus >= 201703L
#include <any>
#else
#endif //  __cplusplus >= 201703L

namespace Caro {

class Player_Context {
private:
#if __cplusplus >= 201703L
    std::any player_info;
#else
    void* player_info;
#endif //  __cplusplus >= 201703L
    std::vector<Coordinate> moves_history;
    std::vector<Coordinate> undone_moves;
    std::set<Coordinate> moves_set;

public:
    Player_Context() : player_info(nullptr) {}

    ~Player_Context() = default;

    MOVE_RESULT
    move(Coordinate move_) {
        MOVE_RESULT ret = MOVE_RESULT::SUCCESS;
        if ( ( move_.x < 0 ) || ( move_.y < 0 ) ) {
            ret = MOVE_RESULT::OUT_OF_BOUNDS;
        } else if ( ( moves_set.find(move_) ) != 
                    ( moves_set.end() ) ) {
            ret = MOVE_RESULT::ALREADY_OCCUPIED;
        } else {
            moves_history.push_back(move_);
            moves_set.insert(move_);
            if ( !undone_moves.empty() ) {
                undone_moves.pop_back();
            }
            ret = MOVE_RESULT::SUCCESS;
        }
        return ret;
    }

    MOVE_RESULT
    undo() {
        if ( !moves_history.empty() ) {
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
    redo() {
        if ( !undone_moves.empty() ) {
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
    reset_context() {
        undone_moves.clear();
        moves_history.clear();
        moves_set.clear();
    }

    const std::vector<Coordinate>
    get_moves_history() const {
        return moves_history;
    }

    const std::vector<Coordinate>
    get_undone_moves() const {
        return undone_moves;
    }
    
    const std::set<Coordinate>
    get_moves_set() const {
        return moves_set;
    }

    template<typename T>
    void
    make_info(const T& info_) {
#if __cplusplus >= 201703L
        player_info = std::make_any<T>(info_);
#else
        player_info = new T(info_);
#endif //  __cplusplus >= 201703L
    }

    template<typename T>
    T*
    try_access_info() {
#if __cplusplus >= 201703L
        return std::any_cast<T>(&player_info);
#else
    return static_cast<T*>(player_info);
#endif //  __cplusplus >= 201703L
    }
};

class Board_Context {
private:
    std::shared_ptr<std::vector<std::vector<TILE_STATE>>> board;

public:
    Board_Context(uint32_t width, uint32_t height)
        : board(std::make_shared<std::vector<std::vector<TILE_STATE>>>(
            height, std::vector<TILE_STATE>(width, TILE_STATE::EMPTY))) {}

    ~Board_Context() = default;

    std::shared_ptr<const std::vector<std::vector<TILE_STATE>>>
    get_board()
        const {
        return board;
    }

    MOVE_RESULT
    set_tile(Coordinate pos_, TILE_STATE state) {
        MOVE_RESULT ret = MOVE_RESULT::SUCCESS;
        bool board_has_tiles = (board->size() > 0) &&
                               (board->at(0).size() > 0);
        bool pos_on_board = (board_has_tiles) &&
            (pos_.x >= 0) && (pos_.y >= 0) &&
            (pos_.x < board->size()) &&
            (pos_.y < board->at(0).size());
        if ( !pos_on_board ) {
            ret = MOVE_RESULT::OUT_OF_BOUNDS;
        } else if (board->at(pos_.x)[pos_.y] != TILE_STATE::EMPTY) {
            ret = MOVE_RESULT::ALREADY_OCCUPIED;
        } else {
            (*board)[pos_.x][pos_.y] = state;
            ret = MOVE_RESULT::SUCCESS;
        }
        return ret;
    }

    MOVE_RESULT
    unset_tile(Coordinate pos_) {
        MOVE_RESULT ret = MOVE_RESULT::SUCCESS;
        bool board_has_tiles = (board->size() > 0) &&
                               (board->at(0).size() > 0);
        bool pos_on_board = (board_has_tiles) &&
            (pos_.x >= 0) && (pos_.y >= 0) &&
            (pos_.x < board->size()) &&
            (pos_.y < board->at(0).size());
        if ( !pos_on_board ) {
            ret = MOVE_RESULT::OUT_OF_BOUNDS;
        } else {
            (*board)[pos_.x][pos_.y] = TILE_STATE::EMPTY;
            ret = MOVE_RESULT::SUCCESS;
        }
        return ret;
    }

    void
    reset_context() {
        board->clear();
    }
};

} // namespace Caro

#endif /* __GAME_CONTEXT_H__ */