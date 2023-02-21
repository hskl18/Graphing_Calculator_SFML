#ifndef OPERATOR_H
#define OPERATOR_H


#include "token.h"
#include "constants.h"

using namespace std;


class Operator : public Token
{
private:
    string _operator;

    // true stands left associative, false stands right associative
    bool _associativity;
    void set_associativity();
public:
    // CTORS
    Operator(string op);
    ~Operator();

    // virtuals
    const int tokenType() const override;
    void Print(ostream &outs) const override;
    const int precedence() const override;
    bool get_associativity(){return this->_associativity;}
    // member functions
    string get_operator() const;
    
    // couts
    friend ostream& operator <<(ostream &outs, const Operator &op);
    friend ostream& operator <<(ostream& outs, const Operator* op);
    
};

#endif // !OPERATOR_H