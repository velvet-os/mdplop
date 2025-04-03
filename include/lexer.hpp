#pragma once
#include <string>
#include <vector>
class token{
    public:
        enum tokenType{
            openSqrBracket,
            closeSqrBracket,
            openCircleBracket,
            closeCircleBracket,
            apostrophe,
            triApostrophe,
            star,//*
            triStar,
            flor,
            hashtag,
            dot,
            text,
            word,
            newline
        } type;
        token(tokenType t,std::string d = "");
        std::string data;
        void print();
};

std::vector<token> lex(std::string content);