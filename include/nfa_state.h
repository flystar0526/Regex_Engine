#pragma once

#include <memory>
#include <set>
#include <unordered_map>

#include "state.h"

static constexpr char EPSILON = '\0';

class NFAState : public State {
 public:
  typedef std::set<std::shared_ptr<NFAState>, StateCmp> Set;
  typedef std::unordered_map<char, Set> Table;

  NFAState(int id, int is_end);

  inline const Table& Transitions() const { return transitions; };

  void AddTransition(char symbol, std::shared_ptr<NFAState> next_state);
  const NFAState::Set& MoveTo(char symbol) const;
  bool AcceptChar(char symbol) const;

 private:
  Table transitions;
};
