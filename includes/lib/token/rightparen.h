#ifndef RIGHTPAREN_H
#define RIGHTPAREN_H


#include "token.h"
#include "constants.h"
using namespace std;


class RightParen : public Token
{
private:
    string _right_paren;
public:
    RightParen();
    ~RightParen();
    const int tokenType() const override;
    void Print(ostream &outs) const override;
    friend ostream& operator <<(ostream &outs, const RightParen &rp);
    friend ostream& operator <<(ostream& outs, const RightParen* rp);
    
};

#endif // !RIGHTPAREN_H