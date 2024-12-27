#include <string>
class stringAnalizer{
    std::string data;
    int index = 0;
    public:
    stringAnalizer(std::string date){
        this->data = data;
    }
    char pop(){
        if(index>=data.length()){
            return 0;
        }
        index++;
        
        return data.at(index-1);
    }
    char seek(int offset = 0){
        if(index+offset<0||index+offset>=data.length())
            return 0;
        
        return data.at(index+offset);
    }

};  