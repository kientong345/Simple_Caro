#include "../include/simple_caro.hpp"

namespace Caro {

void
Simple_Caro::update_context() {
    GAME_EVENT event = GAME_EVENT::NO_EVENT;
    switch (mState) {
    case GAME_STATE::PLAYER1_TURN:
        event = mJudge->judge(mBoard->get_board(), mLatest_player1_move);
        break;
    case GAME_STATE::PLAYER2_TURN:
        event = mJudge->judge(mBoard->get_board(), mLatest_player2_move);
        break;
    default:
        // brute force check all board
        event = mJudge->judge(mBoard->get_board());
        break;
    }

    if (
        (event != GAME_EVENT::NO_AVAILABLE_RULE) &&
        (event != GAME_EVENT::NO_EVENT)
    ) {
        switch (event) {
        case GAME_EVENT::PLAYER1_FULL_SEQUENCED:
            mState = GAME_STATE::PLAYER1_WON;
            break;
        case GAME_EVENT::PLAYER2_FULL_SEQUENCED:
            mState = GAME_STATE::PLAYER2_WON;
            break;
        case GAME_EVENT::BOARD_FULL:
            mState = GAME_STATE::DREW;
            break;
        default:
            break;
        }
    }
}

Simple_Caro::Simple_Caro()
    : mPlayer1(nullptr),
    mPlayer2(nullptr),
    mBoard(nullptr),
    mJudge(nullptr),
    mState(GAME_STATE::NOT_INPROGRESS),
    mLatest_player1_move{-1, -1},
    mLatest_player2_move{-1, -1} {}

void
Simple_Caro::set_board_size(int32_t width, int32_t height) {
    mBoard = std::make_unique<Board_Context>(width, height);
}

size_t
Simple_Caro::get_board_width() {
    return mBoard->get_board().width();
}

size_t
Simple_Caro::get_board_height() {
    return mBoard->get_board().height();
}

void
Simple_Caro::set_rule(RULE_TYPE rule) {
    if (!mJudge) {
        mJudge = std::make_unique<Game_Judge>();
    }
    mJudge->set_rule(rule);
}

void
Simple_Caro::unset_rule() {
    mJudge = nullptr;
}

void
Simple_Caro::start(GAME_STATE first_turn) {
    if (!mPlayer1) {
        mPlayer1 = std::make_unique<Player_Context>();
    }
    if (!mPlayer2) {
        mPlayer2 = std::make_unique<Player_Context>();
    }
    if (!mBoard) {
        mBoard = std::make_unique<Board_Context>(1000, 1000);
    }
    if (!mJudge) {
        mJudge = std::make_unique<Game_Judge>();
        mJudge->set_rule(RULE_TYPE::FOUR_BLOCK_1);
    }
    mState = first_turn;
}

void
Simple_Caro::stop() {
    if (mPlayer1) {
        mPlayer1->reset_context();
    }
    if (mPlayer2) {
        mPlayer2->reset_context();
    }
    if (mBoard) {
        mBoard->reset_context();
    }
    mState = GAME_STATE::NOT_INPROGRESS;
}

MOVE_RESULT
Simple_Caro::player_move(PARTICIPANT who, Coordinate move) {
    MOVE_RESULT ret = MOVE_RESULT::SUCCESS;
    switch (who) {
    case PARTICIPANT::PLAYER1:
        if (mState != GAME_STATE::PLAYER1_TURN) {
            ret = MOVE_RESULT::WRONG_TURN;
        } else {
            ret = mBoard->set_tile(move, TILE_STATE::PLAYER1);
            if (ret == MOVE_RESULT::SUCCESS) {
                ret = mPlayer1->move(move);
                mLatest_player1_move = move;
            }
        }
        break;
    case PARTICIPANT::PLAYER2:
        if (mState != GAME_STATE::PLAYER2_TURN) {
            ret = MOVE_RESULT::WRONG_TURN;
        } else {
            ret = mBoard->set_tile(move, TILE_STATE::PLAYER2);
            if (ret == MOVE_RESULT::SUCCESS) {
                ret = mPlayer2->move(move);
                mLatest_player2_move = move;
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
Simple_Caro::player_undo(PARTICIPANT who) {
    MOVE_RESULT ret = MOVE_RESULT::SUCCESS;
    switch (who) {
    case PARTICIPANT::PLAYER1:
        if (mState != GAME_STATE::PLAYER1_TURN) {
            ret = MOVE_RESULT::WRONG_TURN;
        } else {
            ret = mPlayer1->undo();
            if (ret == MOVE_RESULT::SUCCESS) {
                ret = mBoard->unset_tile(mPlayer1->get_undone_moves().back());
                mLatest_player1_move = mPlayer1->get_moves_history().back();
            }
        }
        break;
    case PARTICIPANT::PLAYER2:
        if (mState != GAME_STATE::PLAYER2_TURN) {
            ret = MOVE_RESULT::WRONG_TURN;
        } else {
            ret = mPlayer2->undo();
            if (ret == MOVE_RESULT::SUCCESS) {
                ret = mBoard->unset_tile(mPlayer2->get_undone_moves().back());
                mLatest_player2_move = mPlayer2->get_moves_history().back();
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
Simple_Caro::player_redo(PARTICIPANT who) {
    MOVE_RESULT ret = MOVE_RESULT::SUCCESS;
    switch (who) {
    case PARTICIPANT::PLAYER1:
        if (mState != GAME_STATE::PLAYER1_TURN) {
            ret = MOVE_RESULT::WRONG_TURN;
        } else {
            ret = mPlayer1->redo();
            if (ret == MOVE_RESULT::SUCCESS) {
                ret = mBoard->set_tile(mPlayer1->get_moves_history().back(),
                                       TILE_STATE::PLAYER1);
                mLatest_player1_move = mPlayer1->get_moves_history().back();
            }
        }
        break;
    case PARTICIPANT::PLAYER2:
        if (mState != GAME_STATE::PLAYER2_TURN) {
            ret = MOVE_RESULT::WRONG_TURN;
        } else {
            ret = mPlayer2->redo();
            if (ret == MOVE_RESULT::SUCCESS) {
                ret = mBoard->set_tile(mPlayer2->get_moves_history().back(),
                                       TILE_STATE::PLAYER2);
                mLatest_player2_move = mPlayer2->get_moves_history().back();
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
    switch (mState) {
    case GAME_STATE::PLAYER1_TURN:
        mState = GAME_STATE::PLAYER2_TURN;
        break;
    case GAME_STATE::PLAYER2_TURN:
        mState = GAME_STATE::PLAYER1_TURN;
        break;
    default:
        break;
    }
}

long
Simple_Caro::occupied_tiles_count() const {
    return mBoard->occupied_tiles_count();
}

Board
Simple_Caro::get_board() const {
    return Board(mBoard->get_board());
}

GAME_STATE
Simple_Caro::get_state() const {
    return mState;
}

bool
Simple_Caro::is_over() const {
    return (
        (mState == GAME_STATE::PLAYER1_WON) ||
        (mState == GAME_STATE::PLAYER2_WON) ||
        (mState == GAME_STATE::DREW)
    );
}

const std::vector<Coordinate>
Simple_Caro::get_moves_history(PARTICIPANT who) const {
    std::vector<Coordinate> ret;
    switch (who) {
    case PARTICIPANT::PLAYER1:
        if (mPlayer1) {
            ret = std::move(mPlayer1->get_moves_history());
        }
        break;
    case PARTICIPANT::PLAYER2:
        if (mPlayer2) {
            ret = std::move(mPlayer2->get_moves_history());
        }
        break;
    default:
        break;
    }
    return ret;
}

const std::vector<Coordinate>
Simple_Caro::get_undone_moves(PARTICIPANT who) const {
    std::vector<Coordinate> ret;
    switch (who) {
    case PARTICIPANT::PLAYER1:
        if (mPlayer1) {
            ret = std::move(mPlayer1->get_undone_moves());
        }
        break;
    case PARTICIPANT::PLAYER2:
        if (mPlayer2) {
            ret = std::move(mPlayer2->get_undone_moves());
        }
        break;
    default:
        break;
    }
    return ret;
}

} // namespace Caro