#include <iostream>
#include "fs.hpp"
#include <exception>
#include "parser.hpp"

std::string input = "../test";
std::string output = "../output";
std::string def;

void generateHTML(std::string path)
{
    if (!path.ends_with(".md"))
        throw std::runtime_error(path + " is not md file");

    std::cout << "parsing " << path << std::endl;
    std::string content = readFile(path);

    std::string out = def;

    std::string title = path;
    title = title.substr(path.find_last_of("/") + 1);
    title = title.substr(0, title.size() - 3);
    title[0] = std::toupper(title[0]);
    std::cout << title << std::endl;

    out.replace(out.find("%Title%"), 7, title);
    out.replace(out.find("%Content%"), 9, parse(content));

    path.replace(path.size() - 3, 3, ".html");
    path = path.replace(0, input.length(), output);


    file(path).createDirectory();
    std::cout << "saving " << path << std::endl;
    writeFile(path, out);
}
void move(file f)
{
    std::string path = f.path;
    path = path.replace(0, input.length(), output);
    
    
    file(path).remove();//in case there already is one
    std::cout << "copying "+f.path+" to "+path << std::endl;
    f.copy(path);
}

void handleFile(file f)
{
    generateHTML(f.path);
}

void handleDir(dir d)
{
    d.forEachFile(move);
    d.forEachFileExt(handleFile, ".md");
    d.forEachDir(handleDir);
}

int main(int argc, char **args)
{
    if (argc > 2)
    {
        input = args[1];
        output = args[2];
    }
    else
    {
        std::cout << "provide input and output" << std::endl;
        return 0;
    }

    std::cout << "loading default html " + input + "/default.html" << std::endl;
    def = readFile(input + "/default.html");

    handleDir(input); // and now recursion
}