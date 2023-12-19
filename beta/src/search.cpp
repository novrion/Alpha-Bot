#include "search.h"

using namespace std;
using namespace std::chrono;


bool end_game = false;



void IterativeDeepening(Board& b, const bool kSide, const double kMaxTime, int& evaluation_out, U64& move_out, const bool kEndGame, std::unordered_map<U64, U64> hash) {

    end_game = kEndGame;

    pair<int, U64> search_data;

    for (int depth = 1; depth < 256; ++depth) {

        duration<double> time;
        time_point<high_resolution_clock> start_time = high_resolution_clock::now();

        search_data = LayerOneNegaMax(b, depth, kSide, kMaxTime, hash);

        time = high_resolution_clock::now() - start_time;


        if (!(search_data.second == kTimeFlag)) {

            evaluation_out = search_data.first;
            move_out = search_data.second;


            cout << depth << " ";

            cout << "[ ";

            cout << time.count();

            cout << " ]   ";

        }


        if (search_data.second == kTimeFlag || time.count() > 0.2 * kMaxTime) return;

        if (time.count() < kMaxTime / 100) ++depth;
    }
}

inline pair<int, U64> LayerOneNegaMax(Board& b, const int kDepth, const bool kSide, const double kMaxTime, std::unordered_map<U64, U64> hash) {

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

        int score = -NegaMax(b_copy, kDepth - 1, !kSide, -beta, -alpha, hash);

        if (score > alpha) {
            alpha = score;
            best_move = moves[i];
        }

        if ((time = high_resolution_clock::now() - start_time).count() > kMaxTime) return { 0, kTimeFlag };
    }

    return { alpha, best_move };
}

inline int NegaMax(Board& b, const int kDepth, const bool kSide, int alpha, int beta, std::unordered_map<U64, U64> hash) {
	
	// Transposition Table
	U64 key = Zobrist(b, kSide);
	if (hash.find(key) != hash.end() && GET_HASH_DEPTH(hash[key]) >= kDepth) {cout << "1"; return GET_HASH_EVAL(hash[key]);}

	if (!kDepth) return Quiescence(b, 4, kSide, alpha, beta);

    // Null Move Heuristic
    if (kDepth >= 3 && !end_game && !InCheck(b, kSide)) {

        Board b_copy = b;
        int eval = -NegaMax(b_copy, kDepth - 1 - kR, !kSide, -beta, -beta + 1, hash);
        if (eval >= beta) { 
		hash[key] = ENCODE_HASH(beta, kDepth);
		return beta;
	}
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

        int score = -NegaMax(b_copy, kDepth - 1, !kSide, -beta, -alpha, hash);

        if (score > alpha) {
            alpha = score;

            if (score >= beta) {
		hash[key] = ENCODE_HASH(beta, kDepth);
                return beta;
            }
        }
    }

	hash[key] = ENCODE_HASH(alpha, kDepth);
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
