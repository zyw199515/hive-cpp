
#pragma once

#include <optional>
#include <unordered_map>

#include ""

namespace hive {

class HiveState : public State {
 public:
  void print(std::ostream &strm) override;

  bool IsEmpty(const Pos &pos);
  bool IsStacked(const Pos &pos);
  PieceType GetPieceType(const Pos &pos);

  // Get Non-empty positions that are neighbour to given position.
  std::vector<Pos> GetNeighbours(const Pos &pos);

  // pieces on the board.
  std::unordered_map<Pos, PieceType> pieces;

  // To enforce queen bee placing.
  int black_queen_turn_countdown = 4;
  int white_queen_turn_countdown = 4;

  // Extra state for pillbug.
  // The piece that just get moved last turn.
  std::optional<Pos> last_moved_piece;
  // The piece get moved by pillbug last turn.
  std::optional<Pos> immobile_piece_by_pillbug;

  // The player that would take action.
  Side active_player;

  // spare pieces not placed.
  std::unordered_map<PieceType, int> spare_piece_count;
};

}  // namespace hive