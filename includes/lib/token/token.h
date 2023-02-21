#ifndef TOKEN_H
#define TOKEN_H
#include "constants.h"
#include <iostream>
using namespace std;


class Token
{
public:
    Token(){}
    ~Token(){}
    virtual const int precedence() const {return -1;}
    virtual const int tokenType() const {return -1;}
    virtual void Print(ostream &outs) const;
    friend ostream &operator<<(ostream &outs, const Token &token);
    friend ostream& operator <<(ostream& outs, const Token* token);
};





#endif // TOKEN_H