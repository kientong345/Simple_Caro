#ifndef __GAME_JUDGE_H__
#define __GAME_JUDGE_H__

#include "common.h"
#include "Game_Context.h"

#include <memory>

class Ruling {
protected:
    uint32_t win_count;

    GAME_CHECK brute_force_check_win(
        std::shared_ptr<const std::vector<std::vector<TILE_STATE>>> board_
    ) {
        size_t col_num_ = board_->size();
        size_t row_num_ = board_->at(0).size();
        for (int i = 0; i < col_num_; ++i) {
            for (int j = 0; j < row_num_; ++j) {
                if (board_->at(i)[j] == TILE_STATE::EMPTY) {
                    continue;
                }
                LINE_PROPERTY horizontal_ = Caro::check_line_property(
                    board_, i, j, win_count, LINE_TYPE::HORIZONTAL
                );
                LINE_PROPERTY vertical_ = Caro::check_line_property(
                    board_, i, j, win_count, LINE_TYPE::VERTICAL
                );
                LINE_PROPERTY back_diag_ = Caro::check_line_property(
                    board_, i, j, win_count, LINE_TYPE::BACK_DIAGONAL
                );
                LINE_PROPERTY forward_diag_ = Caro::check_line_property(
                    board_, i, j, win_count, LINE_TYPE::FORWARD_DIAGONAL
                );
            }
        }
        return GAME_CHECK::ONGOING;
    }
    GAME_CHECK brute_force_check_draw(
        std::shared_ptr<const std::vector<std::vector<TILE_STATE>>> board_
    ) {
        // Implement the brute force check logic
        return GAME_CHECK::ONGOING;
    }
public:
    Ruling() : win_count(0) {}
    virtual GAME_CHECK
    check_win(
        std::shared_ptr<const std::vector<std::vector<TILE_STATE>>> board_
    ) = 0;
    virtual GAME_CHECK
    check_draw(
        std::shared_ptr<const std::vector<std::vector<TILE_STATE>>> board_
    ) = 0;
};

class Tic_Tac_Toe_Rule : public Ruling {
public:
    Tic_Tac_Toe_Rule() : Ruling() {
        win_count = 3;
    }
    GAME_CHECK
    check_win(
        std::shared_ptr<const std::vector<std::vector<TILE_STATE>>> board_
    ) override {
        // Implement the logic for checking the winning condition
        return brute_force_check_win(board_);
    }
    GAME_CHECK
    check_draw(
        std::shared_ptr<const std::vector<std::vector<TILE_STATE>>> board_
    ) override {
        // Implement the logic for checking the draw condition
        return brute_force_check_draw(board_);
    }
};

class Four_Block_1_Rule : public Ruling {
public:
    Four_Block_1_Rule() : Ruling() {
        win_count = 4;
    }
    GAME_CHECK
    check_win(
        std::shared_ptr<const std::vector<std::vector<TILE_STATE>>> board_
    ) override {
        // Implement the logic for checking the winning condition
        return brute_force_check_win(board_);
    }
    GAME_CHECK
    check_draw(
        std::shared_ptr<const std::vector<std::vector<TILE_STATE>>> board_
    ) override {
        // Implement the logic for checking the draw condition
        return brute_force_check_draw(board_);
    }
};

class Five_Block_2_Rule : public Ruling {
public:
    Five_Block_2_Rule() : Ruling() {
        win_count = 5;
    }
    GAME_CHECK
    check_win(
        std::shared_ptr<const std::vector<std::vector<TILE_STATE>>> board_
    ) override {
        // Implement the logic for checking the winning condition
        return brute_force_check_win(board_);
    }
    GAME_CHECK
    check_draw(
        std::shared_ptr<const std::vector<std::vector<TILE_STATE>>> board_
    ) override {
        // Implement the logic for checking the draw condition
        return brute_force_check_draw(board_);
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

    GAME_CHECK
    check_end_condition(
        std::shared_ptr<const std::vector<std::vector<TILE_STATE>>> board_
    ) {
        GAME_CHECK ret = GAME_CHECK::ONGOING;
        if (!ruler) {
            ret = GAME_CHECK::RULE_NOT_FOUND;
        } else {
            GAME_CHECK anyone_win_ = ruler->check_win(board_);
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
};

#endif /* __GAME_JUDGE_H__ */