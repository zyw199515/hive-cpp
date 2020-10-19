#include "backend/pos.h"

#include <boost/functional/hash.hpp>
#include <vector>

namespace hive {

Pos::Pos(int xx, int yy, int zz) : x(xx), y(yy), z(zz) {}

bool Pos::operator==(const Pos& other) const {
  return other.x == x && other.y == y && other.z == z;
}

std::string Pos::DebugString() const {
  std::string debug = "(";
  debug += std::to_string(x) + "," + std::to_string(y);
  if (z != 0) {
    debug += "," + std::to_string(z);
  }
  debug += ")";
  return debug;
}

std::vector<Pos> Pos::GetAdjacentPositions() const {
  std::vector<Pos> positions;
  positions.reserve(6);
  for (const auto& direction : kAdjacentDirections) {
    positions.push_back({x + direction.first, y + direction.second});
  }
  return positions;
}

}  // namespace hive

namespace std {

size_t hash<hive::Pos>::operator()(const hive::Pos& pos) const {
  std::size_t seed = 0;
  boost::hash_combine(seed, pos.x);
  boost::hash_combine(seed, pos.y);
  boost::hash_combine(seed, pos.z);
  return seed;
}

}  // namespace std