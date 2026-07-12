#ifndef COMMA_H
#define COMMA_H

#include "token.h"

class Comma : public Token {
public:
    int tokenType() const override { return TOKEN; }
    void Print(ostream& outs) const override { outs << ','; }
};

#endif
