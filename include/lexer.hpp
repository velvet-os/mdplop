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
            exclamation,
            triStar,
            flor,
            hashtag,
            dot,
            dash,
            relaticeLink,
            text,
            word,
            newline,
            linkliteral,
            eof//end of file
        } type;
        token(tokenType t,std::string d = "");
        std::string data;
        void print();
};

std::vector<token> lex(std::string content);