#include"common.h"
using std::cout;

#define CRLF "\r\n"


map<string,string> Header::mimeTypes;

void Header::init()
{
    mimeTypes[".xhtml"]="application/xhtml+xml";
    mimeTypes[".txt"] = "text/plain";
    mimeTypes[".rtf"] = "application/rtf";
    mimeTypes[".word"]="application/msword";
    mimeTypes[".au"]="audio/basic";
    mimeTypes[".mpeg"]="video/mpeg";
    mimeTypes[".mpg"]="video/x-msvideo";
    mimeTypes[".avi"]="video/x-msvideo";
    mimeTypes[".gz"]="application/x-gzip";
    mimeTypes[".tar"]="application/x-tar";
    mimeTypes[".css"]="text/css";
    mimeTypes[".gif"]="image/gif";
    mimeTypes[".htm"]="text/html";
    mimeTypes[".html"]="text/html";
    mimeTypes[".jpeg"]="image/jpeg";
    mimeTypes[".jpg"]="image/jpeg";
    mimeTypes[".ico"]="image/x-icon";
    mimeTypes[".js"] = "application/javascript";
    mimeTypes[".pdf"]="application/pdf";
    mimeTypes[".mp4"]="video/mp4";
    mimeTypes[".png"]="image/png";
    mimeTypes[".svg"]="image/svg+xml";
    mimeTypes[".xml"]="image/svg+xml";
    mimeTypes["NULL"]="NULL";
}

Header::Header()
{
    clear();
}

Header::Header(string header)
{
    set(header);
}

void Header::clear()
{
    header.str("");
}

string Header::generate()
{
    return header.str();
}

bool Header::getType(string &type)
{
    string str = generate();
    size_t npos;
    npos = str.find(" ");
    if(npos == string::npos)
        return false;
    type = str.substr(0,npos);
    return true;
}

bool Header::getPath(string &path)
{
    string str = generate();
    size_t pos,npos;
    //pos = str.find_first_of(" ");
    pos = str.find(" ");
    if(pos == string::npos)
        return false;


    npos = str.find(" ",++pos);
    if(npos == string::npos)
        return false;
//    path=".";
    path = str.substr(pos,npos-pos);
    
    cout << path << "\n";
    return true;
}

void Header::set(string header)
{
    this->header << header;

}

void Header::set(const char *header)
{
    string s(header);
    set(s);
}

void Header::addReply(int reply)
{
    header << "HTTP/1.1 "<< reply << " " << Http::getDescription(reply)<< CRLF;

}




void Header::addType(const char *path)
{

    
    cout <<"***"<< get_mime_type(path) << "***"<< endl;
    header << "Content-type: "<<get_mime_type(path) << CRLF;
    header << "Cache-Control: no-cache" << CRLF;


}

void Header::addLength(size_t len)
{
    header << "Content-length: " << len << CRLF;

}


void Header::addDate(void)
{
    time_t rawtime;
    struct tm *timeinfo;
    char date[128];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(date,sizeof(date),"%a,%d %b %Y %X %Z",timeinfo);
    header << "Date: " << date << CRLF;

}

void Header::addServer()
{
    header<< "Server: " << APP_NAME << "/" << APP_VERSION << CRLF;
}

void Header::addEnd()
{
    header << CRLF;
}

string Header::get_mime_type(const char *path)
{
    
    mimeTypes[".xhtml"]="application/xhtml+xml";
    mimeTypes[".txt"] = "text/plain";
    mimeTypes[".rtf"] = "application/rtf";
    mimeTypes[".word"]="application/msword";
    mimeTypes[".au"]="audio/basic";
    mimeTypes[".mpeg"]="video/mpeg";
    mimeTypes[".mpg"]="video/x-msvideo";
    mimeTypes[".avi"]="video/x-msvideo";
    mimeTypes[".gz"]="application/x-gzip";
    mimeTypes[".tar"]="application/x-tar";
    mimeTypes[".css"]="text/css";
    mimeTypes[".gif"]="image/gif";
    mimeTypes[".htm"]="text/html";
    mimeTypes[".html"]="text/html";
    mimeTypes[".jpeg"]="image/jpeg";
    mimeTypes[".jpg"]="image/jpeg";
    mimeTypes[".ico"]="image/x-icon";
    mimeTypes[".js"] = "application/javascript";
    mimeTypes[".pdf"]="application/pdf";
    mimeTypes[".mp4"]="video/mp4";
    mimeTypes[".png"]="image/png";
    mimeTypes[".svg"]="image/svg+xml";
    mimeTypes[".xml"]="image/svg+xml";
    mimeTypes[".NULL"]="NULL";


    const char *dot = strrchr(path,'.');
    printf("%s\n",dot);

    cout << mimeTypes[string(dot)] << "**11**" << endl;


    if(mimeTypes.find(string(dot))!=mimeTypes.end())
    {
        cout << "223" << endl;
        return mimeTypes[string(dot)];
    }
    else
        return string("text/plain");
}
