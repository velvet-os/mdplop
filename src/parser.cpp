#include <vector>
#include <string>
#include <variant>
#include <iostream>
#include <sstream>

class element{
    public:
    enum elementType{
        text,
        line,
        codeblock,
        bulletpoint,
        header,
        bold,
        italic,
        special,
    } type;
    std::vector<element> elements;
    std::string content;
    element(std::string& text,int32_t& index){
        
    }
    element(element::elementType type){
        this->type = type;
    }
    element(element::elementType type,std::string content){
        this->type = type;
        this->content = content;
    }
    element& addElement(element::elementType type){
        this->elements.push_back(element(type));
        return elements[elements.size()-1];
    }
    element& setContent(std::string content){
        this->content = content;
    }
    void dump(std::stringstream& stream,int tabin = 0){

    }
    std::string toHTML(){
        std::stringstream ss;
        dump(ss);
        return ss.str();
    }
};