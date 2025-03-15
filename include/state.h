#pragma once

#include <memory>

class State {
 public:
  class StateCmp {
   public:
    bool operator()(const std::shared_ptr<State>& left,
                    const std::shared_ptr<State>& right) const;
  };

  State(int id, bool is_end);

  inline int Id() const { return id; }
  inline bool IsEnd() const { return is_end; }

 protected:
  int id;
  bool is_end;
};
