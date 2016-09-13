#ifndef __PLAYLIST__
#define __PLAYLIST__

#include <string>
#include <list>
#include <vector>

using namespace std;

class Track
{
    friend class Playlist;
private:
    string title;
    string location;
public:
    Track();
    ~Track(){};
    void construct_valid_filename();
};

class Playlist
{
private:
    string name;
    list<Track> tracks;
    list<Track>::iterator it;
public:
    Playlist();
    ~Playlist(){};
    void set_name(const string& name);
    const string& get_name();
    void add_track(const string& title, const string& location);
    void construct_valid_filename();
    void first();
    bool next();
    void last();
    bool get_track(string& title, string& location);
    int get_count();

};


#endif // __PLAYLIST__
