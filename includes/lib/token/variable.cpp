#include "variable.h"

Variable::Variable()
{
    this->_x = 'x';
}

Variable::~Variable(){}   

const int Variable::tokenType() const
{
    return VAR;
}




void Variable::Print(ostream& outs) const
{
    outs << this->_x; 
}


ostream& operator <<(ostream &outs, const Variable &x)
{
    outs << x._x;
    return outs;
}

ostream& operator <<(ostream& outs, const Variable* x)
{
    outs << x->_x;
    return outs;
}
