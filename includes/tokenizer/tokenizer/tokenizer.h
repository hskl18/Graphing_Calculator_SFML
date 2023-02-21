#ifndef TOKENIZER_H
#define TOKENIZER_H
#include "../../lib/queue/MyQueue.h"
#include "../../lib/stack/MyStack.h"
#include "../../lib/token/token.h"
#include "../../lib/token/function.h"
#include "../../lib/token/integer.h"
#include "../../lib/token/operator.h"
#include "../../lib/token/leftparen.h"
#include "../../lib/token/rightparen.h"
#include "../tokenizer_functions/tokenizer_functions.h"
#include "../../lib/token/variable.h"



class Tokenizer
{
private:
    // holders
    Queue<string> _tokens;
    Queue<int> _types;
    // input
    string _input;

    // output
    Queue<Token*> _output;

    // private helper
    void tokenize(string input);
    string remove_space(string str);

public:
    Tokenizer(string input);
    ~Tokenizer();
    void set_input(string input);
    Queue<Token*> infix();
   
  
};


#endif // !TOKENIZER_H
