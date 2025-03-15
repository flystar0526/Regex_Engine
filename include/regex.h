#pragma once

#include <memory>
#include <string>

#include "dfa.h"
#include "nfa.h"
#include "nfa_parser.h"
#include "dfa_builder.h"
#include "dfa_minifier.h"

class Regex {
 public:
  Regex(const std::string& pattern);
  bool Match(const std::string& str);

 private:
  std::shared_ptr<DFA> dfa;
};
