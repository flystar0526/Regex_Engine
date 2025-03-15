#pragma once

#include "nfa_state.h"

#include <memory>

class NFA {
 public:
  NFA(const std::shared_ptr<NFAState>& start,
      const std::shared_ptr<NFAState>& end);

  std::shared_ptr<NFAState> start = nullptr;
  std::shared_ptr<NFAState> end = nullptr;
};
