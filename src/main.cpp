#include <iostream>
#include "fs.hpp"
#include <exception>
#include "parser.hpp"

std::string def;

void generateHTML(std::string path){
    if(!path.ends_with(".md"))
        throw std::runtime_error(path+" is not md file");
    
    std::cout << "parsing " << path << std::endl;
    std::string content = readFile(path);

    std::string out = def;

    std::string title = path;
    title = title.substr(path.find_last_of("/")+1);
    title = title.substr(0,title.size()-3);
    title[0] = std::toupper(title[0]);
    std::cout << title << std::endl;

    out.replace(out.find("%Title%"),7,title);
    out.replace(out.find("%Content%"),9,parse(content));
    
    path.replace(path.size()-3,3,".html");
    std::cout << "saving " << path << std::endl;
    writeFile(path,out);
}

void handleFile(file f){
    generateHTML(f.path);
}

void handleDir(dir d){
    d.forEachFileExt(handleFile,".md");
    d.forEachDir(handleDir);
}



int main(int argc,char** args){
    std::string in = "../test";
    if(argc>1)
        in = args[1];
    else{
        std::cout << "provide a path to working directory";
        return;
    }

    std::cout << "loading default html "+in+"/default.html" << std::endl;
    def = readFile(in+"/default.html");

    handleDir(in);//and now recursion
}