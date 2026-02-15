/**
 * author: datavorous
 * date: 15/2/2026
 * description: simple chess engine using negamax with alpha-beta pruning with a mailbox grid
 * references: 
 * [1] https://www.chessprogramming.org/Negamax
 * [2] https://www.chessprogramming.org/Alpha-Beta
 * [3] https://www.chessprogramming.org/Mailbox
 */

// same as abs() function
#define j(x) ((x) < 0 ? -(x) : (x))

// shortening for loop syntax
#define r(i, a, b) for (int i = (a); i < (b); i++)

/*
piece encoding:
0: empty cell
1: pawn
2: knight
3: bishop
4: rook
5: queen
6: king
7: off-board sentinel (mailbox border)

+ve: white; -ve: black
 */

// we use a purely material based evaluation
// hence the piece "priority" values are somewhat arbitrary 
// and only relative values matter
const int v[] = {0, 1, 3, 3, 5, 9, 99};

// these are the movement direction offsets for knights 
// and kings in the mailbox board representation

// for a knight, these are the 8 possible moves
// as we are representing our board as an 1D array
// we can use these offsets to quickly change the position of a piece
// for knight:
int N[] = {-21, -19, -12, -8, 8, 12, 19, 21};
// for king/queen:
// this is the fun part tbh; rooks will use the offsets in 0-3rd pos, 
// bishops will use the offsets in 4-7th pos, queens will use all 8
// so essentially K/Q are composite pieces that can move like multiple piece types
int K[] = {-1, 1, -10, 10, -11, -9, 9, 11};

// bs :: best move source square
// bd :: best move destination square
// b :: board representation (mailbox)
int bs, bd, b[120];

/*
the entire program flow can be summarise like this:

1. we initialize the board (recursive)
2. enter the main search function
    a. is there a leaf node? if so, evaluate and return
    b. if not, generate moves in two passes (captures first, then quiet moves)
    c. for each move, 
        i. make the move
        >> additionally determine if king is in check or not
        ii. if legal, then call the search function recursively to evaluate the resulting position
        iii. then unmake the move
    d. if the move's evaluation is better than the best score so far, update the best score
    e. if the best score is greater than or equal to the beta threshold, we can
            stop searching and return the best score (beta cutoff)
    f. if we finish searching all moves and there were no legal moves, then it's either checkmate or stalemate
        i. if the king is in check, it's checkmate, return a very low score
        ii. if the king is not in check, it's stalemate, return 0
    g. return the best score found
*/

void I() {
    /*
    1. iterate through all 120 cells
    2. calc row (i/10) and col (i%10)
    3. assign pieces based on their pos.

    the lookup string is used to assign the back rank pieces for both sides
    and the rest of the pieces are assigned based on their row number 
    (pawns on 2nd/7th rank, empty cells in the middle, and off-board sentinels on the borders)
    */
    r(i, 0, 120) {
        int row = i / 10;
        int col = i % 10;
        int back_rank_piece = "42356324"[col - 1] - '0';

        if (row < 2 || row > 9 || col < 1 || col > 8) {
            b[i] = 7;
        } else if (row == 3) {
            b[i] = 1;
        } else if (row == 8) {
            b[i] = -1;
        } else if (row == 2) {
            b[i] = back_rank_piece;
        } else if (row == 9) {
            b[i] = -back_rank_piece;
        } else {
            b[i] = 0;
        }
    }
}

// trust me, it exists
int S(int s, int d, int a, int be);

// figuring out whether kiing is in check of side s
int C(int s) {
    // s -> the side to check
    int king_sq = 0;
    int enemy = -s;

    // we find the king with this
    r(i, 21, 99) {
        if (b[i] == 6 * s) {
            king_sq = i;
            break;
        }
    }
    // duh
    if (!king_sq) return 0;

    // we check for enemy pawn attacks (diagonal forward for each respective side)
    if (s == 1) {
        if (b[king_sq + 9] == -1 || b[king_sq + 11] == -1) return 1;
    } else {
        if (b[king_sq - 9] == 1 || b[king_sq - 11] == 1) return 1;
    }

    // we check for enemy knight attacks
    r(i, 0, 8) if (b[king_sq + N[i]] == 2 * enemy) return 1;

    // we check for enemy king attacks
    r(i, 0, 8) if (b[king_sq + K[i]] == 6 * enemy) return 1;

    // we check for enemy rook/queen attacks (orthogonal rays)
    // i believe this part of the code can only be understood properly
    // by scribbling it out on a piece of paper and visualizing the board 
    // and the rays being cast out from the king's position
    r(i, 0, 4) {
        // we go in orthogonal directions first
        int t = king_sq;
        while (1) {
            t += K[i];
            // are we hitting a border?
            if (b[t] == 7) break;
            // oh its an empty square, keep going
            if (!b[t]) continue;
            // found enemy rook or queen attacking along this ray
            if ((b[t] > 0) == (enemy > 0) && (j(b[t]) == 4 || j(b[t]) == 5)) 
                return 1;
            // hit a piece, stop this ray
            break;
        }
    }

    // enemy bishop/queen attacks (this time diagonal rays)
    // same logic as above but for diagonal directions with 
    // different piece types
    r(i, 4, 8) {
        int t = king_sq;
        while (1) {
            t += K[i];
            if (b[t] == 7) break;
            if (!b[t]) continue;
            if ((b[t] > 0) == (enemy > 0) && (j(b[t]) == 3 || j(b[t]) == 5)) return 1;
            break;
        }
    }
    // essentially what we did here is simulate attacks FROM the king TO find enemy pieces, which is eqv to the
    // around way around
    return 0;
}

// we execute move and evalute here 
int E(int s, int d, int a, int be, int from, int to, int piece, int captured, int *has_legal_move) {
    // s: side to move 
    // d: depth remaining
    // a: alpha
    // be: beta
    // from: source square of the move
    // to: destination square of the move
    // piece: the piece being moved
    // captured: the piece being captured (if any)
    // has_legal_move: pointer to a flag that indicates if we have found at least one legal move in this position

    // we make the move
    b[to] = piece;
    b[from] = 0;

    // would leave king on check? if yes
    // undo, restore and skip
    if (C(s)) {
        b[from] = piece;
        b[to] = captured;
        return a;
    }

    // mark that, yes a legal move exists
    *has_legal_move = 1;

    // we recursively call the search function to evaluate the resulting position after making the move
    // we reduce thedepth if not at leaf
    // negate scores and swap alpha and beta (thats how negamax works)
    int score = -S(-s, d ? d - 1 : 0, -be, -a);

    // unmake the move
    b[from] = piece;
    b[to] = captured;

    // update the best move
    if (score > a) {
        a = score;
        if (d == 5) {
            bs = from;
            bd = to;
        }
    }

    // and check for beta cutoff
    return a >= be ? be : a;
    // if alpha is exceeds beta, this position is too good 
    // (opponent wont allow it), so prune this branch
}

int S(int s, int d, int a, int be) {
    // the heart of the engine, the search function
    // s: side to move
    // d: depth remaining
    // a: alpha
    // be: beta


    // we first eval leaf nodes
    int at_leaf = !d;
    int has_legal_move = 0;

    if (at_leaf) {
        int score = 0;
        r(i, 21, 99) {
            if (b[i] != 7) score += (b[i] > 0) ? v[b[i]] : -v[-b[i]];
        }
        score *= s;
        if (score > a) a = score;
        if (a >= be) return be;
    }
    // we sum material values for all pieces, 
    // if score is positive, white is at advantage, if negative, black is at advantage
    // we multiply by s to get the perspective of the side to move

    // we use a two pass move generation approach 
    // first we analyse captures (always executed)
    // quiet moves (skipped at leaf nodes)

    // something to be noted: at leaf nodes, we only captures to avoid HORIZON EFFECT
    // take a look into what "quiescence search" is all about
    r(pass, 0, at_leaf ? 1 : 2) {
        r(from, 21, 99) {
            int piece = b[from];
            if (piece == 7 || !piece || (piece > 0) != (s > 0)) continue;

            int type = j(piece);



            // we generate the pawn moves
            if (type == 1) {
                int fwd = (s == 1) ? 10 : -10;

                if (!pass) {
                    // the captures go here
                    for (int dx = -1; dx <= 1; dx += 2) {
                        int to = from + fwd + dx;
                        int captured = b[to];
                        if (captured && captured != 7 && (captured > 0) != (s > 0)) {
                            a = E(s, d, a, be, from, to, piece, captured, &has_legal_move);
                            if (a >= be) return be;
                        }
                    }
                } else if (!b[from + fwd]) {
                    // the quiet move goes here (only if the square in front is empty)
                    int to = from + fwd;
                    a = E(s, d, a, be, from, to, piece, 0, &has_legal_move);
                    if (a >= be) return be;

                    // two squares from starting position
                    if (((s == 1 && from < 40) || (s == -1 && from > 70)) && !b[from + 2 * fwd]) {
                        to = from + 2 * fwd;
                        a = E(s, d, a, be, from, to, piece, 0, &has_legal_move);
                        if (a >= be) return be;
                    }
                }
            } else {
                // we generate other piece' moves

                // this is the direction array
                int *dirs = K;
                int start_dir, end_dir;

                if (type == 2) {
                    // knight 
                    dirs = N;
                    start_dir = 0;
                    end_dir = 8;
                } else if (type == 4) {
                    // rook
                    start_dir = 0;
                    end_dir = 4;
                } else if (type == 3) {
                    // bishop
                    start_dir = 4;
                    end_dir = 8;
                } else {
                    // queen
                    start_dir = 0;
                    end_dir = 8;
                }

                // what we did above is direction selection
                // knights: use the N[] array with all L shaped moves
                // rooks K[0-3]: othogonal directions
                // bishops K[4-7]: diagonal directions
                // kings/queens K[0-7]: all 8 directions


                // here we define the sliding logic for pieces
                // rooks, bishops and queens slide until blockde
                // non sliders move once per direction
                // captures and quiet moves are generated in the same loop, 
                // differentiated by the "pass" variable
                r(i, start_dir, end_dir) {
                    int step = dirs[i];
                    int to = from;
                    int is_slider = (type != 2 && type != 6);

                    while (1) {
                        to += step;
                        int target = b[to];

                        if (target == 7) break;
                        if (target && (target > 0) == (s > 0)) break;

                        if (!pass) {
                            if (target) {
                                a = E(s, d, a, be, from, to, piece, target, &has_legal_move);
                                if (a >= be) return be;
                                break;
                            }
                        } else {
                            if (!target) {
                                a = E(s, d, a, be, from, to, piece, 0, &has_legal_move);
                                if (a >= be) return be;
                            } else {
                                break;
                            }
                        }

                        if (!is_slider) break;
                    }
                }
            }
        }
    }

    // we do the checkmate/stalemate detection here
    // checkmate: we return -9999 (a very low score) to indicate a losing position for the side to move
    // stalemate: we return 0 to indicate a draw
    if (!has_legal_move && !at_leaf) return C(s) ? -9999 : 0;
    return a;
}
