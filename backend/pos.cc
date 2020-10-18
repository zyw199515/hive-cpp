#include "backend/pos.h"

#include <vector>

namespace hive {

// Those directions are ordered in a circle.
const std::vector<std::pair<int,int>> kAdajcentDirections = {
    {1, 0},
    {1, -1},
    {0, -1},
    {-1, 0},
    {-1, 1},
    {0, 1}
};



// To be used as unordered map key
template<> struct std::hash<Pos> {
    size_t operator()(Pos const& pos) const noexcept
    {
        // TO BE DONE
    }
};

} // namespace hive