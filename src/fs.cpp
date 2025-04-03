#include "fs.hpp"
#include "text.hpp"

std::string readFile(std::string path) {
    std::ifstream file(path);
    std::stringstream stream;
    stream << file.rdbuf();
    return stream.str();
}

void writeFile(std::string path,std::string data) {
    std::ofstream file(path);
    file << data;
    file.close();
}

file::file(std::string path) {
    this->path = path;
}

std::string file::getExtension(){
    auto el = split(path,'.');
    return el[el.size()-1]; 
};
std::string file::getFilename(){
    return std::filesystem::path(path).filename();
};

std::string file::readContent() {
    return readFile(path);
}

dir::dir(std::string path) {
    if (path.ends_with("/"))
        path.pop_back();

    this->path = path;

    for (const auto &element : std::filesystem::directory_iterator(path)) {
        if (element.is_directory()) {
            dirs.push_back(element.path().filename());
        }
        if (element.is_regular_file()) {
            files.push_back(element.path().filename());
        }
    }
}

void dir::forEachFile(std::function<void(file)> fn) {
    for (const std::string &f : files) {
        fn(file(path + "/" + f));
    }
}

void dir::forEachFileExt(std::function<void(file)> fn, std::string extension) {
    for (const std::string &f : files) {
        if (f.ends_with(extension))
            fn(file(path + "/" + f));
    }
}

void dir::forEachDir(std::function<void(dir)> fn) {
    for (const std::string &d : dirs) {
        fn(dir(path + "/" + d));
    }
}
