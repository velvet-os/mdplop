#include "text.hpp"

stringAnalizer::stringAnalizer(std::string data) {
    this->data = data;
}

char stringAnalizer::pop() {
    if (index >= data.length()) {
        return 0;
    }
    index++;
    return data.at(index - 1);
}

void stringAnalizer::pop(int count) {
    for(int i = 0;i<count;i++){
        pop();
    }
}
//next char to be popped
char stringAnalizer::seek(int offset) {
    if (index + offset < 0 || index + offset >= data.length()) {
        return 0;
    }
    return data.at(index + offset);
}
//next characters to be poped
bool stringAnalizer::seekMatch(std::string match){
    for(int i = 0;i<match.size();i++){
        if(match[i]!=seek(i))
            return false;
    }
    return true;
}

std::vector<std::string> split(std::string text,char c) {
    std::vector<std::string> out;
    std::string tmp = "";

    for(const char ch : text){
        if(ch==c){
            out.push_back(tmp);
            tmp = "";
        }else{
            tmp += ch;
        }
    }

    return out;
}

std::string stringAnalizer::getData(){
    return data;
};