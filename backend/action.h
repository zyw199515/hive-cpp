#include "backend/moves.h"
#include "backend/place.h"
#include "backend/state.h"

namespace hive {

class HiveAction : Action<HiveState> {
 public:
  HiveAction();
  virtual ~HiveAction() = default;
  bool operator==(const HiveAction& other) const {
    return type == other.type && move == other.move && place == other.place;
  }
  bool operator!=(const HiveAction& other) const { return !operator==(other); }

  // Builders
  void BuildPassAction();
  void BuildMoveAction(const Move& move_);
  void BuildPlaceAction(const Place& place_);

  std::string DebugString() const;

  void execute(HiveState* state) override;
  void print(std::ostream& strm) override;

 protected:
  void ExecuteMove(HiveState* state);
  void ExecutePlace(HiveState* state);
  void ExecutePass(HiveState* state);

  ActionType type = ActionType::kUndefined;
  boost::optional<Move> move;
  boost::optional<Place> place;
};

// Get available actions from current board.
std::vector<HiveAction> GetActionsForBoard(HiveState& state);

}  // namespace hive