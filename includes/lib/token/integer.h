#ifndef INTEGER_H
#define INTEGER_H


#include "token.h"
#include "constants.h"
using namespace std;


class Integer : public Token
{
private:
    double _integer;
public:
    Integer(double num);
    ~Integer();
    const int tokenType() const override;
    const double get_integer() const;
    void Print(ostream &outs) const override;
    friend ostream& operator <<(ostream &outs, const Integer &num);
    friend ostream& operator <<(ostream& outs, const Integer* num);
    
};

#endif // !LeftParen