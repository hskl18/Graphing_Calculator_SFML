#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <cmath>

const int OPERATOR_NUM_NEEDED = 2;
const double PI = atan(1) * 2;

enum TOKEN_TYPES
{
  FUNCTION = 1,
  INTEGER = 2,
  OPERATOR = 3,
  LPAREN = 4,
  RPAREN = 5,
  TOKEN = 6,
  VAR = 7,
};

enum PRECEDENCE
{
  P_1 = 10,
  P_2 = 20,
  P_3 = 30,
  P_4 = 40,
};
#endif //CONSTANTS_H