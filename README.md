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
  Set the board size (width x height) for the game.

- `size_t get_board_width();`  
  Get the current board width.

- `size_t get_board_height();`  
  Get the current board height.

- `void set_rule(RULE_TYPE rule);`  
  Set the rule for the game (Tic Tac Toe, Four Block, Five Block, etc).

- `void unset_rule();`  
  Remove the current rule.

- `void start(GAME_STATE first_turn = GAME_STATE::PLAYER1_TURN);`  
  Start the game, optionally specifying which player goes first.

- `void stop();`  
  Stop the game and reset all contexts.

- `MOVE_RESULT player_move(PARTICIPANT who, Coordinate move);`  
  Make a move for the specified player at the given coordinate.

- `MOVE_RESULT player_undo(PARTICIPANT who);`  
  Undo the last move for the specified player.

- `MOVE_RESULT player_redo(PARTICIPANT who);`  
  Redo the last undone move for the specified player.

- `void switch_turn();`  
  Switch the turn to the next player.

- `long occupied_tiles_count() const;`  
  Get the number of occupied tiles on the board.

- `Board get_board() const;`  
  Get the current board state.

- `GAME_STATE get_state() const;`  
  Get the current game state (whose turn, win, draw, etc).

- `bool is_over() const;`  
  Check if the game is over.

- `const std::vector<Coordinate> get_moves_history(PARTICIPANT who) const;`  
  Get the move history for the specified player.

- `const std::vector<Coordinate> get_undone_moves(PARTICIPANT who) const;`  
  Get the undone moves for the specified player.
  

See the header file for full documentation and advanced usage.