#ifndef __SIMPLE_CARO_H__
#define __SIMPLE_CARO_H__

#include "common.h"
#include "Game_Context.h"
#include "Game_Judge.h"

#include <memory>

namespace Caro {

class Simple_Caro {
private:
    std::unique_ptr<Player_Context> player1;
    std::unique_ptr<Player_Context> player2;
    std::unique_ptr<Board_Context> board;
    std::unique_ptr<Game_Judge> judge;
    GAME_STATE state;

    void
    update_context() {
        GAME_CHECK is_end_ = judge->check_end_condition(
            board->get_board());
        if ((is_end_ != GAME_CHECK::RULE_NOT_FOUND) && 
            (is_end_ != GAME_CHECK::ONGOING)) {
            switch (is_end_) {
            case GAME_CHECK::PLAYER1_WIN:
                state = GAME_STATE::PLAYER1_WON;
                break;
            case GAME_CHECK::PLAYER2_WIN:
                state = GAME_STATE::PLAYER2_WON;
                break;
            case GAME_CHECK::DRAW:
                state = GAME_STATE::DREW;
                break;
            default:
                break;
            }
        }
    }

public:
    Simple_Caro()
        : player1(nullptr),
          player2(nullptr),
          board(nullptr),
          judge(nullptr),
          state(GAME_STATE::NOT_INPROGRESS) {}

    ~Simple_Caro() = default;

    template <typename T>
    void
    register_player_info(PARTICIPANT who_,
                        const T& player_info_) {
        switch (who_) {
        case PARTICIPANT::PLAYER1:
            if (!player1) {
                player1 = std::make_unique<Player_Context>();
            }
            player1->make_info<T>(player_info_);
            break;
        case PARTICIPANT::PLAYER2:
            if (!player2) {
                player2 = std::make_unique<Player_Context>();
            }
            player2->make_info<T>(player_info_);
            break;
        default:
            break;
        }
    }

    template <typename T>
    T*
    access_player_info(PARTICIPANT who_) {
        T* ret = nullptr;
        switch (who_) {
        case PARTICIPANT::PLAYER1:
            if (player1) {
                ret = player1->try_access_info<T>();
            }
            break;
        case PARTICIPANT::PLAYER2:
            if (player2) {
                ret = player2->try_access_info<T>();
            }
            break;
        default:
            break;
        }
        return ret;
    }

    void
    set_board_size(int32_t width_, int32_t height_) {
        board = std::make_unique<Board_Context>(width_, height_);
    }

    void
    set_rule(RULE_TYPE rule_) {
        if (!judge) {
            judge = std::make_unique<Game_Judge>();
        }
        judge->set_rule(rule_);
    }

    void
    unset_rule() {
        judge = nullptr;
    }

    void
    start(GAME_STATE first_turn_ = GAME_STATE::PLAYER1_TURN) {
        if (!player1) {
            player1 = std::make_unique<Player_Context>();
        }
        if (!player2) {
            player2 = std::make_unique<Player_Context>();
        }
        if (!board) {
            board = std::make_unique<Board_Context>(1000, 1000);
        }
        if (!judge) {
            judge = std::make_unique<Game_Judge>();
            judge->set_rule(RULE_TYPE::FOUR_BLOCK_1);
        }
        state = first_turn_;
    }

    void
    stop(GAME_STATE first_turn_ = GAME_STATE::PLAYER1_TURN) {
        if (player1) {
            player1->reset_context();
        }
        if (player2) {
            player2->reset_context();
        }
        if (board) {
            board->reset_context();
        }
        state = GAME_STATE::NOT_INPROGRESS;
    }

    MOVE_RESULT
    player_move(PARTICIPANT who_, Coordinate move_) {
        MOVE_RESULT ret = MOVE_RESULT::SUCCESS;
        switch (who_) {
        case PARTICIPANT::PLAYER1:
            if (state != GAME_STATE::PLAYER1_TURN) {
                ret = MOVE_RESULT::WRONG_TURN;
            } else {
                ret = board->set_tile(move_,
                                    TILE_STATE::PLAYER1);
                if (ret == MOVE_RESULT::SUCCESS) {
                    ret = player1->move(move_);
                }
            }
            break;
        case PARTICIPANT::PLAYER2:
            if (state != GAME_STATE::PLAYER2_TURN) {
                ret = MOVE_RESULT::WRONG_TURN;
            } else {
                ret = board->set_tile(move_,
                                    TILE_STATE::PLAYER2);
                if (ret == MOVE_RESULT::SUCCESS) {
                    ret = player2->move(move_);
                }
            }
            break;
        default:
            ret = MOVE_RESULT::WRONG_TURN;
            break;
        }
        if (ret == MOVE_RESULT::SUCCESS) {
            update_context();
        }
        return ret;
    }

    MOVE_RESULT
    player_undo(PARTICIPANT who_) {
        MOVE_RESULT ret = MOVE_RESULT::SUCCESS;
        switch (who_) {
        case PARTICIPANT::PLAYER1:
            if (state != GAME_STATE::PLAYER1_TURN) {
                ret = MOVE_RESULT::WRONG_TURN;
            } else {
                ret = player1->undo();
                if (ret == MOVE_RESULT::SUCCESS) {
                    ret = board->unset_tile(player1
                                            ->get_undone_moves()
                                            .back());
                }
            }
            break;
        case PARTICIPANT::PLAYER2:
            if (state != GAME_STATE::PLAYER1_TURN) {
                ret = MOVE_RESULT::WRONG_TURN;
            } else {
                ret = player2->undo();
                if (ret == MOVE_RESULT::SUCCESS) {
                    ret = board->unset_tile(player2
                                            ->get_undone_moves()
                                            .back());
                }
            }
            break;
        default:
            ret = MOVE_RESULT::WRONG_TURN;
            break;
        }
        if (ret == MOVE_RESULT::SUCCESS) {
            update_context();
        }
        return ret;
    }

    MOVE_RESULT
    player_redo(PARTICIPANT who_) {
        MOVE_RESULT ret = MOVE_RESULT::SUCCESS;
        switch (who_) {
        case PARTICIPANT::PLAYER1:
            if (state != GAME_STATE::PLAYER1_TURN) {
                ret = MOVE_RESULT::WRONG_TURN;
            } else {
                ret = player1->redo();
                if (ret == MOVE_RESULT::SUCCESS) {
                    ret = board->set_tile(player1
                                        ->get_moves_history()
                                        .back(),
                                        TILE_STATE::PLAYER1);
                }
            }
            break;
        case PARTICIPANT::PLAYER2:
            if (state != GAME_STATE::PLAYER1_TURN) {
                ret = MOVE_RESULT::WRONG_TURN;
            } else {
                ret = player2->redo();
                if (ret == MOVE_RESULT::SUCCESS) {
                    ret = board->set_tile(player2
                                        ->get_moves_history()
                                        .back(),
                                        TILE_STATE::PLAYER2);
                }
            }
            break;
        default:
            ret = MOVE_RESULT::WRONG_TURN;
            break;
        }
        if (ret == MOVE_RESULT::SUCCESS) {
            update_context();
        }
        return ret;
    }

    void
    switch_turn() {
        switch (state) {
        case GAME_STATE::PLAYER1_TURN:
            state = GAME_STATE::PLAYER2_TURN;
            break;
        case GAME_STATE::PLAYER2_TURN:
            state = GAME_STATE::PLAYER1_TURN;
            break;
        default:
            break;
        }
    }

    std::shared_ptr<const std::vector<std::vector<TILE_STATE>>>
    get_board() const {
        return board->get_board();
    }

    GAME_STATE
    get_state() const {
        return state;
    }

    bool
    is_over() const {
        return (state == GAME_STATE::PLAYER1_WON ||
                state == GAME_STATE::PLAYER2_WON ||
                state == GAME_STATE::DREW);
    }

    const std::vector<Coordinate>
    get_moves_history(PARTICIPANT who_) const {
        std::vector<Coordinate> ret;
        switch (who_) {
        case PARTICIPANT::PLAYER1:
            if (player1) {
                ret = std::move(player1->get_moves_history());
            }
            break;
        case PARTICIPANT::PLAYER2:
            if (player2) {
                ret = std::move(player2->get_moves_history());
            }
            break;
        default:
            break;
        }
        return ret;
    }

    const std::vector<Coordinate>
    get_undone_moves(PARTICIPANT who_) const {
        std::vector<Coordinate> ret;
        switch (who_) {
        case PARTICIPANT::PLAYER1:
            if (player1) {
                ret = std::move(player1->get_undone_moves());
            }
            break;
        case PARTICIPANT::PLAYER2:
            if (player2) {
                ret = std::move(player2->get_undone_moves());
            }
            break;
        default:
            break;
        }
        return ret;
    }
};

} // namespace Caro

#endif /* __SIMPLE_CARO_H__ */