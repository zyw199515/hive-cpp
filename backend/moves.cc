
#include <unordered_set>

namespace hive {

std::vector<Move> GetMoveByType(const HiveState& state, const Pos& from, PieceType piece_type) {
    switch(piece_type) {
        case kQueen:
            return GetQueenMovePositions(state, from);
        case kBeetle:
            return GetBeetleMovePositions(state, from);
        case kGrasshopper:
            return GetGrasshopperPositions(state, from);
        case kSpider:
            return GetSpiderMovePositions(state, from);
        case kAnt:
            return GetAntMovePositions(state, from);
        case kLadybug:
            return GetLadybugMovePositions(state, from);
        case kMosquito:
            return GetMosquitoMovePositions(state, from);
        case kPillbug:
            return GetPillbugMovePositions(state, from);
        default:
            LOG(FATAL) << "Invalid piece";
            return {}
    }
}

std::vector<Move> GetMovePositions(const HiveState& state, const Pos& from) {
    CHECK(!state.IsEmpty(from));
    if(state.IsStacked(from) || state.IsImmobileByPillbug(from) || !IsHiveStillConnected(state, from)){
        // In the following cases, a piece cannot move.
        // 1. It is stacked by another piece.
        // 2. It is rendered immobile by pillbug last turn.
        // 3. Hive would be not connected if we remove this piece.
        return {};
    }
    PieceType piece_type = state.pieces_.get(from);
    GetMoveByType(state, from, piece_type);
}

std::vector<Move> GetQueenMovePositions(HiveState& state, const Pos& from) {
    CHECK(pos.z == 0);
    std::vector<Pos> moves;
    for(const auto& to : GetAdjacentPositions(from)) {
        if(!state.IsEmpty(to) ||
           !IsPieceMoveValid(state, from, to) ||
        )
        moves.push_back({.from=from, .to=to});
    }
    return moves;
}

std::vector<Move> GetBeetleMovePositions(const HiveState& state, const Pos& from) {
    std::vector<Move> moves;
    for(const auto& to : GetAdjacentPositions(from)) {
        while(!state.IsEmpty(to)) {
            adjacent_position.z += 1; 
        }
        if(!IsPieceMoveValid(state, from, to)) {
            continue;
        }
        moves.push_back({.from=from, .to=to});
    }
    return moves;
}

std::vector<Move> GetGrasshopperMovePositions(HiveState& state, const Pos& from) {
    CHECK(from.z == 0);
    for (const std::pair<int,int>& direction : kAdjacentDirections) {
        Pos to = {pos.x+direction[0], pos.y+direction[1], pos.z};
        if(state.IsEmpty(to)){
            continue;
        }
        while(!state.IsEmpty(to)){
            to += direction[0];
            to += direction[1];
        }
        moves.push_back({.from=from, .to=to});
    }
    return moves;
}

std::vector<Move> GetSpiderMovePositions(HiveState& state, const Pos& from) {
    CHECK(from.z == 0);
    std::unordered_set<Pos> visited_pos = {from};
    std::unordered_set<Pos> last_visited_pos = {from};
    std::unorder_set<Pos> new_visited_pos;
    for(int depth = 1; depth <= 3; ++depth){
        new_visited_pos.clear();
        for(const auto& pos : last_visited_pos) {
            for(const auto& to : GetAdjacentPositions(pos)) {
                if(!state.IsEmpty(to) ||
                   visited_pos.find(to) != visited_depth.end() ||
                   !IsPieceMoveValid(state, pos, to)){
                    continue;
                }
                visited_pos.push_back(pos);
                new_visited_pos.push_back(pos);
            }
        }
        std::swap(last_visited_pos, new_visited_pos);
    }
    std::vector<Pos> moves;
    for(const auto& to : last_visit_pos){
        moves.push_back({.from=from, .to=to});
    }
    return moves;
}

std::vector<Move> GetAntMovePositions(HiveState& state, const Pos& from) {
    // Ant is exactly like spider, but can move to anywhere that has depth > 1
    CHECK(from.z == 0);
    std::unordered_set<Pos> visited;
    std::deque<Pos> to_be_visited = {from};
    while(!to_be_visited.empty()){
        Pos current_pos = to_be_visit.front();
        to_be_visit.pop_front();
        if(visited.find(current_pos) == visited.end()){
            visited.insert(current_pos);
            for(const auto& to : GetAdjacentPositions(current_pos)) {
                if(!state.IsEmpty(to) ||
                    visited.find(to) != visited.end() ||
                    !IsPieceMoveValid(state, current_pos, to)){
                    continue;
                }
                to_be_visit.push_back(pos);
            }
        }
    }
    visited.erase(from);
    for (const auto& to : visited) {
        moves.push_back({.from=from, .to=to});
    }
    return moves;
}

std::vector<Move> GetLadybugMovePositions(HiveState& state, const Pos& from) {
    // Like spider, except that the first 2 moves must be at level 1.
    CHECK(from.z == 0);
    std::unordered_set<Pos> visited_pos = {from};
    std::unordered_set<Pos> last_visited_pos = {from};
    std::unorder_set<Pos> new_visited_pos;
    for(int depth = 1; depth <= 3; ++depth){
        new_visited_pos.clear();
        for(const auto& pos : last_visited_pos) {
            for(const auto& to : GetAdjacentPositions(pos)) {
                while(!state.IsEmpty(to)) {
                    to.z += 1; 
                }
                if((depth == 3 && to.z != 0) || (depth !=3  to.z != 1)){
                    continue;
                }
                if(visited_pos.find(to) != visited_depth.end() ||
                   !IsPieceMoveValid(state, pos, to)){
                    continue;
                }
                visited_pos.push_back(pos);
                new_visited_pos.push_back(pos);
            }
        }
        std::swap(last_visited_pos, new_visited_pos);
    }
    std::vector<Pos> moves;
    for(const auto& to : last_visit_pos){
        moves.push_back({.from=from, .to=to});
    }
    return moves;
}

std::vector<Move> GetMosquitoMovePositions(HiveState& state, const Pos& from) {
    if(from.z > 0){
        return GetBeetleMovePositions(state, from);
    }
    std::unordered_set<PieceName> adjacent_piece_types;
    for(const auto& pos : state.GetNeighbours(from)) {
        adjacent_piece_types.insert(state.GetPieceType(pos));
    }
    // Ban itself.
    adjacent_piece_types.remove(kMosquito);
    std::unordered_set<Move> moves;
    for(PieceType type : adjacent_piece_types) {
        std::vector<Move> move_for_type = GetMoveByType(state, from, type);
        moves.insert(move_for_type.begin(), move_for_type.end());
    }
    return moves;
}

std::vector<Move> GetPillbugMovePositions(HiveState& state, const Pos& from) {
    CHECK(from.z == 0);
    // Standard move same as a queen
    std::vector<Move> moves = GetQueenMovePositions(state, from);
    // Alternative movement
    std::vector<Pos> empty_positions;
    std::vector<Pos> ground_level_positions;
    for(const auto & pos : GetAdjacentPositions(from)){
        if(state.IsEmpty(from)){
            empty_positions.push_back(pos);
        } else if(pos.z == 0) {
            ground_level_positions.push_back(pos);
        }
    }
    if(empty_positions.empty() || ground_level_positions.empty()) { // early stop
        return moves;
    }
    for(const auto& move_from : ground_level_positions) {
        if(state.IsImmobileByPillbug(move_from) ||
           !IsHiveStillConnected(state, move_from) ||
           IsPieceMoveValid(state, move_from, from)) {
            // In those cases, the piece cannot be moved.
            continue;
        }
        for(const auto& to: empty_positions) {
            if(!IsPieceMoveValid(state, from, to)) {
                continue;
            }
            moves.push_back({.from=move_from, .to=to, .immobile_next_turn=true});
        }
    }
    return moves;
}

bool IsHiveStillConnected(const HiveState& state, const Pos& from) {
    if(from.z > 0){ // If move on higher position, hive will always be connected.
        return true;
    }
    //Precondition: pos.z == 0
    std::vector<Pos> neighbours = state.GetNeighbours(from);
    CHECK_GT(neighbours.size(), 0);
    // Save computation.
    if(neighbours.size() == 1 || neighbours.size() >= 5){
        return true;
    }
    // Just need to check if we can reach any neighbour from neighbour 0, given pos is ignored.
    std::unordered_set<Pos> visited;
    std::deque<Pos> to_be_visit = {neighbours[0]};
    while(!to_be_visit.empty()) {
        neighbours.add();
        Pos current_pos = to_be_visit.front();
        to_be_visit.pop_front();
        if(visited.find(current_pos) == visited.end()){
            visited.insert(current_pos);
            for(const auto& neighbour : state.GetNeighbours(current_pos)){
                if(visited.find(neighbour) == visited.end() &&
                neighbour != from){
                    to_be_visit.push_back(neighbour);
                }
            }
        }
    }
    for(int i = 1; i < neighbours.size(); ++i) {
        if(visited.find(neighbour) == visited.end()){
            return false;
        }
    }
    return true;
}

bool IsPieceMoveValid(const HiveState& state, const Pos& from, const Pos& to){
    // If going up/down, only the highest level needs to be considered.
    int check_z = max(from.z, to.z);
    for(int i =0; i< 6; ++i) {
        if (to.x == from.x + kAdajcentDirections[i][0] &&
            to.y == from.y + kAdajcentDirections[i][1]){
            int previous = (i-1) % 6;
            int next = (i+1) % 6;
            Pos previous_pos = {.x=from.x+kAdajcentDirections[previous][0],
                 .y=from.y+kAdajcentDirections[previous][0],
                 .y=check_z)};
            Pos next_pos = {.x=from.x+kAdajcentDirections[next][0],
                 .y=from.y+kAdajcentDirections[next][0],
                 .y=check_z)};
            if(check_z > 0){
                return state.IsEmpty(previous_pos) || state.IsEmpty(next_pos);
            } else {
                //Exactly one of the position should be occupied.
                return state.IsEmpty(previous_pos) ^ state.IsEmpty(next_pos);
            }
        }
    }
    return false;
}

} // namespace hive


 