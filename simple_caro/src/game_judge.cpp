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

GAME_EVENT
Game_Judge::judge(
    const Board& board,
    const Coordinate& latest_move
) {
    GAME_EVENT event = GAME_EVENT::NO_EVENT;
    if (!mRuler) {
        event = GAME_EVENT::NO_AVAILABLE_RULE;
    } else {
        GAME_EVENT has_event = mRuler->examine(board, latest_move);
        if (has_event != GAME_EVENT::NO_EVENT) {
            event = has_event;
        } else {
            event = GAME_EVENT::NO_EVENT;
        }
    }
    return event;
}

} // namespace Caro