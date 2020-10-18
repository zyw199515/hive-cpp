
#include <vector>

#include "backend/state.h"

namespace hive {

// Represent one move.
struct Move {
    Pos from;
    Pos to;
    bool immobile_next_turn = false; // for pillbug, most likely false.
};

// Given a State, and a piece by position,
// return the position that the specified piece can move to.
// Pillbug move is considered separately, as it would move other piece.
std::vector<Move> GetMovePositions(const HiveState& state, const Pos& from);

// Moves for different pieces.
std::vector<Move> GetQueenMovePositions(const HiveState& state, const Pos& from);
std::vector<Move> GetBeetleMovePositions(const HiveState& state, const Pos& from);
std::vector<Move> GetGrasshopperMovePositions(HiveState& state, const Pos& from);
std::vector<Move> GetSpiderMovePositions(HiveState& state, const Pos& from);
std::vector<Move> GetAntMovePositions(HiveState& state, const Pos& from);
std::vector<Move> GetLadybugMovePositions(HiveState& state, const Pos& from);
std::vector<Move> GetMosquitoMovePositions(HiveState& state, const Pos& from);
std::vector<Move> GetPillbugMovePositions(HiveState& state, const Pos& from);

// === Helpers ===

// For mosquito to share implementation
GetMoveByType(const HiveState& state, const Pos& from, PieceType piece_type);

// One hive rule part 1: hive connectivity.
// Check if hive is still connected after "from" is removed.
bool IsHiveStillConnected(const HiveState& state, const Pos& from);

// Check if the one step move for a piece is valid, including:
// 1. freedom to move rule
// 2. One hive rule Part 2: piece-hive connectivity (only valid on z=0)
// To summarize, for the direction of moving
// "to" should be an empty ajacent position of from.
bool IsPieceMoveValid(const HiveState& state, const Pos& from, const Pos& to);

}



} // namespace hive