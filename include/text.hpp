#pragma once

#include <string>
#include <vector>
#include <filesystem>

class stringAnalizer {
    std::string data;
    int index = 0;
public:
    stringAnalizer(std::string data);
    //get next character
    char pop();
    //skip next x characters
    void pop(int count);
    char seek(int offset = 0);
    bool seekMatch(std::string);
    std::string getData();
};

std::vector<std::string> split(std::string text,char c);