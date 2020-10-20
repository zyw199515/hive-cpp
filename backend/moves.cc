
#include "backend/moves.h"

#include <boost/functional/hash.hpp>
#include <deque>
#include <unordered_set>

#include "glog/logging.h"

namespace hive {

namespace {

void SpiderRecursiveMove(HiveState& state, std::vector<Pos>& history,
                         std::unordered_set<Move>& moves) {
  if (history.size() == 4) {
    moves.emplace(history.front(), history.back());
    return;
  }
  Pos from = history.back();
  for (const auto& to : from.GetAdjacentPositions()) {
    if (!state.IsEmpty(to) || std::find(history.begin(), history.end(), to) != history.end() ||
        !IsPieceMoveValid(state, from, to)) {
      continue;
    }
    history.push_back(to);
    SpiderRecursiveMove(state, history, moves);
    history.pop_back();
  }
}

void LadybugRecursiveMove(HiveState& state, std::vector<Pos>& history,
                          std::unordered_set<Move>& moves) {
  if (history.size() == 4) {
    moves.emplace(history.front(), history.back());
    return;
  }
  const Pos from = history.back();
  for (auto to : from.GetAdjacentPositions()) {
    while (!state.IsEmpty(to)) {
      to.z += 1;
    }
    if (history.size() == 3 && to.z != 0) {
      continue;
    } else if (history.size() < 3 && to.z == 0) {
      continue;
    }
    if (std::find(history.begin(), history.end(), to) != history.end() ||
        !IsPieceMoveValid(state, from, to)) {
      continue;
    }
    history.push_back(to);
    LadybugRecursiveMove(state, history, moves);
    history.pop_back();
  }
}

}  // namespace

bool Move::operator==(const Move& other) const {
  return from == other.from && to == other.to && immobile_next_turn == other.immobile_next_turn;
}

std::vector<Move> GetMoveByType(HiveState& state, const Pos& from, PieceType piece_type) {
  switch (piece_type) {
    case PieceType::kQueen:
      return GetQueenMovePositions(state, from);
    case PieceType::kBeetle:
      return GetBeetleMovePositions(state, from);
    case PieceType::kGrasshopper:
      return GetGrasshopperMovePositions(state, from);
    case PieceType::kSpider:
      return GetSpiderMovePositions(state, from);
    case PieceType::kAnt:
      return GetAntMovePositions(state, from);
    case PieceType::kLadybug:
      return GetLadybugMovePositions(state, from);
    case PieceType::kMosquito:
      return GetMosquitoMovePositions(state, from);
    case PieceType::kPillbug:
      return GetPillbugMovePositions(state, from);
    default:
      LOG(FATAL) << "Invalid piece";
      return {};
  }
}

std::vector<Move> GetMovePositions(HiveState& state) {
  std::vector<Move> moves;
  if (!state.EnforceQueenPlacement()) {
    for (const auto& pair : state.pieces) {
      if (pair.second.side != state.active_player) {
        continue;
      }
      const Pos& from = pair.first;
      std::vector<Move> move_by_piece = GetMovePositions(state, from);
      moves.insert(moves.end(), move_by_piece.begin(), move_by_piece.end());
    }
  }
  return moves;
}

std::vector<Move> GetMovePositions(HiveState& state, const Pos& from) {
  CHECK(!state.IsEmpty(from));
  if (state.IsStacked(from) || state.IsImmobile(from) || !IsHiveStillConnected(state, from)) {
    // In the following cases, a piece cannot move.
    // 1. It is stacked by another piece.
    // 2. It is rendered immobile by pillbug last turn.
    // 3. Hive would be not connected if we remove this piece.
    return {};
  }
  PieceType piece_type = state.GetPieceType(from);
  return GetMoveByType(state, from, piece_type);
}

std::vector<Move> GetQueenMovePositions(const HiveState& state, const Pos& from) {
  CHECK(from.z == 0);
  std::vector<Move> moves;
  for (const auto& to : from.GetAdjacentPositions()) {
    if (!state.IsEmpty(to) || !IsPieceMoveValid(state, from, to)) {
      continue;
    }
    moves.emplace_back(from, to);
  }
  return moves;
}  // namespace hive

std::vector<Move> GetBeetleMovePositions(const HiveState& state, const Pos& from) {
  std::vector<Move> moves;
  for (auto to : from.GetAdjacentPositions()) {
    while (!state.IsEmpty(to)) {
      to.z += 1;
    }
    if (!IsPieceMoveValid(state, from, to)) {
      continue;
    }
    moves.emplace_back(from, to);
  }
  return moves;
}

std::vector<Move> GetGrasshopperMovePositions(const HiveState& state, const Pos& from) {
  CHECK(from.z == 0);
  std::vector<Move> moves;
  for (const std::pair<int, int>& direction : kAdjacentDirections) {
    Pos to(from.x + direction.first, from.y + direction.second, from.z);
    if (state.IsEmpty(to)) {
      continue;
    }
    while (!state.IsEmpty(to)) {
      to.x += direction.first;
      to.y += direction.second;
    }
    moves.emplace_back(from, to);
  }
  return moves;
}  // namespace hive

std::vector<Move> GetSpiderMovePositions(HiveState& state, const Pos& from) {
  CHECK(from.z == 0);
  std::unordered_set<Move> moves;
  std::vector<Pos> history = {from};
  // NOTE: need to remove here to for correct judgement under mutli-step movements.
  Piece from_piece = state.pieces.at(from);
  CHECK_EQ(state.pieces.erase(from), 1);
  SpiderRecursiveMove(state, history, moves);
  state.pieces[from] = from_piece;
  return std::vector<Move>(moves.begin(), moves.end());
}

std::vector<Move> GetAntMovePositions(HiveState& state, const Pos& from) {
  // Ant is exactly like spider, but can move to anywhere that has depth > 1
  CHECK(from.z == 0);
  Piece from_piece = state.pieces.at(from);
  CHECK_EQ(state.pieces.erase(from), 1);
  std::unordered_set<Pos> visited;
  std::deque<Pos> to_visit = {from};
  while (!to_visit.empty()) {
    Pos current_pos = to_visit.front();
    to_visit.pop_front();
    if (visited.find(current_pos) == visited.end()) {
      visited.insert(current_pos);
      for (const auto& to : current_pos.GetAdjacentPositions()) {
        if (!state.IsEmpty(to) || visited.find(to) != visited.end() ||
            !IsPieceMoveValid(state, current_pos, to)) {
          continue;
        }
        to_visit.push_back(to);
      }
    }
  }
  visited.erase(from);
  std::vector<Move> moves;
  for (const auto& to : visited) {
    moves.emplace_back(from, to);
  }
  state.pieces[from] = from_piece;
  return moves;
}

std::vector<Move> GetLadybugMovePositions(HiveState& state, const Pos& from) {
  // Like spider, except that the first 2 moves must be at level 1.
  CHECK(from.z == 0);
  std::unordered_set<Move> moves;
  std::vector<Pos> history = {from};
  Piece from_piece = state.pieces.at(from);
  CHECK_EQ(state.pieces.erase(from), 1);
  LadybugRecursiveMove(state, history, moves);
  state.pieces[from] = from_piece;
  return std::vector<Move>(moves.begin(), moves.end());
}

std::vector<Move> GetMosquitoMovePositions(HiveState& state, const Pos& from) {
  if (from.z > 0) {
    return GetBeetleMovePositions(state, from);
  }
  std::unordered_set<PieceType> adjacent_piece_types;
  for (const auto& pos : state.GetNeighbours(from)) {
    adjacent_piece_types.insert(state.GetPieceType(pos));
  }
  // Ban itself.
  adjacent_piece_types.erase(PieceType::kMosquito);
  std::unordered_set<Move> moves;
  for (PieceType type : adjacent_piece_types) {
    std::vector<Move> move_for_type = GetMoveByType(state, from, type);
    moves.insert(move_for_type.begin(), move_for_type.end());
  }
  return std::vector<Move>(moves.begin(), moves.end());
}

std::vector<Move> GetPillbugMovePositions(const HiveState& state, const Pos& from) {
  CHECK(from.z == 0);
  // Standard move same as a queen
  std::vector<Move> moves = GetQueenMovePositions(state, from);
  // Alternative movement
  std::vector<Pos> empty_positions;
  std::vector<Pos> ground_level_positions;
  for (const auto& pos : from.GetAdjacentPositions()) {
    if (state.IsEmpty(pos)) {
      empty_positions.push_back(pos);
    } else if (pos.z == 0) {
      ground_level_positions.push_back(pos);
    }
  }
  if (empty_positions.empty() || ground_level_positions.empty()) {  // early stop
    return moves;
  }
  Pos over(from.x, from.y, 1);
  for (const auto& move_from : ground_level_positions) {
    if (state.IsImmobile(move_from) || !IsHiveStillConnected(state, move_from) ||
        !IsPieceMoveValid(state, move_from, over)) {
      // In those cases, the piece cannot be moved.
      continue;
    }
    for (const auto& to : empty_positions) {
      if (!IsPieceMoveValid(state, over, to)) {
        continue;
      }
      moves.push_back({.from = move_from, .to = to, .immobile_next_turn = true});
    }
  }
  return moves;
}

bool IsHiveStillConnected(const HiveState& state, const Pos& from) {
  if (from.z > 0) {  // If move on higher position, hive will always be connected.
    return true;
  }
  // Precondition: pos.z == 0
  std::vector<Pos> neighbours = state.GetNeighbours(from);
  CHECK_GT(neighbours.size(), 0);
  if (neighbours.size() == 1 || neighbours.size() >= 5) {
    return true;
  }
  // Just need to check if we can reach any neighbour from neighbour 0, given
  // pos is ignored.
  std::unordered_set<Pos> visited;
  std::deque<Pos> to_be_visit = {neighbours[0]};
  while (!to_be_visit.empty()) {
    Pos current_pos = to_be_visit.front();
    to_be_visit.pop_front();
    if (visited.find(current_pos) == visited.end()) {
      visited.insert(current_pos);
      for (const auto& neighbour : state.GetNeighbours(current_pos)) {
        if (visited.find(neighbour) == visited.end() && neighbour != from) {
          to_be_visit.push_back(neighbour);
        }
      }
    }
  }
  for (size_t i = 1; i < neighbours.size(); ++i) {
    if (visited.find(neighbours[i]) == visited.end()) {
      return false;
    }
  }
  return true;
}

bool IsPieceMoveValid(const HiveState& state, const Pos& from, const Pos& to) {
  // If going up/down, only the highest level needs to be considered.
  int check_z = std::max(from.z, to.z);
  for (int i = 0; i < 6; ++i) {
    if (to.x == from.x + kAdjacentDirections[i].first &&
        to.y == from.y + kAdjacentDirections[i].second) {
      int previous = (i + 5) % 6;
      int next = (i + 1) % 6;
      Pos previous_pos(from.x + kAdjacentDirections[previous].first,
                       from.y + kAdjacentDirections[previous].second, check_z);
      Pos next_pos(from.x + kAdjacentDirections[next].first,
                   from.y + kAdjacentDirections[next].second, check_z);
      if (check_z > 0) {
        return state.IsEmpty(previous_pos) || state.IsEmpty(next_pos);
      } else {
        // Exactly one of the position should be occupied.
        return state.IsEmpty(previous_pos) ^ state.IsEmpty(next_pos);
      }
    }
  }
  LOG(FATAL) << "should not come here. From = " << from.DebugString()
             << ", To = " << to.DebugString();
  return false;
}

}  // namespace hive

namespace std {

size_t hash<hive::Move>::operator()(const hive::Move& move) const {
  std::size_t seed = 0;
  std::hash<hive::Pos> pos_hasher;
  boost::hash_combine(seed, pos_hasher(move.from));
  boost::hash_combine(seed, pos_hasher(move.to));
  boost::hash_combine(seed, move.immobile_next_turn);
  return seed;
}

}  // namespace std