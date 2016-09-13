#ifndef __INI_PARSER__
#define __INI_PARSER__

#include <string>
#include <cstdio>
#include <cstdlib>
using namespace std;

enum {INI_NONE=0, INI_COMMENT, INI_ASSIGN_CMD, INI_ASSIGN_TYPE};

class Initier{
private:
    string cmd;
    string type;

    string line;
    string value;
    int get_line(char* buffer, unsigned long& pos);
public:
    Initier();
    ~Initier(){};
    int retrieve_data(const string& ini_filename);
    int include_title(string& proc, const string& title);
    int include_location(string& proc, const string& location);
    const string& get_cmd();
    const string& get_type();

};


#endif // __INI_PARSER__
