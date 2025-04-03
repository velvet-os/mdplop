#pragma once

#include <string>
#include <vector>
#include <functional>
#include <sstream>
#include <fstream>
#include <filesystem>

std::string readFile(std::string path);
void writeFile(std::string path,std::string data);

class file {
public:
    std::string path;
    file(std::string path);
    std::string readContent();
    std::string getExtension();
    std::string getFilename();
};

class dir {
public:
    std::string path;
    std::vector<std::string> files;
    std::vector<std::string> dirs;

    dir(std::string path);
    void forEachFile(std::function<void(file)> fn);
    void forEachFileExt(std::function<void(file)> fn, std::string extension);
    void forEachDir(std::function<void(dir)> fn);
};
