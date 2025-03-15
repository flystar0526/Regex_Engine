#pragma once

#include <memory>
#include <set>
#include <stack>
#include <string>
#include <utility>
#include <vector>

#include "nfa.h"
#include "nfa_state.h"
#include "operator.h"

class NFAParser {
 public:
  typedef std::stack<std::shared_ptr<NFA>> NFAStack;
  typedef std::stack<Operation> OpStack;
  typedef std::pair<char, char> Range;

  std::shared_ptr<NFA> Parse(const std::string& regex);

 private:
  std::shared_ptr<NFAState> NewState(bool is_end);
  std::shared_ptr<NFA> CreateNFAFromSymbol(char symbol);
  std::shared_ptr<NFA> CreateNFAFromSymbols(const std::set<char>& symbols);
  std::shared_ptr<NFA> CreateNFAFromRanges(const std::vector<Range>& ranges);
  std::shared_ptr<NFA> NFAConcat(const std::shared_ptr<NFA>& nfa1,
                                 const std::shared_ptr<NFA>& nfa2);
  std::shared_ptr<NFA> NFAUnion(const std::shared_ptr<NFA>& nfa1,
                                const std::shared_ptr<NFA>& nfa2);
  std::shared_ptr<NFA> NFAClosure(const std::shared_ptr<NFA>& nfa);
  std::shared_ptr<NFA> NFAOptional(const std::shared_ptr<NFA>& nfa);
  std::shared_ptr<NFA> NFAPlus(const std::shared_ptr<NFA>& nfa);
  std::string Normalize(const std::string& str);
  void Calc(NFAStack& nfa_stack, OpStack& op_stack);

  int state_id = 0;
};
