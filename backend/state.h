
#pragma once

#include <boost/optional.hpp>
#include <optional>
#include <unordered_map>

#include "backend/enums.h"
#include "backend/pos.h"
#include "third_party/mcts/mcts.hpp"

namespace hive {

struct Piece {
  PieceType type = PieceType::kUndefined;
  Side side = Side::kUndefined;
  Piece(PieceType type_ = PieceType::kUndefined, Side side_ = Side::kUndefined)
      : type(type_), side(side_) {}
};

// This board limit is computed based buffer coordinate.
struct VisualBoardLimit {
  int x_min;
  int x_max;
  int y_min;
  int y_max;
};

struct InitOption {
  bool mosquito = false;
  bool ladybug = false;
  bool pillbug = false;
};

class HiveState : public State {
 public:
  HiveState() = default;
  ~HiveState() override = default;

  void Initialize(const InitOption &option);

  void print(std::ostream &strm) override;

  std::string DebugString() const;

  bool IsEmpty(const Pos &pos) const;
  bool IsStacked(const Pos &pos) const;
  bool IsImmobile(const Pos &pos) const;
  PieceType GetPieceType(const Pos &pos) const;
  Side GetSide(const Pos &pos) const;
  bool HasOpponentNeighbour(const Pos &pos, Side side) const;

  // Get Non-empty positions that are neighbour to given position.
  std::vector<Pos> GetNeighbours(const Pos &pos) const;

  // If movement is allow (i.e. queen is placed)
  bool AllowMovement() const;

  // If this turn a queen must be placed.
  bool EnforceQueenPlacement() const;

  // pieces on the board.
  std::unordered_map<Pos, Piece> pieces;

  // To enforce queen bee placing.
  // Starts from 4, and become 0 indicating queen must be placed.
  int black_queen_turn_countdown = 4;
  int white_queen_turn_countdown = 4;

  // Extra state for pillbug.
  // If set, the piece that just move last turn.
  boost::optional<Pos> last_moved_piece;
  // If set, the piece get moved by pillbug last turn.
  boost::optional<Pos> immobile_piece;

  // The player that would take action.
  Side active_player = Side::kUndefined;

  // spare pieces not placed.
  std::unordered_map<PieceType, int> black_piece_count;
  std::unordered_map<PieceType, int> white_piece_count;

 protected:
  std::pair<int, int> GetVisualPos(const Pos &pos) const;
  VisualBoardLimit GetVisualBoardLimit() const;
};

}  // namespace hive