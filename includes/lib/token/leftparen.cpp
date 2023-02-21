#include "leftparen.h"

LeftParen::LeftParen()
{
    this->_left_paren = "(";
}

LeftParen::~LeftParen(){}   

const int LeftParen::tokenType() const
{
    return LPAREN;
}




void LeftParen::Print(ostream& outs) const
{
    outs << this->_left_paren; 
}


ostream& operator <<(ostream &outs, const LeftParen &lp)
{
    outs << lp._left_paren;
    return outs;
}

ostream& operator <<(ostream& outs, const LeftParen* lp)
{
    outs << lp->_left_paren;
    return outs;
}
