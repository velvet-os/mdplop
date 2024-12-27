#include <fstream>
#include <string>
#include <sstream>

std::string readFile(std::string path){
    std::ifstream file(path);
    std::stringstream stream;
    stream << file.rdbuf();
    return stream.str();
}