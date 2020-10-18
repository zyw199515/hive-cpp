
#include "backend/state.h"

namespace hive {

class HiveAction<HiveState> : Action {
    void execute(HiveState* ) override;
};

} // namespace hive