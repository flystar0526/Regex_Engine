#include "dfa_builder.h"

DFABuilder::DFABuilder(std::shared_ptr<NFA> nfa) : nfa(nfa) {}

std::shared_ptr<DFA> DFABuilder::Build() {
  NFAState::Set start_nfa = NFAState::Set{nfa->start};
  EpsilonClosure(start_nfa);
  std::shared_ptr<DFAState> start_dfa = MakeDFAState(start_nfa);

  std::queue<std::shared_ptr<DFAState>> queue;
  DFAState::Set queue_set;

  queue.push(start_dfa);
  queue_set.insert(start_dfa);

  std::shared_ptr<DFA> dfa = std::make_shared<DFA>(start_dfa);

  while (!queue.empty()) {
    std::shared_ptr<DFAState> state = queue.front();
    queue.pop();
    queue_set.erase(state);

    int id = state->Id();

    if (transitions.find(id) != transitions.end()) {
      for (auto& pair : transitions.at(id)) {
        char symbol = pair.first;
        std::shared_ptr<DFAState> next_state = Move(state, symbol);

        state->AddTransition(symbol, next_state);

        if (!dfa->HasState(next_state) &&
            queue_set.find(next_state) == queue_set.end()) {
          queue.push(next_state);
          queue_set.insert(next_state);
        }
      }
    }

    dfa->states.insert(state);
  }

  return dfa;
}

int DFABuilder::MakeDFAStateId(const NFAState::Set& states) {
  std::string str;

  for (auto& state : states) {
    str += "," + std::to_string(state->Id());
  }

  return std::hash<std::string>{}(str);
}

std::shared_ptr<DFAState> DFABuilder::MakeDFAState(const NFAState::Set& states,
                                                   int id) {
  if (!id) {
    id = MakeDFAStateId(states);
  }

  bool is_end = false;

  for (auto& state : states) {
    if (state->IsEnd()) {
      is_end = true;
      break;
    }
  }

  for (auto& state : states) {
    for (auto& pair : state->Transitions()) {
      char symbol = pair.first;
      NFAState::Set next_states = pair.second;

      if (symbol != EPSILON) {
        transitions[id][symbol].insert(next_states.begin(), next_states.end());
      }
    }
  }

  std::shared_ptr<DFAState> state = std::make_shared<DFAState>(id, is_end);
  total_states[id] = state;

  return state;
}

void DFABuilder::EpsilonClosure(NFAState::Set& states) {
  std::stack<std::shared_ptr<NFAState>> stack;

  for (auto& state : states) {
    stack.push(state);
  }

  // DFS
  while (!stack.empty()) {
    std::shared_ptr<NFAState> state = stack.top();
    stack.pop();

    if (!state->AcceptChar(EPSILON)) {
      continue;
    }

    for (auto& next_states : state->MoveTo(EPSILON)) {
      if (states.find(next_states) == states.end()) {
        states.insert(next_states);
        stack.push(next_states);
      }
    }
  }
}

std::shared_ptr<DFAState> DFABuilder::Move(
    const std::shared_ptr<DFAState>& state, char c) {
  NFAState::Set next_states = transitions.at(state->Id()).at(c);

  EpsilonClosure(next_states);

  int id = MakeDFAStateId(next_states);

  if (total_states.find(id) == total_states.end()) {
    return MakeDFAState(next_states, id);
  }

  return total_states.at(id);
}
