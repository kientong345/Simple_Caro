#ifndef __SIMPLE_CARO_WRAPPER__
#define __SIMPLE_CARO_WRAPPER__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdbool.h>
#include <stddef.h>

typedef enum {
    CARO_PLAYER1,
    CARO_PLAYER2,
} CARO_PARTICIPANT;

typedef enum {
    CARO_TIC_TAC_TOE,
    CARO_FOUR_BLOCK_1,
    CARO_FIVE_BLOCK_2,
} CARO_RULE_TYPE;

typedef struct {
    long x;
    long y;
} CARO_Coordinate;

typedef enum {
    CARO_TILE_EMPTY,
    CARO_TILE_PLAYER1,
    CARO_TILE_PLAYER2,
} CARO_TILE_STATE;

typedef enum {
    CARO_SUCCESS,
    CARO_ALREADY_OCCUPIED,
    CARO_WRONG_TURN,
    CARO_OUT_OF_BOUNDS,
} CARO_MOVE_RESULT;

typedef enum {
    CARO_PLAYER1_TURN,
    CARO_PLAYER2_TURN,
    CARO_PLAYER1_WON,
    CARO_PLAYER2_WON,
    CARO_DREW,
    CARO_NOT_INPROGRESS,
} CARO_GAME_STATE;

typedef struct {
    CARO_TILE_STATE** board;
    long width;
    long height;
} CARO_Board_Struct;

typedef struct {
    CARO_Coordinate* moves_set;
    size_t length;
} CARO_Moves_Set;

int caro_init_game();
void caro_deinit_game(int gid_);
void caro_set_board_size(int gid_, int width_, int height_);
void caro_set_rule(int gid_, CARO_RULE_TYPE rule_);
void caro_unset_rule(int gid_);
void caro_start(int gid_, CARO_GAME_STATE first_turn_);
void caro_stop(int gid_);
CARO_MOVE_RESULT caro_player_move(int gid_, CARO_PARTICIPANT who_, CARO_Coordinate move_);
CARO_MOVE_RESULT caro_player_undo(int gid_, CARO_PARTICIPANT who_);
CARO_MOVE_RESULT caro_player_redo(int gid_, CARO_PARTICIPANT who_);
void caro_switch_turn(int gid_);
void caro_get_board(int gid_, CARO_Board_Struct* data_);
CARO_GAME_STATE caro_get_state(int gid_);
bool caro_is_over(int gid_);
void caro_get_moves_history(int gid_, CARO_Moves_Set* data_, CARO_PARTICIPANT who_);
void caro_get_undone_moves(int gid_, CARO_Moves_Set* data_, CARO_PARTICIPANT who_);

void caro_free_board(CARO_Board_Struct* data_);
void caro_free_move_set(CARO_Moves_Set* data_);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* __SIMPLE_CARO_WRAPPER__ */