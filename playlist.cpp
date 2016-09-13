#include "playlist.hpp"


void Playlist::add_track(const string& title, const string& location)
{
    Track t;
    t.title = title;
    t.location = location;


    tracks.push_back(t);
    it = tracks.end();
}

void Playlist::set_name(const string& name)
{
    this->name = name;
}

Playlist::Playlist()
{
    name = "";
}


void Playlist::first()
{
    it = tracks.begin();
}

bool Playlist::next()
{
    if (it != tracks.end())
    {
        ++it;
        return 1;
    }

    return 0;
}

void Playlist::last()
{
    it = tracks.end();
    if(!tracks.empty())
        --it;
}

bool Playlist::get_track(string& title, string& location)
{
    if(tracks.empty() || (it == tracks.end()))
        return 0;

    title = (*it).title;
    location = (*it).location;

    return 1;
}

int Playlist::get_count()
{
    return tracks.size();
}

const string& Playlist::get_name()
{
    return name;
}

void Playlist::construct_valid_filename()
{
    if(tracks.empty() || (it == tracks.end()))
        return;

    it->construct_valid_filename();
}

Track::Track()
{
    title = "";
    location = "";
}

void Track::construct_valid_filename()
{
    int c = 0;
    int v = 0;
    int invalid = 0;
    const char file_invalid[] = "\\/:*?\"<>|";

    while(title[c])
    {
        v = 0;
        invalid = 0;
        while(file_invalid[v] && !invalid)
        {
            if(title[c] == file_invalid[v])
            {
                title[c] = '-';
                invalid = 1;
            }
            ++v;
        }
        ++c;
    }
}
