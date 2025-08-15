#ifndef __SIMPLE_CARO_HPP__
#define __SIMPLE_CARO_HPP__

#include "defines.hpp"
#include "player_context.hpp"
#include "board_context.hpp"
#include "game_judge.hpp"

namespace Caro {

class Simple_Caro {
private:
    std::unique_ptr<Player_Context> mPlayer1;
    std::unique_ptr<Player_Context> mPlayer2;
    std::unique_ptr<Board_Context> mBoard;
    std::unique_ptr<Game_Judge> mJudge;
    GAME_STATE mState;
    Coordinate mLatest_player1_move;
    Coordinate mLatest_player2_move;

    void
    update_context();

public:
    Simple_Caro();

    ~Simple_Caro() = default;

    void
    set_board_size(int32_t width, int32_t height);

    size_t
    get_board_width();

    size_t
    get_board_height();

    void
    set_rule(RULE_TYPE rule);

    void
    unset_rule();

    void
    start(GAME_STATE first_turn = GAME_STATE::PLAYER1_TURN);

    void
    stop();

    MOVE_RESULT
    player_move(PARTICIPANT who, Coordinate move);

    MOVE_RESULT
    player_undo(PARTICIPANT who);

    MOVE_RESULT
    player_redo(PARTICIPANT who);

    void
    switch_turn();

    long
    occupied_tiles_count() const;

    Board
    get_board() const;

    GAME_STATE
    get_state() const;

    bool
    is_over() const;

    const std::vector<Coordinate>
    get_moves_history(PARTICIPANT who) const;

    const std::vector<Coordinate>
    get_undone_moves(PARTICIPANT who) const;
};

} // namespace Caro

#endif /* __SIMPLE_CARO_HPP__ */