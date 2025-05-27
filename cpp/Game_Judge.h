#ifndef __GAME_JUDGE_H__
#define __GAME_JUDGE_H__

#include "common.h"
#include "Game_Context.h"

#include <memory>

namespace Caro {

class Ruling {
protected:
    // uint32_t win_count;

    /**
     * common coordinate check function (Cartesian coordinate system)
     *   y
     *   ^
     *   |
     *   |
     *   |
     *   |
     *   |
     *   |<---win_count--->|
     *   +-----------------+-----> x
     * Base               End
     */
    // boilerplate as hell, though it would work right!
    /**
     * @brief check the property of a line
     * @param board_ the board
     * @param x_ x position on the board
     * @param y_ y position on the board
     * @param line_type_ direction of the line
     * @note x, y would be the base of the line
     * @return the line property
     */
    virtual LINE_PROPERTY check_line_property(
        std::shared_ptr<const std::vector<std::vector<TILE_STATE>>> board_,
        size_t x_, size_t y_, LINE_TYPE line_type_) = 0;

    GAME_CHECK brute_force_check_win(
        std::shared_ptr<const std::vector<std::vector<TILE_STATE>>> board_
    ) {
        size_t row_num_ = board_->size();
        size_t col_num_ = board_->at(0).size();
        GAME_CHECK ret = GAME_CHECK::ONGOING;
        auto ret_update_ = [&ret](LINE_PROPERTY line_property_) {
            if (line_property_ ==
                LINE_PROPERTY::PLAYER1_SEQUENCE_WITHOUT_BLOCKED) {
                ret = GAME_CHECK::PLAYER1_WIN;
            } else if (line_property_ ==
                LINE_PROPERTY::PLAYER2_SEQUENCE_WITHOUT_BLOCKED) {
                ret = GAME_CHECK::PLAYER2_WIN;
            } else {
                // do nothing
            }
        };
        for (int i = 0; i < row_num_; ++i) {
            for (int j = 0; j < col_num_; ++j) {
                if (board_->at(i)[j] == TILE_STATE::EMPTY) {
                    continue;
                }

                LINE_PROPERTY horizontal_ = check_line_property(
                    board_, i, j, LINE_TYPE::HORIZONTAL
                );
                ret_update_(horizontal_);
                
                LINE_PROPERTY vertical_ = check_line_property(
                    board_, i, j, LINE_TYPE::VERTICAL
                );
                ret_update_(vertical_);

                LINE_PROPERTY back_diag_ = check_line_property(
                    board_, i, j, LINE_TYPE::BACK_DIAGONAL
                );
                ret_update_(back_diag_);

                LINE_PROPERTY forward_diag_ = check_line_property(
                    board_, i, j, LINE_TYPE::FORWARD_DIAGONAL
                );
                ret_update_(forward_diag_);

                if ( ( ret == GAME_CHECK::PLAYER1_WIN )||
                    ( ret == GAME_CHECK::PLAYER2_WIN ) ) {
                    return ret;
                }
            }
        }
        return GAME_CHECK::ONGOING;
    }

    GAME_CHECK brute_force_check_draw(
        std::shared_ptr<const std::vector<std::vector<TILE_STATE>>> board_
    ) {
        size_t row_num_ = board_->size();
        size_t col_num_ = board_->at(0).size();
        for (int i = 0; i < row_num_; ++i) {
            for (int j = 0; j < col_num_; ++j) {
                if (board_->at(i)[j] == TILE_STATE::EMPTY) {
                    return GAME_CHECK::ONGOING;
                }
            }
        }
        return GAME_CHECK::DRAW;
    }

public:
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
private:
    virtual LINE_PROPERTY check_line_property(
        std::shared_ptr<const std::vector<std::vector<TILE_STATE>>> board_,
        size_t x_, size_t y_, LINE_TYPE line_type_
    ) override {
        LINE_PROPERTY ret_ = LINE_PROPERTY::OTHER;
        size_t row_num_ = board_->size();
        size_t col_num_ = board_->at(0).size();

        switch (line_type_) {
        case LINE_TYPE::HORIZONTAL:
            if (y_+2 < col_num_) {
                if ( ( board_->at(x_)[y_] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_)[y_+1] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_)[y_+2] == TILE_STATE::PLAYER1 )) {
                    ret_ = LINE_PROPERTY::PLAYER1_SEQUENCE_WITHOUT_BLOCKED;
                } else if ( ( board_->at(x_)[y_] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_)[y_+1] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_)[y_+2] == TILE_STATE::PLAYER2 ) ) {
                    ret_ = LINE_PROPERTY::PLAYER2_SEQUENCE_WITHOUT_BLOCKED;
                } else {
                    ret_ = LINE_PROPERTY::OTHER;
                }
            } else {
                ret_ = LINE_PROPERTY::OTHER;
            }
            break;
        case LINE_TYPE::VERTICAL:
            if (x_+2 < row_num_) {
                if ( ( board_->at(x_)[y_] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_+1)[y_] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_+2)[y_] == TILE_STATE::PLAYER1 ) ) {
                    ret_ = LINE_PROPERTY::PLAYER1_SEQUENCE_WITHOUT_BLOCKED;
                } else if ( ( board_->at(x_)[y_] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_+1)[y_] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_+2)[y_] == TILE_STATE::PLAYER2 ) ) {
                    ret_ = LINE_PROPERTY::PLAYER2_SEQUENCE_WITHOUT_BLOCKED;
                } else {
                    ret_ = LINE_PROPERTY::OTHER;
                }
            } else {
                ret_ = LINE_PROPERTY::OTHER;
            }
            break;
        case LINE_TYPE::BACK_DIAGONAL:
            if ( ( x_+2 < row_num_ ) && ( y_-2 >= 0 ) ) {
                if ( ( board_->at(x_)[y_] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_+1)[y_-1] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_+2)[y_-2] == TILE_STATE::PLAYER1 ) ) {
                    ret_ = LINE_PROPERTY::PLAYER1_SEQUENCE_WITHOUT_BLOCKED;
                } else if ( ( board_->at(x_)[y_] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_+1)[y_-1] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_+2)[y_-2] == TILE_STATE::PLAYER2 ) ) {
                    ret_ = LINE_PROPERTY::PLAYER2_SEQUENCE_WITHOUT_BLOCKED;
                } else {
                    ret_ = LINE_PROPERTY::OTHER;
                }
            } else {
                ret_ = LINE_PROPERTY::OTHER;
            }
            break;
        case LINE_TYPE::FORWARD_DIAGONAL:
            if ( ( x_+2 < row_num_ ) && ( y_+2 >= col_num_ ) ) {
                if ( ( board_->at(x_)[y_] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_+1)[y_+1] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_+2)[y_+2] == TILE_STATE::PLAYER1 ) ) {
                    ret_ = LINE_PROPERTY::PLAYER1_SEQUENCE_WITHOUT_BLOCKED;
                } else if ( ( board_->at(x_)[y_] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_+1)[y_+1] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_+2)[y_+2] == TILE_STATE::PLAYER2 ) ) {
                    ret_ = LINE_PROPERTY::PLAYER2_SEQUENCE_WITHOUT_BLOCKED;
                } else {
                    ret_ = LINE_PROPERTY::OTHER;
                }
            } else {
                ret_ = LINE_PROPERTY::OTHER;
            }
            break;
        default:
            ret_ = LINE_PROPERTY::OTHER;
            break;
        }
        return ret_;
    }

public:
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
private:
    virtual LINE_PROPERTY check_line_property(
        std::shared_ptr<const std::vector<std::vector<TILE_STATE>>> board_,
        size_t x_, size_t y_, LINE_TYPE line_type_
    ) override {
        LINE_PROPERTY ret_ = LINE_PROPERTY::OTHER;
        size_t row_num_ = board_->size();
        size_t col_num_ = board_->at(0).size();

        switch (line_type_) {
        case LINE_TYPE::HORIZONTAL:
            if (y_+3 < col_num_) {
                if ( ( board_->at(x_)[y_] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_)[y_+1] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_)[y_+2] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_)[y_+3] == TILE_STATE::PLAYER1 ) ) {
                    if ( ( ( y_+4 < col_num_ ) &&
                        ( board_->at(x_)[y_+4] == TILE_STATE::PLAYER2 ) ) ||
                        ( ( y_-1 >= 0 ) &&
                        ( board_->at(x_)[y_-1] == TILE_STATE::PLAYER2 ) ) ) {
                        ret_ = LINE_PROPERTY::PLAYER1_SEQUENCE_BLOCKED;
                    } else {
                        ret_ = LINE_PROPERTY::PLAYER1_SEQUENCE_WITHOUT_BLOCKED;
                    }
                } else if ( ( board_->at(x_)[y_] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_)[y_+1] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_)[y_+2] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_)[y_+3] == TILE_STATE::PLAYER2 ) ) {
                    if ( ( ( y_+4 < col_num_ ) &&
                        ( board_->at(x_)[y_+4] == TILE_STATE::PLAYER1 ) ) ||
                        ( ( y_-1 >= 0 ) &&
                        ( board_->at(x_)[y_-1] == TILE_STATE::PLAYER1 ) ) ) {
                        ret_ = LINE_PROPERTY::PLAYER2_SEQUENCE_BLOCKED;
                    } else {
                        ret_ = LINE_PROPERTY::PLAYER2_SEQUENCE_WITHOUT_BLOCKED;
                    }
                } else {
                    ret_ = LINE_PROPERTY::OTHER;
                }
            } else {
                ret_ = LINE_PROPERTY::OTHER;
            }
            break;
        case LINE_TYPE::VERTICAL:
            if (x_+3 < row_num_) {
                if ( ( board_->at(x_)[y_] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_+1)[y_] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_+2)[y_] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_+3)[y_] == TILE_STATE::PLAYER1 ) ) {
                    if ( ( ( x_+4 < row_num_ ) &&
                        ( board_->at(x_+4)[y_] == TILE_STATE::PLAYER2 ) ) ||
                        ( ( x_-1 >= 0 ) &&
                        ( board_->at(x_-1)[y_] == TILE_STATE::PLAYER2 ) ) ) {
                        ret_ = LINE_PROPERTY::PLAYER1_SEQUENCE_BLOCKED;
                    } else {
                        ret_ = LINE_PROPERTY::PLAYER1_SEQUENCE_WITHOUT_BLOCKED;
                    }
                } else if ( ( board_->at(x_)[y_] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_+1)[y_] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_+2)[y_] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_+3)[y_] == TILE_STATE::PLAYER2 ) ) {
                    if ( ( ( x_+4 < row_num_ ) &&
                        ( board_->at(x_+4)[y_] == TILE_STATE::PLAYER1 ) ) ||
                        ( ( x_-1 >= 0 ) &&
                        ( board_->at(x_-1)[y_] == TILE_STATE::PLAYER1 ) ) ) {
                        ret_ = LINE_PROPERTY::PLAYER2_SEQUENCE_BLOCKED;
                    } else {
                        ret_ = LINE_PROPERTY::PLAYER2_SEQUENCE_WITHOUT_BLOCKED;
                    }
                } else {
                    ret_ = LINE_PROPERTY::OTHER;
                }
            } else {
                ret_ = LINE_PROPERTY::OTHER;
            }
            break;
        case LINE_TYPE::BACK_DIAGONAL:
            if ( ( x_+3 < row_num_ ) && ( y_-3 >= 0 ) ) {
                if ( ( board_->at(x_)[y_] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_+1)[y_-1] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_+2)[y_-2] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_+3)[y_-3] == TILE_STATE::PLAYER1 ) ) {
                    if ( ( ( x_+4 < row_num_ ) && ( y_-4 >= 0 ) &&
                        ( board_->at(x_+4)[y_-4] == TILE_STATE::PLAYER2 ) ) ||
                        ( ( x_-1 >= 0 ) && ( y_+1 < col_num_ ) &&
                        ( board_->at(x_-1)[y_+1] == TILE_STATE::PLAYER2 ) ) ) {
                        ret_ = LINE_PROPERTY::PLAYER1_SEQUENCE_BLOCKED;
                    } else {
                        ret_ = LINE_PROPERTY::PLAYER1_SEQUENCE_WITHOUT_BLOCKED;
                    }
                } else if ( ( board_->at(x_)[y_] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_+1)[y_-1] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_+2)[y_-2] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_+3)[y_-3] == TILE_STATE::PLAYER2 ) ) {
                    if ( ( ( x_+4 < row_num_ ) && ( y_-4 >= 0 ) &&
                        ( board_->at(x_+4)[y_-4] == TILE_STATE::PLAYER1 ) ) ||
                        ( ( x_-1 >= 0 ) && ( y_+1 < col_num_ ) &&
                        ( board_->at(x_-1)[y_+1] == TILE_STATE::PLAYER1 ) ) ) {
                        ret_ = LINE_PROPERTY::PLAYER2_SEQUENCE_BLOCKED;
                    } else {
                        ret_ = LINE_PROPERTY::PLAYER2_SEQUENCE_WITHOUT_BLOCKED;
                    }
                } else {
                    ret_ = LINE_PROPERTY::OTHER;
                }
            } else {
                ret_ = LINE_PROPERTY::OTHER;
            }
            break;
        case LINE_TYPE::FORWARD_DIAGONAL:
            if ( ( x_+3 < row_num_ ) && ( y_+3 >= col_num_ ) ) {
                if ( ( board_->at(x_)[y_] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_+1)[y_+1] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_+2)[y_+2] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_+3)[y_+3] == TILE_STATE::PLAYER1 ) ) {
                    if ( ( ( x_+4 < row_num_ ) && ( y_+4 < col_num_ ) &&
                        ( board_->at(x_+4)[y_+4] == TILE_STATE::PLAYER2 ) ) ||
                        ( ( x_-1 >= 0 ) && ( y_-1 >= 0 ) &&
                        ( board_->at(x_-1)[y_-1] == TILE_STATE::PLAYER2 ) ) ) {
                        ret_ = LINE_PROPERTY::PLAYER1_SEQUENCE_BLOCKED;
                    } else {
                        ret_ = LINE_PROPERTY::PLAYER1_SEQUENCE_WITHOUT_BLOCKED;
                    }
                } else if ( ( board_->at(x_)[y_] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_+1)[y_+1] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_+2)[y_+2] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_+3)[y_+3] == TILE_STATE::PLAYER2 ) ) {
                    if ( ( ( x_+4 < row_num_ ) && ( y_+4 < col_num_ ) &&
                        ( board_->at(x_+4)[y_+4] == TILE_STATE::PLAYER1 ) ) ||
                        ( ( x_-1 >= 0 ) && ( y_-1 >= 0 ) &&
                        ( board_->at(x_-1)[y_-1] == TILE_STATE::PLAYER1 ) ) ) {
                        ret_ = LINE_PROPERTY::PLAYER2_SEQUENCE_BLOCKED;
                    } else {
                        ret_ = LINE_PROPERTY::PLAYER2_SEQUENCE_WITHOUT_BLOCKED;
                    }
                } else {
                    ret_ = LINE_PROPERTY::OTHER;
                }
            } else {
                ret_ = LINE_PROPERTY::OTHER;
            }
            break;
        default:
            ret_ = LINE_PROPERTY::OTHER;
            break;
        }
        return ret_;
    }
public:
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
private:
    virtual LINE_PROPERTY check_line_property(
        std::shared_ptr<const std::vector<std::vector<TILE_STATE>>> board_,
        size_t x_, size_t y_, LINE_TYPE line_type_
    ) override {
        LINE_PROPERTY ret_ = LINE_PROPERTY::OTHER;
        size_t row_num_ = board_->size();
        size_t col_num_ = board_->at(0).size();

        switch (line_type_) {
        case LINE_TYPE::HORIZONTAL:
            if (y_+4 < col_num_) {
                if ( ( board_->at(x_)[y_] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_)[y_+1] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_)[y_+2] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_)[y_+3] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_)[y_+4] == TILE_STATE::PLAYER1 ) ) {
                    if ( ( ( y_+5 < col_num_ ) &&
                        ( board_->at(x_)[y_+5] == TILE_STATE::PLAYER2 ) ) &&
                        ( ( y_-1 >= 0 ) &&
                        ( board_->at(x_)[y_-1] == TILE_STATE::PLAYER2 ) ) ) {
                        ret_ = LINE_PROPERTY::PLAYER1_SEQUENCE_BLOCKED;
                    } else {
                        ret_ = LINE_PROPERTY::PLAYER1_SEQUENCE_WITHOUT_BLOCKED;
                    }
                } else if ( ( board_->at(x_)[y_] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_)[y_+1] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_)[y_+2] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_)[y_+3] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_)[y_+4] == TILE_STATE::PLAYER2 ) ) {
                    if ( ( ( y_+5 < col_num_ ) &&
                        ( board_->at(x_)[y_+5] == TILE_STATE::PLAYER1 ) ) &&
                        ( ( y_-1 >= 0 ) &&
                        ( board_->at(x_)[y_-1] == TILE_STATE::PLAYER1 ) ) ) {
                        ret_ = LINE_PROPERTY::PLAYER2_SEQUENCE_BLOCKED;
                    } else {
                        ret_ = LINE_PROPERTY::PLAYER2_SEQUENCE_WITHOUT_BLOCKED;
                    }
                } else {
                    ret_ = LINE_PROPERTY::OTHER;
                }
            } else {
                ret_ = LINE_PROPERTY::OTHER;
            }
            break;
        case LINE_TYPE::VERTICAL:
            if (x_+4 < row_num_) {
                if ( ( board_->at(x_)[y_] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_+1)[y_] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_+2)[y_] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_+3)[y_] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_+4)[y_] == TILE_STATE::PLAYER1 ) ) {
                    if ( ( ( x_+5 < row_num_ ) &&
                        ( board_->at(x_+5)[y_] == TILE_STATE::PLAYER2 ) ) &&
                        ( ( x_-1 >= 0 ) &&
                        ( board_->at(x_-1)[y_] == TILE_STATE::PLAYER2 ) ) ) {
                        ret_ = LINE_PROPERTY::PLAYER1_SEQUENCE_BLOCKED;
                    } else {
                        ret_ = LINE_PROPERTY::PLAYER1_SEQUENCE_WITHOUT_BLOCKED;
                    }
                } else if ( ( board_->at(x_)[y_] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_+1)[y_] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_+2)[y_] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_+3)[y_] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_+4)[y_] == TILE_STATE::PLAYER2 ) ) {
                    if ( ( ( x_+5 < row_num_ ) &&
                        ( board_->at(x_+5)[y_] == TILE_STATE::PLAYER1 ) ) &&
                        ( ( x_-1 >= 0 ) &&
                        ( board_->at(x_-1)[y_] == TILE_STATE::PLAYER1 ) ) ) {
                        ret_ = LINE_PROPERTY::PLAYER2_SEQUENCE_BLOCKED;
                    } else {
                        ret_ = LINE_PROPERTY::PLAYER2_SEQUENCE_WITHOUT_BLOCKED;
                    }
                } else {
                    ret_ = LINE_PROPERTY::OTHER;
                }
            } else {
                ret_ = LINE_PROPERTY::OTHER;
            }
            break;
        case LINE_TYPE::BACK_DIAGONAL:
            if ( ( x_+4 < row_num_ ) && ( y_-4 >= 0 ) ) {
                if ( ( board_->at(x_)[y_] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_+1)[y_-1] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_+2)[y_-2] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_+3)[y_-3] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_+4)[y_-4] == TILE_STATE::PLAYER1 ) ) {
                    if ( ( ( x_+5 < row_num_ ) && ( y_-5 >= 0 ) &&
                        ( board_->at(x_+5)[y_-5] == TILE_STATE::PLAYER2 ) ) &&
                        ( ( x_-1 >= 0 ) && ( y_+1 < col_num_ ) &&
                        ( board_->at(x_-1)[y_+1] == TILE_STATE::PLAYER2 ) ) ) {
                        ret_ = LINE_PROPERTY::PLAYER1_SEQUENCE_BLOCKED;
                    } else {
                        ret_ = LINE_PROPERTY::PLAYER1_SEQUENCE_WITHOUT_BLOCKED;
                    }
                } else if ( ( board_->at(x_)[y_] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_+1)[y_-1] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_+2)[y_-2] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_+3)[y_-3] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_+4)[y_-4] == TILE_STATE::PLAYER2 ) ) {
                    if ( ( ( x_+5 < row_num_ ) && ( y_-5 >= 0 ) &&
                        ( board_->at(x_+5)[y_-5] == TILE_STATE::PLAYER1 ) ) &&
                        ( ( x_-1 >= 0 ) && ( y_+1 < col_num_ ) &&
                        ( board_->at(x_-1)[y_+1] == TILE_STATE::PLAYER1 ) ) ) {
                        ret_ = LINE_PROPERTY::PLAYER2_SEQUENCE_BLOCKED;
                    } else {
                        ret_ = LINE_PROPERTY::PLAYER2_SEQUENCE_WITHOUT_BLOCKED;
                    }
                } else {
                    ret_ = LINE_PROPERTY::OTHER;
                }
            } else {
                ret_ = LINE_PROPERTY::OTHER;
            }
            break;
        case LINE_TYPE::FORWARD_DIAGONAL:
            if ( ( x_+4 < row_num_ ) && ( y_+4 >= col_num_ ) ) {
                if ( ( board_->at(x_)[y_] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_+1)[y_+1] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_+2)[y_+2] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_+3)[y_+3] == TILE_STATE::PLAYER1 ) &&
                    ( board_->at(x_+4)[y_+4] == TILE_STATE::PLAYER1 ) ) {
                    if ( ( ( x_+5 < row_num_ ) && ( y_+5 < col_num_ ) &&
                        ( board_->at(x_+5)[y_+5] == TILE_STATE::PLAYER2 ) ) &&
                        ( ( x_-1 >= 0 ) && ( y_-1 >= 0 ) &&
                        ( board_->at(x_-1)[y_-1] == TILE_STATE::PLAYER2 ) ) ) {
                        ret_ = LINE_PROPERTY::PLAYER1_SEQUENCE_BLOCKED;
                    } else {
                        ret_ = LINE_PROPERTY::PLAYER1_SEQUENCE_WITHOUT_BLOCKED;
                    }
                } else if ( ( board_->at(x_)[y_] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_+1)[y_+1] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_+2)[y_+2] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_+3)[y_+3] == TILE_STATE::PLAYER2 ) &&
                    ( board_->at(x_+4)[y_+4] == TILE_STATE::PLAYER2 ) ) {
                    if ( ( ( x_+5 < row_num_ ) && ( y_+5 < col_num_ ) &&
                        ( board_->at(x_+5)[y_+5] == TILE_STATE::PLAYER1 ) ) &&
                        ( ( x_-1 >= 0 ) && ( y_-1 >= 0 ) &&
                        ( board_->at(x_-1)[y_-1] == TILE_STATE::PLAYER1 ) ) ) {
                        ret_ = LINE_PROPERTY::PLAYER2_SEQUENCE_BLOCKED;
                    } else {
                        ret_ = LINE_PROPERTY::PLAYER2_SEQUENCE_WITHOUT_BLOCKED;
                    }
                } else {
                    ret_ = LINE_PROPERTY::OTHER;
                }
            } else {
                ret_ = LINE_PROPERTY::OTHER;
            }
            break;
        default:
            ret_ = LINE_PROPERTY::OTHER;
            break;
        }
        return ret_;
    }
public:
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

} // namespace Caro

#endif /* __GAME_JUDGE_H__ */