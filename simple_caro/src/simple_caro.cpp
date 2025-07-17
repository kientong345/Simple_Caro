#include "../include/simple_caro.hpp"

namespace Caro {

void
Simple_Caro::update_context() {
    GAME_CHECK is_end_ = GAME_CHECK::ONGOING;
    switch (state) {
    case GAME_STATE::PLAYER1_TURN:
        is_end_ = judge->check_end_condition(board->get_board(),
                                            latest_player1_move);
        break;
    case GAME_STATE::PLAYER2_TURN:
        is_end_ = judge->check_end_condition(board->get_board(),
                                            latest_player2_move);
        break;
    default:
        // brute force check all board
        is_end_ = judge->check_end_condition(board->get_board(),
                                            {-1, -1});
        break;
    }

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

Simple_Caro::Simple_Caro()
    : player1(nullptr),
        player2(nullptr),
        board(nullptr),
        judge(nullptr),
        state(GAME_STATE::NOT_INPROGRESS),
        latest_player1_move{-1, -1},
        latest_player2_move{-1, -1} {}

void
Simple_Caro::set_board_size(int32_t width_, int32_t height_) {
    board = std::make_unique<Board_Context>(width_, height_);
}

size_t
Simple_Caro::get_board_width() {
    return board->get_board().width();
}

size_t
Simple_Caro::get_board_height() {
    return board->get_board().height();
}

void
Simple_Caro::set_rule(RULE_TYPE rule_) {
    if (!judge) {
        judge = std::make_unique<Game_Judge>();
    }
    judge->set_rule(rule_);
}

void
Simple_Caro::unset_rule() {
    judge = nullptr;
}

void
Simple_Caro::start(GAME_STATE first_turn_) {
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
Simple_Caro::stop() {
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
Simple_Caro::player_move(PARTICIPANT who_, Coordinate move_) {
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
                latest_player1_move = move_;
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
                latest_player2_move = move_;
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
Simple_Caro::player_undo(PARTICIPANT who_) {
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
                latest_player1_move = player1
                                    ->get_moves_history()
                                    .back();
            }
        }
        break;
    case PARTICIPANT::PLAYER2:
        if (state != GAME_STATE::PLAYER2_TURN) {
            ret = MOVE_RESULT::WRONG_TURN;
        } else {
            ret = player2->undo();
            if (ret == MOVE_RESULT::SUCCESS) {
                ret = board->unset_tile(player2
                                        ->get_undone_moves()
                                        .back());
                latest_player2_move = player2
                                    ->get_moves_history()
                                    .back();
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
Simple_Caro::player_redo(PARTICIPANT who_) {
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
                latest_player1_move = player1
                                    ->get_moves_history()
                                    .back();
            }
        }
        break;
    case PARTICIPANT::PLAYER2:
        if (state != GAME_STATE::PLAYER2_TURN) {
            ret = MOVE_RESULT::WRONG_TURN;
        } else {
            ret = player2->redo();
            if (ret == MOVE_RESULT::SUCCESS) {
                ret = board->set_tile(player2
                                    ->get_moves_history()
                                    .back(),
                                    TILE_STATE::PLAYER2);
                latest_player2_move = player2
                                    ->get_moves_history()
                                    .back();
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
Simple_Caro::switch_turn() {
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

long
Simple_Caro::occupied_tiles_count() const {
    return board->occupied_tiles_count();
}

Board
Simple_Caro::get_board() const {
    return Board(board->get_board());
}

GAME_STATE
Simple_Caro::get_state() const {
    return state;
}

bool
Simple_Caro::is_over() const {
    return (state == GAME_STATE::PLAYER1_WON ||
            state == GAME_STATE::PLAYER2_WON ||
            state == GAME_STATE::DREW);
}

const std::vector<Coordinate>
Simple_Caro::get_moves_history(PARTICIPANT who_) const {
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
Simple_Caro::get_undone_moves(PARTICIPANT who_) const {
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

} // namespace Caro