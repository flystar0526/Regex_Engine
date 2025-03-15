#pragma once

#include <memory>
#include <string>

#include "dfa_state.h"

/* DFA Group */
class Group {
 public:
  class GroupCmp {
   public:
    bool operator()(const std::shared_ptr<Group>& left,
                    const std::shared_ptr<Group>& right) const;
  };

  class GroupHash {
   public:
    std::size_t operator()(const std::shared_ptr<Group>& group) const;
  };

  typedef std::set<std::shared_ptr<Group>, GroupCmp> GroupSet;

  Group(const DFAState::Set& group);

  inline int Id() const { return id; }
  inline const DFAState::Set& Set() const { return group; }

  static void SetSub(const DFAState::Set& set1, const DFAState::Set& set2,
                     DFAState::Set& set3);
  bool HasEndState() const;

  bool operator==(const Group& other) const;
  bool operator!=(const Group& other) const;

 private:
  DFAState::Set group;
  int id;

  void InitId();
};
