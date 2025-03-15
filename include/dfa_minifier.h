#pragma once

#include <memory>
#include <stack>
#include <unordered_map>
#include <utility>

#include "dfa.h"
#include "dfa_state.h"
#include "group.h"

class DFAMinifier {
 public:
  DFAMinifier(std::shared_ptr<DFA> dfa);

  void Minify();

  friend class Group;

 private:
  std::shared_ptr<Group> MakeGroup(const DFAState::Set& states);
  std::shared_ptr<Group> GroupOf(const std::shared_ptr<DFAState>& state) const;
  std::shared_ptr<Group> FindDistinguishable(
      const std::shared_ptr<Group>& group,
      const std::shared_ptr<DFAState>& state);
  bool Refine(Group::GroupSet& group_set);
  void RemoveUnreachableStates();
  void RemoveDeadStates();
  void InitGroupSet(Group::GroupSet& group_set);
  void RewriteDFA(Group::GroupSet& group_set);

  std::shared_ptr<DFA> dfa;
  std::unordered_map<int, std::shared_ptr<Group>> groups;
};
