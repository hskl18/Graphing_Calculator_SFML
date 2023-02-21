#ifndef FUNCTION_H
#define FUNCTION_H


#include "token.h"
#include "constants.h"
#include "../queue/MyQueue.h"
using namespace std;


class Function : public Token
{
private:
    string _function;
    int _args;

    void set_args();
public:
    Function(string func);
    ~Function();
    const int tokenType() const override;
    const int precedence() const override;
    void Print(ostream &outs) const override;

    // member functions
    double evaluate(double num, double optional = 0);
    int get_args(){return this->_args;}
    friend ostream& operator <<(ostream &outs, const Function &func);
    friend ostream& operator <<(ostream& outs, const Function* func);
    
};

#endif // !LeftParen