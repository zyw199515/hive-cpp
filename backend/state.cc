
#include "backend/state.h"

#include <limits>
#include <unordered_set>

#include "glog/logging.h"

namespace hive {

namespace {

// Draw enclosures from outside.
void DrawEnclosures(std::vector<std::vector<char>>& buffer) {
  CHECK_GE(buffer.size(), 2);
  int y_max = buffer.size();
  int x_max = buffer[0].size();
  buffer[0][0] = '+';
  buffer[0][x_max - 1] = '+';
  buffer[y_max - 1][0] = '+';
  buffer[y_max - 1][x_max - 1] = '+';
  for (int x = 1; x < x_max - 1; x++) {
    buffer[0][x] = '-';
    buffer[y_max - 1][x] = '-';
  }
  for (int y = 1; y < y_max - 1; y++) {
    buffer[y][0] = '|';
    buffer[y][x_max - 1] = '|';
  }
}

}  // namespace

void HiveState::print(std::ostream& strm) { strm << DebugString(); }

bool HiveState::IsEmpty(const Pos& pos) const { return pieces.count(pos) == 0; }
bool HiveState::IsStacked(const Pos& pos) const { return !IsEmpty(Pos(pos.x, pos.y, pos.z + 1)); }
bool HiveState::IsImmobile(const Pos& pos) const {
  if (!immobile_piece) {
    return false;
  }
  return (*immobile_piece) == pos;
}
PieceType HiveState::GetPieceType(const Pos& pos) const { return pieces.at(pos).type; }
Side HiveState::GetSide(const Pos& pos) const { return pieces.at(pos).side; }
bool HiveState::HasOpponentNeighbour(const Pos& pos, Side side) const {
  for (const auto& adjacent_position : GetNeighbours(pos)) {
    if (GetSide(adjacent_position) != side) {
      return true;
    }
  }
  return false;
}

std::vector<Pos> HiveState::GetNeighbours(const Pos& pos) const {
  std::vector<Pos> neighbours;
  for (const auto& adjacent_position : pos.GetAdjacentPositions()) {
    if (!IsEmpty(adjacent_position)) {
      neighbours.push_back(adjacent_position);
    }
  }
  return neighbours;
}

std::vector<Pos> HiveState::GetPlacePositions() const {
  // Special case for empty board.
  if (pieces.size() == 0) {
    return {Pos(0, 0)};
  }
  // Special case for second move.
  if (pieces.size() == 1) {
    return Pos(0, 0).GetAdjacentPositions();
  }
  // General case
  std::unordered_set<Pos> positions;
  for (const auto& pair : pieces) {
    if (pair.second.side == active_player) {
      for (const auto& pos : pair.first.GetAdjacentPositions()) {
        if (positions.find(pos) == positions.end() && IsEmpty(pos) &&
            !HasOpponentNeighbour(pos, active_player)) {
          positions.insert(pos);
        }
      }
    }
  }
  return std::vector<Pos>(positions.begin(), positions.end());
}

bool HiveState::AllowMovement() const {
  CHECK(active_player != Side::kUndefined);
  if (active_player == Side::kBlack) {
    return black_queen_turn_countdown == -1;
  } else {
    return white_queen_turn_countdown == -1;
  }
}

bool HiveState::EnforceQueenPlacement() const {
  CHECK(active_player != Side::kUndefined);
  if (active_player == Side::kBlack) {
    return black_queen_turn_countdown == 0;
  } else {
    return white_queen_turn_countdown == 0;
  }
}

std::pair<int, int> HiveState::GetVisualPos(const Pos& pos) const {
  return std::make_pair(2 * pos.x + pos.y, -pos.y);
}

VisualBoardLimit HiveState::GetVisualBoardLimit() const {
  VisualBoardLimit limit;
  limit.x_max = std::numeric_limits<int>::min();
  limit.y_max = std::numeric_limits<int>::min();
  limit.x_min = std::numeric_limits<int>::max();
  limit.y_min = std::numeric_limits<int>::max();
  for (const auto& pair : pieces) {
    std::pair<int, int> visual_pos = GetVisualPos(pair.first);
    limit.x_max = std::max(visual_pos.first, limit.x_max);
    limit.y_max = std::max(visual_pos.second, limit.y_max);
    limit.x_min = std::min(visual_pos.first, limit.x_min);
    limit.y_min = std::min(visual_pos.second, limit.y_min);
  }
  return limit;
}

std::string HiveState::DebugString() const {
  // Output a debug string representing the game state.
  VisualBoardLimit limit = GetVisualBoardLimit();
  std::vector<std::vector<char>> output_buffer(
      limit.y_max - limit.y_min + 3, std::vector<char>(limit.x_max - limit.x_min + 3, ' '));
  // Draw enclosure
  DrawEnclosures(output_buffer);
  for (const auto& pair : pieces) {
    const Pos& pos = pair.first;
    const Piece& piece = pair.second;
    if (IsStacked(pos)) {
      continue;
    }
    char piece_name = kPieceTypeName.at(piece.type);
    if (piece.side == Side::kWhite) {
      piece_name = std::toupper(piece_name);
    }
    std::pair<int, int> visual_pos = GetVisualPos(pos);
    visual_pos.first -= limit.x_min - 1;
    visual_pos.second -= limit.y_min - 1;
    output_buffer.at(visual_pos.second).at(visual_pos.first) = piece_name;
  }
  std::string debug_string = "board:\n";
  for (const auto& line : output_buffer) {
    debug_string += std::string(line.begin(), line.end());
    debug_string += "\n";
  }

  // TODO: add other states.
  return debug_string;
}

}  // namespace hive