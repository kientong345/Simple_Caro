#ifndef __COMMON_H__
#define __COMMON_H__

#include <cstdint>
#include <memory>
#include <vector>
#include <math.h>

struct Coordinate {
    int64_t x;
    int64_t y;
    // for std::set<Coordinate>
    bool operator<(const Coordinate& other) const {
        return (x < other.x) || (x == other.x && y < other.y);
    }
};

enum class TILE_STATE {
    EMPTY,
    PLAYER1,
    PLAYER2,
};

enum class MOVE_RESULT {
    SUCCESS,
    ALREADY_OCCUPIED,
    WRONG_TURN,
    OUT_OF_BOUNDS,
};

enum class GAME_STATE {
    PLAYER1_TURN,
    PLAYER2_TURN,
    PLAYER1_WON,
    PLAYER2_WON,
    DREW,
    NOT_INPROGRESS,
};

enum class RULE_TYPE {
    TIC_TAC_TOE,
    FOUR_BLOCK_1,
    FIVE_BLOCK_2,
};

enum class PARTICIPANT {
    PLAYER1,
    PLAYER2,
};

enum class GAME_CHECK {
    ONGOING,
    PLAYER1_WIN,
    PLAYER2_WIN,
    DRAW,
    RULE_NOT_FOUND,
};

enum class LINE_TYPE {
    HORIZONTAL,
    VERTICAL,
    BACK_DIAGONAL,
    FORWARD_DIAGONAL,
};

enum class LINE_PROPERTY {
    PLAYER1_SEQUENCE_WITHOUT_BLOCKED,
    PLAYER2_SEQUENCE_WITHOUT_BLOCKED,
    PLAYER1_SEQUENCE_BLOCKED,
    PLAYER2_SEQUENCE_BLOCKED,
    OTHER,
};

namespace Caro {
/**
 * common coordinate check function (Cartesian coordinate system)
 *   y
 *   ^
 *   |
 *   |
 *   |
 *   |
 *   |
 *   |<----offset---->|
 *   +----------------+-----> x
 * Base              End
 */
inline LINE_PROPERTY check_line_property(
    std::shared_ptr<const std::vector<std::vector<TILE_STATE>>> board_,
    size_t x_, size_t y_, int offset_, LINE_TYPE line_type_
) {
    LINE_PROPERTY ret_ = LINE_PROPERTY::OTHER;
    size_t col_num_ = board_->size();
    size_t row_num_ = board_->at(0).size();
    int line_sum_ = 0;

    switch (line_type_) {
    case LINE_TYPE::HORIZONTAL:
        for (size_t i = 0; i < offset_; i+=(offset_/abs(offset_))) {
            if ( ( y_+i >= col_num_ ) || ( x_ >= row_num_ ) ) {
                break;
            }
            line_sum_ += static_cast<int>(board_->at(x_)[y_+i]);
        }
        if ( static_cast<int>(TILE_STATE::PLAYER1) * offset_ 
            == line_sum_ ) {
            if ( ( y_-1 >= 0 ) &&
                ( y_+offset_ < col_num_ ) &&
                ( board_->at(x_)[y_-1] == TILE_STATE::PLAYER2 ) &&
                ( board_->at(x_)[y_+offset_] == TILE_STATE::PLAYER2 ) )
            {
                ret_ = LINE_PROPERTY::PLAYER1_SEQUENCE_BLOCKED;
            } else {
                ret_ = LINE_PROPERTY::PLAYER1_SEQUENCE_WITHOUT_BLOCKED;
            }
        } else if ( static_cast<int>(TILE_STATE::PLAYER2) * offset_
            == line_sum_ ) {
            if ( ( y_-1 >= 0 ) &&
                ( y_+offset_ < col_num_ ) &&
                ( board_->at(x_)[y_-1] == TILE_STATE::PLAYER1 ) &&
                ( board_->at(x_)[y_+offset_] == TILE_STATE::PLAYER1 ) )
            {
                ret_ = LINE_PROPERTY::PLAYER2_SEQUENCE_BLOCKED;
            } else {
                ret_ = LINE_PROPERTY::PLAYER2_SEQUENCE_WITHOUT_BLOCKED;
            }
        } else {
            ret_ = LINE_PROPERTY::OTHER;
        }
        break;
    case LINE_TYPE::VERTICAL:
        for (size_t j = 0; j < offset_; j+=(offset_/abs(offset_))) {
            if ( ( y_ >= col_num_ ) || ( x_+j >= row_num_ ) ) {
                break;
            }
            line_sum_ += static_cast<int>(board_->at(x_+j)[y_]);
        }
        
        break;
    case LINE_TYPE::BACK_DIAGONAL:
        for (size_t i = 0; i < offset_; i+=(offset_/abs(offset_))) {

        }
        break;
    case LINE_TYPE::FORWARD_DIAGONAL:
        for (size_t i = 0; i < offset_; i+=(offset_/abs(offset_))) {

        }
        break;
    default:
        break;
    }
        
    return ret_;
}

}; // namespace Caro

#endif /* __COMMON_H__ */