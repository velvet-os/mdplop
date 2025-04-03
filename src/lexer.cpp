#include "lexer.hpp"
#include "text.hpp"
#include <iostream>
#include <functional>

token::token(tokenType t, std::string d)
{
    type = t;
    data = d;
};

void token::print()
{
    std::cout << type << ": " << this->data << std::endl;
};

std::vector<token> lex(std::string content)
{
    std::vector<token> out;
    stringAnalizer data(content);
    char c = 0;
    std::string tmp;

    std::function<void()> flush = [&tmp,&out](){
        //not a fan of this solution but it needs to be dry
        if(tmp!=""){
            if(tmp.contains(' '))
                out.push_back(token(token::text,tmp));
            else
                out.push_back(token(token::word,tmp));
        }
        tmp = "";
    };
    

    while (true)
    {
        c = data.pop();
        switch (c)
        {
        case '[':
            flush();
            out.push_back(token(token::openSqrBracket,"["));
            continue;
        case ']':
            flush();
            out.push_back(token(token::closeSqrBracket,"]"));
            continue;
        case '(':
            flush();
            out.push_back(token(token::openCircleBracket,"("));
            continue;
        case ')':
            flush();
            out.push_back(token(token::closeCircleBracket,")"));
            continue;
        case '`':
            flush();
            if(data.seekMatch("``")){//are there 2 more?
                data.pop(2);
                out.push_back(token(token::triApostrophe,"```"));
            }else{
                out.push_back(token(token::apostrophe,"`"));
            }
            continue;
        case '*':
            flush();
            if(data.seekMatch("**")){//are there 2 more?
                data.pop(2);
                out.push_back(token(token::triApostrophe,"***"));
            }else{
                out.push_back(token(token::apostrophe,"*"));
            }
            continue;
        case '_':
            flush();
            out.push_back(token(token::flor,"_"));
            continue;
        case '#':
            flush();
            out.push_back(token(token::hashtag,"#"));
            continue;
        case '.':
            flush();
            out.push_back(token(token::dot,"."));
            continue;
        case '\n':
            flush();
            out.push_back(token(token::newline,"\\n"));
            continue;
        case 0:
            flush();
            return out;
        }
        tmp += c;
    }
};