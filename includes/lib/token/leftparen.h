#ifndef LEFTPAREN_H
#define LEFTPAREN_H


#include "token.h"
#include "constants.h"
using namespace std;


class LeftParen : public Token
{
private:
    string _left_paren;
public:
    LeftParen();
    ~LeftParen();
    const int tokenType() const override;
    void Print(ostream &outs) const override;
    friend ostream& operator <<(ostream &outs, const LeftParen &lp);
    friend ostream& operator <<(ostream& outs, const LeftParen* lp);
    
};

#endif // !LeftParen