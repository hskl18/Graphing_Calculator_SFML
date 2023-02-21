#include "tokenizer.h"

void Tokenizer::tokenize(string input)
{
    this->_tokens.clear();
    this->_input.clear();
    this->_types = _tokenize(input, this->_tokens);
}
Tokenizer::Tokenizer(string input)
{
    this->_input = input;
    if(this->_input != "") this->tokenize(this->_input);
}
void Tokenizer::set_input(string input)
{
    this->_input = input;
    if(this->_input != "") this->tokenize(this->_input);
}


string Tokenizer::remove_space(string str)
{
    // removing white space so can be tokenized
    string res = "";
    for(int i = 0; i < str.length(); ++i)
    {
        if(str[i] == ' ') continue;
        res += str[i];
    }
    return res;
}




Tokenizer::~Tokenizer()
{
    this->_output.clear();
}

Queue<Token*> Tokenizer::infix()
{

    while(!this->_tokens.empty())
    {
        string tk = this->_tokens.pop();
        int type = this->_types.pop();
        Token* token;
        if(type == INTEGER)
        {
            token = new Integer(stod(tk));
            //cout << token << " is an integer" << endl;
            this->_output.push(token);
            continue;
        }
        
        if(type == VAR)
        {
            token = new Variable();
            this->_output.push(token);
            continue;
        }

        if(type == FUNCTION)
        {
            token = new Function(tk);
            //cout << token << " is a function" << endl;
            this->_output.push(token);
            continue;
        }
        if(type == LPAREN)
        {
            token = new LeftParen();
            //cout << token << " is a Left Paren" << endl;
            this->_output.push(token);
            continue;
        }

        if(type == RPAREN)
        {
            token = new RightParen();
            //cout << token << " is a Right Paren" << endl;
            this->_output.push(token);
            continue;
        }

        if(type == OPERATOR)
        {
            token = new Operator(tk);
            //cout << token << " is an Operator" << endl;
            this->_output.push(token);
            continue;
        }
    }
    
    Queue<Token*> infix = this->_output;
    return infix;
}
