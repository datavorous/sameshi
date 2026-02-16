# sameshi

![full_video](https://github.com/user-attachments/assets/6c7b4f16-c16d-488c-b068-7fa48a61c655)


A minimal chess engine supporting a constrained subset of the game.

`sameshi.h`: 1.95 KB

The readable version of the code can be found [here](readable/sameshi.h).

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
