#include "search.h"

using namespace std;
using namespace std::chrono;


bool end_game = false;


inline bool InCheck(Board& b, const bool kSide) {

    const U64 kWBlock = W_BLOCK(b.bb);
    const U64 kBBlock = B_BLOCK(b.bb);

    const U64 kWBishopQueenBlock = b.bb[3] | b.bb[5];
    const U64 kWRookQueenBlock = b.bb[4] | b.bb[5];

    const U64 kBBishopQueenBlock = b.bb[9] | b.bb[11];
    const U64 kBRookQueenBlock = b.bb[10] | b.bb[11];


    if (kSide) {

        int square = __builtin_ctzll(b.bb[6]);

        const int t_rank = square / 8;
        const int t_file = square % 8;

        // Diagonal
        for (int x = t_rank + 1, y = t_file + 1; x < 8 && y < 8; ++x, ++y) {

            if (1ULL << (x * 8 + y) & kWBlock) break;
            else if (1ULL << (x * 8 + y) & kBBishopQueenBlock) return true;
            else if (1ULL << (x * 8 + y) & kBBlock) break;
        }

        for (int x = t_rank - 1, y = t_file + 1; x > -1 && y < 8; x--, ++y) {

            if (1ULL << (x * 8 + y) & kWBlock) break;
            else if (1ULL << (x * 8 + y) & kBBishopQueenBlock) return true;
            else if (1ULL << (x * 8 + y) & kBBlock) break;
        }

        for (int x = t_rank + 1, y = t_file - 1; x < 8 && y > -1; ++x, y--) {

            if (1ULL << (x * 8 + y) & kWBlock) break;
            else if (1ULL << (x * 8 + y) & kBBishopQueenBlock) return true;
            else if (1ULL << (x * 8 + y) & kBBlock) break;
        }

        for (int x = t_rank - 1, y = t_file - 1; x > -1 && y > -1; x--, y--) {

            if (1ULL << (x * 8 + y) & kWBlock) break;
            else if (1ULL << (x * 8 + y) & kBBishopQueenBlock) return true;
            else if (1ULL << (x * 8 + y) & kBBlock) break;
        }


        // Straight
        for (int y = t_rank + 1; y < 8; ++y) {

            if (1ULL << (y * 8 + t_file) & kWBlock) break;
            else if (1ULL << (y * 8 + t_file) & kBRookQueenBlock) return true;
            else if (1ULL << (y * 8 + t_file) & kBBlock) break;
        }

        for (int y = t_rank - 1; y > -1; y--) {

            if (1ULL << (y * 8 + t_file) & kWBlock) break;
            else if (1ULL << (y * 8 + t_file) & kBRookQueenBlock) return true;
            else if (1ULL << (y * 8 + t_file) & kBBlock) break;
        }

        for (int x = t_file + 1; x < 8; ++x) {

            if (1ULL << (t_rank * 8 + x) & kWBlock) break;
            else if (1ULL << (t_rank * 8 + x) & kBRookQueenBlock) return true;
            else if (1ULL << (t_rank * 8 + x) & kBBlock) break;
        }

        for (int x = t_file - 1; x > -1; x--) {

            if (1ULL << (t_rank * 8 + x) & kWBlock) break;
            else if (1ULL << (t_rank * 8 + x) & kBRookQueenBlock) return true;
            else if (1ULL << (t_rank * 8 + x) & kBBlock) break;
        }
    }

    else {

        int square = __builtin_ctzll(b.bb[12]);

        const int t_rank = square / 8;
        const int t_file = square % 8;

        // Diagonal
        for (int x = t_rank + 1, y = t_file + 1; x < 8 && y < 8; ++x, ++y) {

            if (1ULL << (x * 8 + y) & kBBlock) break;
            else if (1ULL << (x * 8 + y) & kWBishopQueenBlock) return true;
            else if (1ULL << (x * 8 + y) & kWBlock) break;
        }

        for (int x = t_rank - 1, y = t_file + 1; x > -1 && y < 8; x--, ++y) {

            if (1ULL << (x * 8 + y) & kBBlock) break;
            else if (1ULL << (x * 8 + y) & kWBishopQueenBlock) return true;
            else if (1ULL << (x * 8 + y) & kWBlock) break;
        }

        for (int x = t_rank + 1, y = t_file - 1; x < 8 && y > -1; ++x, y--) {

            if (1ULL << (x * 8 + y) & kBBlock) break;
            else if (1ULL << (x * 8 + y) & kWBishopQueenBlock) return true;
            else if (1ULL << (x * 8 + y) & kWBlock) break;
        }

        for (int x = t_rank - 1, y = t_file - 1; x > -1 && y > -1; x--, y--) {

            if (1ULL << (x * 8 + y) & kBBlock) break;
            else if (1ULL << (x * 8 + y) & kWBishopQueenBlock) return true;
            else if (1ULL << (x * 8 + y) & kWBlock) break;
        }


        // Straight
        for (int y = t_rank + 1; y < 8; ++y) {

            if (1ULL << (y * 8 + t_file) & kBBlock) break;
            else if (1ULL << (y * 8 + t_file) & kWRookQueenBlock) return true;
            else if (1ULL << (y * 8 + t_file) & kWBlock) break;
        }

        for (int y = t_rank - 1; y > -1; y--) {

            if (1ULL << (y * 8 + t_file) & kBBlock) break;
            else if (1ULL << (y * 8 + t_file) & kWRookQueenBlock) return true;
            else if (1ULL << (y * 8 + t_file) & kWBlock) break;
        }

        for (int x = t_file + 1; x < 8; ++x) {

            if (1ULL << (t_rank * 8 + x) & kBBlock) break;
            else if (1ULL << (t_rank * 8 + x) & kWRookQueenBlock) return true;
            else if (1ULL << (t_rank * 8 + x) & kWBlock) break;
        }

        for (int x = t_file - 1; x > -1; x--) {

            if (1ULL << (t_rank * 8 + x) & kBBlock) break;
            else if (1ULL << (t_rank * 8 + x) & kWRookQueenBlock) return true;
            else if (1ULL << (t_rank * 8 + x) & kWBlock) break;
        }
    }

    return false;
}

inline void GetMoveTargets(Board& b, U64 moves[100], const bool kSide) {

    if (kSide) {

        for (int i = 0; i < moves[99]; ++i) {
            
            if (!GET_MOVE_CAPTURE(moves[i])) continue;
            int target = GET_MOVE_TARGET(moves[i]);

            for (int j = 7; j < 13; ++j) {

                if (b.bb[j] & (1ULL << target)) {

                    RESET_MOVE_CAPTURE(moves[i]);
                    SET_MOVE_CAPTURE(moves[i], j);

                    break;
                }
            }
        }
    }

    else {

        for (int i = 0; i < moves[99]; ++i) {
            
            if (!GET_MOVE_CAPTURE(moves[i])) continue;
            int target = GET_MOVE_TARGET(moves[i]);

            for (int j = 1; j < 7; ++j) {

                if (b.bb[j] & (1ULL << target)) {

                    RESET_MOVE_CAPTURE(moves[i]);
                    SET_MOVE_CAPTURE(moves[i], j);

                    break;
                }
            }
        }
    }
}





void IterativeDeepening(Board& b, const bool kSide, const double kMaxTime, int& evaluation_out, U64& move_out, const bool kEndGame) {

    end_game = kEndGame;

    pair<int, U64> search_data;

    for (int depth = 1; depth < 256; ++depth) {

        duration<double> time;
        time_point<high_resolution_clock> start_time = high_resolution_clock::now();

        search_data = LayerOneNegaMax(b, depth, kSide, kMaxTime);

        time = high_resolution_clock::now() - start_time;


        if (!(search_data.second == kTimeFlag)) {

            evaluation_out = search_data.first;
            move_out = search_data.second;


            cout << depth << " ";

            SetConsoleTextAttribute(hConsole, 14);
            cout << "[ ";

            SetConsoleTextAttribute(hConsole, 12);
            cout << time.count();

            SetConsoleTextAttribute(hConsole, 14);
            cout << " ]   ";

            SetConsoleTextAttribute(hConsole, 15);
        }


        if (search_data.second == kTimeFlag || time.count() > 0.2 * kMaxTime) return;

        if (time.count() < kMaxTime / 100) ++depth;
    }
}

inline pair<int, U64> LayerOneNegaMax(Board& b, const int kDepth, const bool kSide, const double kMaxTime) {

    duration<double> time;
    time_point<high_resolution_clock> start_time = high_resolution_clock::now();

    int alpha = -999999;
    int beta = 999999;
    U64 best_move;
    U64 moves[100];
    moves[99] = 0;

    GenerateMoves(b.bb, moves, kSide);
    GetMoveTargets(b, moves, kSide);
	sort(moves, moves + moves[99], [](const U64 c, const U64 d) {return kMVVLVA[GET_MOVE_PIECE(c)][GET_MOVE_CAPTURE(c)] > kMVVLVA[GET_MOVE_PIECE(d)][GET_MOVE_CAPTURE(d)]; });


    Board b_copy;

    for (int i = 0; i < moves[99]; ++i) {

        if ((1ULL << GET_MOVE_TARGET(moves[i])) & b.bb[6] || (1ULL << GET_MOVE_TARGET(moves[i])) & b.bb[12]) return { 100000 + kDepth, moves[i] };

        b_copy = b;
        MakeMove(b_copy.bb, moves[i], kSide);

        int score = -NegaMax(b_copy, kDepth - 1, !kSide, -beta, -alpha);

        if (score > alpha) {
            alpha = score;
            best_move = moves[i];
        }

        if ((time = high_resolution_clock::now() - start_time).count() > kMaxTime) return { 0, kTimeFlag };
    }

    return { alpha, best_move };
}

inline int NegaMax(Board& b, const int kDepth, const bool kSide, int alpha, int beta) {

    if (!kDepth) return Quiescence(b, 4, kSide, alpha, beta);

    // Null Move Heuristic
    if (kDepth >= 3 && !end_game && !InCheck(b, kSide)) {

        Board b_copy = b;
        int eval = -NegaMax(b_copy, kDepth - 1 - kR, !kSide, -beta, -beta + 1);
        if (eval >= beta) return beta;
    }

    U64 moves[100];
    moves[99] = 0;

    GenerateMoves(b.bb, moves, kSide);
    GetMoveTargets(b, moves, kSide);
	sort(moves, moves + moves[99], [](const U64 c, const U64 d) {return kMVVLVA[GET_MOVE_PIECE(c)][GET_MOVE_CAPTURE(c)] > kMVVLVA[GET_MOVE_PIECE(d)][GET_MOVE_CAPTURE(d)]; });


    Board b_copy;

    for (int i = 0; i < moves[99]; ++i) {

        if ((1ULL << GET_MOVE_TARGET(moves[i])) & b.bb[6] || (1ULL << GET_MOVE_TARGET(moves[i])) & b.bb[12]) return (100000 + kDepth);

        b_copy = b;
        MakeMove(b_copy.bb, moves[i], kSide);

        int score = -NegaMax(b_copy, kDepth - 1, !kSide, -beta, -alpha);

        if (score > alpha) {
            alpha = score;

            if (score >= beta) {
                return beta;
            }
        }
    }

    return alpha;
}

inline int Quiescence(Board& b, const int kDepth, const bool kSide, int alpha, int beta) {

    if (kDepth == 0) return kSide ? Evaluate(b.bb) : -Evaluate(b.bb);

    int eval = kSide ? Evaluate(b.bb) : -Evaluate(b.bb);
    if (eval >= beta) return beta;
    if (eval > alpha) alpha = eval;

    U64 moves[100];
    moves[99] = 0;

    GenerateCaptures(b.bb, moves, kSide);
    GetMoveTargets(b, moves, kSide);
	sort(moves, moves + moves[99], [](const U64 c, const U64 d) {return kMVVLVA[GET_MOVE_PIECE(c)][GET_MOVE_CAPTURE(c)] > kMVVLVA[GET_MOVE_PIECE(d)][GET_MOVE_CAPTURE(d)]; });


    Board b_copy;

    for (int i = 0; i < moves[99]; ++i) {

        if ((1ULL << GET_MOVE_TARGET(moves[i])) & b.bb[6] || (1ULL << GET_MOVE_TARGET(moves[i])) & b.bb[12]) return (100000 + kDepth);

        b_copy = b;
        MakeMove(b_copy.bb, moves[i], kSide);

        int score = -Quiescence(b_copy, kDepth - 1, !kSide, -beta, -alpha);

        if (score > alpha) {
            alpha = score;

            if (score >= beta) {
                return beta;
            }
        }
    }

    return alpha;
}