#pragma once
#include "state/state/State.hpp"
#include <memory>

class StateManager
{
public:
    static inline State *curr_state;
};