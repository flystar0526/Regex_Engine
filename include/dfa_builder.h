#pragma once

#include <memory>
#include <queue>
#include <stack>
#include <string>
#include <unordered_map>

#include "dfa.h"
#include "dfa_state.h"
#include "nfa.h"
#include "nfa_state.h"

class DFABuilder {
 public:
  DFABuilder(std::shared_ptr<NFA> nfa);

  std::shared_ptr<DFA> Build();

 private:
  int MakeDFAStateId(const NFAState::Set& states);
  std::shared_ptr<DFAState> MakeDFAState(const NFAState::Set& states,
                                         int id = 0);
  void EpsilonClosure(NFAState::Set& states);
  std::shared_ptr<DFAState> Move(const std::shared_ptr<DFAState>& state,
                                 char c);

  std::shared_ptr<NFA> nfa;
  std::unordered_map<int, NFAState::Table> transitions;
  std::unordered_map<int, std::shared_ptr<DFAState>> total_states;
};
