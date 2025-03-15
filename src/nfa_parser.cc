#include "nfa_parser.h"

std::shared_ptr<NFA> NFAParser::Parse(const std::string& regex) {
  // Dijkstra's Two-Stack Algorithm
  NFAStack nfa_stack;
  OpStack op_stack;

  std::string str = Normalize(regex);

  // Avoid empty string
  nfa_stack.push(CreateNFAFromSymbol(EPSILON));

  int index = 0;

  while (index < str.size()) {
    char c = str[index];
    index++;

    switch (c) {
      case OP_CONCAT:
      case OP_UNION:
      case OP_PLUS:
      case OP_OPTIONAL:
      case OP_CLOSURE: {
        Operation op = static_cast<Operation>(c);

        while (!op_stack.empty() && IsCalculationOperator(op_stack.top()) &&
               GetOperatorPrecedence(op_stack.top()) >=
                   GetOperatorPrecedence(op)) {
          Calc(nfa_stack, op_stack);
        }

        op_stack.push(op);
        break;
      }
      case OP_LEFT_PAIR: {
        op_stack.push(static_cast<Operation>(c));
        break;
      }
      case OP_RIGHT_PAIR: {
        while (!op_stack.empty() && op_stack.top() != OP_LEFT_PAIR) {
          Calc(nfa_stack, op_stack);
        }

        op_stack.pop();
        break;
      }
      case OP_RANGE_START: {
        std::vector<Range> ranges;
        char range_start = EPSILON;

        while (str[index] != OP_RANGE_END && index < str.size()) {
          char c = str[index];
          index++;

          if (c != OP_RANGE_TO) {
            if (range_start == EPSILON) {
              range_start = c;
            } else {
              ranges.push_back(std::make_pair(range_start, c));
              range_start = EPSILON;
            }
          }
        }

        nfa_stack.push(CreateNFAFromRanges(ranges));
        break;
      }
      case OP_RANGE_END:
        break;
      case '\\':  // Escape character
        c = str[index];
        index++;
      default:
        nfa_stack.push(CreateNFAFromSymbol(c));
        break;
    }
  }

  while (!op_stack.empty()) {
    Calc(nfa_stack, op_stack);
  }

  return nfa_stack.top();
}

std::shared_ptr<NFAState> NFAParser::NewState(bool is_end) {
  state_id++;
  return std::make_shared<NFAState>(state_id, is_end);
}

std::shared_ptr<NFA> NFAParser::CreateNFAFromSymbol(char symbol) {
  std::shared_ptr<NFAState> start = NewState(false);
  std::shared_ptr<NFAState> end = NewState(true);
  start->AddTransition(symbol, end);
  return std::make_shared<NFA>(start, end);
}

std::shared_ptr<NFA> NFAParser::CreateNFAFromSymbols(
    const std::set<char>& symbols) {
  std::shared_ptr<NFAState> start = NewState(false);
  std::shared_ptr<NFAState> end = NewState(true);

  if (!symbols.empty()) {
    for (char symbol : symbols) {
      start->AddTransition(symbol, end);
    }
  } else {
    start->AddTransition(EPSILON, end);
  }

  return std::make_shared<NFA>(start, end);
}

std::shared_ptr<NFA> NFAParser::CreateNFAFromRanges(
    const std::vector<Range>& ranges) {
  std::set<char> symbols;

  for (auto& range : ranges) {
    for (char c = range.first; c <= range.second; c++) {
      symbols.insert(c);
    }
  }

  return CreateNFAFromSymbols(symbols);
}

std::shared_ptr<NFA> NFAParser::NFAConcat(const std::shared_ptr<NFA>& nfa1,
                                          const std::shared_ptr<NFA>& nfa2) {
  nfa1->end->AddTransition(EPSILON, nfa2->start);
  return std::make_shared<NFA>(nfa1->start, nfa2->end);
}

std::shared_ptr<NFA> NFAParser::NFAUnion(const std::shared_ptr<NFA>& nfa1,
                                         const std::shared_ptr<NFA>& nfa2) {
  std::shared_ptr<NFAState> start = NewState(false);
  std::shared_ptr<NFAState> end = NewState(true);

  start->AddTransition(EPSILON, nfa1->start);
  start->AddTransition(EPSILON, nfa2->start);
  nfa1->end->AddTransition(EPSILON, end);
  nfa2->end->AddTransition(EPSILON, end);

  return std::make_shared<NFA>(start, end);
}

std::shared_ptr<NFA> NFAParser::NFAClosure(const std::shared_ptr<NFA>& nfa) {
  std::shared_ptr<NFAState> start = NewState(false);
  std::shared_ptr<NFAState> end = NewState(true);

  start->AddTransition(EPSILON, nfa->start);
  start->AddTransition(EPSILON, end);
  nfa->end->AddTransition(EPSILON, nfa->start);
  nfa->end->AddTransition(EPSILON, end);

  return std::make_shared<NFA>(start, end);
}

std::shared_ptr<NFA> NFAParser::NFAOptional(const std::shared_ptr<NFA>& nfa) {
  std::shared_ptr<NFAState> start = NewState(false);
  std::shared_ptr<NFAState> end = NewState(true);

  start->AddTransition(EPSILON, nfa->start);
  start->AddTransition(EPSILON, end);
  nfa->end->AddTransition(EPSILON, end);

  return std::make_shared<NFA>(start, end);
}

std::shared_ptr<NFA> NFAParser::NFAPlus(const std::shared_ptr<NFA>& nfa) {
  return NFAConcat(nfa, NFAClosure(nfa));
}

// Normalize the regex string by inserting concatenation operators.
// For example, "ab" -> "a&b", "a(ab)" -> "a&(a&b)".
std::string NFAParser::Normalize(const std::string& str) {
  std::string normalized;
  bool is_in_range = false;

  for (auto& c : str) {
    if (IsAbleInsertConcat(c) && !normalized.empty() &&
        !IsRightActingOperator(normalized.back()) && !is_in_range) {
      normalized.push_back(OP_CONCAT);
    }

    if (c == OP_RANGE_START) {
      is_in_range = true;
    } else if (c == OP_RANGE_END) {
      is_in_range = false;
    }

    normalized.push_back(c);
  }

  return normalized;
}

void NFAParser::Calc(NFAStack& nfa_stack, OpStack& op_stack) {
  if (op_stack.empty()) {
    return;
  }

  Operation op = op_stack.top();
  op_stack.pop();

  switch (op) {
    case OP_CONCAT: {
      std::shared_ptr<NFA> nfa1 = nfa_stack.top();
      nfa_stack.pop();
      std::shared_ptr<NFA> nfa2 = nfa_stack.top();
      nfa_stack.pop();
      nfa_stack.push(NFAConcat(nfa2, nfa1));
      break;
    }
    case OP_UNION: {
      std::shared_ptr<NFA> nfa1 = nfa_stack.top();
      nfa_stack.pop();
      std::shared_ptr<NFA> nfa2 = nfa_stack.top();
      nfa_stack.pop();
      nfa_stack.push(NFAUnion(nfa2, nfa1));
      break;
    }
    case OP_CLOSURE: {
      std::shared_ptr<NFA> nfa = nfa_stack.top();
      nfa_stack.pop();
      nfa_stack.push(NFAClosure(nfa));
      break;
    }
    case OP_PLUS: {
      std::shared_ptr<NFA> nfa = nfa_stack.top();
      nfa_stack.pop();
      nfa_stack.push(NFAPlus(nfa));
      break;
    }
    case OP_OPTIONAL: {
      std::shared_ptr<NFA> nfa = nfa_stack.top();
      nfa_stack.pop();
      nfa_stack.push(NFAOptional(nfa));
      break;
    }
    default:
      break;
  }
}
