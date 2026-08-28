# Chess Engine (C)

A chess engine written in C, implementing board representation and legal move validation from scratch — no external chess libraries.

## Status: Work in Progress

This is research/hobby code, not production-hardened. Docstrings and full test coverage are still to come.

### Working

- Full 8x8 board representation and initialization
- Legal move validation for every piece type (pawn, knight, bishop, rook, queen, king)
- Pawn promotion (to queen, rook, bishop, or knight)
- Castling, both kingside and queenside, with proper tracking of whether the king or rooks have already moved
- Same-color-piece and path-obstruction checks (a piece can't move through or onto its own pieces)
- Terminal-based board display with alternating square coloring

### Not yet implemented

- **En passant** — not handled
- **Check detection** — `is_in_check()` is currently stubbed to always return `0`
- **Checkmate / stalemate detection** — depends on check detection, so also not implemented
- No move history, undo, or game-over conditions yet

## Why

This is a step toward a longer-term goal: a physical "smart chessboard" that can track and validate moves in real time. The engine here is the move-validation core that a hardware/sensor layer would eventually sit on top of.

## Running it

```
gcc chess.c -o chess
./chess
```

Moves are entered as `from`/`to` square pairs (e.g. `e2` → `e4`) via terminal prompts.

## Roadmap

- [ ] En passant
- [ ] Check / checkmate / stalemate detection
- [ ] Move history and undo
- [ ] Basic engine evaluation (eventually)
- [ ] Hardware integration for the smart chessboard concept
