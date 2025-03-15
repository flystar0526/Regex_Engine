#include "dfa_minifier.h"

DFAMinifier::DFAMinifier(std::shared_ptr<DFA> dfa) : dfa(dfa) {}

void DFAMinifier::Minify() {
  RemoveUnreachableStates();
  RemoveDeadStates();

  if (dfa->Size() <= 1) {
    return;
  }

  Group::GroupSet group_set;

  InitGroupSet(group_set);

  while (Refine(group_set));

  RewriteDFA(group_set);
}

std::shared_ptr<Group> DFAMinifier::MakeGroup(const DFAState::Set& states) {
  std::shared_ptr<Group> group = std::make_shared<Group>(states);

  for (auto& state : group->Set()) {
    groups[state->Id()] = group;
  }

  return group;
}

std::shared_ptr<Group> DFAMinifier::GroupOf(
    const std::shared_ptr<DFAState>& state) const {
  return groups.at(state->Id());
}

std::shared_ptr<Group> DFAMinifier::FindDistinguishable(
    const std::shared_ptr<Group>& group,
    const std::shared_ptr<DFAState>& state) {
  std::shared_ptr<DFAState::Set> new_set = std::make_shared<DFAState::Set>();

  for (auto& pair : state->Transitions()) {
    char symbol = pair.first;
    std::shared_ptr<DFAState> next_state = pair.second;

    for (auto other_state : group->Set()) {
      if (*state == *other_state) {
        continue;
      }

      std::shared_ptr<DFAState> other_next_state = other_state->MoveTo(symbol);

      if (other_next_state == nullptr ||
          *GroupOf(next_state) != *GroupOf(other_next_state)) {
        new_set->insert(other_state);
      }
    }

    if (!new_set->empty()) {
      break;
    }
  }

  if (new_set->empty()) {
    return nullptr;
  }

  std::shared_ptr<Group> new_group = MakeGroup(*new_set);

  return new_group;
}

void DFAMinifier::RemoveUnreachableStates() {
  std::unique_ptr<DFAState::Set> reachable_states =
      std::make_unique<DFAState::Set>();
  std::stack<std::shared_ptr<DFAState>> stack;

  reachable_states->insert(dfa->start);
  stack.push(dfa->start);

  while (!stack.empty()) {
    std::shared_ptr<DFAState> state = stack.top();
    stack.pop();

    for (auto& pair : state->Transitions()) {
      std::shared_ptr<DFAState> next_state = pair.second;

      if (reachable_states->find(next_state) == reachable_states->end()) {
        reachable_states->insert(next_state);
        stack.push(next_state);
      }
    }
  }

  dfa->States().swap(*reachable_states);
}

void DFAMinifier::RemoveDeadStates() {
  std::unique_ptr<DFAState::Set> alive_states =
      std::make_unique<DFAState::Set>();
  alive_states->insert(dfa->start);

  for (auto state : dfa->States()) {
    if (!state->IsEnd() && state->Transitions().empty()) {
      continue;
    }
    alive_states->insert(state);
  }

  dfa->States().swap(*alive_states);
}

void DFAMinifier::InitGroupSet(Group::GroupSet& group_set) {
  std::shared_ptr<DFAState::Set> end_states = std::make_shared<DFAState::Set>();
  std::shared_ptr<DFAState::Set> other_states =
      std::make_shared<DFAState::Set>();

  dfa->EndStates(*end_states);
  dfa->NotEndStates(*other_states);

  std::shared_ptr<Group> group1 = MakeGroup(*end_states);
  std::shared_ptr<Group> group2 = MakeGroup(*other_states);

  group_set.insert(group1);
  group_set.insert(group2);
}

void DFAMinifier::RewriteDFA(Group::GroupSet& group_set) {
  std::unordered_map<std::shared_ptr<Group>, std::shared_ptr<DFAState>,
                     Group::GroupHash>
      group_map;
  std::unique_ptr<DFAState::Set> new_states = std::make_unique<DFAState::Set>();

  for (auto group : group_set) {
    std::shared_ptr<DFAState> state =
        std::make_shared<DFAState>(group->Id(), group->HasEndState());

    new_states->insert(state);
    group_map[group] = state;
  }

  for (auto group : group_set) {
    for (auto state : group->Set()) {
      for (auto pair : state->Transitions()) {
        char symbol = pair.first;
        std::shared_ptr<DFAState> next_state = pair.second;
        std::shared_ptr<Group> next_group = GroupOf(next_state);
        std::shared_ptr<DFAState> mapped_next_state = group_map[next_group];
        group_map[group]->AddTransition(symbol, mapped_next_state);
      }
    }
  }

  dfa->start = group_map[GroupOf(dfa->start)];
  dfa->States().swap(*new_states);
}

bool DFAMinifier::Refine(Group::GroupSet& group_set) {
  for (auto group : group_set) {
    for (auto state : group->Set()) {
      std::shared_ptr<Group> new_group2 = FindDistinguishable(group, state);
      if (new_group2 != nullptr) {
        std::shared_ptr<DFAState::Set> new_set =
            std::make_shared<DFAState::Set>();
        Group::SetSub(group->Set(), new_group2->Set(), *new_set);
        std::shared_ptr<Group> new_group1 = MakeGroup(*new_set);

        group_set.erase(group);
        group_set.insert(new_group1);
        group_set.insert(new_group2);

        return true;
      }
    }
  }
  return false;
}
