#include "operator.h"

// Check if a concatenation operator is able to be inserted to the left of char.
bool IsAbleInsertConcat(char c) {
  switch (c) {
    case OP_CONCAT:
    case OP_UNION:
    case OP_CLOSURE:
    case OP_RIGHT_PAIR:
    case OP_PLUS:
    case OP_OPTIONAL:
    case OP_RANGE_END:
    case OP_RANGE_TO:
      return false;
    default:
      // Only left pair and symbols are able to be concatenated.
      return true;
  }
}

// Check if the operator acts on the right operand
bool IsRightActingOperator(char c) {
  switch (c) {
    case OP_CONCAT:
    case OP_UNION:
    case OP_LEFT_PAIR:
      return true;
    default:
      return false;
  }
}

bool IsCalculationOperator(char c) {
  switch (c) {
    case OP_CONCAT:
    case OP_UNION:
    case OP_CLOSURE:
    case OP_PLUS:
    case OP_OPTIONAL:
      return true;
    default:
      return false;
  }
}

int GetOperatorPrecedence(Operation c) {
  switch (c) {
    case OP_CONCAT:
    case OP_UNION:
      return 1;
    case OP_CLOSURE:
    case OP_PLUS:
    case OP_OPTIONAL:
      return 2;
    default:
      return 0;
  }
}
