#ifndef __DEFINES_HPP__
#define __DEFINES_HPP__

#include <cstdint>
#include <memory>
#include <vector>

namespace Caro {

/**
 * @brief Coordinate structure representing a position on the board.
 * @param latitude The row index (y-coordinate).
 * @param longitude The column index (x-coordinate).
 * @note The coordinate system is zero-indexed, meaning the top-left corner is (0, 0).
 */
struct Coordinate {
    int64_t latitude;
    int64_t longitude;
    bool operator<(const Coordinate& other) const {
        return (
            (latitude < other.latitude) || 
            ((latitude == other.latitude) && (longitude < other.longitude))
        );
    }
};

/**
 * @brief Tile state enumeration representing the state of a tile on the board.
 * @note The tile states are used to represent the players' moves and empty tiles.
 */
enum class TILE_STATE {
    EMPTY,
    PLAYER1,
    PLAYER2,
};

/**
 * @brief Game state enumeration representing the current state of the game.
 * @note The game state is used to determine the progress and outcome of the game.
 */
enum class MOVE_RESULT {
    SUCCESS,
    ALREADY_OCCUPIED,
    WRONG_TURN,
    OUT_OF_BOUNDS,
};

/**
 * @brief Game state enumeration representing the current state of the game.
 * @note The game state is used to determine the progress and outcome of the game.
 */
enum class GAME_STATE {
    PLAYER1_TURN,
    PLAYER2_TURN,
    PLAYER1_WON,
    PLAYER2_WON,
    DREW,
    NOT_INPROGRESS,
};

/**
 * @brief Rule type enumeration representing the rules applied to the game.
 * @note The rule type is used to determine the winning conditions and game mechanics.
 */
enum class RULE_TYPE {
    TIC_TAC_TOE,
    FOUR_BLOCK_1,
    FIVE_BLOCK_2,
};

/**
 * @brief Participant enumeration representing the players in the game.
 * @note The participant is used to identify which player is making a move or taking an action.
 */
enum class PARTICIPANT {
    PLAYER1,
    PLAYER2,
};

/**
 * @brief Game event enumeration representing the result of a game event check operation.
 * @note The game event is used to determine if a player has won, if the game is ongoing, or if it has ended in a draw.
 */
enum class GAME_EVENT {
    NO_EVENT,
    PLAYER1_FULL_SEQUENCED,
    PLAYER2_FULL_SEQUENCED,
    BOARD_FULL,
    NO_AVAILABLE_RULE,
};

/**
 * @brief Board class representing the game board.
 * @note The board is used to store the state of the game and perform operations on it.
 */
class Board {
private:
    std::shared_ptr<const std::vector<std::vector<TILE_STATE>>> mBoard;

public:
    Board(std::shared_ptr<const std::vector<std::vector<TILE_STATE>>> board)
    : mBoard(board) {}

    /**
     * @brief Get the height of the board.
     * @return The number of rows in the board.
     */
    uint32_t
    height() const {
        return mBoard->size();
    }

    /**
     * @brief Get the width of the board.
     * @return The number of columns in the board.
     */
    uint32_t
    width() const {
        if (mBoard->size() == 0) {
            return 0;
        }
        return mBoard->at(0).size();
    }

    /**
     * @brief Get a row from the board.
     * @param latitude The row index to retrieve.
     * @return A vector representing the specified row.
     */
    std::vector<TILE_STATE>
    row(uint32_t latitude) const {
		return std::vector<TILE_STATE>(mBoard->at(latitude));
	}

    /**
     * @brief Get all rows from the board.
     * @return A vector of vectors representing all rows.
     */
    std::vector<std::vector<TILE_STATE>>
    rows() const {
        std::vector<std::vector<TILE_STATE>> rows;
        rows.reserve(mBoard->size());
        for (const auto& row : *mBoard) {
            rows.push_back(row);
        }
        return rows;
    }

    /**
     * @brief Get a column from the board.
     * @param longitude The column index to retrieve.
     * @return A vector representing the specified column.
     */
	std::vector<TILE_STATE>
    column(uint32_t longitude) const {
		std::vector<TILE_STATE> column;
		column.reserve(mBoard->at(0).size());
		for (uint32_t i = 0; i < mBoard->size(); ++i) {
			column.push_back(mBoard->at(i)[longitude]);
		}
		return column;
	}

    /**
     * @brief Get all columns from the board.
     * @return A vector of vectors representing all columns.
     */
    std::vector<std::vector<TILE_STATE>>
    columns() const {
        std::vector<std::vector<TILE_STATE>> columns;
        columns.reserve(mBoard->at(0).size());
        for (uint32_t i = 0; i < mBoard->at(0).size(); ++i) {
            columns.push_back(column(i));
        }
        return columns;
    }

    /**
     * @brief Get the tile state at a specific coordinate.
     * @param latitude The row index (y-coordinate).
     * @param longitude The column index (x-coordinate).
     * @return The tile state at the specified coordinate.
     */
	TILE_STATE
    tile(uint32_t latitude, uint32_t longitude) const {
		return mBoard->at(latitude)[longitude];
	}

};

} // namespace Caro

#endif /* __DEFINES_HPP__ */