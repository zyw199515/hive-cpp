#include "backend/action.h"

#include "glog/logging.h"

namespace hive {

namespace {

// This should be called in the end
void SwitchActivePlayer(HiveState* state) {
  CHECK(state->active_player != Side::kUndefined);
  if (state->active_player == Side::kBlack) {
    state->active_player = Side::kWhite;
  } else {
    state->active_player = Side::kBlack;
  }
}

void UpdateCountdown(HiveState* state) {
  if (state->active_player == Side::kBlack && state->black_queen_turn_countdown >= 0) {
    state->black_queen_turn_countdown -= 1;
  } else if (state->active_player == Side::kWhite && state->white_queen_turn_countdown >= 0) {
    state->white_queen_turn_countdown -= 1;
  }
}

}  // namespace

HiveAction::HiveAction() {}

void HiveAction::BuildPassAction() { type = ActionType::kPass; }
void HiveAction::BuildMoveAction(const Move& move_) {
  type = ActionType::kMove;
  move = move_;
}
void HiveAction::BuildPlaceAction(const Place& place_) {
  type = ActionType::kPlace;
  place = place_;
}

void HiveAction::ExecuteMove(HiveState* state) {
  Piece piece = state->pieces.at(move->from);
  // Sanity checks, can be commented out after testing.
  CHECK(!state->IsStacked(move->from));
  CHECK(!state->IsImmobile(move->from));
  CHECK(state->IsEmpty(move->to));
  CHECK(state->AllowMovement());
  CHECK(piece.side == state->active_player);

  state->pieces.erase(move->from);
  state->pieces[move->to] = piece;

  state->last_moved_piece = move->to;
  if (move->immobile_next_turn) {
    state->immobile_piece = move->to;
  }
}

void HiveAction::ExecutePlace(HiveState* state) {
  CHECK(state->IsEmpty(place->pos));
  if (state->active_player == Side::kBlack) {
    CHECK_GT(state->black_piece_count.at(place->piece_type), 0);
    state->black_piece_count.at(place->piece_type) -= 1;
    if (place->piece_type == PieceType::kQueen) {
      state->black_queen_turn_countdown = -1;
    }
  } else {
    CHECK_GT(state->white_piece_count.at(place->piece_type), 0);
    state->white_piece_count.at(place->piece_type) -= 1;
    if (place->piece_type == PieceType::kQueen) {
      state->white_queen_turn_countdown = -1;
    }
  }
  state->pieces[place->pos] = Piece(place->piece_type, state->active_player);

  state->last_moved_piece = boost::none;
  state->immobile_piece = boost::none;
  UpdateCountdown(state);
  SwitchActivePlayer(state);
}

void HiveAction::ExecutePass(HiveState* state) {
  state->last_moved_piece = boost::none;
  state->immobile_piece = boost::none;
  UpdateCountdown(state);
  SwitchActivePlayer(state);
}

std::string HiveAction::DebugString() const {
  switch (type) {
    case ActionType::kMove:
      return move->DebugString();
    case ActionType::kPlace:
      return place->DebugString();
    case ActionType::kPass:
      return "Pass";
    default:
      LOG(FATAL) << "Undefined action!";
  }
}

void HiveAction::print(std::ostream& strm) { strm << DebugString(); }

void HiveAction::execute(HiveState* state) {
  CHECK(type != ActionType::kUndefined);
  switch (type) {
    case ActionType::kMove:
      ExecuteMove(state);
      return;
    case ActionType::kPlace:
      ExecutePlace(state);
      return;
    case ActionType::kPass:
      ExecutePass(state);
      return;
    default:
      LOG(FATAL) << "Undefined action!";
  }
}

std::vector<HiveAction> GetActionsForBoard(HiveState& state) {
  std::vector<HiveAction> actions;
  for (auto& place : GetPlaceActions(state)) {
    HiveAction action;
    action.BuildPlaceAction(std::move(place));
    actions.push_back(std::move(action));
  }
  for (auto& move : GetMoveActions(state)) {
    HiveAction action;
    action.BuildMoveAction(std::move(move));
    actions.push_back(std::move(action));
  }
  if (actions.size() == 0) {
    // Only pass if no available move.
    HiveAction action;
    action.BuildPassAction();
    actions.push_back(std::move(action));
  }
  return actions;
}

}  // namespace hive