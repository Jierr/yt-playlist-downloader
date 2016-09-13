#include "iniparser.hpp"

#include <iostream>

Initier::Initier()
{
    cmd = "";
    line = "";
    type = "";
    value = "";
//    cmd="vlc -I --no-interact --play-and-exit --network-caching=3000 \"%LOCATION%\" \
//    --no-sout-video --sout #transcode{acodec=mp2a,ab=192,scale=1,channels=2,deinterlace,audio-sync}:std{access=file,mux=ps,dst=\"%TITLE%\"} vlc://quit";
}

const string& Initier::get_cmd()
{
    return cmd;
}

const string& Initier::get_type()
{
    return type;
}

int Initier::get_line(char* buffer, unsigned long& pos)
{
    int c = 0;
    int d = 0;
    int stop = 0;
    int token = INI_NONE;
    string ident = "";
    const string ini_cmd = "CMD=";
    const string ini_type = "TYPE=";
    const string ini_com = "//";
    string delim = "\n";
    this->line = "";
    this->value = "";
    c = pos;




    while(buffer[c] && !stop)
    {
        d = 0;
        while (d<delim.length())
        {
            if(buffer[c] == delim[d])
                stop = 1;
            ++d;
        }

        if(!stop)
        {
            this->line += buffer[c];
            if(token==INI_NONE)
            {
                ident+=buffer[c];
                if (ident.compare(ini_com)==0)
                    token = INI_COMMENT;
                else if(ident.compare(ini_cmd)==0)
                    token = INI_ASSIGN_CMD;
                else if(ident.compare(ini_type)==0)
                    token = INI_ASSIGN_TYPE;
            }
            else if(token==INI_COMMENT      ||
                    token==INI_ASSIGN_CMD   ||
                    token==INI_ASSIGN_TYPE )
                this->value+=buffer[c];

        }
        ++c;
    }

    pos=c;
    return token;
}

int Initier::retrieve_data(const string& ini_filename)
{
    FILE* ini = 0;
    unsigned long bytes = 0;
    unsigned long len = 1;
    unsigned long pos = 0;
    char* buffer = 0;
    int token = 0;
    int assignments = 0;

    ini = fopen(ini_filename.c_str(), "r");
    if(!ini)
        return 0;

    fseek(ini, 0, SEEK_END);
    bytes = ftell(ini);
    rewind(ini);

    buffer = (char*)malloc((bytes+1) * sizeof(char));

    fread(buffer, bytes, 1, ini);
    buffer[bytes] = 0;
    fclose(ini);

    len = strlen(buffer);
    while(pos < len)
    {
        token = this->get_line(buffer, pos);
        //cerr<< "DEBUG<Initier>::[" << pos << "]" << this->line << endl;
        if(token==INI_ASSIGN_CMD)
        {
            this->cmd = this->value;
            ++assignments;
        }
        else if(token==INI_ASSIGN_TYPE)
        {
            this->type = this->value;
            ++assignments;
        }
    }
    free(buffer);
    return assignments;
}


int Initier::include_title(string& proc, const string& title)
{
    const string ident="%TITLE%";
    string sub = "";
    int len = 0;
    unsigned int last = -1;
    unsigned int pos = 0;
    len = ident.length();

    while(pos < proc.length())
    {
        sub = proc.substr(pos, len);
        if(ident.compare(sub)==0)
        {
            proc.erase(pos, len);
            proc.insert(pos, title);
            last = pos;
            pos += title.length()-1;
        }
        ++pos;
    }

    return last;
}

int Initier::include_location(string& proc, const string& location)
{
    const string ident="%LOCATION%";
    string sub = "";
    int len = 0;
    unsigned int last = -1;
    unsigned int pos = 0;
    len = ident.length();

    while(pos < proc.length())
    {
        sub = proc.substr(pos, len);
        if(ident.compare(sub)==0)
        {
            proc.erase(pos, len);
            proc.insert(pos, location);
            last = pos;
            pos += location.length()-1;
        }
        ++pos;
    }

    return last;
}
