#include <lexer.hpp>

class pars
{
public:
    int i = 0;

    std::string out;
    std::vector<token> tokens;

    token pop(){
        i++;
        return tokens[i-1];
    }

    void htmlOpen(std::string tag){
        out += "<"+tag+">";
    }
    void htmlClose(std::string tag){
        out += "</"+tag+">";
    }
    void htmlClose(std::string tag,std::string id){
        out += "</"+tag+" id=\""+id+"\">";
    }
    void htmlImage(std::string src,std::string alt){
        out += "<img src=\""+src+"\" alt=\""+alt+"\">";
    }
    void htmlLink(std::string text,std::string url){

    }

    pars(std::vector<token> ts)
    // memory getting copied all over the place
    // but at least there won't be a seqfault
    {
        tokens = ts;
        htmlOpen("p");
        while (true)
        {
            token t = pop();
            if(t.type == token::eof)
                break;
            
            switch (t.type){
                case token::newline:
                    htmlClose("p");
                    htmlOpen("p");
                    break;
                default:
                    out += t.data;
            }
            
            

            
        }

        htmlClose("p");
    }
};

std::string parse(std::string content)
{
    auto tokens = lex(content);

    return pars(tokens).out;
}