#pragma once

#include <unordered_map>

enum Operation : char {
  OP_CONCAT = '&',
  OP_UNION = '|',
  OP_CLOSURE = '*',
  OP_LEFT_PAIR = '(',
  OP_RIGHT_PAIR = ')',
  OP_PLUS = '+',
  OP_OPTIONAL = '?',
  OP_RANGE_START = '[',
  OP_RANGE_END = ']',
  OP_RANGE_TO = '-',
};

bool IsAbleInsertConcat(char c);
bool IsRightActingOperator(char c);
bool IsCalculationOperator(char c);
int GetOperatorPrecedence(Operation c);
