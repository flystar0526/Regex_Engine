#include "group.h"

Group::Group(const DFAState::Set& group) : group(group), id(0) { InitId(); }

void Group::SetSub(const DFAState::Set& set1, const DFAState::Set& set2,
                   DFAState::Set& set3) {
  for (auto& group : set1) {
    if (set2.find(group) == set2.end()) {
      set3.insert(group);
    }
  }
}

bool Group::HasEndState() const {
  for (auto& state : group) {
    if (state->IsEnd()) {
      return true;
    }
  }
  return false;
}

bool Group::operator==(const Group& other) const { return id == other.id; }

bool Group::operator!=(const Group& other) const { return id != other.id; }

void Group::InitId() {
  std::string str;
  for (auto& state : group) {
    str += "," + std::to_string(state->Id());
  }
  id = std::hash<std::string>{}(str);
}

bool Group::GroupCmp::operator()(const std::shared_ptr<Group>& left,
                                 const std::shared_ptr<Group>& right) const {
  return left->Id() < right->Id();
}

std::size_t Group::GroupHash::operator()(
    const std::shared_ptr<Group>& group) const {
  return std::hash<int>{}(group->Id());
}
