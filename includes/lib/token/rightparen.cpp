#include "rightparen.h"

RightParen::RightParen()
{
    this->_right_paren = ")";
}

RightParen::~RightParen(){}   

const int RightParen::tokenType() const
{
    return RPAREN;
}




void RightParen::Print(ostream& outs) const
{
    outs << this->_right_paren; 
}


ostream& operator <<(ostream &outs, const RightParen &rp)
{
    outs << rp._right_paren;
    return outs;
}

ostream& operator <<(ostream& outs, const RightParen* rp)
{
    outs << rp->_right_paren;
    return outs;
}
