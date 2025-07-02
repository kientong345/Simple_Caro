#ifndef __SIMPLE_CARO_H__
#define __SIMPLE_CARO_H__
#include <iostream>
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
    int64_t latitude;
    int64_t longtitude;
    // for std::set<Coordinate>
    bool operator<(const Coordinate& other) const {
        return (latitude < other.latitude) || 
            (latitude == other.latitude && longtitude < other.longtitude);
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

class Board {
private:
    std::shared_ptr<const std::vector<std::vector<TILE_STATE>>> board;

public:
    Board(
        std::shared_ptr<const std::vector<std::vector<TILE_STATE>>> board_
    ) : board(board_) {}

    size_t
    height() const {
        return board->size();
    }

    size_t
    width() const {
        if (board->size() == 0) {
            return 0;
        }
        return board->at(0).size();
    }

    std::vector<TILE_STATE>
    row(size_t latitude_) const {
		return std::vector<TILE_STATE>(board->at(latitude_));
	}

	std::vector<TILE_STATE>
    column(size_t longtitude_) const {
		std::vector<TILE_STATE> column_;
		column_.reserve(board->at(0).size());
		for (int i = 0; i < board->size(); ++i) {
			column_.push_back(board->at(i)[longtitude_]);
		}
		return column_;
	}

	TILE_STATE
    tile(size_t latitude_, size_t longtitude_) const {
		return board->at(latitude_)[longtitude_];
	}

};

inline bool is_valid_coordinate(
    const Board& board_,
    const Coordinate& coor_) {
    return  board_.height() > 0 &&
            board_.width() > 0 &&
            coor_.latitude >= 0 && coor_.latitude < board_.height() &&
            coor_.longtitude >= 0 && coor_.longtitude < board_.width();
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
        if ( ( move_.latitude < 0 ) || ( move_.longtitude < 0 ) ) {
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
    long occupied_tiles_counter;

public:
    Board_Context(uint32_t height, uint32_t width)
        : board(std::make_shared<std::vector<std::vector<TILE_STATE>>>(
            height, std::vector<TILE_STATE>(width, TILE_STATE::EMPTY))),
          occupied_tiles_counter(0) {}

    ~Board_Context() = default;

    Board
    get_board()
        const {
        return Board(board);
    }

    MOVE_RESULT
    set_tile(Coordinate pos_, TILE_STATE state) {
        MOVE_RESULT ret = MOVE_RESULT::SUCCESS;
        bool board_has_tiles = (board->size() > 0) &&
                               (board->at(0).size() > 0);
        bool pos_on_board = is_valid_coordinate(Board(board), pos_);
        if ( !pos_on_board ) {
            ret = MOVE_RESULT::OUT_OF_BOUNDS;
        } else if (board->at(pos_.latitude)[pos_.longtitude] != TILE_STATE::EMPTY) {
            ret = MOVE_RESULT::ALREADY_OCCUPIED;
        } else {
            (*board)[pos_.latitude][pos_.longtitude] = state;
            ++occupied_tiles_counter;
            ret = MOVE_RESULT::SUCCESS;
        }
        return ret;
    }

    MOVE_RESULT
    unset_tile(Coordinate pos_) {
        MOVE_RESULT ret = MOVE_RESULT::SUCCESS;
        bool board_has_tiles = (board->size() > 0) &&
                               (board->at(0).size() > 0);
        bool pos_on_board = is_valid_coordinate(Board(board), pos_);
        if ( !pos_on_board ) {
            ret = MOVE_RESULT::OUT_OF_BOUNDS;
        } else {
            (*board)[pos_.latitude][pos_.longtitude] = TILE_STATE::EMPTY;
            --occupied_tiles_counter;
            ret = MOVE_RESULT::SUCCESS;
        }
        return ret;
    }

    long
    occupied_tiles_count() {
        return occupied_tiles_counter;
    }

    void
    reset_context() {
        board->clear();
        occupied_tiles_counter = 0;
    }
};

class Check_Tiles_Sequence {
private:
    Board board;
    const unsigned int seq_count;

    bool
    is_winning_sequence (
        const Coordinate& coor_,
        const size_t dx_, const size_t dy_,
        bool& blocked_start_, bool& blocked_end_
    ) const {
        unsigned int move_counter_ = 0;
        const TILE_STATE tile_state_ = board.tile(coor_.latitude, coor_.longtitude);
        TILE_STATE opposite_tile_state_ = TILE_STATE::EMPTY;

        switch (tile_state_) {
        case TILE_STATE::PLAYER1:
            opposite_tile_state_ = TILE_STATE::PLAYER2;
            break;
        case TILE_STATE::PLAYER2:
            opposite_tile_state_ = TILE_STATE::PLAYER1;
            break;
        default:
            return false;
        }

        // negative moving
        Coordinate cur_coor_ = {
            coor_.latitude,
            coor_.longtitude,
        };
        while ( ( is_valid_coordinate(board, cur_coor_) ) &&
                ( board.tile(cur_coor_.latitude, cur_coor_.longtitude) == tile_state_) ) {
            cur_coor_.latitude -= dx_;
            cur_coor_.longtitude -= dy_;
            ++move_counter_;
        }
        blocked_start_ = (is_valid_coordinate(board, cur_coor_)) &&
            ((board.tile(cur_coor_.latitude, cur_coor_.longtitude) == opposite_tile_state_)) ?
            true : false;
        
        // positive moving
        cur_coor_.latitude = coor_.latitude;
        cur_coor_.longtitude = coor_.longtitude;
        while ( ( is_valid_coordinate(board, cur_coor_) ) &&
                ( board.tile(cur_coor_.latitude, cur_coor_.longtitude) == tile_state_) ) {
            cur_coor_.latitude += dx_;
            cur_coor_.longtitude += dy_;
            ++move_counter_;
        }
        blocked_end_ = (is_valid_coordinate(board, cur_coor_)) &&
            ((board.tile(cur_coor_.latitude, cur_coor_.longtitude) == opposite_tile_state_)) ?
            true : false;

        return (move_counter_-1 >= seq_count) ? true : false;
    }

public:
    Check_Tiles_Sequence(
        const Board& board_,
        unsigned int seq_count_)
    : board(std::move(board_)), seq_count(seq_count_) {}

    GAME_CHECK
    operator()(
        const Coordinate& coor_, unsigned char block_num_
    ) const {
        if (!is_valid_coordinate(board, coor_)) {
            return GAME_CHECK::RULE_NOT_FOUND;
        }
        if ( board.tile(coor_.latitude, coor_.longtitude) == TILE_STATE::EMPTY ) {
            return GAME_CHECK::ONGOING;
        }
        if ( block_num_ > 2 ) {
            return GAME_CHECK::RULE_NOT_FOUND;
        }

        const std::vector<std::pair<size_t, size_t>> direction_units_ = {
            {0, 1},     // unit of movement to the right
            {1, 0},     // unit of movement upward
            {1, 1},     // unit of movement upward in forward diagonal
            {1, -1},    // unit of movement upward in backward diagonal
        };

        bool blocked_start_ = false, blocked_end_ = false;
        for ( const auto& [dx_, dy_] : direction_units_ ) {
            bool winning_sequence_ = is_winning_sequence(coor_, dx_, dy_,
                                                        blocked_start_, blocked_end_);

            bool nonblocked_winning_ = winning_sequence_;
            bool blocked1_winning_ = winning_sequence_ &&
                                    !blocked_start_ &&
                                    !blocked_end_;
            bool blocked2_winning_ = winning_sequence_ &&
                                    !(blocked_start_ && blocked_end_);

            if ( ( ( block_num_ == 0 ) && ( nonblocked_winning_ ) ) ||
                ( ( block_num_ == 1 ) && ( blocked1_winning_ ) ) ||
                ( ( block_num_ == 2 ) && ( blocked2_winning_ ) ) ) {
                switch (board.tile(coor_.latitude, coor_.longtitude)) {
                case TILE_STATE::PLAYER1:
                    return GAME_CHECK::PLAYER1_WIN;
                case TILE_STATE::PLAYER2:
                    return GAME_CHECK::PLAYER2_WIN;
                case TILE_STATE::EMPTY:
                default:
                    break;
                }
            }
        }

        return GAME_CHECK::ONGOING;
    }

};

class Ruling {
public:
    virtual GAME_CHECK
    check_win(
        const Board& board_,
        const Coordinate& latest_move = {-1, -1}
    ) = 0;

    virtual GAME_CHECK
    check_draw(
        const Board& board_
    ) = 0;
};

class Tic_Tac_Toe_Rule : public Ruling {
public:
    GAME_CHECK
    check_win(
        const Board& board_,
        const Coordinate& latest_move_ = {-1, -1}
    ) override {
        if (is_valid_coordinate(board_, latest_move_)) {
            Check_Tiles_Sequence win_checker_(board_, 3);
            return win_checker_(latest_move_, 0);
        } else {
            for (int latitude_ = 0; latitude_ < board_.height(); ++latitude_) {
                for (int longtitude_ = 0; longtitude_ < board_.width(); ++longtitude_) {
                    Check_Tiles_Sequence tile_checker_(board_, 3);
                    GAME_CHECK result_ = tile_checker_({latitude_, longtitude_}, 0);
                    if (result_ != GAME_CHECK::ONGOING) {
                        return result_;
                    }
                }
            }
            return GAME_CHECK::ONGOING;
        }
    }

    GAME_CHECK
    check_draw(
        const Board& board_
    ) override {
        // Implement the logic for checking the draw condition
        return GAME_CHECK::ONGOING;
    }
};

class Four_Block_1_Rule : public Ruling {
public:
    GAME_CHECK
    check_win(
        const Board& board_,
        const Coordinate& latest_move_ = {-1, -1}
    ) override {
        if (is_valid_coordinate(board_, latest_move_)) {
            Check_Tiles_Sequence win_checker_(board_, 4);
            return win_checker_(latest_move_, 1);
        } else {
            for (int latitude_ = 0; latitude_ < board_.height(); ++latitude_) {
                for (int longtitude_ = 0; longtitude_ < board_.width(); ++longtitude_) {
                    Check_Tiles_Sequence tile_checker_(board_, 4);
                    GAME_CHECK result_ = tile_checker_({latitude_, longtitude_}, 1);
                    if (result_ != GAME_CHECK::ONGOING) {
                        return result_;
                    }
                }
            }
            return GAME_CHECK::ONGOING;
        }
    }

    GAME_CHECK
    check_draw(
        const Board& board_
    ) override {
        // Implement the logic for checking the draw condition
        return GAME_CHECK::ONGOING;
    }
};

class Five_Block_2_Rule : public Ruling {
public:
    GAME_CHECK
    check_win(
        const Board& board_,
        const Coordinate& latest_move_ = {-1, -1}
    ) override {
        if (is_valid_coordinate(board_, latest_move_)) {
            Check_Tiles_Sequence win_checker_(board_, 5);
            return win_checker_(latest_move_, 2);
        } else {
            for (int latitude_ = 0; latitude_ < board_.height(); ++latitude_) {
                for (int longtitude_ = 0; longtitude_ < board_.width(); ++longtitude_) {
                    Check_Tiles_Sequence tile_checker_(board_, 5);
                    GAME_CHECK result_ = tile_checker_({latitude_, longtitude_}, 2);
                    if (result_ != GAME_CHECK::ONGOING) {
                        return result_;
                    }
                }
            }
            return GAME_CHECK::ONGOING;
        }
    }

    GAME_CHECK
    check_draw(
        const Board& board_
    ) override {
        // Implement the logic for checking the draw condition
        return GAME_CHECK::ONGOING;
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
        const Board& board_,
        const Coordinate& latest_move_ = {-1, -1}
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
};

class Simple_Caro {
private:
    std::unique_ptr<Player_Context> player1;
    std::unique_ptr<Player_Context> player2;
    std::unique_ptr<Board_Context> board;
    std::unique_ptr<Game_Judge> judge;
    GAME_STATE state;
    Coordinate latest_player1_move;
    Coordinate latest_player2_move;

    void
    update_context() {
        GAME_CHECK is_end_ = GAME_CHECK::ONGOING;
        switch (state) {
        case GAME_STATE::PLAYER1_TURN:
            is_end_ = judge->check_end_condition(board->get_board(),
                                                latest_player1_move);
            break;
        case GAME_STATE::PLAYER2_TURN:
            is_end_ = judge->check_end_condition(board->get_board(),
                                                latest_player2_move);
            break;
        default:
            // brute force check all board
            is_end_ = judge->check_end_condition(board->get_board(),
                                                {-1, -1});
            break;
        }

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
          state(GAME_STATE::NOT_INPROGRESS),
          latest_player1_move{-1, -1},
          latest_player2_move{-1, -1} {}

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

    size_t
    get_board_width() {
        return board->get_board().width();
    }

    size_t
    get_board_height() {
        return board->get_board().height();
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
                    latest_player1_move = move_;
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
                    latest_player2_move = move_;
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
                    latest_player1_move = player1
                                        ->get_moves_history()
                                        .back();
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
                    latest_player2_move = player2
                                        ->get_moves_history()
                                        .back();
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
                    latest_player1_move = player1
                                        ->get_moves_history()
                                        .back();
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
                    latest_player2_move = player2
                                        ->get_moves_history()
                                        .back();
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

    long
    occupied_tiles_count() {
        return board->occupied_tiles_count();
    }

    Board
    get_board() const {
        return Board(board->get_board());
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