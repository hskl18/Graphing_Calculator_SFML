#ifndef VARIABLE
#define VARIABLE
#include "token.h"
#include "constants.h"
using namespace std;


class Variable : public Token
{
private:
    string _x;
public:
    Variable();
    ~Variable();
    const int tokenType() const override;
    void Print(ostream &outs) const override;
    friend ostream& operator <<(ostream &outs, const Variable &x);
    friend ostream& operator <<(ostream& outs, const Variable* x);
    
};

#endif