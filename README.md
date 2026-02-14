# sameshi

A minimal chess engine supporting a constrained subset of the game.

`sameshi.h`: 2.06 KB

## core

1. 120 cell mailbox board
2. negamax search
3. alpha beta pruning
4. material only eval
5. capture first move ordering
6. full legal move validation (check / mate / stalemate)

> [!NOTE]
> not implemented: castling, en passant, promotion, repetition, 50-move rule.

## strength

**~1170 Elo** (95% CI: 1110-1225)  
240 games vs stockfish (1320-1600 levels)  
fixed depth 5, constrained rules, max 60 plies.  