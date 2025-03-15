#include "regex.h"

Regex::Regex(const std::string& pattern) : dfa(nullptr) {
  std::shared_ptr<NFA> nfa = NFAParser().Parse(pattern);
  std::shared_ptr<DFA> dfa = DFABuilder(nfa).Build();
  DFAMinifier(dfa).Minify();
  this->dfa = dfa;
}

bool Regex::Match(const std::string& str) { return dfa->Match(str); }
