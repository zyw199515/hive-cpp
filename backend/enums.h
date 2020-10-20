#pragma once

namespace hive {

enum class Side {
  kUndefined = 0,
  kBlack = 1,
  kWhite = 2,
};

enum class ActionType {
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

const std::unordered_map<PieceType, char> kPieceTypeName = {
    {PieceType::kQueen, 'q'},    {PieceType::kBeetle, 'b'},  {PieceType::kGrasshopper, 'g'},
    {PieceType::kSpider, 's'},   {PieceType::kAnt, 'a'},     {PieceType::kLadybug, 'l'},
    {PieceType::kMosquito, 'm'}, {PieceType::kPillbug, 'p'},
};

struct EnumClassHash {
  template <typename T>
  std::size_t operator()(T t) const {
    return static_cast<std::size_t>(t);
  }
};

}  // namespace hive
