#include "Simple_Caro_wrapper.h"
#include "../single_include/Simple_Caro.h"

#include <memory>
#include <vector>
#include <mutex>

std::vector<std::unique_ptr<Caro::Simple_Caro>> game_pool;
std::mutex pool_mutex;

int caro_init_game() {
    std::lock_guard<std::mutex> glck(pool_mutex);
    int index = 0;
    while (index < game_pool.size()) {
        if (!game_pool[index]) {
            break;
        }
        ++index;
    }
    if (index < game_pool.size()) {
        game_pool[index] = std::make_unique<Caro::Simple_Caro>();
    } else {
        game_pool.push_back(std::make_unique<Caro::Simple_Caro>());
    }
    return index;
}

void caro_deinit_game(int gid_) {
    std::lock_guard<std::mutex> glck(pool_mutex);
    if (gid_ >= 0 && gid_ < game_pool.size()) {
        game_pool[gid_].reset();
    }
}

void caro_set_board_size(int gid_, size_t width_, size_t height_) {
    std::lock_guard<std::mutex> glck(pool_mutex);
    if ((gid_ < 0) || (gid_ >= game_pool.size()) || (!game_pool[gid_])) {
        return;
    }
    game_pool[gid_]->set_board_size(width_, height_);
}

size_t caro_get_board_width(int gid_) {
    std::lock_guard<std::mutex> glck(pool_mutex);
    if ((gid_ < 0) || (gid_ >= game_pool.size()) || (!game_pool[gid_])) {
        return -1;
    }
    return game_pool[gid_]->get_board_width();
}

size_t caro_get_board_height(int gid_) {
    std::lock_guard<std::mutex> glck(pool_mutex);
    if ((gid_ < 0) || (gid_ >= game_pool.size()) || (!game_pool[gid_])) {
        return -1;
    }
    return game_pool[gid_]->get_board_height();
}

void caro_set_rule(int gid_, CARO_RULE_TYPE rule_) {
    std::lock_guard<std::mutex> glck(pool_mutex);
    if ((gid_ < 0) || (gid_ >= game_pool.size()) || (!game_pool[gid_])) {
        return;
    }
    switch (rule_) {
    case CARO_TIC_TAC_TOE:
        game_pool[gid_]->set_rule(Caro::RULE_TYPE::TIC_TAC_TOE);
        break;
    case CARO_FOUR_BLOCK_1:
        game_pool[gid_]->set_rule(Caro::RULE_TYPE::FOUR_BLOCK_1);
        break;
    case CARO_FIVE_BLOCK_2:
        game_pool[gid_]->set_rule(Caro::RULE_TYPE::FIVE_BLOCK_2);
        break;
    default:
        break;
    }
}

void caro_unset_rule(int gid_) {
    std::lock_guard<std::mutex> glck(pool_mutex);
    if ((gid_ < 0) || (gid_ >= game_pool.size()) || (!game_pool[gid_])) {
        return;
    }
    game_pool[gid_]->unset_rule();
}

void caro_start(int gid_, CARO_GAME_STATE first_turn_) {
    std::lock_guard<std::mutex> glck(pool_mutex);
    if ((gid_ < 0) || (gid_ >= game_pool.size()) || (!game_pool[gid_])) {
        return;
    }
    switch (first_turn_) {
    case CARO_PLAYER1_TURN:
        game_pool[gid_]->start(Caro::GAME_STATE::PLAYER1_TURN);
        break;
    case CARO_PLAYER2_TURN:
        game_pool[gid_]->start(Caro::GAME_STATE::PLAYER2_TURN);
        break;
    default:
        break;
    }
}

void caro_stop(int gid_) {
    std::lock_guard<std::mutex> glck(pool_mutex);
    if ((gid_ < 0) || (gid_ >= game_pool.size()) || (!game_pool[gid_])) {
        return;
    }
    game_pool[gid_]->stop();
}

CARO_MOVE_RESULT caro_player_move(int gid_, CARO_PARTICIPANT who_, CARO_Coordinate move_) {
    std::lock_guard<std::mutex> glck(pool_mutex);
    if ((gid_ < 0) || (gid_ >= game_pool.size()) || (!game_pool[gid_])) {
        return CARO_OUT_OF_BOUNDS;
    }
    Caro::MOVE_RESULT ret = Caro::MOVE_RESULT::SUCCESS;
    Caro::Coordinate lib_move_ = {
        move_.latitude,
        move_.longtitude,
    };
    switch (who_) {
    case CARO_PLAYER1:
        ret = game_pool[gid_]->player_move(Caro::PARTICIPANT::PLAYER1, lib_move_);
        break;
    case CARO_PLAYER2:
        ret = game_pool[gid_]->player_move(Caro::PARTICIPANT::PLAYER2, lib_move_);
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

CARO_MOVE_RESULT caro_player_undo(int gid_, CARO_PARTICIPANT who_) {
    std::lock_guard<std::mutex> glck(pool_mutex);
    if ((gid_ < 0) || (gid_ >= game_pool.size()) || (!game_pool[gid_])) {
        return CARO_OUT_OF_BOUNDS;
    }
    Caro::MOVE_RESULT ret = Caro::MOVE_RESULT::SUCCESS;
    switch (who_) {
    case CARO_PLAYER1:
        ret = game_pool[gid_]->player_undo(Caro::PARTICIPANT::PLAYER1);
        break;
    case CARO_PLAYER2:
        ret = game_pool[gid_]->player_undo(Caro::PARTICIPANT::PLAYER2);
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

CARO_MOVE_RESULT caro_player_redo(int gid_, CARO_PARTICIPANT who_) {
    std::lock_guard<std::mutex> glck(pool_mutex);
    if ((gid_ < 0) || (gid_ >= game_pool.size()) || (!game_pool[gid_])) {
        return CARO_OUT_OF_BOUNDS;
    }
    Caro::MOVE_RESULT ret = Caro::MOVE_RESULT::SUCCESS;
    switch (who_) {
    case CARO_PLAYER1:
        ret = game_pool[gid_]->player_redo(Caro::PARTICIPANT::PLAYER1);
        break;
    case CARO_PLAYER2:
        ret = game_pool[gid_]->player_redo(Caro::PARTICIPANT::PLAYER2);
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

void caro_switch_turn(int gid_) {
    std::lock_guard<std::mutex> glck(pool_mutex);
    if ((gid_ < 0) || (gid_ >= game_pool.size()) || (!game_pool[gid_])) {
        return;
    }
    game_pool[gid_]->switch_turn();
}

long caro_occupied_tiles_count(int gid_) {
    std::lock_guard<std::mutex> glck(pool_mutex);
    if ((gid_ < 0) || (gid_ >= game_pool.size()) || (!game_pool[gid_])) {
        return -1;
    }
    return game_pool[gid_]->occupied_tiles_count();
}

void caro_get_board_row(int gid_, CARO_Board_Line* data_, size_t latitude_) {
    std::lock_guard<std::mutex> glck(pool_mutex);
    if ((gid_ < 0) || (gid_ >= game_pool.size()) || (!game_pool[gid_]) || (!data_)) {
        return;
    }
    std::vector<Caro::TILE_STATE> row_ = game_pool[gid_]->get_board().row(latitude_);
    data_->length = row_.size();
    data_->board_line = new CARO_TILE_STATE[data_->length];
    for (int i = 0; i < data_->length; ++i) {
        switch (row_[i]) {
        case Caro::TILE_STATE::PLAYER1:
            data_->board_line[i] = CARO_TILE_PLAYER1;
            break;
        case Caro::TILE_STATE::PLAYER2:
            data_->board_line[i] = CARO_TILE_PLAYER2;
            break;
        case Caro::TILE_STATE::EMPTY:
        default:
            data_->board_line[i] = CARO_TILE_EMPTY;
            break;
        }
    }
}

void caro_get_board_column(int gid_, CARO_Board_Line* data_, size_t longtitude_) {
    std::lock_guard<std::mutex> glck(pool_mutex);
    if ((gid_ < 0) || (gid_ >= game_pool.size()) || (!game_pool[gid_]) || (!data_)) {
        return;
    }
    std::vector<Caro::TILE_STATE> column_ = game_pool[gid_]->get_board().column(longtitude_);
    data_->length = column_.size();
    data_->board_line = new CARO_TILE_STATE[data_->length];
    for (int i = 0; i < data_->length; ++i) {
        switch (column_[i]) {
        case Caro::TILE_STATE::PLAYER1:
            data_->board_line[i] = CARO_TILE_PLAYER1;
            break;
        case Caro::TILE_STATE::PLAYER2:
            data_->board_line[i] = CARO_TILE_PLAYER2;
            break;
        case Caro::TILE_STATE::EMPTY:
        default:
            data_->board_line[i] = CARO_TILE_EMPTY;
            break;
        }
    }
}

CARO_TILE_STATE caro_get_tile_state(int gid_, size_t latitude_, size_t longtitude_) {
    std::lock_guard<std::mutex> glck(pool_mutex);
    if ((gid_ < 0) || (gid_ >= game_pool.size()) || (!game_pool[gid_])) {
        return CARO_TILE_EMPTY;
    }
    Caro::TILE_STATE ret = game_pool[gid_]->get_board().tile(latitude_, longtitude_);
    switch (ret) {
    case Caro::TILE_STATE::PLAYER1:
        return CARO_TILE_PLAYER1;
    case Caro::TILE_STATE::PLAYER2:
        return CARO_TILE_PLAYER2;
    case Caro::TILE_STATE::EMPTY:
    default:
        return CARO_TILE_EMPTY;
    }
}

CARO_GAME_STATE caro_get_state(int gid_) {
    std::lock_guard<std::mutex> glck(pool_mutex);
    if ((gid_ < 0) || (gid_ >= game_pool.size()) || (!game_pool[gid_])) {
        return CARO_NOT_INPROGRESS;
    }
    Caro::GAME_STATE ret = game_pool[gid_]->get_state();
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

bool caro_is_over(int gid_) {
    std::lock_guard<std::mutex> glck(pool_mutex);
    if ((gid_ < 0) || (gid_ >= game_pool.size()) || (!game_pool[gid_])) {
        return false;
    }
    return game_pool[gid_]->is_over();
}

void caro_get_moves_history(int gid_, CARO_Moves_Set* data_, CARO_PARTICIPANT who_) {
    std::lock_guard<std::mutex> glck(pool_mutex);
    if ((gid_ < 0) || (gid_ >= game_pool.size()) || (!game_pool[gid_]) || (!data_)) {
        return;
    }
    std::vector<Caro::Coordinate> move_history_;
    switch (who_) {
    case CARO_PLAYER1:
        move_history_ = game_pool[gid_]->get_moves_history(Caro::PARTICIPANT::PLAYER1);
        break;
    case CARO_PLAYER2:
        move_history_ = game_pool[gid_]->get_moves_history(Caro::PARTICIPANT::PLAYER2);
        break;
    default:
        break;
    }
    data_->length = move_history_.size();
    data_->moves_set = new CARO_Coordinate[data_->length];
    for (int i = 0; i < data_->length; ++i) {
        CARO_Coordinate c_move = {
            move_history_[i].latitude,
            move_history_[i].longtitude,
        };
        data_->moves_set[i] = c_move;
    }
}

void caro_get_undone_moves(int gid_, CARO_Moves_Set* data_, CARO_PARTICIPANT who_) {
    std::lock_guard<std::mutex> glck(pool_mutex);
    if ((gid_ < 0) || (gid_ >= game_pool.size()) || (!game_pool[gid_]) || (!data_)) {
        return;
    }
    std::vector<Caro::Coordinate> undone_moves_;
    switch (who_) {
    case CARO_PLAYER1:
        undone_moves_ = game_pool[gid_]->get_undone_moves(Caro::PARTICIPANT::PLAYER1);
        break;
    case CARO_PLAYER2:
        undone_moves_ = game_pool[gid_]->get_undone_moves(Caro::PARTICIPANT::PLAYER2);
        break;
    default:
        break;
    }
    data_->length = undone_moves_.size();
    data_->moves_set = new CARO_Coordinate[data_->length];
    for (int i = 0; i < data_->length; ++i) {
        CARO_Coordinate c_move = {
            undone_moves_[i].latitude,
            undone_moves_[i].longtitude,
        };
        data_->moves_set[i] = c_move;
    }
}

void caro_free_board_line(CARO_Board_Line* data_) {
    delete[] data_->board_line;
}

void caro_free_move_set(CARO_Moves_Set* data_) {
    delete[] data_->moves_set;
}