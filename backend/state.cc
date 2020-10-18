
#include "backend/state.h"

namespace hive {

bool HiveState::IsEmpty(const Pos& pos) {
    return pieces.find(pos) == pieces.end();
}

bool IsStacked(const Pos& pos){
    return !IsEmpty({.x=pos.x, .y=pos.y, .z=pos.z+1});
}

std::vector<Pos> HiveState::GetNeighbours(const Pos& pos) {
    std::vector<Pos> neighbours;
    for(const auto& adjacent_position: GetAjacentPositions(pos)){
        if(!IsEmpty(pos)){
            neighbours.push_back(adjacent_position);
        }
    }
    return neighbours;
}


}