#ifndef __DEFINES_HPP__
#define __DEFINES_HPP__

#include <cstdint>
#include <memory>
#include <vector>

namespace Caro {

struct Coordinate {
    int64_t latitude;
    int64_t longitude;
    bool operator<(const Coordinate& other) const {
        return (latitude < other.latitude) || 
            (latitude == other.latitude && longitude < other.longitude);
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
    std::shared_ptr<const std::vector<std::vector<TILE_STATE>>> mBoard;

public:
    Board(std::shared_ptr<const std::vector<std::vector<TILE_STATE>>> board)
    : mBoard(board) {}

    uint32_t
    height() const {
        return mBoard->size();
    }

    uint32_t
    width() const {
        if (mBoard->size() == 0) {
            return 0;
        }
        return mBoard->at(0).size();
    }

    std::vector<TILE_STATE>
    row(uint32_t latitude) const {
		return std::vector<TILE_STATE>(mBoard->at(latitude));
	}

	std::vector<TILE_STATE>
    column(uint32_t longitude) const {
		std::vector<TILE_STATE> column;
		column.reserve(mBoard->at(0).size());
		for (uint32_t i = 0; i < mBoard->size(); ++i) {
			column.push_back(mBoard->at(i)[longitude]);
		}
		return column;
	}

	TILE_STATE
    tile(uint32_t latitude, uint32_t longitude) const {
		return mBoard->at(latitude)[longitude];
	}

};

} // namespace Caro

#endif /* __DEFINES_HPP__ */