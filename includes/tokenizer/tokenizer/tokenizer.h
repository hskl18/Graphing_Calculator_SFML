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
#include "../../lib/token/comma.h"
#include "../tokenizer_functions/tokenizer_functions.h"
#include "../../lib/token/variable.h"
#include <memory>
#include <vector>



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
    vector<unique_ptr<Token>> _owned_tokens;

    // private helper
    void tokenize(const string& input);

public:
    explicit Tokenizer(const string& input);
    ~Tokenizer() = default;
    Tokenizer(const Tokenizer&) = delete;
    Tokenizer& operator=(const Tokenizer&) = delete;
    void set_input(const string& input);
    Queue<Token*> infix();
   
  
};


#endif // !TOKENIZER_H
