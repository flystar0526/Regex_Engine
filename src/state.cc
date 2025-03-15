#include "state.h"

State::State(int id, bool is_end) : id(id), is_end(is_end) {}

bool State::StateCmp::operator()(const std::shared_ptr<State>& left,
                                 const std::shared_ptr<State>& right) const {
  return left->Id() < right->Id();
}
