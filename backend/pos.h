#pragma once

#include <iostream>
#include <vector>
namespace hive {

// Adjacent directions, ordered in a circle.
const std::vector<std::pair<int, int>> kAdjacentDirections = {{1, 0},  {1, -1}, {0, -1},
                                                              {-1, 0}, {-1, 1}, {0, 1}};

// Position of a piece on the board.
/* Graph demonstration: (x, y)
 *                       y+
 *            (-1, 2) (0, 2)
 *        (-1, 1) (0, 1) (1, 1)
 * x-  (-1, 0) (0, 0) (1, 0)  x+
 *        (0, -1) (1, -1)
 *       y-
 */
struct Pos {
  int x = 0;
  int y = 0;
  int z = 0;  // layer, only for beetle

  Pos(int xx, int yy, int zz = 0);

  std::string DebugString() const;
  friend std::ostream& operator<<(std::ostream& strm, Pos& p) {
    strm << p.DebugString();
    return strm;
  }

  bool operator==(const Pos& other) const;
  bool operator!=(const Pos& other) const { return !operator==(other); }

  // Return ajacent positions to Given position. z axis would be always 0.
  std::vector<Pos> GetAdjacentPositions() const;
};

}  // namespace hive

namespace std {
// To be used a unordered map key.
template <>
struct hash<hive::Pos> {
  size_t operator()(const hive::Pos& pos) const;
};

}  // namespace std