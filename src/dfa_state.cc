#include "dfa_state.h"

DFAState::DFAState(int id, bool is_end) : State(id, is_end) {}

std::shared_ptr<DFAState> DFAState::MoveTo(char symbol) const {
  if (transitions.find(symbol) == transitions.end()) {
    return nullptr;
  }
  return transitions.at(symbol);
}

void DFAState::AddTransition(char symbol,
                             std::shared_ptr<DFAState> next_state) {
  transitions[symbol] = next_state;
}

bool DFAState::operator==(const DFAState& other) const {
  return Id() == other.Id();
}

bool DFAState::operator!=(const DFAState& other) const {
  return Id() != other.Id();
}
