#include"common.h"



Processor::Processor(Socket *socket)
{
    this->socket = socket;

}

Processor::~Processor()
{
    disconnect();
    delete socket;

}

void Processor::disconnect()
{
    socket->destroy();

}

size_t Processor::read(void *buffer,size_t len)
{
    return socket->receive(buffer,len);

}

size_t Processor::write(const void *buffer, size_t len)
{
    unsigned char *ptr = (unsigned char *) buffer;
    size_t cnt = 0;
    int res;

    while(cnt<len){
        res = socket->send(ptr+cnt,len-cnt);
        if(res == -1)
            return res;
        if(res == 0)
            break;
        cnt+=res;

    }

    return cnt;

}


bool Processor::send(const void *buffer,size_t len,int reply)
{
    Header header;
    size_t res;
    bool ret;

    header.addReply(reply);
    header.addDate();
    header.addServer();
    if(len>0) header.addLength(len);
    header.addEnd();

    ret = sendHeader(header);
    if(!ret)
        return false;
    if(len>0){
        res = write(buffer,len);
        if(res<=0)
            return false;
    }    
    return true;
}

bool Processor::sendHeader(Header &header)
{
    string hdr;
    size_t len,res;
    hdr = header.generate();
    len = hdr.length();
    res = write(hdr.c_str(),len);
    if(res != len)
        return false;
    return true;
}

bool Processor::sendFile(string filepath)
{
    const char *s  = filepath.c_str();
    return sendFile(s);

}

bool Processor::sendRegFile(const char *filepath,File &file,Header &header)
{

    char buffer[BUF_MAXLEN];
    size_t len;
    bool res = file.open(filepath);


    if(!res)
    {
        handleError(REPLY_NOENT);
        goto out;

    }

    len = file.size();
    if(!len){
        handleError(REPLY_INTERR);
        res = false;
        goto out;
    }

    header.addReply(REPLY_OK);
    header.addDate();
    header.addServer();
    header.addLength(len);
    header.addType(filepath);
    header.addEnd();

    res = sendHeader(header);

    if(!res)
        goto out;
    while(len){
        size_t ret,size;

        size = (len>BUF_MAXLEN) ? BUF_MAXLEN:len;
        res = file.read(buffer,size);
        if(!res)
            break;
        ret = write(buffer,size);
        if(ret!=size){
            res = false;
            break;
        }

        len -= size;
    }
out:
    file.close();
    return res;
}

bool Processor::sendDir(const char *filepath,File &file,Header &header)
{

    bool res = true;

    const char *buffer;
    int len;
    struct stat sbuf;
    string data;
    file.type(filepath,&sbuf);

    if(!res)
    {
        handleError(REPLY_NOENT);
        goto out;

    }

    len = sbuf.st_size;
    cout << len << "***"<<endl;
    if(!len){
        handleError(REPLY_INTERR);
        res = false;
        goto out;
    }

    header.addReply(REPLY_OK);
    header.addDate();
    header.addServer();
    header.addLength(len);
    header.addEnd();

    res = sendHeader(header);

    if(!res)
        goto out;
  //  while(len){
   //     size_t ret,size;

    //    size = (len>BUF_MAXLEN) ? BUF_MAXLEN:len;
     //   res = file.read(buffer,size);

        //if(!res)
         //   break;
    size_t size;
        data = Page::dirPage(filepath);
        buffer = data.c_str();
        size = data.length();
        res = write(buffer,size);
     //   if(ret!=size){
      //      res = false;
       //     break;
        //}

        //len -= size;
   // }
    
   // file.close();
out:

    return res;

}




bool Processor::sendFile(const char *filepath)
{
    File file;
    Header header;

    struct stat sbuf;
    
    char buf[256];
    char *path = getcwd(buf,256);
    file.setPath(path);
    file.type(filepath,&sbuf);

    if(S_ISREG(sbuf.st_mode))
    {
        return sendRegFile(filepath,file,header);

    }
    if(S_ISDIR(sbuf.st_mode))
    {
        return sendDir(filepath,file,header);
    }


    return false;





}



bool Processor::recvRequest(Header &header)
{
    char *buffer;
    size_t res;
    buffer = new char [HDR_MAXLEN];

    if(!buffer)
        return false;
    res = read(buffer,HDR_MAXLEN);
    if(res>0)
        header.set(buffer);

    delete buffer;

    return (res>0)? true : false;

}


bool Processor::handleError(int reply)
{
    const char *buffer;
    size_t len;
    
    string data;

    data = Page::get(reply);
    buffer = data.c_str();
    len = data.length();

    return send(buffer,len,reply);
}
