#ifndef __GAME_JUDGE_H__
#define __GAME_JUDGE_H__

#include "common.h"
#include "Game_Context.h"

#include <memory>

class Ruling {
public:
    virtual GAME_STATE
    check_win(
        std::shared_ptr<const std::vector<std::vector<TILE_STATE>>> board_
    ) = 0;
    virtual GAME_STATE
    check_draw(
        std::shared_ptr<const std::vector<std::vector<TILE_STATE>>> board_
    ) = 0;
};

class Tic_Tac_Toe_Rule : public Ruling {
public:
    GAME_STATE
    check_win(
        std::shared_ptr<const std::vector<std::vector<TILE_STATE>>> board_
    ) override {
        // Implement the logic for checking the winning condition
        return GAME_STATE::PLAYER1_TURN;
    }
    GAME_STATE
    check_draw(
        std::shared_ptr<const std::vector<std::vector<TILE_STATE>>> board_
    ) override {
        // Implement the logic for checking the draw condition
        return GAME_STATE::PLAYER1_TURN;
    }
};

class Four_Block_1_Rule : public Ruling {
public:
    GAME_STATE
    check_win(
        std::shared_ptr<const std::vector<std::vector<TILE_STATE>>> board_
    ) override {
        // Implement the logic for checking the winning condition
        return GAME_STATE::PLAYER1_TURN;
    }
    GAME_STATE
    check_draw(
        std::shared_ptr<const std::vector<std::vector<TILE_STATE>>> board_
    ) override {
        // Implement the logic for checking the draw condition
        return GAME_STATE::PLAYER1_TURN;
    }
};

class Five_Block_2_Rule : public Ruling {
public:
    GAME_STATE
    check_win(
        std::shared_ptr<const std::vector<std::vector<TILE_STATE>>> board_
    ) override {
        // Implement the logic for checking the winning condition
        return GAME_STATE::PLAYER1_TURN;
    }
    GAME_STATE
    check_draw(
        std::shared_ptr<const std::vector<std::vector<TILE_STATE>>> board_
    ) override {
        // Implement the logic for checking the draw condition
        return GAME_STATE::PLAYER1_TURN;
    }
};

class Game_Judge {
private:
    std::unique_ptr<Ruling> ruler;

public:
    Game_Judge(std::unique_ptr<Ruling> ruler_ = nullptr)
        : ruler(std::move(ruler_)) {}

    ~Game_Judge() = default;

    void
    set_rule(RULE_TYPE rule_)
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

    GAME_STATE
    check_end_condition(
        std::shared_ptr<const std::vector<std::vector<TILE_STATE>>> board_,
        GAME_STATE current_state_) 
    {
        GAME_STATE ret = current_state_;
        if (!ruler) {
            ret = GAME_STATE::RULE_NOT_FOUND;
        } else {
            GAME_STATE anyone_win_ = ruler->check_win(board_);
            GAME_STATE is_draw_ = ruler->check_draw(board_);
            if (anyone_win_ != GAME_STATE::PLAYER1_TURN) {
                ret = anyone_win_;
            } else if (is_draw_ != GAME_STATE::PLAYER1_TURN) {
                ret = is_draw_;
            } else {
                ret = current_state_;
            }
        }
        return ret;
    }
};

#endif /* __GAME_JUDGE_H__ */