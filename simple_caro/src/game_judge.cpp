#include "../include/game_judge.hpp"

namespace Caro {

Game_Judge::Game_Judge(std::unique_ptr<Ruling> ruler)
    : mRuler(std::move(ruler)) {}

void
Game_Judge::set_rule(RULE_TYPE rule)
{
    switch (rule) {
    case RULE_TYPE::TIC_TAC_TOE:
        mRuler = std::make_unique<Tic_Tac_Toe_Rule>();
        break;
    case RULE_TYPE::FOUR_BLOCK_1:
        mRuler = std::make_unique<Four_Block_1_Rule>();
        break;
    case RULE_TYPE::FIVE_BLOCK_2:
        mRuler = std::make_unique<Five_Block_2_Rule>();
        break;
    default:
        break;
    }
}

GAME_CHECK
Game_Judge::check_end_condition(
    const Board& board,
    const Coordinate& latest_move
) {
    GAME_CHECK ret = GAME_CHECK::ONGOING;
    if (!mRuler) {
        ret = GAME_CHECK::RULE_NOT_FOUND;
    } else {
        GAME_CHECK anyone_win = mRuler->check_win(board, latest_move);
        GAME_CHECK is_draw = mRuler->check_draw(board);
        if (anyone_win != GAME_CHECK::ONGOING) {
            ret = anyone_win;
        } else if (is_draw != GAME_CHECK::ONGOING) {
            ret = is_draw;
        } else {
            ret = GAME_CHECK::ONGOING;
        }
    }
    return ret;
}

} // namespace Caro