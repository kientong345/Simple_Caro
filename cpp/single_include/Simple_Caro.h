#ifndef __SIMPLE_CARO_H__
#define __SIMPLE_CARO_H__

#include <cstdint>
#include <vector>
#include <set>
#include <memory>
#if __cplusplus >= 201703L
#include <any>
#else
#endif //  __cplusplus >= 201703L

namespace Caro {

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
    HORIZONTAL,             // '-' to-the-right
    VERTICAL,               // '|' upward
    BACK_DIAGONAL,          // '\\' upward
    FORWARD_DIAGONAL,       // '/' upward
};

enum class LINE_PROPERTY {
    PLAYER1_SEQUENCE_WITHOUT_BLOCKED,
    PLAYER2_SEQUENCE_WITHOUT_BLOCKED,
    PLAYER1_SEQUENCE_BLOCKED,
    PLAYER2_SEQUENCE_BLOCKED,
    OTHER,
};

class Player_Context {
private:
#if __cplusplus >= 201703L
    std::any player_info;
#else
    void* player_info;
#endif //  __cplusplus >= 201703L
    std::vector<Coordinate> moves_history;
    std::vector<Coordinate> undone_moves;
    std::set<Coordinate> moves_set;

public:
    Player_Context() : player_info(nullptr) {}

    ~Player_Context() = default;

    MOVE_RESULT
    move(Coordinate move_) {
        MOVE_RESULT ret = MOVE_RESULT::SUCCESS;
        if ( ( move_.x < 0 ) || ( move_.y < 0 ) ) {
            ret = MOVE_RESULT::OUT_OF_BOUNDS;
        } else if ( ( moves_set.find(move_) ) != 
                    ( moves_set.end() ) ) {
            ret = MOVE_RESULT::ALREADY_OCCUPIED;
        } else {
            moves_history.push_back(move_);
            moves_set.insert(move_);
            if ( !undone_moves.empty() ) {
                undone_moves.pop_back();
            }
            ret = MOVE_RESULT::SUCCESS;
        }
        return ret;
    }

    MOVE_RESULT
    undo() {
        if ( !moves_history.empty() ) {
            Coordinate move_ = moves_history.back();
            moves_history.pop_back();
            undone_moves.push_back(move_);
            moves_set.erase(move_);
            return MOVE_RESULT::SUCCESS;
        } else {
            return MOVE_RESULT::OUT_OF_BOUNDS;
        }
    }

    MOVE_RESULT
    redo() {
        if ( !undone_moves.empty() ) {
            Coordinate move_ = undone_moves.back();
            undone_moves.pop_back();
            moves_history.push_back(move_);
            moves_set.insert(move_);
            return MOVE_RESULT::SUCCESS;
        } else {
            return MOVE_RESULT::OUT_OF_BOUNDS;
        }
    }

    void
    reset_context() {
        undone_moves.clear();
        moves_history.clear();
        moves_set.clear();
    }

    const std::vector<Coordinate>
    get_moves_history() const {
        return moves_history;
    }

    const std::vector<Coordinate>
    get_undone_moves() const {
        return undone_moves;
    }
    
    const std::set<Coordinate>
    get_moves_set() const {
        return moves_set;
    }

    template<typename T>
    void
    make_info(const T& info_) {
#if __cplusplus >= 201703L
        player_info = std::make_any<T>(info_);
#else
        player_info = new T(info_);
#endif //  __cplusplus >= 201703L
    }

    template<typename T>
    T*
    try_access_info() {
#if __cplusplus >= 201703L
        return std::any_cast<T>(&player_info);
#else
    return static_cast<T*>(player_info);
#endif //  __cplusplus >= 201703L
    }
};

class Board_Context {
private:
    std::shared_ptr<std::vector<std::vector<TILE_STATE>>> board;

public:
    Board_Context(uint32_t width, uint32_t height)
        : board(std::make_shared<std::vector<std::vector<TILE_STATE>>>(
            height, std::vector<TILE_STATE>(width, TILE_STATE::EMPTY))) {}

    ~Board_Context() = default;

    std::shared_ptr<const std::vector<std::vector<TILE_STATE>>>
    get_board()
        const {
        return board;
    }

    MOVE_RESULT
    set_tile(Coordinate pos_, TILE_STATE state) {
        MOVE_RESULT ret = MOVE_RESULT::SUCCESS;
        bool board_has_tiles = (board->size() > 0) &&
                               (board->at(0).size() > 0);
        bool pos_on_board = (board_has_tiles) &&
            (pos_.x >= 0) && (pos_.y >= 0) &&
            (pos_.x < board->size()) &&
            (pos_.y < board->at(0).size());
        if ( !pos_on_board ) {
            ret = MOVE_RESULT::OUT_OF_BOUNDS;
        } else if (board->at(pos_.x)[pos_.y] != TILE_STATE::EMPTY) {
            ret = MOVE_RESULT::ALREADY_OCCUPIED;
        } else {
            (*board)[pos_.x][pos_.y] = state;
            ret = MOVE_RESULT::SUCCESS;
        }
        return ret;
    }

    MOVE_RESULT
    unset_tile(Coordinate pos_) {
        MOVE_RESULT ret = MOVE_RESULT::SUCCESS;
        bool board_has_tiles = (board->size() > 0) &&
                               (board->at(0).size() > 0);
        bool pos_on_board = (board_has_tiles) &&
            (pos_.x >= 0) && (pos_.y >= 0) &&
            (pos_.x < board->size()) &&
            (pos_.y < board->at(0).size());
        if ( !pos_on_board ) {
            ret = MOVE_RESULT::OUT_OF_BOUNDS;
        } else {
            (*board)[pos_.x][pos_.y] = TILE_STATE::EMPTY;
            ret = MOVE_RESULT::SUCCESS;
        }
        return ret;
    }

    void
    reset_context() {
        board->clear();
    }
};

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
            if ( ( x_+2 < row_num_ ) && ( y_+2 < col_num_ ) ) {
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
            if ( ( x_+3 < row_num_ ) && ( y_+3 < col_num_ ) ) {
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
            if ( ( x_+4 < row_num_ ) && ( y_+4 < col_num_ ) ) {
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

class Simple_Caro {
private:
    std::unique_ptr<Player_Context> player1;
    std::unique_ptr<Player_Context> player2;
    std::unique_ptr<Board_Context> board;
    std::unique_ptr<Game_Judge> judge;
    GAME_STATE state;

    void
    update_context() {
        GAME_CHECK is_end_ = judge->check_end_condition(
            board->get_board());
        if ((is_end_ != GAME_CHECK::RULE_NOT_FOUND) && 
            (is_end_ != GAME_CHECK::ONGOING)) {
            switch (is_end_) {
            case GAME_CHECK::PLAYER1_WIN:
                state = GAME_STATE::PLAYER1_WON;
                break;
            case GAME_CHECK::PLAYER2_WIN:
                state = GAME_STATE::PLAYER2_WON;
                break;
            case GAME_CHECK::DRAW:
                state = GAME_STATE::DREW;
                break;
            default:
                break;
            }
        }
    }

public:
    Simple_Caro()
        : player1(nullptr),
          player2(nullptr),
          board(nullptr),
          judge(nullptr),
          state(GAME_STATE::NOT_INPROGRESS) {}

    ~Simple_Caro() = default;

    template <typename T>
    void
    register_player_info(PARTICIPANT who_,
                        const T& player_info_) {
        switch (who_) {
        case PARTICIPANT::PLAYER1:
            if (!player1) {
                player1 = std::make_unique<Player_Context>();
            }
            player1->make_info<T>(player_info_);
            break;
        case PARTICIPANT::PLAYER2:
            if (!player2) {
                player2 = std::make_unique<Player_Context>();
            }
            player2->make_info<T>(player_info_);
            break;
        default:
            break;
        }
    }

    template <typename T>
    T*
    access_player_info(PARTICIPANT who_) {
        T* ret = nullptr;
        switch (who_) {
        case PARTICIPANT::PLAYER1:
            if (player1) {
                ret = player1->try_access_info<T>();
            }
            break;
        case PARTICIPANT::PLAYER2:
            if (player2) {
                ret = player2->try_access_info<T>();
            }
            break;
        default:
            break;
        }
        return ret;
    }

    void
    set_board_size(int32_t width_, int32_t height_) {
        board = std::make_unique<Board_Context>(width_, height_);
    }

    void
    set_rule(RULE_TYPE rule_) {
        if (!judge) {
            judge = std::make_unique<Game_Judge>();
        }
        judge->set_rule(rule_);
    }

    void
    unset_rule() {
        judge = nullptr;
    }

    void
    start(GAME_STATE first_turn_ = GAME_STATE::PLAYER1_TURN) {
        if (!player1) {
            player1 = std::make_unique<Player_Context>();
        }
        if (!player2) {
            player2 = std::make_unique<Player_Context>();
        }
        if (!board) {
            board = std::make_unique<Board_Context>(1000, 1000);
        }
        if (!judge) {
            judge = std::make_unique<Game_Judge>();
            judge->set_rule(RULE_TYPE::FOUR_BLOCK_1);
        }
        state = first_turn_;
    }

    void
    stop(GAME_STATE first_turn_ = GAME_STATE::PLAYER1_TURN) {
        if (player1) {
            player1->reset_context();
        }
        if (player2) {
            player2->reset_context();
        }
        if (board) {
            board->reset_context();
        }
        state = GAME_STATE::NOT_INPROGRESS;
    }

    MOVE_RESULT
    player_move(PARTICIPANT who_, Coordinate move_) {
        MOVE_RESULT ret = MOVE_RESULT::SUCCESS;
        switch (who_) {
        case PARTICIPANT::PLAYER1:
            if (state != GAME_STATE::PLAYER1_TURN) {
                ret = MOVE_RESULT::WRONG_TURN;
            } else {
                ret = board->set_tile(move_,
                                    TILE_STATE::PLAYER1);
                if (ret == MOVE_RESULT::SUCCESS) {
                    ret = player1->move(move_);
                }
            }
            break;
        case PARTICIPANT::PLAYER2:
            if (state != GAME_STATE::PLAYER2_TURN) {
                ret = MOVE_RESULT::WRONG_TURN;
            } else {
                ret = board->set_tile(move_,
                                    TILE_STATE::PLAYER2);
                if (ret == MOVE_RESULT::SUCCESS) {
                    ret = player2->move(move_);
                }
            }
            break;
        default:
            ret = MOVE_RESULT::WRONG_TURN;
            break;
        }
        if (ret == MOVE_RESULT::SUCCESS) {
            update_context();
        }
        return ret;
    }

    MOVE_RESULT
    player_undo(PARTICIPANT who_) {
        MOVE_RESULT ret = MOVE_RESULT::SUCCESS;
        switch (who_) {
        case PARTICIPANT::PLAYER1:
            if (state != GAME_STATE::PLAYER1_TURN) {
                ret = MOVE_RESULT::WRONG_TURN;
            } else {
                ret = player1->undo();
                if (ret == MOVE_RESULT::SUCCESS) {
                    ret = board->unset_tile(player1
                                            ->get_undone_moves()
                                            .back());
                }
            }
            break;
        case PARTICIPANT::PLAYER2:
            if (state != GAME_STATE::PLAYER2_TURN) {
                ret = MOVE_RESULT::WRONG_TURN;
            } else {
                ret = player2->undo();
                if (ret == MOVE_RESULT::SUCCESS) {
                    ret = board->unset_tile(player2
                                            ->get_undone_moves()
                                            .back());
                }
            }
            break;
        default:
            ret = MOVE_RESULT::WRONG_TURN;
            break;
        }
        if (ret == MOVE_RESULT::SUCCESS) {
            update_context();
        }
        return ret;
    }

    MOVE_RESULT
    player_redo(PARTICIPANT who_) {
        MOVE_RESULT ret = MOVE_RESULT::SUCCESS;
        switch (who_) {
        case PARTICIPANT::PLAYER1:
            if (state != GAME_STATE::PLAYER1_TURN) {
                ret = MOVE_RESULT::WRONG_TURN;
            } else {
                ret = player1->redo();
                if (ret == MOVE_RESULT::SUCCESS) {
                    ret = board->set_tile(player1
                                        ->get_moves_history()
                                        .back(),
                                        TILE_STATE::PLAYER1);
                }
            }
            break;
        case PARTICIPANT::PLAYER2:
            if (state != GAME_STATE::PLAYER2_TURN) {
                ret = MOVE_RESULT::WRONG_TURN;
            } else {
                ret = player2->redo();
                if (ret == MOVE_RESULT::SUCCESS) {
                    ret = board->set_tile(player2
                                        ->get_moves_history()
                                        .back(),
                                        TILE_STATE::PLAYER2);
                }
            }
            break;
        default:
            ret = MOVE_RESULT::WRONG_TURN;
            break;
        }
        if (ret == MOVE_RESULT::SUCCESS) {
            update_context();
        }
        return ret;
    }

    void
    switch_turn() {
        switch (state) {
        case GAME_STATE::PLAYER1_TURN:
            state = GAME_STATE::PLAYER2_TURN;
            break;
        case GAME_STATE::PLAYER2_TURN:
            state = GAME_STATE::PLAYER1_TURN;
            break;
        default:
            break;
        }
    }

    std::shared_ptr<const std::vector<std::vector<TILE_STATE>>>
    get_board() const {
        return board->get_board();
    }

    GAME_STATE
    get_state() const {
        return state;
    }

    bool
    is_over() const {
        return (state == GAME_STATE::PLAYER1_WON ||
                state == GAME_STATE::PLAYER2_WON ||
                state == GAME_STATE::DREW);
    }

    const std::vector<Coordinate>
    get_moves_history(PARTICIPANT who_) const {
        std::vector<Coordinate> ret;
        switch (who_) {
        case PARTICIPANT::PLAYER1:
            if (player1) {
                ret = std::move(player1->get_moves_history());
            }
            break;
        case PARTICIPANT::PLAYER2:
            if (player2) {
                ret = std::move(player2->get_moves_history());
            }
            break;
        default:
            break;
        }
        return ret;
    }

    const std::vector<Coordinate>
    get_undone_moves(PARTICIPANT who_) const {
        std::vector<Coordinate> ret;
        switch (who_) {
        case PARTICIPANT::PLAYER1:
            if (player1) {
                ret = std::move(player1->get_undone_moves());
            }
            break;
        case PARTICIPANT::PLAYER2:
            if (player2) {
                ret = std::move(player2->get_undone_moves());
            }
            break;
        default:
            break;
        }
        return ret;
    }
};

} // namespace Caro

#endif /* __SIMPLE_CARO_H__ */