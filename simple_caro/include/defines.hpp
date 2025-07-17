#ifndef __DEFINES_HPP__
#define __DEFINES_HPP__

#include <cstdint>
#include <memory>
#include <vector>

namespace Caro {

struct Coordinate {
    int64_t latitude;
    int64_t longtitude;
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

class Board {
private:
    std::shared_ptr<const std::vector<std::vector<TILE_STATE>>> board;

public:
    Board(
        std::shared_ptr<const std::vector<std::vector<TILE_STATE>>> board_
    ) : board(board_) {}

    uint32_t
    height() const {
        return board->size();
    }

    uint32_t
    width() const {
        if (board->size() == 0) {
            return 0;
        }
        return board->at(0).size();
    }

    std::vector<TILE_STATE>
    row(uint32_t latitude_) const {
		return std::vector<TILE_STATE>(board->at(latitude_));
	}

	std::vector<TILE_STATE>
    column(uint32_t longtitude_) const {
		std::vector<TILE_STATE> column_;
		column_.reserve(board->at(0).size());
		for (uint32_t i = 0; i < board->size(); ++i) {
			column_.push_back(board->at(i)[longtitude_]);
		}
		return column_;
	}

	TILE_STATE
    tile(uint32_t latitude_, uint32_t longtitude_) const {
		return board->at(latitude_)[longtitude_];
	}

};

} // namespace Caro

#endif /* __DEFINES_HPP__ */