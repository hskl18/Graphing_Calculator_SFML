#include "shunting_yard.h"


ShuntingYard::ShuntingYard(){}

ShuntingYard::ShuntingYard(const Queue<Token*>& input_q)
{
    this->_queue = input_q;
    this->_error = this->_queue.empty() ? true : false;
}
  

void ShuntingYard::infix(const Queue<Token*>& input_q)
{
    this->_queue = input_q;
    this->_error = this->_queue.empty() ? true : false;
}
  
// generate postfix queue from infix queue
Queue<Token*> ShuntingYard::postfix()
{
    return this->shunting_yard();
}


Queue<Token*> ShuntingYard::postfix(const Queue<Token*>& input_q)
{
    this->_queue = input_q;
    return this->shunting_yard();
}


// called by postfix() 
Queue<Token*> ShuntingYard::shunting_yard()
{
    Queue<Token*> input_queue = this->_queue;
    Queue<Token*> output_queue;
    Stack<Token*> operator_stack;
    if(input_queue.empty())
    {
        this->_error = true;
        return output_queue;
    }

    while(!input_queue.empty())
    {
        Token* token = input_queue.pop();

        if(token->tokenType() == INTEGER)
        {
            //cout << "Integer" << endl;
            output_queue.push(token);
            continue;
        }
        if(token->tokenType() == VAR)
        {
            output_queue.push(token);
            continue;
        }
        if(token->tokenType() == FUNCTION)
        {
            //cout << "Function" << endl;
            operator_stack.push(token);
            continue;
        }
        if(token->tokenType() == LPAREN)
        {
            //cout << "LeftParen" << endl;
            operator_stack.push(token);
            continue;
        }
        if(token->tokenType() == RPAREN)
        {
            //cout << "RightParen" << endl;
            while(!operator_stack.empty())
            {
                if(operator_stack.top()->tokenType() == LPAREN) break;
                if(operator_stack.empty()) 
                {
                    this->_error = true;
                    return output_queue;
                }
                Token* tk = operator_stack.pop();
                output_queue.push(tk);
            }
            if(!operator_stack.empty() && operator_stack.top()->tokenType() != LPAREN) 
            {
                this->_error = true;
                return output_queue;
            }
            if(operator_stack.empty())
            {
                this->_error = true;
                return output_queue;
            }
            operator_stack.pop();

            if(!operator_stack.empty() && operator_stack.top()->tokenType() == FUNCTION)
            {
                Token* tk = operator_stack.pop();
                output_queue.push(tk);
            }
            continue;
        }
        if(token->tokenType() == OPERATOR)
        {
            //cout << "Operator" << endl;
            Operator* op = static_cast<Operator*>(token);
            while(!operator_stack.empty() && operator_stack.top()->tokenType() != LPAREN)
            {
                Operator* top_token = static_cast<Operator*>(operator_stack.top());
                bool check_greater_precedence = (operator_stack.top()->precedence() > token->precedence());
                bool check_equal_precedence = ((operator_stack.top()->precedence() == token->precedence()) && op->get_associativity());
                if(!(check_greater_precedence || check_equal_precedence)) break;
                Token* tk = operator_stack.pop();
                output_queue.push(tk);
            }
            operator_stack.push(token);
            continue;
        }

    }
    while(!operator_stack.empty())
    {
        if(operator_stack.top()->tokenType() == LPAREN) 
        {
            this->_error = true;
            return output_queue;
        }
        Token* tk = operator_stack.pop();
        output_queue.push(tk);
    }
    return output_queue;
}










