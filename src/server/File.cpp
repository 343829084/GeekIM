#include"common.h"

string File::basepath = ".";

string File::getPath(void)
{
    return basepath;

}

void File::setPath(const string dirpath)
{
    basepath = dirpath;

}
bool File::open(const string filepath)
{
    
    const char *s = filepath.c_str();
    return open(s);
   

}

bool File::open(const char *filepath)
{
    stringstream spath;
    const char *fpath;
    close();
    spath<< basepath<<filepath;
    fpath = spath.str().c_str();

    file.open(fpath);
    return file.is_open();

}

void File::close()
{
    file.close();

}

size_t File::size()
{
    size_t pos,size;
    pos = file.tellg();
    file.seekg(0,ios::end);
    size = file.tellg();
    file.seekg(pos,ios::beg);
    return size;
}

bool File::read(void *buffer,size_t len)
{
    char *ptr = static_cast<char *>(buffer);

    file.read(ptr,len);
    return true;  

}

bool File::write(void *buffer,size_t len)
{
    char *ptr = static_cast<char *> (buffer);
    file.write(ptr,len);
    return true;
}

void File::type(const char *filepath,struct stat *sbuf)
{
    string str(basepath);
    str+=filepath;
    stat(str.c_str(),sbuf);
    return ;

}
