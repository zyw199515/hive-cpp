#pragma once

namespace hive {

enum class Side {
  kBlack = 0,
  kWhite = 1,
};

enum class PieceType {
  kQueen,
  kBeetle,
  kGrasshopper,
  kSpider,
  kAnt,
  kLadybug,
  kMosquito,
  kPillbug,
};

struct EnumClassHash {
  template <typename T>
  std::size_t operator()(T t) const {
    return static_cast<std::size_t>(t);
  }
};

}  // namespace hive
