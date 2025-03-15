#pragma once

#include <memory>
#include <set>
#include <unordered_map>

#include "state.h"

class DFAState : public State {
 public:
  typedef std::set<std::shared_ptr<DFAState>, StateCmp> Set;
  typedef std::unordered_map<char, std::shared_ptr<DFAState>> Table;

  DFAState(int id, bool is_end);

  inline const Table& Transitions() const { return transitions; };

  std::shared_ptr<DFAState> MoveTo(char symbol) const;
  void AddTransition(char symbol, std::shared_ptr<DFAState> next_state);

  bool operator==(const DFAState& other) const;
  bool operator!=(const DFAState& other) const;

 private:
  Table transitions;
};
