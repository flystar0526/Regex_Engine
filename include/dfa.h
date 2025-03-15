#pragma once

#include <memory>
#include <string>

#include "dfa_state.h"

class DFA {
 public:
  DFA(std::shared_ptr<DFAState> start);

  inline std::size_t Size() const { return states.size(); }
  inline DFAState::Set States() const { return states; }

  bool HasState(const std::shared_ptr<DFAState>& state);
  bool Match(const std::string& str);
  void EndStates(DFAState::Set& states);
  void NotEndStates(DFAState::Set& states);

  std::shared_ptr<DFAState> start = nullptr;
  DFAState::Set states;
};
