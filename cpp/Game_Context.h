#ifndef __GAME_CONTEXT_H__
#define __GAME_CONTEXT_H__

#include "common.h"

#include <vector>
#include <stack>
#include <set>
#include <memory>

template <typename T>
class Player_Context {
private:
    std::shared_ptr<T> player_info;
    std::shared_ptr<std::stack<Coordinate>> moves_history;
    std::shared_ptr<std::stack<Coordinate>> undone_moves;
    std::shared_ptr<std::set<Coordinate>> moves_set;

public:
    Player_Context(const T& info_)
        : player_info(std::make_shared<T>(info_)),
          moves_history(std::make_shared<std::stack<Coordinate>>()),
          undone_moves(std::make_shared<std::stack<Coordinate>>()),
          moves_set(std::make_shared<std::set<Coordinate>>()) {}

    ~Player_Context() = default;

    MOVE_RESULT
    move(Coordinate move_) {
        MOVE_RESULT ret = MOVE_RESULT::SUCCESS;
        if ( ( move_.x < 0 ) || ( move_.y < 0 ) ) {
            ret = MOVE_RESULT::OUT_OF_BOUNDS;
        } else if ( ( moves_set->find(move_) ) != 
                    ( moves_set->end() ) ) {
            ret = MOVE_RESULT::ALREADY_OCCUPIED;
        } else {
            moves_history->push(move_);
            moves_set->insert(move_);
            if ( !undone_moves->empty() ) {
                undone_moves->pop();
            }
            ret = MOVE_RESULT::SUCCESS;
        }
        return ret;
    }

    MOVE_RESULT
    undo() {
        if ( !moves_history->empty() ) {
            Coordinate move_ = moves_history->top();
            moves_history->pop();
            undone_moves->push(move_);
            moves_set->erase(move_);
            return MOVE_RESULT::SUCCESS;
        } else {
            return MOVE_RESULT::OUT_OF_BOUNDS;
        }
    }

    MOVE_RESULT
    redo() {
        if ( !undone_moves->empty() ) {
            Coordinate move_ = undone_moves->top();
            undone_moves->pop();
            moves_history->push(move_);
            moves_set->insert(move_);
            return MOVE_RESULT::SUCCESS;
        } else {
            return MOVE_RESULT::OUT_OF_BOUNDS;
        }
    }

    void
    reset() {
        undone_moves = std::make_shared<std::stack<Coordinate>>();
        moves_history = std::make_shared<std::stack<Coordinate>>();
        moves_set->clear();
    }

    std::shared_ptr<const std::stack<Coordinate>>
    get_moves_history() const {
        return moves_history;
    }

    std::shared_ptr<const std::stack<Coordinate>>
    get_undone_moves() const {
        return undone_moves;
    }
    
    std::shared_ptr<const std::set<Coordinate>>
    get_moves_set() const {
        return moves_set;
    }

    std::shared_ptr<T>
    info() const {
        return player_info;
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
    set_tile(Coordinate coord, TILE_STATE state) {
        MOVE_RESULT ret = MOVE_RESULT::SUCCESS;
        if ( ( coord.x < 0 ) || ( coord.y < 0 ) ||
            ( coord.x >= board->size() ) ||
            ( coord.y >= board->at(0).size()) ) {
            ret = MOVE_RESULT::OUT_OF_BOUNDS;
        } else if (board->at(coord.x)[coord.y] != TILE_STATE::EMPTY) {
            ret = MOVE_RESULT::ALREADY_OCCUPIED;
        } else {
            (*board)[coord.x][coord.y] = state;
            ret = MOVE_RESULT::SUCCESS;
        }
        return ret;
    }

    MOVE_RESULT
    unset_tile(Coordinate coord) {
        MOVE_RESULT ret = MOVE_RESULT::SUCCESS;
        if ( ( coord.x < 0 ) || ( coord.y < 0 ) ||
            ( coord.x >= board->size() ) ||
            ( coord.y >= board->at(0).size()) ) {
            ret = MOVE_RESULT::OUT_OF_BOUNDS;
        } else {
            (*board)[coord.x][coord.y] = TILE_STATE::EMPTY;
            ret = MOVE_RESULT::SUCCESS;
        }
        return ret;
    }

    void
    reset() {
        board->clear();
    }
};

#endif /* __GAME_CONTEXT_H__ */