#pragma once

namespace hive {

enum class Side {
  kUndefined = 0,
  kBlack = 1,
  kWhite = 2,
};

enum class Action {
  kUndefined = 0,
  kPlace,
  kMove,
  kPass,
};

enum class PieceType {
  kUndefined = 0,
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
