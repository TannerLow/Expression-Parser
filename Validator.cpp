#include "Validator.h"

using namespace std;

Validator::Validator(vector<string> Tokens){
    tokens = Tokens;
    it = tokens.begin();
}

Validator::Validator(vector<string> Tokens, vector<string>::iterator start){
    tokens = Tokens;
    it = start;
}

void Validator::reset(vector<string> Tokens){
    tokens = Tokens;
    it = tokens.begin();
}

bool Validator::endOfExpr(){
    return it >= tokens.end();
}

bool Validator::terminal(string token){
    if(*it++ == token) return true;
    else it--;
    return false;
}

bool Validator::library(){
    vector<string>::iterator save = it;
    return (it = save, function()  && library())
        || (it = save, statement() && library())
        || (it = save, endOfExpr());
}

bool Validator::function(){
    vector<string>::iterator save = it;
    return (it = save, terminal("Identifier") && terminal("Identifier") && terminal("(") && terminal(")") && statement());
}

bool Validator::statement(){
    vector<string>::iterator save = it;
    return (it = save, terminal(";"))
        || (it = save, terminal("{")     && statement1())
        || (it = save, terminal("if")    && terminal("(") && expression(")") && statement())
        || (it = save, terminal("while") && terminal("(") && expression(")") && statement())
        || (it = save, terminal("for")   && terminal("(") && statement()     && expression(";") && expression(")") && statement())
        || (it = save, terminal("return")&& expression(";"))
        || (it = save, expression(";"));
}

bool Validator::statement1(){
    while(!terminal("}")){
        if(endOfExpr()) return false;
        else if(statement());
        else return false;
    }
    return true;
}

bool Validator::expression(string endToken){
    vector<string>::iterator save = it;
    return (it = save, expression1() && terminal(endToken))
        || (it = save, terminal("Identifier") && expression4() && terminal(endToken));
}

bool Validator::expression(){
    vector<string>::iterator save = it;
    return (it = save, expression1() && endOfExpr())
        || (it = save, terminal("Identifier") && expression4() && endOfExpr());
}

bool Validator::expression1(){
    return expression2() && expression3();
}

bool Validator::expression2(){
    if     (terminal("INT_LITERAL")) return true;
    else if(terminal("Identifier"))  return true;
    else if(terminal("("))           return expression1() && terminal(")");
    else                             return false;
}

bool Validator::expression3(){
    if     (terminal("+")) return expression1();
    else if(terminal("-")) return expression1();
    else if(terminal("*")) return expression1();
    else if(terminal("/")) return expression1();
    else if(terminal("%")) return expression1();
    else                   return true;
}

bool Validator::expression4(){
    if     (terminal("=") ) return expression1();
    else if(terminal("+=")) return expression1();
    else if(terminal("-=")) return expression1();
    else if(terminal("*=")) return expression1();
    else if(terminal("/=")) return expression1();
    else if(terminal("%=")) return expression1();
    else                    return false;
}
