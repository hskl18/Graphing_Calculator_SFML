#include "function.h"

Function::Function(string func)
{
    this->_function = func;
    this->set_args();
}

Function::~Function(){}   

const int Function::tokenType() const
{
    return FUNCTION;
}



const int Function::precedence() const
{
    return P_4;
}


void Function::set_args()
{
    if(this->_function == "max" || this->_function == "min")
    {
        this->_args = 2;
        return;
    }
    this->_args = 1;   
}

double Function::evaluate(double num, double optional)
{
    
    if(this->_function == "sin") return sin(num);
    
    if(this->_function == "cos") return cos(num);
    
    if(this->_function == "tan") return tan(num);
    
    if(this->_function == "cot") return (1/tan(num));

    if(this->_function == "sec") return (1/cos(num));

    if(this->_function == "csc") return (1/sin(num));

    if(this->_function == "arcsin") return asin(num);
    
    if(this->_function == "arccos") return acos(num);
    
    if(this->_function == "arctan") return atan(num);
    
    if(this->_function == "sinh") return sinh(num);
    
    if(this->_function == "cosh") return cosh(num);
    
    if(this->_function == "tanh") return tanh(num);
    
    if(this->_function == "ln") return log(num);
    
    if(this->_function == "log") return log10(num);
    
    if(this->_function == "abs") return fabs(num);
    
    if(this->_function == "max") return max(num, optional);
    
    if(this->_function == "min") return min(num, optional);
    
    return 0;
}
     


void Function::Print(ostream& outs) const
{
    outs << this->_function; 
}


ostream& operator <<(ostream &outs, const Function &func)
{
    outs << func._function;
    return outs;
}

ostream& operator <<(ostream& outs, const Function* func)
{
    outs << func->_function;
    return outs;
}
