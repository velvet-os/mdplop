#include <lexer.hpp>

std::string parse(std::string content){
    auto tokens = lex(content);
    for(token t : lex(content)){
        t.print();
    }
    return "";
}