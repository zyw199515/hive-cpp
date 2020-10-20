
#include "backend/state.h"

#include "glog/logging.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace hive;

// Test samples are constructed according to official ruling book examples.
TEST(StateTest, GetPlacePieceTypes) {
  HiveState state;
  state.active_player = Side::kBlack;
  state.black_piece_count = {{PieceType::kAnt, 1}, {PieceType::kBeetle, 0}, {PieceType::kQueen, 1}};
  EXPECT_THAT(state.GetPlacePieceTypes(),
              testing::UnorderedElementsAre(PieceType::kAnt, PieceType::kQueen));
  state.black_queen_turn_countdown = 0;
  EXPECT_THAT(state.GetPlacePieceTypes(), testing::UnorderedElementsAre(PieceType::kQueen));
}

TEST(StateTest, GetPlacePositions) {
  // Initial state must start from (0, 0)
  HiveState state;
  state.active_player = Side::kBlack;
  EXPECT_THAT(state.GetPlacePositions(), testing::UnorderedElementsAre(Pos(0, 0)));

  // The second move must be neighbour.
  state.pieces[Pos(0, 0)] = Piece(PieceType::kAnt, Side::kBlack);
  state.active_player = Side::kWhite;
  EXPECT_THAT(state.GetPlacePositions(),
              testing::UnorderedElementsAre(Pos(0, 1), Pos(1, 0), Pos(-1, 1), Pos(-1, 0),
                                            Pos(0, -1), Pos(1, -1)));
  // Case from ruling
  state.pieces[Pos(0, 1)] = Piece(PieceType::kAnt, Side::kWhite);
  state.active_player = Side::kBlack;
  EXPECT_THAT(state.GetPlacePositions(),
              testing::UnorderedElementsAre(Pos(-1, 0), Pos(0, -1), Pos(1, -1)));
}