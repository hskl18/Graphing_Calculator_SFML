#include "tokenizer_functions.h"


string remove_space(string str)
{
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    return str;
}



Queue<int> _tokenize(string input, Queue<string>& tokens)
{
    input = remove_space(input);
    int index = 0;  
    Queue<int> token_types;
    while(index < input.length())
    {   
        string token = "";
        int token_type = 0;

        int idx_hold = index;
        int prev_type = (!token_types.empty()) ? token_types.back() : -1;
        _get_token(input, index, token, token_type, prev_type);

        
        if(token == "")
        {
            tokens.clear();
            token_types.clear();
            return token_types;
        }
        tokens.push(token);
        token_types.push(token_type);
    }
    return token_types;
}




void _get_token(string input, int& index, string& token, int& token_type, int previous_type)
{
    token = "";
    string tk1, tk2, tk3, tk4, tk5, tk6;

    if(input[index] == ',')
    {
        index++;
        token = ",";
        token_type = TOKEN;
        return;
    }
    bool is_func = _is_function(input, index, tk4);
    if(is_func) 
    {
        token = tk4;
        //cout <<"token is: " <<token << " and length is: "<< token.length()<< endl;
        index += token.length();
        token_type = FUNCTION;
        return;
    }
    bool is_var = _is_variable(input, index, tk6);
    if(is_var)
    {
        token = tk6;
        index += token.length();
        token_type = VAR;
        return;
    }

    bool is_num = _is_number(input, index, tk5, previous_type);
    if(is_num) 
    {
        token = tk5;
        //cout <<"token is: " <<token << " and length is: "<< token.length()<< endl;
        index += token.length();
        token_type = INTEGER;
        return;
    }


    bool is_lp = _is_left_paren(input, index, tk2);
    if(is_lp)
    {
        token = tk2;
        //cout <<"token is: " <<token << " and length is: "<< token.length()<< endl;
        index += token.length();
        token_type = LPAREN;
        return;
    } 

    bool is_rp = _is_right_paren(input, index, tk3);
    if(is_rp) 
    {
        token = tk3;
        //cout <<"token is: " <<token << " and length is: "<< token.length()<< endl;
        index += token.length();
        token_type = RPAREN;
        return;
    }


    bool is_op = _is_operator(input, index, tk1);
    if(is_op)
    {
        
        token = tk1;
        //cout <<"token is: " <<token << " and length is: "<< token.length()<< endl;
        index += token.length();
        token_type = OPERATOR;
        return;
    }

}






// check if is operator
bool _is_operator(string input, int pos, string& op)
{
    op = "";
    if(input[pos] == '+')
    {
        op = "+";
        return true;
    }
    if(input[pos] == '-')
    {
        op = "-";
        return true;
    }
    if(input[pos] == '*')
    {
        op = "*";
        return true;
    }
    if(input[pos] == '/')
    {
        op = "/";
        return true;
    }
    if(input[pos] == '^')
    {
        op = "^";
        return true;
    }
    return false;
}


// check if is left paren
bool _is_left_paren(string input, int pos, string& left_paren)
{
    left_paren = "";
    if(input[pos] == '(')
    {
        left_paren = "(";
        return true;
    }
    return false;
}



// check if is right paren
bool _is_right_paren(string input, int pos, string& right_paren)
{
    right_paren = "";
    if(input[pos] == ')')
    {
        right_paren = ")";
        return true;
    }
    return false;
}

bool _is_variable(string str, int pos, string& var)
{
    if(str[pos] == 'X' || str[pos] == 'x')
    {
        var = 'x';
        return true;
    }
    return false;
}

// check if at a certain position has a function token
bool _is_function(string str, int pos, string& func)
{

    func = "";

    if(str.substr(pos, 3) == "max")
    {
        func += "max";
        return true;
    }

    if(str.substr(pos, 3) == "min")
    {
        func += "min";
        return true;
    }

    if(str.substr(pos, 4) == "sinh")
    {
        func += "sinh";
        return true;
    }
    
    if(str.substr(pos, 4) == "cosh")
    {
        func += "cosh";
        return true;
    }
    
    if(str.substr(pos, 4) == "tanh")
    {
        func += "tanh";
        return true;
    }

    if(str.substr(pos, 3) == "sec")
    {
        func += "sec";
        return true;  
    }

    if(str.substr(pos, 3) == "csc")
    {
        func += "csc";
        return true;
    }

    if(str.substr(pos, 3) == "cot")
    {
        func += "cot";
        return true;
    }
    
    if(str.substr(pos, 3) == "sin")
    {
        func += "sin";
        return true;   
    }
    if(str.substr(pos, 3) == "cos")
    {
        func += "cos";
        return true;
    }
    
    if(str.substr(pos, 3) == "tan")
    {
        func += "tan";
        return true;
    }

    if(str.substr(pos, 6) == "arcsin")
    {
        func += "arcsin";
        return true;
    }
    

    if(str.substr(pos, 6) == "arccos")
    {
        func += "arccos";
        return true;
    }

    if(str.substr(pos, 6) == "arctan")
    {
        func += "arctan";
        return true;
    }
    

    
    if(str.substr(pos, 2) == "ln")
    {
        func += "ln";
        return true;
    }
    
    if(str.substr(pos, 3) == "log")
    {
        func += "log";
        return true;
    }
    if(str.substr(pos, 3) == "abs")
    {
        func += "abs";
        return true;
    }
    return false;
}


// check if it's a digit or a dot or not
bool _is_digit(char c)
{
    if(c <= 57 && c >= 48) return true;
    return false;
}


// check for negative sign
bool _is_unary_minus(string input, int pos, int prev_type)
{
    bool is_unary_check = ((pos == 0) || (prev_type == LPAREN) || (prev_type == OPERATOR) || (prev_type == TOKEN));
    if(input[pos] == '-' && is_unary_check) return true;
    return false;
}




// check if it's number or not
bool _is_number(string str, int pos, string& number, int prev_type)
{

    if(!_is_digit(str[pos]) && !_is_unary_minus(str, pos, prev_type)) return false;
       

    number = "";
 
    if(str[pos] == '-')
    {
        number += str[pos];
        pos++;
    }
    for(int i = pos; i < str.length(); ++i)
    {
        if(!_is_digit(str[i]) && str[i] != '.' && str[i] != ' ') break;
        
        number += str[i];
 
        pos++;
    }
    //cout << number << endl;
    if(number[0] == '-')
    {
        for(int i = 1; i < number.length(); ++i)
        {
            if(_is_digit(number[i])) return true;
        }
        return false;
    }
    return true;
}