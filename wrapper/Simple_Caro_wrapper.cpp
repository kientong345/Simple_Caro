#include "Simple_Caro_wrapper.h"
#include "../simple_caro/include/simple_caro.hpp"

#include <memory>
#include <vector>
#include <mutex>

std::vector<std::unique_ptr<Caro::Simple_Caro>> gGame_pool;
std::mutex gPool_mutex;

int caro_init_game() {
    std::lock_guard<std::mutex> glck(gPool_mutex);
    int index = 0;
    while (index < gGame_pool.size()) {
        if (!gGame_pool[index]) {
            break;
        }
        ++index;
    }
    if (index < gGame_pool.size()) {
        gGame_pool[index] = std::make_unique<Caro::Simple_Caro>();
    } else {
        gGame_pool.push_back(std::make_unique<Caro::Simple_Caro>());
    }
    return index;
}

void caro_deinit_game(int gid) {
    std::lock_guard<std::mutex> glck(gPool_mutex);
    if (gid >= 0 && gid < gGame_pool.size()) {
        gGame_pool[gid].reset();
    }
}

void caro_set_board_size(int gid, size_t width, size_t height) {
    std::lock_guard<std::mutex> glck(gPool_mutex);
    if ((gid < 0) || (gid >= gGame_pool.size()) || (!gGame_pool[gid])) {
        return;
    }
    gGame_pool[gid]->set_board_size(width, height);
}

size_t caro_get_board_width(int gid) {
    std::lock_guard<std::mutex> glck(gPool_mutex);
    if ((gid < 0) || (gid >= gGame_pool.size()) || (!gGame_pool[gid])) {
        return -1;
    }
    return gGame_pool[gid]->get_board_width();
}

size_t caro_get_board_height(int gid) {
    std::lock_guard<std::mutex> glck(gPool_mutex);
    if ((gid < 0) || (gid >= gGame_pool.size()) || (!gGame_pool[gid])) {
        return -1;
    }
    return gGame_pool[gid]->get_board_height();
}

void caro_set_rule(int gid, CARO_RULE_TYPE rule) {
    std::lock_guard<std::mutex> glck(gPool_mutex);
    if ((gid < 0) || (gid >= gGame_pool.size()) || (!gGame_pool[gid])) {
        return;
    }
    switch (rule) {
    case CARO_TIC_TAC_TOE:
        gGame_pool[gid]->set_rule(Caro::RULE_TYPE::TIC_TAC_TOE);
        break;
    case CARO_FOUR_BLOCK_1:
        gGame_pool[gid]->set_rule(Caro::RULE_TYPE::FOUR_BLOCK_1);
        break;
    case CARO_FIVE_BLOCK_2:
        gGame_pool[gid]->set_rule(Caro::RULE_TYPE::FIVE_BLOCK_2);
        break;
    default:
        break;
    }
}

void caro_unset_rule(int gid) {
    std::lock_guard<std::mutex> glck(gPool_mutex);
    if ((gid < 0) || (gid >= gGame_pool.size()) || (!gGame_pool[gid])) {
        return;
    }
    gGame_pool[gid]->unset_rule();
}

void caro_start(int gid, CARO_GAME_STATE first_turn) {
    std::lock_guard<std::mutex> glck(gPool_mutex);
    if ((gid < 0) || (gid >= gGame_pool.size()) || (!gGame_pool[gid])) {
        return;
    }
    switch (first_turn) {
    case CARO_PLAYER1_TURN:
        gGame_pool[gid]->start(Caro::GAME_STATE::PLAYER1_TURN);
        break;
    case CARO_PLAYER2_TURN:
        gGame_pool[gid]->start(Caro::GAME_STATE::PLAYER2_TURN);
        break;
    default:
        break;
    }
}

void caro_stop(int gid) {
    std::lock_guard<std::mutex> glck(gPool_mutex);
    if ((gid < 0) || (gid >= gGame_pool.size()) || (!gGame_pool[gid])) {
        return;
    }
    gGame_pool[gid]->stop();
}

CARO_MOVE_RESULT caro_player_move(int gid, CARO_PARTICIPANT who, CARO_Coordinate move) {
    std::lock_guard<std::mutex> glck(gPool_mutex);
    if ((gid < 0) || (gid >= gGame_pool.size()) || (!gGame_pool[gid])) {
        return CARO_OUT_OF_BOUNDS;
    }
    Caro::MOVE_RESULT ret = Caro::MOVE_RESULT::SUCCESS;
    Caro::Coordinate lib_move = {
        move.latitude,
        move.longitude,
    };
    switch (who) {
    case CARO_PLAYER1:
        ret = gGame_pool[gid]->player_move(Caro::PARTICIPANT::PLAYER1, lib_move);
        break;
    case CARO_PLAYER2:
        ret = gGame_pool[gid]->player_move(Caro::PARTICIPANT::PLAYER2, lib_move);
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

CARO_MOVE_RESULT caro_player_undo(int gid, CARO_PARTICIPANT who) {
    std::lock_guard<std::mutex> glck(gPool_mutex);
    if ((gid < 0) || (gid >= gGame_pool.size()) || (!gGame_pool[gid])) {
        return CARO_OUT_OF_BOUNDS;
    }
    Caro::MOVE_RESULT ret = Caro::MOVE_RESULT::SUCCESS;
    switch (who) {
    case CARO_PLAYER1:
        ret = gGame_pool[gid]->player_undo(Caro::PARTICIPANT::PLAYER1);
        break;
    case CARO_PLAYER2:
        ret = gGame_pool[gid]->player_undo(Caro::PARTICIPANT::PLAYER2);
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

CARO_MOVE_RESULT caro_player_redo(int gid, CARO_PARTICIPANT who) {
    std::lock_guard<std::mutex> glck(gPool_mutex);
    if ((gid < 0) || (gid >= gGame_pool.size()) || (!gGame_pool[gid])) {
        return CARO_OUT_OF_BOUNDS;
    }
    Caro::MOVE_RESULT ret = Caro::MOVE_RESULT::SUCCESS;
    switch (who) {
    case CARO_PLAYER1:
        ret = gGame_pool[gid]->player_redo(Caro::PARTICIPANT::PLAYER1);
        break;
    case CARO_PLAYER2:
        ret = gGame_pool[gid]->player_redo(Caro::PARTICIPANT::PLAYER2);
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

void caro_switch_turn(int gid) {
    std::lock_guard<std::mutex> glck(gPool_mutex);
    if ((gid < 0) || (gid >= gGame_pool.size()) || (!gGame_pool[gid])) {
        return;
    }
    gGame_pool[gid]->switch_turn();
}

long caro_occupied_tiles_count(int gid) {
    std::lock_guard<std::mutex> glck(gPool_mutex);
    if ((gid < 0) || (gid >= gGame_pool.size()) || (!gGame_pool[gid])) {
        return -1;
    }
    return gGame_pool[gid]->occupied_tiles_count();
}

void caro_get_board_row(int gid, CARO_Board_Line* data, size_t latitude) {
    std::lock_guard<std::mutex> glck(gPool_mutex);
    if ((gid < 0) || (gid >= gGame_pool.size()) || (!gGame_pool[gid]) || (!data)) {
        return;
    }
    std::vector<Caro::TILE_STATE> row = gGame_pool[gid]->get_board().row(latitude);
    data->length = row.size();
    data->board_line = new CARO_TILE_STATE[data->length];
    for (int i = 0; i < data->length; ++i) {
        switch (row[i]) {
        case Caro::TILE_STATE::PLAYER1:
            data->board_line[i] = CARO_TILE_PLAYER1;
            break;
        case Caro::TILE_STATE::PLAYER2:
            data->board_line[i] = CARO_TILE_PLAYER2;
            break;
        case Caro::TILE_STATE::EMPTY:
        default:
            data->board_line[i] = CARO_TILE_EMPTY;
            break;
        }
    }
}

void caro_get_board_column(int gid, CARO_Board_Line* data, size_t longitude) {
    std::lock_guard<std::mutex> glck(gPool_mutex);
    if ((gid < 0) || (gid >= gGame_pool.size()) || (!gGame_pool[gid]) || (!data)) {
        return;
    }
    std::vector<Caro::TILE_STATE> column = gGame_pool[gid]->get_board().column(longitude);
    data->length = column.size();
    data->board_line = new CARO_TILE_STATE[data->length];
    for (int i = 0; i < data->length; ++i) {
        switch (column[i]) {
        case Caro::TILE_STATE::PLAYER1:
            data->board_line[i] = CARO_TILE_PLAYER1;
            break;
        case Caro::TILE_STATE::PLAYER2:
            data->board_line[i] = CARO_TILE_PLAYER2;
            break;
        case Caro::TILE_STATE::EMPTY:
        default:
            data->board_line[i] = CARO_TILE_EMPTY;
            break;
        }
    }
}

CARO_TILE_STATE caro_get_tile_state(int gid, size_t latitude, size_t longitude) {
    std::lock_guard<std::mutex> glck(gPool_mutex);
    if ((gid < 0) || (gid >= gGame_pool.size()) || (!gGame_pool[gid])) {
        return CARO_TILE_EMPTY;
    }
    Caro::TILE_STATE ret = gGame_pool[gid]->get_board().tile(latitude, longitude);
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

CARO_GAME_STATE caro_get_state(int gid) {
    std::lock_guard<std::mutex> glck(gPool_mutex);
    if ((gid < 0) || (gid >= gGame_pool.size()) || (!gGame_pool[gid])) {
        return CARO_NOT_INPROGRESS;
    }
    Caro::GAME_STATE ret = gGame_pool[gid]->get_state();
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

bool caro_is_over(int gid) {
    std::lock_guard<std::mutex> glck(gPool_mutex);
    if ((gid < 0) || (gid >= gGame_pool.size()) || (!gGame_pool[gid])) {
        return false;
    }
    return gGame_pool[gid]->is_over();
}

void caro_get_moves_history(int gid, CARO_Moves_Set* data, CARO_PARTICIPANT who) {
    std::lock_guard<std::mutex> glck(gPool_mutex);
    if ((gid < 0) || (gid >= gGame_pool.size()) || (!gGame_pool[gid]) || (!data)) {
        return;
    }
    std::vector<Caro::Coordinate> move_history;
    switch (who) {
    case CARO_PLAYER1:
        move_history = gGame_pool[gid]->get_moves_history(Caro::PARTICIPANT::PLAYER1);
        break;
    case CARO_PLAYER2:
        move_history = gGame_pool[gid]->get_moves_history(Caro::PARTICIPANT::PLAYER2);
        break;
    default:
        break;
    }
    data->length = move_history.size();
    data->moves_set = new CARO_Coordinate[data->length];
    for (int i = 0; i < data->length; ++i) {
        CARO_Coordinate c_move = {
            move_history[i].latitude,
            move_history[i].longitude,
        };
        data->moves_set[i] = c_move;
    }
}

void caro_get_undone_moves(int gid, CARO_Moves_Set* data, CARO_PARTICIPANT who) {
    std::lock_guard<std::mutex> glck(gPool_mutex);
    if ((gid < 0) || (gid >= gGame_pool.size()) || (!gGame_pool[gid]) || (!data)) {
        return;
    }
    std::vector<Caro::Coordinate> undone_moves;
    switch (who) {
    case CARO_PLAYER1:
        undone_moves = gGame_pool[gid]->get_undone_moves(Caro::PARTICIPANT::PLAYER1);
        break;
    case CARO_PLAYER2:
        undone_moves = gGame_pool[gid]->get_undone_moves(Caro::PARTICIPANT::PLAYER2);
        break;
    default:
        break;
    }
    data->length = undone_moves.size();
    data->moves_set = new CARO_Coordinate[data->length];
    for (int i = 0; i < data->length; ++i) {
        CARO_Coordinate c_move = {
            undone_moves[i].latitude,
            undone_moves[i].longitude,
        };
        data->moves_set[i] = c_move;
    }
}

void caro_free_board_line(CARO_Board_Line* data) {
    delete[] data->board_line;
}

void caro_free_move_set(CARO_Moves_Set* data) {
    delete[] data->moves_set;
}