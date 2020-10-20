#include "backend/state.h"

namespace hive {

struct Place {
  Pos pos;
  PieceType piece_type;
  Place(Pos pos_, PieceType piece_type_) : pos(pos_), piece_type(piece_type_) {}
  bool operator==(const Place& other) const {
    return pos == other.pos && piece_type == other.piece_type;
  }
  bool operator!=(const Place& other) const { return !operator==(other); }
  std::string DebugString() const {
    return std::string("Place[") + kPieceTypeName.at(piece_type) + "," + pos.DebugString() + "]";
  }
};

// Get available places from current state
std::vector<Place> GetPlaceActions(const HiveState& state);

std::vector<PieceType> GetPlacePieceTypes(const HiveState& state);
// Get the empty position that can be placed for the active player.
std::vector<Pos> GetPlacePositions(const HiveState& state);

}  // namespace hive