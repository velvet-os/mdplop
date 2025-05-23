#include <lexer.hpp>
#include <iostream>

class pars
{
public:
    int i = 0;

    std::string out;
    std::vector<token> tokens;

    int lastoperated = 0;

    token pop()
    {
        if (i >= tokens.size())
            return token(token::eof);
        i++;

        return tokens[i - 1];
    }
    token seek(int off = 0)
    {
        if (i + off < 0)
            return token(token::newline, "\\n");
        if (i + off >= tokens.size())
            return token(token::eof);
        return tokens[i + off];
    }

    void htmlOpen(std::string tag)
    {
        out += "<" + tag + ">";
    }
    void htmlClose(std::string tag)
    {
        out += "</" + tag + ">";
    }
    void htmlOpen(std::string tag, std::string id)
    {
        out += "<" + tag + " id=\"" + id + "\">";
    }
    void htmlImage(std::string src, std::string alt)
    {
        out += "<img src=\"" + src + "\" alt=\"" + alt + "\">";
    }
    void htmlLink(std::string text, std::string url)
    {
        out += "<a href=\"" + url + "\">" + text + "</a>";
    }

    int cursive = 0; // 0 no, 1 outer, 2 inner
    int bold = 0;    // 0 no, 1 outer, 2 inner
    bool special = false;
    bool header = false;
    pars(std::vector<token> ts)
    // memory getting copied all over the place
    // but at least there won't be a seqfault
    {
        tokens = ts;
        htmlOpen("p");
        while (true)
        {
            token t = pop();
            if (t.type == token::eof)
                break;

            switch (t.type)
            {
            case token::newline:
                parseNewLine();
                break;
            case token::exclamation:
                if (!tryParseImage()) // is it an image?
                    out += t.data;    // just append the !
                break;
            case token::hashtag:
                if (!tryParseHashtag())
                    out += t.data;
                break;
            case token::triApostrophe:
                parserTriApostrophy();
                break;
            case token::openSqrBracket:
                parseLink();
                break;
            case token::openFlor: // add special support
                parseOpenFlor();
                break;
            case token::closeFlor: // add special support
                parseCloseFlor();
                break;
            case token::linkliteral:
                htmlLink(t.data,t.data);
                break;
            case token::star:
                parseStar();
                break;
            default:
                out += t.data;
            }
        }

        closeMode();
        htmlClose("p");
    }

    void parseOpenFlor()
    {
        if (tryParseSpecial())
            return;

        if (cursive)
        {
            warning("double open cursive");
        }
        else
        {
            cursive = bold + 1;
            htmlOpen("cur");
        }
    }
    void parseCloseFlor()
    {

        if (!cursive)
        {
            out+="_";
            return;
        }
        else
        {
            if (bold == 2)
                error("incorrect _/* nesting");

            if (special)
                htmlClose("special");
            else
                htmlClose("cur");

            special = false;

            cursive = 0;
        }
    }
    void parseLink()
    {
        std::string text = "";
        std::string url = "";
        bool local = false;
        while (true)
        {
            token t = pop();
            if (t.type == token::newline)
                error("expected ] after [" + text);

            if (t.type == token::eof)
                error("expected ] after [" + text);

            if (t.type == token::closeSqrBracket)
                break;

            text += t.data;
        }

        if (text == "")
            warning("[] is empty");

        if (seek().type != token::openCircleBracket)
        {
            out += "[" + text + "]";
            return;
        }
        pop();//(

        if (seek().type == token::dot)
            local = true;

        while (true)
        {
            token t = pop();
            if (t.type == token::newline)
                error("expected ) after [" + text + "](" + url);

            if (t.type == token::eof)
                error("expected ) after [" + text + "](" + url);

            if (t.type == token::closeCircleBracket)
                break;

            if (t.type == token::dot && seek().type == token::word && seek(1).type == token::closeCircleBracket)
            {
                if (local && seek().data == "md")
                {
                    pop(); // md
                    url += ".html";
                    continue;
                }
            }

            url += t.data;
        }

        htmlLink(text, url);
    }
    bool tryParseHashtag()
    {
        if (seek(-2).type != token::newline)
            return false;

        int lvl = 0;
        while (seek().type == token::hashtag)
        {
            lvl++;
            pop();
        }

        htmlOpen("header", "l" + std::to_string(lvl));
        header = true;
        return true;
    }
    bool tryParseSpecial()
    {
        if (seek(-2).type != token::newline || seek().type != token::word || seek(1).type != token::dot)
            return false;
        std::string type = pop().data;
        htmlOpen("special", type);
        htmlOpen("bold");
        out += type;
        out += ".";
        htmlClose("bold");
        pop(); //.

        cursive = bold + 1;
        special = true;

        return true;
    }
    void parseStar()
    {

        if (!bold)
        {
            bold = cursive + 1;
            htmlOpen("bold");
        }
        else
        {
            if (cursive == 2)
                error("incorrect _/* nesting");
            htmlClose("bold");
            bold = 0;
        }
    }
    void openMode()
    {
        if (bold || cursive)
        {
            if (bold > cursive)
            {
                if (cursive)
                    htmlOpen("cur");
                htmlOpen("bold");
            }
            else
            {
                if (bold)
                    htmlClose("bold");
                htmlOpen("cur");
            }
        }
    }
    void closeMode()
    {
        if (special)
            error("special section has to be one line long");

        if (bold || cursive)
        {
            if (bold > cursive)
            {

                htmlClose("bold");
                if (cursive)
                    htmlClose("cur");
            }
            else
            {
                htmlClose("cur");
                if (bold)
                    htmlClose("bold");
            }
        }
        if (header)
            htmlClose("header");
        header = false;
    }
    void parseBool()
    {
        htmlClose("bold");
    }
    void parseNewLine()
    {
        closeMode();
        htmlClose("p");

        htmlOpen("p");
        openMode();
    }

    void parserTriApostrophy()
    {
        lastoperated = i-1;
        token::tokenType next = seek().type;
        token::tokenType next2 = seek(1).type;
        //-1```
        token::tokenType prev = seek(-2).type;//\n
        if (prev==token::newline&&((next == token::word && next2 == token::newline) || next == token::newline))
        {
            parseBlock();
            return;
        }

        htmlOpen("raw");

        token t = seek();
        while ((t = pop()).type != token::triApostrophe)
        {
            if (t.type == token::eof)
            {
                error("``` never ended");
            }

            if (t.type == token::newline)
            {
                htmlClose("raw");
                closeMode();
                htmlClose("p");
                htmlOpen("p");
                openMode();
                htmlOpen("raw");
            }
            else
                out += t.data;
        }
        htmlClose("raw");
    }
    void parseBlock()
    {
        closeMode();
        if(seek().type==token::word)
            htmlOpen("block", pop().data);
        else
            htmlOpen("block");

        pop();

        token t = seek();
        while (true)
        {
            t = pop();

            if (t.type == token::triApostrophe)
                break;

            if (t.type == token::newline)
            {
                out += "\n";
            }
            else
                out += t.data;
        }

        htmlClose("block");
        openMode();
    }
    bool tryParseImage()
    {
        if (seek().type != token::openSqrBracket)
            return false;
        pop(); //[

        // std::cout << "paring image " << seek().data << std::endl;

        std::string title = "";
        while (true)
        {
            token t = pop();
            if (t.type == token::newline)
                error("expected ] after [" + title);

            if (t.type == token::eof)
                error("expected ] after [" + title);

            if (t.type == token::closeSqrBracket)
                break;

            title += t.data;
        }

        if (seek().type != token::openCircleBracket)
        {
            out += "![" + title + "]";
            warning("begining of image declaration without a link ![" + title + "] treating as text");
            return true;
        }
        pop();//(

        std::string url = "";

        while (true)
        {
            token t = pop();
            if (t.type == token::closeCircleBracket)
                break;

            if (t.type == token::newline || t.type == token::eof)
                error("missing ) after " + seek(-2).data);

            url += t.data;
        }

        if (url == "")
            error("no url inside [" + title + "](");

        htmlImage(url, title);

        return true;
    }

    void warning(std::string warning)
    {
        std::cout << "[warning] " << warning << std::endl;
    }
    void error(std::string error)
    {
        std::cout << "[data dump] " << out;
        std::cout << "[token dump] last operated" << std::endl;
        for (int i =lastoperated;i<tokens.size();i++)
        {
            tokens[i].print();
        }
        std::cout << "[error] " << error << std::endl;
        throw std::runtime_error(error);
    }
};

std::string parse(std::string content)
{
    auto tokens = lex(content);

    if(getenv("DEBUG")!=nullptr){
        for(auto token : tokens) token.print();
    }

    return pars(tokens).out;
    
    return "";
}