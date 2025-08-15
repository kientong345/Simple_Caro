#ifndef __SIMPLE_CARO_HPP__
#define __SIMPLE_CARO_HPP__

#include "defines.hpp"
#include "player_context.hpp"
#include "board_context.hpp"
#include "game_judge.hpp"

namespace Caro {

class Simple_Caro {
private:
    std::unique_ptr<Player_Context> mPlayer1;
    std::unique_ptr<Player_Context> mPlayer2;
    std::unique_ptr<Board_Context> mBoard;
    std::unique_ptr<Game_Judge> mJudge;
    GAME_STATE mState;
    Coordinate mLatest_player1_move;
    Coordinate mLatest_player2_move;

    void
    update_context();

public:
    Simple_Caro();

    ~Simple_Caro() = default;

    /**
     * @brief Set the board size for the game.
     * @param width Width of the board.
     * @param height Height of the board.
     * @return none
     */
    void
    set_board_size(int32_t width, int32_t height);

    /**
     * @brief Get the width of the board.
     * @return Width of the board.
     */
    size_t
    get_board_width();

    /**
     * @brief Get the height of the board.
     * @return Height of the board.
     */
    size_t
    get_board_height();

    /**
     * @brief Set the game rule.
     * @param rule The rule type to set.
     *        + TIC_TAC_TOE: Tic Tac Toe rule.
     *        + FOUR_BLOCK_1: Four Block 1 rule.
     *        + FIVE_BLOCK_2: Five Block 2 rule.
     * @return none
     */
    void
    set_rule(RULE_TYPE rule);

    /**
     * @brief Unset the game rule.
     * @return void
     */
    void
    unset_rule();

    /**
     * @brief Start the game with the specified first turn.
     * @param first_turn The first turn to start the game.
     *        + PLAYER1_TURN: Player 1 starts first.
     *        + PLAYER2_TURN: Player 2 starts first.
     * @return none
     */
    void
    start(GAME_STATE first_turn = GAME_STATE::PLAYER1_TURN);

    /**
     * @brief Stop the game and reset all contexts.
     * @return none
     */
    void
    stop();

    /**
     * @brief Player makes a move.
     * @param who The participant making the move.
     *        + PLAYER1: Player 1.
     *        + PLAYER2: Player 2.
     * @param move The coordinate of the move.
     * @return Result of the move operation.
     *        + SUCCESS: Move was successful.
     *        + ALREADY_OCCUPIED: The tile is already occupied.
     *        + WRONG_TURN: It's not the player's turn.
     *        + OUT_OF_BOUNDS: The move is out of bounds.
     */
    MOVE_RESULT
    player_move(PARTICIPANT who, Coordinate move);

    /**
     * @brief Player undoes the last move.
     * @param who The participant undoing the move.
     *        + PLAYER1: Player 1.
     *        + PLAYER2: Player 2.
     * @return Result of the undo operation.
     *        + SUCCESS: Undo was successful.
     *        + WRONG_TURN: It's not the player's turn.
     */
    MOVE_RESULT
    player_undo(PARTICIPANT who);

    /**
     * @brief Player redoes the last undone move.
     * @param who The participant redoing the move.
     *        + PLAYER1: Player 1.
     *        + PLAYER2: Player 2.
     * @return Result of the redo operation.
     *        + SUCCESS: Redo was successful.
     *        + WRONG_TURN: It's not the player's turn.
     */
    MOVE_RESULT
    player_redo(PARTICIPANT who);

    /**
     * @brief Switch the turn to the next player.
     * @return none
     */
    void
    switch_turn();

    /**
     * @brief Get the count of occupied tiles on the board.
     * @return Count of occupied tiles.
     */
    long
    occupied_tiles_count() const;

    /**
     * @brief Get the current board state.
     * @return Current board state.
     */
    Board
    get_board() const;

    /**
     * @brief Get the current game state.
     * @return Current game state.
     *        + PLAYER1_TURN: Player 1's turn.
     *        + PLAYER2_TURN: Player 2's turn.
     *        + PLAYER1_WON: Player 1 has won.
     *        + PLAYER2_WON: Player 2 has won.
     *        + DREW: The game is a draw.
     *        + NOT_INPROGRESS: The game is not in progress.
     */
    GAME_STATE
    get_state() const;

    /**
     * @brief Check if the game is over.
     * @return true if the game is over, false otherwise.
     */
    bool
    is_over() const;

    /**
     * @brief Get the moves history for a participant.
     * @param who The participant whose moves history to retrieve.
     *        + PLAYER1: Player 1.
     *        + PLAYER2: Player 2.
     * @return A vector of coordinates representing the moves history.
     */
    const std::vector<Coordinate>
    get_moves_history(PARTICIPANT who) const;

    /**
     * @brief Get the undone moves for a participant.
     * @param who The participant whose undone moves to retrieve.
     *        + PLAYER1: Player 1.
     *        + PLAYER2: Player 2.
     * @return A vector of coordinates representing the undone moves.
     */
    const std::vector<Coordinate>
    get_undone_moves(PARTICIPANT who) const;
};

} // namespace Caro

#endif /* __SIMPLE_CARO_HPP__ */