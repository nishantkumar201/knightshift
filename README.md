# KnightShift

A chess move-validation engine written in C, built from scratch without external chess libraries.

KnightShift explores chess rules, board representation, move validation, and game-state management.

The long-term goal is to use this software as the foundation for a **physical smart chessboard** capable of detecting, tracking, and validating moves in real time.

## Status: Work in Progress

The project currently supports standard piece movement validation and king-safety checking. Special moves, endgame detection, and hardware integration are still under development.

### Implemented

- 8×8 chessboard representation and initialization
- Terminal-based board display with alternating square colors
- Standard movement rules for all six piece types
- Pawn movement and standard captures
- Path-obstruction detection for sliding pieces
- Turn-based move validation
- Prevention of capturing friendly pieces
- Rejection of moves that leave the moving player's king in check
- Check detection using piece attack patterns
- Game-state simulation for move validation
- Interactive gameplay supporting up to 10 valid moves per session

### In Progress

- Preventing direct king captures
- Displaying check notifications during gameplay

### Not Yet Implemented

- Checkmate and stalemate detection
- Kingside and queenside castling
- Pawn promotion
- En passant
- Move history and undo
- Complete game-over handling
- Computer opponent / move evaluation
- Physical chessboard integration

## Running KnightShift

Compile using GCC:

```bash
gcc -Wall -Wextra -std=c11 chess.c -o chess
./chess
```

Moves are entered using algebraic square coordinates, such as `e2` followed by `e4`.

The current interface is terminal-based and supports **10 valid moves per session**. Invalid moves can be retried without counting toward this limit.

## Project Roadmap

- [ ] Implement castling
- [ ] Implement pawn promotion
- [ ] Implement en passant
- [ ] Implement checkmate and stalemate detection
- [ ] Expand automated tests
- [ ] Add move history and undo functionality
- [ ] Explore basic computer move evaluation
- [ ] Investigate hardware and sensor integration for a physical smart chessboard

## Long-Term Vision

KnightShift is intended to become the software foundation of a physical smart chessboard.

The proposed system would combine a chess move-validation engine with hardware capable of identifying piece movements and maintaining an accurate representation of the board.

Future development may explore sensor-based move detection, synchronization between physical and digital board states, and real-time feedback for invalid moves.
