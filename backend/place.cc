#include "backend/place.h"

#include <unordered_set>

#include "glog/logging.h"

namespace hive {

std::vector<Place> GetPlaceActions(const HiveState& state) {
  std::vector<Place> actions;
  std::vector<PieceType> piece_types = GetPlacePieceTypes(state);
  if (piece_types.size() > 0) {
    std::vector<Pos> place_positions = GetPlacePositions(state);
    for (const auto& piece_type : piece_types) {
      for (const auto& pos : place_positions) {
        actions.push_back(Place(pos, piece_type));
      }
    }
  }
  return actions;
}

std::vector<PieceType> GetPlacePieceTypes(const HiveState& state) {
  std::vector<PieceType> result;
  CHECK(state.active_player != Side::kUndefined);
  if (state.active_player == Side::kBlack) {
    std::vector<PieceType> available_piece_types;
    if (state.EnforceQueenPlacement()) {
      CHECK(state.black_piece_count.at(PieceType::kQueen) == 1);
      return {PieceType::kQueen};
    } else {
      for (const auto& pair : state.black_piece_count) {
        if (pair.second > 0) {
          result.push_back(pair.first);
        }
      }
      return result;
    }
  } else {
    std::vector<PieceType> available_piece_types;
    if (state.EnforceQueenPlacement()) {
      CHECK(state.white_piece_count.at(PieceType::kQueen) == 1);
      return {PieceType::kQueen};
    } else {
      for (const auto& pair : state.white_piece_count) {
        if (pair.second > 0) {
          result.push_back(pair.first);
        }
      }
      return result;
    }
  }
}

std::vector<Pos> GetPlacePositions(const HiveState& state) {
  // Special case for empty board.
  if (state.pieces.size() == 0) {
    return {Pos(0, 0)};
  }
  // Special case for second move.
  if (state.pieces.size() == 1) {
    return Pos(0, 0).GetAdjacentPositions();
  }
  // General case
  std::unordered_set<Pos> positions;
  for (const auto& pair : state.pieces) {
    if (pair.second.side == state.active_player) {
      for (const auto& pos : pair.first.GetAdjacentPositions()) {
        if (positions.find(pos) == positions.end() && state.IsEmpty(pos) &&
            !state.HasOpponentNeighbour(pos, state.active_player)) {
          positions.insert(pos);
        }
      }
    }
  }
  return std::vector<Pos>(positions.begin(), positions.end());
}

}  // namespace hive