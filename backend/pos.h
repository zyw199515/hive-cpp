
#include<vector>

namespace hive {

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
    int z = 0; // layer, only for beetle
    // TODO: define equality
};

// Those directions are ordered in a circle.
const std::vector<std::pair<int,int>> kAdajcentDirections = {
    {1, 0},
    {1, -1},
    {0, -1},
    {-1, 0},
    {-1, 1},
    {0, 1}
};

// Return ajacent positions to Given position. z axis would be preserved.
std::vector<Pos> GetAdjacentPositions(const Pos& pos) {
    std::vector<Pos> positions;
    positions.reserve(6);
    for(const auto& direction : kAdajcentDirections) {
        positions.push_back({.x=pos.x+direction[0], .y=pos.y+direction[1], .z=pos.z})
    }
    return positions;
}


};