# Simple_Caro

> A modern, extensible C++ library for Caro (Gomoku, Five in a Row) and its rule variants, designed for easy integration and clear OOP structure.

---

## ✨ Features

- **Flexible Board Management:** Easily configure board size and retrieve the current board state.
- **Rule Customization:** Switch between different rule sets (Tic Tac Toe, Four Block, Five Block, etc.) at runtime.
- **Turn-Based Logic:** Built-in management of player turns, move validation, and game state transitions.
- **Move History & Undo/Redo:** Track move history for each player and support undo/redo operations.
- **Clear API:** Intuitive and well-documented interface for all common game operations.

---

## 🚀 Installation

```bash
git clone <your_repo_url>
cd Simple_Caro
mkdir build
cd build
cmake ..
make
# Optional:
sudo make install
```

---

## 🛠 Usage
### 1. Create a new game
```cpp
    #include <simple_caro.hpp>
    Caro::Simple_Caro game;
```

### 2. Configure the game
Set board size and rule:
```cpp
    game.set_board_size(10, 10);
    game.set_rule(Caro::RULE_TYPE::FIVE_BLOCK_2);
```

### 3. Start the game
```cpp
    game.start(); // Default first turn is PLAYER1
    // Or specify which player goes first:
    game.start(Caro::GAME_STATE::PLAYER2_TURN);
```

*If you don't configure the game before calling start(), the game will start with default configuration.*

### 4. Make a move
```cpp
    game.player_move(Caro::PARTICIPANT::PLAYER1, {0, 0});
    game.player_move(Caro::PARTICIPANT::PLAYER2, {1, 1});
```

### 5. Undo/Redo
```cpp
    game.player_undo(Caro::PARTICIPANT::PLAYER1);
    game.player_redo(Caro::PARTICIPANT::PLAYER1);
```

### 6. Get board state and history
```cpp
    auto board = game.get_board();
    auto moves = game.get_moves_history(Caro::PARTICIPANT::PLAYER1);
    auto undones = game.get_undone_moves(Caro::PARTICIPANT::PLAYER1);
```

### 7. Check game state
```cpp
    if (game.is_over()) {
        auto state = game.get_state();
        // handle win/draw
    }
```

---

## 📖 API Overview

**Main class:** `Caro::Simple_Caro`

- `void set_board_size(int32_t width, int32_t height);`  
  Set the board size for the game.

- `size_t get_board_width();`  
  Get the width of the board.

- `size_t get_board_height();`  
  Get the height of the board.

- `void set_rule(RULE_TYPE rule);`  
  Set the game rule.  
  + `TIC_TAC_TOE`: Tic Tac Toe rule  
  + `FOUR_BLOCK_1`: Four Block 1 rule  
  + `FIVE_BLOCK_2`: Five Block 2 rule

- `void unset_rule();`  
  Unset the current game rule.

- `void start(GAME_STATE first_turn = GAME_STATE::PLAYER1_TURN);`  
  Start the game with the specified first turn.  
  + `PLAYER1_TURN`: Player 1 starts first  
  + `PLAYER2_TURN`: Player 2 starts first

- `void stop();`  
  Stop the game and reset all contexts.

- `MOVE_RESULT player_move(PARTICIPANT who, Coordinate move);`  
  Player makes a move.  
  + `SUCCESS`: Move was successful  
  + `ALREADY_OCCUPIED`: The tile is already occupied  
  + `WRONG_TURN`: It's not the player's turn  
  + `OUT_OF_BOUNDS`: The move is out of bounds

- `MOVE_RESULT player_undo(PARTICIPANT who);`  
  Player undoes the last move.  
  + `SUCCESS`: Undo was successful  
  + `WRONG_TURN`: It's not the player's turn

- `MOVE_RESULT player_redo(PARTICIPANT who);`  
  Player redoes the last undone move.  
  + `SUCCESS`: Redo was successful  
  + `WRONG_TURN`: It's not the player's turn

- `void switch_turn();`  
  Switch the turn to the next player.

- `long occupied_tiles_count() const;`  
  Get the count of occupied tiles on the board.

- `Board get_board() const;`  
  Get the current board state.

- `GAME_STATE get_state() const;`  
  Get the current game state.  
  + `PLAYER1_TURN`: Player 1's turn  
  + `PLAYER2_TURN`: Player 2's turn  
  + `PLAYER1_WON`: Player 1 has won  
  + `PLAYER2_WON`: Player 2 has won  
  + `DREW`: The game is a draw  
  + `NOT_INPROGRESS`: The game is not in progress

- `bool is_over() const;`  
  Check if the game is over.

- `const std::vector<Coordinate> get_moves_history(PARTICIPANT who) const;`  
  Get the moves history for a participant.

- `const std::vector<Coordinate> get_undone_moves(PARTICIPANT who) const;`  
  Get the undone moves for a participant.

> See the header file for full documentation and advanced usage.