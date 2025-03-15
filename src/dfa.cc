#include "dfa.h"

DFA::DFA(std::shared_ptr<DFAState> start) : start(start) {}

bool DFA::HasState(const std::shared_ptr<DFAState>& state) {
  return states.find(state) != states.end();
}

bool DFA::Match(const std::string& str) {
  // TODO: Implement this function
  std::shared_ptr<DFAState> current = start;

  for (char c : str) {
    std::shared_ptr<DFAState> next = current->MoveTo(c);

    if (next == nullptr) {
      return false;
    }

    current = next;
  }

  return current->IsEnd();
}

void DFA::EndStates(DFAState::Set& states) {
  for (auto& state : this->states) {
    if (state->IsEnd()) {
      states.insert(state);
    }
  }
}

void DFA::NotEndStates(DFAState::Set& states) {
  for (auto& state : this->states) {
    if (!state->IsEnd()) {
      states.insert(state);
    }
  }
}
