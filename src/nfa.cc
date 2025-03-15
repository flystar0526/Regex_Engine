#include "nfa.h"

NFA::NFA(const std::shared_ptr<NFAState>& start,
         const std::shared_ptr<NFAState>& end)
    : start(start), end(end) {}
