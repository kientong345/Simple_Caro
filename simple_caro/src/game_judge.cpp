#include "../include/game_judge.hpp"

namespace Caro {

Game_Judge::Game_Judge(std::unique_ptr<Ruling> ruler_)
    : ruler(std::move(ruler_)) {}

void
Game_Judge::set_rule(RULE_TYPE rule_)
{
    switch (rule_) {
    case RULE_TYPE::TIC_TAC_TOE:
        ruler = std::make_unique<Tic_Tac_Toe_Rule>();
        break;
    case RULE_TYPE::FOUR_BLOCK_1:
        ruler = std::make_unique<Four_Block_1_Rule>();
        break;
    case RULE_TYPE::FIVE_BLOCK_2:
        ruler = std::make_unique<Five_Block_2_Rule>();
        break;
    default:
        break;
    }
}

GAME_CHECK
Game_Judge::check_end_condition(
    const Board& board_,
    const Coordinate& latest_move_
) {
    GAME_CHECK ret = GAME_CHECK::ONGOING;
    if (!ruler) {
        ret = GAME_CHECK::RULE_NOT_FOUND;
    } else {
        GAME_CHECK anyone_win_ = ruler->check_win(board_, latest_move_);
        GAME_CHECK is_draw_ = ruler->check_draw(board_);
        if (anyone_win_ != GAME_CHECK::ONGOING) {
            ret = anyone_win_;
        } else if (is_draw_ != GAME_CHECK::ONGOING) {
            ret = is_draw_;
        } else {
            ret = GAME_CHECK::ONGOING;
        }
    }
    return ret;
}

} // namespace Caro