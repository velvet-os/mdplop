#include <iostream>
#include "fs.hpp"
#include "parser.hpp"

std::string def;

std::string generateHTML(std::string path){
    std::cout << "openning " << path << std::endl;
    std::string content = readFile(path);

    std::cout << "parsing " << path << std::endl;
    return def.replace(def.find("%Content%"),10,parse(content));
}

int main(int argc,char** args){
    std::string in("../test");
    if(argc>1)
        in = args[1];
    
    def = readFile(in+"/default.html");

    std::cout << "default" << std::endl;

    std::cout << def << std::endl;

    
    std::cout << "[Parsing]" << std::endl;
    writeFile(in+"/test.html",generateHTML(in+"/test.md"));//hack
    std::cout << "saving" << std::endl;
}