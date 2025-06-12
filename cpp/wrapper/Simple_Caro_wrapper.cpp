#include "Simple_Caro_wrapper.h"
#include "../single_include/Simple_Caro.h"

std::unique_ptr<Caro::Simple_Caro> game(nullptr);

void caro_init_game() {
    game = std::make_unique<Caro::Simple_Caro>();
}

void caro_deinit_game() {
    game.reset();
}

void caro_set_board_size(int width_, int height_) {
    if (!game) return;
    game->set_board_size(width_, height_);
}

void caro_set_rule(CARO_RULE_TYPE rule_) {
    if (!game) return;
    switch (rule_) {
    case CARO_TIC_TAC_TOE:
        game->set_rule(Caro::RULE_TYPE::TIC_TAC_TOE);
        break;
    case CARO_FOUR_BLOCK_1:
        game->set_rule(Caro::RULE_TYPE::FOUR_BLOCK_1);
        break;
    case CARO_FIVE_BLOCK_2:
        game->set_rule(Caro::RULE_TYPE::FIVE_BLOCK_2);
        break;
    default:
        break;
    }
}

void caro_unset_rule() {
    if (!game) return;
    game->unset_rule();
}

void caro_start(CARO_GAME_STATE first_turn_) {
    if (!game) return;
    switch (first_turn_) {
    case CARO_PLAYER1_TURN:
        game->start(Caro::GAME_STATE::PLAYER1_TURN);
        break;
    case CARO_PLAYER2_TURN:
        game->start(Caro::GAME_STATE::PLAYER2_TURN);
        break;
    default:
        break;
    }
}

void caro_stop() {
    if (!game) return;
    game->stop();
}

CARO_MOVE_RESULT caro_player_move(CARO_PARTICIPANT who_, CARO_Coordinate move_) {
    if (!game) return CARO_OUT_OF_BOUNDS;
    Caro::MOVE_RESULT ret = Caro::MOVE_RESULT::SUCCESS;
    Caro::Coordinate lib_move_ = {
        move_.x,
        move_.y,
    };
    switch (who_) {
    case CARO_PLAYER1:
        ret = game->player_move(Caro::PARTICIPANT::PLAYER1, lib_move_);
        break;
    case CARO_PLAYER2:
        ret = game->player_move(Caro::PARTICIPANT::PLAYER2, lib_move_);
        break;
    default:
        ret = Caro::MOVE_RESULT::WRONG_TURN;
        break;
    }
    switch (ret) {
    case Caro::MOVE_RESULT::SUCCESS:
        return CARO_SUCCESS;
    case Caro::MOVE_RESULT::ALREADY_OCCUPIED:
        return CARO_ALREADY_OCCUPIED;
    case Caro::MOVE_RESULT::WRONG_TURN:
        return CARO_WRONG_TURN;
    case Caro::MOVE_RESULT::OUT_OF_BOUNDS:
        return CARO_OUT_OF_BOUNDS;
    default:
        return CARO_WRONG_TURN;
    }
}

CARO_MOVE_RESULT caro_player_undo(CARO_PARTICIPANT who_) {
    if (!game) return CARO_OUT_OF_BOUNDS;
    Caro::MOVE_RESULT ret = Caro::MOVE_RESULT::SUCCESS;
    switch (who_) {
    case CARO_PLAYER1:
        ret = game->player_undo(Caro::PARTICIPANT::PLAYER1);
        break;
    case CARO_PLAYER2:
        ret = game->player_undo(Caro::PARTICIPANT::PLAYER2);
        break;
    default:
        ret = Caro::MOVE_RESULT::WRONG_TURN;
        break;
    }
    switch (ret) {
    case Caro::MOVE_RESULT::SUCCESS:
        return CARO_SUCCESS;
    case Caro::MOVE_RESULT::ALREADY_OCCUPIED:
        return CARO_ALREADY_OCCUPIED;
    case Caro::MOVE_RESULT::WRONG_TURN:
        return CARO_WRONG_TURN;
    case Caro::MOVE_RESULT::OUT_OF_BOUNDS:
        return CARO_OUT_OF_BOUNDS;
    default:
        return CARO_WRONG_TURN;
    }
}

CARO_MOVE_RESULT caro_player_redo(CARO_PARTICIPANT who_) {
    if (!game) return CARO_OUT_OF_BOUNDS;
    Caro::MOVE_RESULT ret = Caro::MOVE_RESULT::SUCCESS;
    switch (who_) {
    case CARO_PLAYER1:
        ret = game->player_redo(Caro::PARTICIPANT::PLAYER1);
        break;
    case CARO_PLAYER2:
        ret = game->player_redo(Caro::PARTICIPANT::PLAYER2);
        break;
    default:
        ret = Caro::MOVE_RESULT::WRONG_TURN;
        break;
    }
    switch (ret) {
    case Caro::MOVE_RESULT::SUCCESS:
        return CARO_SUCCESS;
    case Caro::MOVE_RESULT::ALREADY_OCCUPIED:
        return CARO_ALREADY_OCCUPIED;
    case Caro::MOVE_RESULT::WRONG_TURN:
        return CARO_WRONG_TURN;
    case Caro::MOVE_RESULT::OUT_OF_BOUNDS:
        return CARO_OUT_OF_BOUNDS;
    default:
        return CARO_WRONG_TURN;
    }
}

void caro_switch_turn() {
    if (!game) return;
    game->switch_turn();
}

void caro_get_board(CARO_Board_Struct* data_) {
    if ( ( !game ) || ( !data_ ) ) {
        return;
    }
    auto board_ = game->get_board();
    data_->height = board_->size();
    data_->width = board_->at(0).size();
    data_->board = new CARO_TILE_STATE*[data_->height];
    for (int k = 0; k < data_->height; ++k) {
        data_->board[k] = new CARO_TILE_STATE[data_->width];
    }

    for (long i = 0; i < data_->height; ++i) {
        for (long j = 0; j < data_->width; ++j) {
            switch (board_->at(i)[j]) {
            case Caro::TILE_STATE::EMPTY:
                data_->board[i][j] = CARO_TILE_EMPTY;
                break;
            case Caro::TILE_STATE::PLAYER1:
                data_->board[i][j] = CARO_TILE_PLAYER1;
                break;
            case Caro::TILE_STATE::PLAYER2:
                data_->board[i][j] = CARO_TILE_PLAYER2;
                break;
            default:
                data_->board[i][j] = CARO_TILE_EMPTY;
                break;
            }
        }
    }
}

CARO_GAME_STATE caro_get_state() {
    if (!game) return CARO_NOT_INPROGRESS;
    Caro::GAME_STATE ret = game->get_state();
    switch (ret) {
    case Caro::GAME_STATE::PLAYER1_TURN:
        return CARO_PLAYER1_TURN;
    case Caro::GAME_STATE::PLAYER2_TURN:
        return CARO_PLAYER2_TURN;
    case Caro::GAME_STATE::PLAYER1_WON:
        return CARO_PLAYER1_WON;
    case Caro::GAME_STATE::PLAYER2_WON:
        return CARO_PLAYER2_WON;
    case Caro::GAME_STATE::DREW:
        return CARO_DREW;
    case Caro::GAME_STATE::NOT_INPROGRESS:
        return CARO_NOT_INPROGRESS;
    default:
        return CARO_NOT_INPROGRESS;
    }
}

bool caro_is_over() {
    if (!game) return false;
    return game->is_over();
}

void caro_get_moves_history(CARO_Moves_Set* data_, CARO_PARTICIPANT who_) {
    if ( ( !game ) || ( !data_ ) ) {
        return;
    }
    std::vector<Caro::Coordinate> move_history_;
    switch (who_) {
    case CARO_PLAYER1:
        move_history_ = game->get_moves_history(Caro::PARTICIPANT::PLAYER1);
        break;
    case CARO_PLAYER2:
        move_history_ = game->get_moves_history(Caro::PARTICIPANT::PLAYER2);
        break;
    default:
        break;
    }
    data_->length = move_history_.size();
    data_->moves_set = new CARO_Coordinate[data_->length];
    for (int i = 0; i < data_->length; ++i) {
        CARO_Coordinate c_move = {
            move_history_[i].x,
            move_history_[i].y,
        };
        data_->moves_set[i] = c_move;
    }
}

void caro_get_undone_moves(CARO_Moves_Set* data_, CARO_PARTICIPANT who_) {
    if ( ( !game ) || ( !data_ ) ) {
        return;
    }
    std::vector<Caro::Coordinate> undone_moves_;
    switch (who_) {
    case CARO_PLAYER1:
        undone_moves_ = game->get_undone_moves(Caro::PARTICIPANT::PLAYER1);
        break;
    case CARO_PLAYER2:
        undone_moves_ = game->get_undone_moves(Caro::PARTICIPANT::PLAYER2);
        break;
    default:
        break;
    }
    data_->length = undone_moves_.size();
    data_->moves_set = new CARO_Coordinate[data_->length];
    for (int i = 0; i < data_->length; ++i) {
        CARO_Coordinate c_move = {
            undone_moves_[i].x,
            undone_moves_[i].y,
        };
        data_->moves_set[i] = c_move;
    }
}

void caro_free_board(CARO_Board_Struct* data_) {
    for (int i = 0; i < data_->height; ++i) {
        delete[] data_->board[i];
    }
    delete[] data_->board;
}

void caro_free_move_set(CARO_Moves_Set* data_) {
    delete[] data_->moves_set;
}