#ifndef __SIMPLE_CARO_WRAPPER__
#define __SIMPLE_CARO_WRAPPER__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include <stdbool.h>

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
    CARO_TILE_STATE board[1024][1024];
    long width;
    long height;
} CARO_Board_Struct;

void caro_init_game();
void caro_deinit_game();
void caro_set_board_size(int width_, int height_);
void caro_set_rule(CARO_RULE_TYPE rule_);
void caro_unset_rule();
void caro_start();
void caro_stop();
CARO_MOVE_RESULT caro_player_move(CARO_PARTICIPANT who_, CARO_Coordinate move_);
void caro_switch_turn();
void caro_get_board(CARO_Board_Struct* data_);
CARO_GAME_STATE caro_get_state();
bool caro_is_over();

#ifdef __cplusplus
}
#endif // __cplusplus

#endif /* __SIMPLE_CARO_WRAPPER__ */