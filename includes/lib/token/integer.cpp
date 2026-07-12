#include "integer.h"

Integer::Integer(double num)
{
    this->_integer = num;
}

Integer::~Integer(){}   

int Integer::tokenType() const
{
    return INTEGER;
}

double Integer::get_integer() const
{
    return this->_integer;
}


void Integer::Print(ostream& outs) const
{
    outs << this->_integer; 
}


ostream& operator <<(ostream &outs, const Integer &num)
{
    outs << num.get_integer();
    return outs;
}

ostream& operator <<(ostream& outs, const Integer* num)
{
    outs << num->_integer;
    return outs;
}
