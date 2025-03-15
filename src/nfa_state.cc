#include "nfa_state.h"

NFAState::NFAState(int id, int is_end) : State(id, is_end) {}

void NFAState::AddTransition(char symbol,
                             std::shared_ptr<NFAState> next_state) {
  transitions[symbol].insert(next_state);

  if (is_end) {
    is_end = false;
  }
}

const NFAState::Set& NFAState::MoveTo(char symbol) const {
  return transitions.at(symbol);
}

bool NFAState::AcceptChar(char symbol) const { 
    return transitions.find(symbol) != transitions.end();
}
