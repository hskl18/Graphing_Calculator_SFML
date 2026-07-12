#include "rpn.h"


RPN::RPN()
{
    this->_error = false;
}

RPN::RPN(const Queue<Token*>& input_q)
{
    this->_error = false;
    this->_queue = input_q;
}

  
// save input_q to member variable queue
void RPN::set_input(const Queue<Token *> &input_q)
{
    this->_queue = input_q;
}


// return result from member variable queue
double RPN::operator()(double value)
{
    return this->rpn(value);
}

// called by operator ()
double RPN::rpn(double value)
{
    this->_error = false;
    Stack<double> output;
    Queue<Token*> queue_cache = this->_queue;
    while(!queue_cache.empty())
    {
        Token* token = queue_cache.pop();
        if(token->tokenType() == INTEGER)
        {
            Integer* int_temp = static_cast<Integer*>(token);
            double val = int_temp->get_integer();
            output.push(val);
            continue;
        }
        if(token->tokenType() == VAR)
        {
            output.push(value);
            continue;
        }
        if(token->tokenType() == OPERATOR)
        {
            Operator* operator_temp = static_cast<Operator*>(token);
            if (operator_temp->get_args() == 1)
            {
                if (output.empty())
                {
                    this->_error = true;
                    return -1;
                }
                output.push(-output.pop());
                continue;
            }
            if(output.size() < OPERATOR_NUM_NEEDED)
            {
                this->_error = true;
                return -1;
            }
            double secondNum = output.pop();
            double firstNum = output.pop();
            double result = this->evaluate(firstNum, secondNum, operator_temp->get_operator());
            output.push(result);
            continue;
        }
        if(token->tokenType() == FUNCTION)
        {
            Function* function_temp = static_cast<Function*>(token);
            if(output.empty()) 
            {
                this->_error = true;
                return -1;
            }
            double num1 = output.pop();
            double num2 = 0;
            if(function_temp->get_args() == 2)
            {
                if(output.empty())
                {
                    this->_error = true;
                    return -1;
                }
                num2 = output.pop();
            }
            double val = function_temp->evaluate(num1, num2);
            output.push(val);
            continue;
        }
    }
    if(output.size() != 1) 
    {
        this->_error = true;
        return -1;
    }
    return output.top();
}

double RPN::evaluate(double num1, double num2, string operation)
{
    if(operation == "+") return num1 + num2;
    if(operation == "-") return num1 - num2;
    if(operation == "*") return num1 * num2;
    if(operation == "/") return num1 / num2;
    return pow(num1, num2);
}



























