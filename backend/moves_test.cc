
#include "backend/moves.h"

#include "glog/logging.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

using namespace hive;

// Test samples are constructed according to official ruling book examples.

TEST(MovesTest, IsHiveStillConnected) {
  /* Case from ruling
   *  +-------+
   *  |q a b Q|
   *  | A     |
   *  +-------+
   */
  HiveState state;
  state.pieces[Pos(0, 0)] = Piece(PieceType::kAnt, Side::kBlack);
  state.pieces[Pos(1, 0)] = Piece(PieceType::kBeetle, Side::kBlack);
  state.pieces[Pos(2, 0)] = Piece(PieceType::kQueen, Side::kWhite);
  state.pieces[Pos(-1, 0)] = Piece(PieceType::kQueen, Side::kBlack);
  state.pieces[Pos(0, -1)] = Piece(PieceType::kAnt, Side::kWhite);
  EXPECT_EQ(IsHiveStillConnected(state, Pos(0, 0)), false);
  EXPECT_EQ(IsHiveStillConnected(state, Pos(1, 0)), false);
  EXPECT_EQ(IsHiveStillConnected(state, Pos(2, 0)), true);
  EXPECT_EQ(IsHiveStillConnected(state, Pos(-1, 0)), true);
  EXPECT_EQ(IsHiveStillConnected(state, Pos(0, -1)), true);
}

TEST(MovesTest, PieceMoveValid) {
  {
    /* Case from ruling
     *  +-----+
     *  | B Q |
     *  |q a S|
     *  | A   |
     *  +-----+
     */
    HiveState state;
    state.pieces[Pos(0, 0)] = Piece(PieceType::kAnt, Side::kBlack);
    state.pieces[Pos(1, 0)] = Piece(PieceType::kSpider, Side::kWhite);
    state.pieces[Pos(0, 1)] = Piece(PieceType::kQueen, Side::kWhite);
    state.pieces[Pos(-1, 1)] = Piece(PieceType::kBeetle, Side::kWhite);
    state.pieces[Pos(-1, 0)] = Piece(PieceType::kQueen, Side::kBlack);
    state.pieces[Pos(0, -1)] = Piece(PieceType::kAnt, Side::kWhite);
    EXPECT_EQ(IsPieceMoveValid(state, Pos(0, 0), Pos(1, -1)), false);
    state.pieces.erase(Pos(0, -1));
    EXPECT_EQ(IsPieceMoveValid(state, Pos(0, 0), Pos(1, -1)), true);
    state.pieces.erase(Pos(-1, 0));
    EXPECT_EQ(IsPieceMoveValid(state, Pos(0, 0), Pos(1, -1)), true);
    EXPECT_EQ(IsPieceMoveValid(state, Pos(0, 0), Pos(0, -1)), false);
  }

  {  // Corner case for beetle. Add a beetle above.
    HiveState state;
    state.pieces[Pos(0, 0)] = Piece(PieceType::kAnt, Side::kBlack);
    state.pieces[Pos(0, 0, 1)] = Piece(PieceType::kBeetle, Side::kBlack);
    state.pieces[Pos(1, 0)] = Piece(PieceType::kSpider, Side::kWhite);
    state.pieces[Pos(0, 1)] = Piece(PieceType::kQueen, Side::kWhite);
    state.pieces[Pos(-1, 1)] = Piece(PieceType::kBeetle, Side::kWhite);
    state.pieces[Pos(-1, 0)] = Piece(PieceType::kQueen, Side::kBlack);
    state.pieces[Pos(0, -1)] = Piece(PieceType::kAnt, Side::kWhite);
    EXPECT_EQ(IsPieceMoveValid(state, Pos(0, 0, 1), Pos(1, -1)), true);
    state.pieces[Pos(1, 0, 1)] = Piece(PieceType::kMosquito, Side::kWhite);
    state.pieces[Pos(0, -1, 1)] = Piece(PieceType::kMosquito, Side::kBlack);
    EXPECT_EQ(IsPieceMoveValid(state, Pos(0, 0, 1), Pos(1, -1)), false);  // Blocked.
  }
  {  // Corner case from ruling
    HiveState state;
    state.pieces[Pos(1, 0)] = Piece(PieceType::kSpider, Side::kWhite);
    state.pieces[Pos(0, 1)] = Piece(PieceType::kQueen, Side::kWhite);
    state.pieces[Pos(-1, 1)] = Piece(PieceType::kBeetle, Side::kWhite);
    state.pieces[Pos(-1, 0)] = Piece(PieceType::kQueen, Side::kBlack);
    state.pieces[Pos(0, -1)] = Piece(PieceType::kAnt, Side::kWhite);
    EXPECT_EQ(IsPieceMoveValid(state, Pos(1, 0), Pos(1, -1)),
              false);  // Cannot move since temporary disconnect from hive
  }
}

TEST(MovesTest, Queen) {
  /* Case from ruling
   * +------+
   * |   S  |
   * |  a s |
   * | q   B|
   * |G   b |
   * | g A Q|
   * +------+
   */
  HiveState state;
  state.pieces[Pos(-1, 0)] = Piece(PieceType::kQueen, Side::kBlack);
  state.pieces[Pos(-1, 1)] = Piece(PieceType::kAnt, Side::kBlack);
  state.pieces[Pos(-1, 2)] = Piece(PieceType::kSpider, Side::kWhite);
  state.pieces[Pos(0, 1)] = Piece(PieceType::kSpider, Side::kBlack);
  state.pieces[Pos(1, 0)] = Piece(PieceType::kBeetle, Side::kWhite);
  state.pieces[Pos(1, -1)] = Piece(PieceType::kBeetle, Side::kBlack);
  state.pieces[Pos(1, -2)] = Piece(PieceType::kAnt, Side::kWhite);
  state.pieces[Pos(2, -2)] = Piece(PieceType::kQueen, Side::kWhite);
  state.pieces[Pos(-1, -1)] = Piece(PieceType::kGrasshopper, Side::kWhite);
  state.pieces[Pos(0, -2)] = Piece(PieceType::kGrasshopper, Side::kBlack);

  Pos from(-1, 0);
  ASSERT_THAT(GetMovePositions(state, Pos(-1, 0)),
              testing::UnorderedElementsAre(Move(from, Pos(0, 0)), Move(from, Pos(0, -1)),
                                            Move(from, Pos(-2, 1)), Move(from, Pos(-2, 0))));
}

TEST(MovesTest, Beetle) {
  /* Case from ruling
   * +------+
   * | B   G|
   * |s a Q |
   * +------+
   */
  HiveState state;
  state.pieces[Pos(0, 0)] = Piece(PieceType::kBeetle, Side::kWhite);
  state.pieces[Pos(0, -1)] = Piece(PieceType::kSpider, Side::kBlack);
  state.pieces[Pos(1, -1)] = Piece(PieceType::kAnt, Side::kBlack);
  state.pieces[Pos(2, -1)] = Piece(PieceType::kQueen, Side::kWhite);
  state.pieces[Pos(2, 0)] = Piece(PieceType::kGrasshopper, Side::kWhite);
  Pos from(0, 0);
  ASSERT_THAT(GetMovePositions(state, Pos(0, 0)),
              testing::UnorderedElementsAre(Move(from, Pos(-1, 0)), Move(from, Pos(1, 0)),
                                            Move(from, Pos(0, -1, 1)), Move(from, Pos(1, -1, 1))));
}

TEST(MovesTest, Grasshopper) {
  /* Case from ruling
   * +----------+
   * | a a      |
   * |a   a a a |
   * | G a a   a|
   * |  a       |
   * +----------+
   */
  HiveState state;
  state.pieces[Pos(0, 0)] = Piece(PieceType::kGrasshopper, Side::kWhite);
  state.pieces[Pos(1, 0)] = Piece(PieceType::kAnt, Side::kBlack);
  state.pieces[Pos(2, 0)] = Piece(PieceType::kAnt, Side::kBlack);
  state.pieces[Pos(4, 0)] = Piece(PieceType::kAnt, Side::kBlack);
  state.pieces[Pos(1, 1)] = Piece(PieceType::kAnt, Side::kBlack);
  state.pieces[Pos(2, 1)] = Piece(PieceType::kAnt, Side::kBlack);
  state.pieces[Pos(3, 1)] = Piece(PieceType::kAnt, Side::kBlack);
  state.pieces[Pos(0, 2)] = Piece(PieceType::kAnt, Side::kBlack);
  state.pieces[Pos(-1, 2)] = Piece(PieceType::kAnt, Side::kBlack);
  state.pieces[Pos(-1, 1)] = Piece(PieceType::kAnt, Side::kBlack);
  state.pieces[Pos(1, -1)] = Piece(PieceType::kAnt, Side::kBlack);
  Pos from(0, 0);
  ASSERT_THAT(GetMovePositions(state, Pos(0, 0)),
              testing::UnorderedElementsAre(Move(from, Pos(3, 0)), Move(from, Pos(2, -2)),
                                            Move(from, Pos(-2, 2))));
}

TEST(MovesTest, Spider) {
  /* Case from ruling
   * +--------+
   * |     S  |
   * |  a   a |
   * | a     a|
   * |a     a |
   * | a a a  |
   * +--------+
   */
  HiveState state;
  state.pieces[Pos(0, 2)] = Piece(PieceType::kSpider, Side::kWhite);
  state.pieces[Pos(1, 1)] = Piece(PieceType::kAnt, Side::kBlack);
  state.pieces[Pos(2, 0)] = Piece(PieceType::kAnt, Side::kBlack);
  state.pieces[Pos(2, -1)] = Piece(PieceType::kAnt, Side::kBlack);
  state.pieces[Pos(1, 1)] = Piece(PieceType::kAnt, Side::kBlack);
  state.pieces[Pos(2, -1)] = Piece(PieceType::kAnt, Side::kBlack);
  state.pieces[Pos(2, -2)] = Piece(PieceType::kAnt, Side::kBlack);
  state.pieces[Pos(1, -2)] = Piece(PieceType::kAnt, Side::kBlack);
  state.pieces[Pos(0, -2)] = Piece(PieceType::kAnt, Side::kBlack);
  state.pieces[Pos(-1, -1)] = Piece(PieceType::kAnt, Side::kBlack);
  state.pieces[Pos(-1, 0)] = Piece(PieceType::kAnt, Side::kBlack);
  state.pieces[Pos(-1, 1)] = Piece(PieceType::kAnt, Side::kBlack);
  Pos from(0, 2);
  ASSERT_THAT(GetMovePositions(state, Pos(0, 2)),
              testing::UnorderedElementsAre(Move(from, Pos(3, 0)), Move(from, Pos(0, -1)),
                                            Move(from, Pos(1, -1)), Move(from, Pos(-2, 2))));
}

TEST(MovesTest, Ant) {
  /* Case from ruling
   * +-----+
   * | a   |
   * |a   a|
   * | a a |
   * |  A  |
   * +-----+
   */
  HiveState state;
  state.pieces[Pos(1, -2)] = Piece(PieceType::kAnt, Side::kWhite);
  state.pieces[Pos(0, -1)] = Piece(PieceType::kAnt, Side::kBlack);
  state.pieces[Pos(1, -1)] = Piece(PieceType::kAnt, Side::kBlack);
  state.pieces[Pos(-1, 0)] = Piece(PieceType::kAnt, Side::kBlack);
  state.pieces[Pos(-1, 1)] = Piece(PieceType::kAnt, Side::kBlack);
  state.pieces[Pos(1, 0)] = Piece(PieceType::kAnt, Side::kBlack);
  Pos from(1, -2);
  ASSERT_THAT(GetMovePositions(state, Pos(1, -2)),
              testing::UnorderedElementsAre(
                  Move(from, Pos(0, -2)), Move(from, Pos(-1, -1)), Move(from, Pos(-2, 0)),
                  Move(from, Pos(-2, 1)), Move(from, Pos(-2, 2)), Move(from, Pos(-1, 2)),
                  Move(from, Pos(0, 1)), Move(from, Pos(1, 1)), Move(from, Pos(2, 0)),
                  Move(from, Pos(2, -1)), Move(from, Pos(2, -2))));
}

TEST(MovesTest, Ladybug) {
  /* Case from ruling
   * +-----+
   * | a   |
   * |a   a|
   * | a a |
   * |  L  |
   * +-----+
   */
  HiveState state;
  state.pieces[Pos(1, -2)] = Piece(PieceType::kLadybug, Side::kWhite);
  state.pieces[Pos(0, -1)] = Piece(PieceType::kAnt, Side::kBlack);
  state.pieces[Pos(1, -1)] = Piece(PieceType::kAnt, Side::kBlack);
  state.pieces[Pos(-1, 0)] = Piece(PieceType::kAnt, Side::kBlack);
  state.pieces[Pos(-1, 1)] = Piece(PieceType::kAnt, Side::kBlack);
  state.pieces[Pos(1, 0)] = Piece(PieceType::kAnt, Side::kBlack);
  Pos from(1, -2);
  ASSERT_THAT(GetMovePositions(state, Pos(1, -2)),
              testing::UnorderedElementsAre(Move(from, Pos(0, -2)), Move(from, Pos(-1, -1)),
                                            Move(from, Pos(-2, 0)), Move(from, Pos(-2, 1)),
                                            Move(from, Pos(0, 0)), Move(from, Pos(0, 1)),
                                            Move(from, Pos(1, 1)), Move(from, Pos(2, 0)),
                                            Move(from, Pos(2, -1)), Move(from, Pos(2, -2))));
}