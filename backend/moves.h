
#include <iostream>
#include <vector>

#include "backend/state.h"

namespace hive {

// Represent one move.
struct Move {
  Pos from;
  Pos to;
  bool immobile_next_turn = false;  // for pillbug, most likely false.

  bool operator==(const Move& other) const;
  bool operator!=(const Move& other) const { return !operator==(other); }

  Move(Pos from_, Pos to_, bool immobile_next_turn_ = false)
      : from(from_), to(to_), immobile_next_turn(immobile_next_turn_) {}
  std::string DebugString() const {
    std::string str = "Move";
    if (immobile_next_turn) {
      str += "(I)";
    }
    str += "[" + from.DebugString() + "->" + to.DebugString() + "]";
    return str;
  }

  friend std::ostream& operator<<(std::ostream& strm, Move& m) {
    strm << m.DebugString();
    return strm;
  };
};

// Given a State, and a piece by position, return the moves available.
// For performance consideration, state could be mutated for multiple-step movements,
// the function must guarantee that state is unchanged.
std::vector<Move> GetMovePositions(HiveState& state, const Pos& from);

// Moves for different pieces.
std::vector<Move> GetQueenMovePositions(const HiveState& state, const Pos& from);
std::vector<Move> GetBeetleMovePositions(const HiveState& state, const Pos& from);
std::vector<Move> GetGrasshopperMovePositions(const HiveState& state, const Pos& from);
std::vector<Move> GetSpiderMovePositions(HiveState& state, const Pos& from);
std::vector<Move> GetAntMovePositions(HiveState& state, const Pos& from);
std::vector<Move> GetLadybugMovePositions(HiveState& state, const Pos& from);
std::vector<Move> GetMosquitoMovePositions(HiveState& state, const Pos& from);
std::vector<Move> GetPillbugMovePositions(const HiveState& state, const Pos& from);

// === Helpers ===

// For mosquito to share implementation
std::vector<Move> GetMoveByType(HiveState& state, const Pos& from, PieceType piece_type);

// One hive rule part 1: hive connectivity.
// Check if hive is still connected after "from" is removed.
bool IsHiveStillConnected(const HiveState& state, const Pos& from);

// Check if the one step move for a piece is valid, including:
// 1. freedom to move rule
// 2. One hive rule Part 2: piece-hive connectivity (only valid on z=0)
// To summarize, for the direction of moving
// "to" should be an empty ajacent position of from.
bool IsPieceMoveValid(const HiveState& state, const Pos& from, const Pos& to);

}  // namespace hive

namespace std {
// To be used a unordered map key.
template <>
struct hash<hive::Move> {
  size_t operator()(const hive::Move& move) const;
};

}  // namespace std