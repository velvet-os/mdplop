#include <iostream>
#include "fs.hpp"
#include "parser.hpp"

std::string def;

std::string generateHTML(std::string content){
    
    return def.replace(def.find("%Content%"),10,parse(content));
}

int main(int argc,char** args){
    std::string in("../test");
    if(argc>1)
        in = args[1];
    
    def = readFile(in+"/default.html");

    std::cout << "default" << std::endl;

    std::cout << def << std::endl;

    std::string testfile = readFile(in+"/test.md");

    writeFile(in+"/test.html",generateHTML(testfile));
    std::cout << "saving" << std::endl;
}