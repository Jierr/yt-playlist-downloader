#include <iostream>
#include <sstream>
#include <cstdio>
#include <entities.h>
#include <windows.h>

#include <direct.h>

#include "definitions.hh"
#include "iniparser.hpp"
#include "playlist.hpp"



using namespace std;





int main(int argc, char** argv)
{
    Initier inip;
    const string ini_name="config.ini";
    string title = "";
    string location = "";
    string tmp = "";
    string name = "";
    const unsigned long minimal_size = 100000;
    const int min_tries = 5;
    int fail = 0;
    int success = 0;
    int all_fail = 0;
    int all_success = 0;
    int all = 0;
    unsigned long vsize = 0;
    FILE* out = 0;
    int pos = 0;

    string::iterator end_it;
    vector<unsigned short> utf16line;

    int c = 0;
    wchar_t* wnew = 0;
    wchar_t* wold = 0;
    int len = 0;
    string u8str;

    stringstream ss;
    string cmd="";
    string type="";

    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );

	if(argc == 1)
	{
		cerr<< "Usage: " << argv[0] << " <filename>" << endl;
		return -1;
	}

	FILE *file = fopen(argv[1], "r");
	if (!file) {
		cerr << "Can't open "<< argv[1] << "!" << endl;
		return -1;
	}
	yyin = file;
    tmp = "";
    pos = 0;
    while(argv[1][pos])
    {
        if(argv[1][pos] == '.')
        {
            name+=tmp;
            tmp="";
        }
        tmp+=argv[1][pos];
        ++pos;
    }
    if (name.length()==0 && tmp.length()!=0)
        name = tmp;
    else if (name.length()==0)
        name = "Downloads";

	xpl.set_name(name);

	init_xml_ctx(&xctx);
	do {
		yyparse();
	} while (!feof(yyin));
	reset_xml_ctx(&xctx);

    all = xpl.get_count();
    cout<< "Playlist Name \"" << xpl.get_name() << "\"" << endl << "Anzahl Tracks: " << xpl.get_count() << endl;
    _mkdir(xpl.get_name().c_str());
    xpl.first();
    pos = 1;

    inip.retrieve_data(ini_name);
    do
    {
        xpl.construct_valid_filename();
        if(xpl.get_track(title, location))
        {
            //construct_valid_filename(title);
            title = xpl.get_name() + "\\" + title;
            cout<< "[" << pos << "/" << all << "]====================" << endl
                << "    Title=" << title << endl
                << "    Location=" << location << endl;

//            out = _wfopen(u16str, L"w, ccs=UTF-8");
//            if(out)
//                fclose(out);

            ss.str("");
//            ss<< "vlc " << "-I --no-interact --play-and-exit --network-caching=3000 "
//              <<  "\"" << location << "\"" << " --no-sout-video" << " --sout"
//              <<  " #transcode{acodec=mp2a,ab=192,scale=1,channels=2,deinterlace,audio-sync}:std{access=file,mux=ps,dst=\""
//              << xpl.get_name() << "\\##vlc_track" << pos << ".mpg\"}" << " vlc://quit";

            type = inip.get_type();
            ss << xpl.get_name() << "\\##vlc_track" << pos << "." << type;
            name = ss.str();
            cmd = inip.get_cmd();
            if(type.length() == 0) type = "mpg";
            inip.include_title(cmd, name);
            inip.include_location(cmd, location);
            //cerr<<"<DEGUB<command>::" << cmd << endl;

            fail = 0;
            success = 0;

            name = title;
            name += "." + type;
            u8str = name;
            end_it = utf8::find_invalid(u8str.begin(), u8str.end());
            if (end_it != u8str.end()) {
                cout << "Invalid UTF-8 encoding detected" << endl;
                cout << "This part is fine: " << string(u8str.begin(), end_it) << endl;
            }
            utf16line.clear();
            utf8::utf8to16(u8str.begin(), end_it, back_inserter(utf16line));

            len = utf16line.size();
            wnew = new wchar_t[len+1];
            for(c=0; c< len; ++c)
            {
                wnew[c]=utf16line[c];
            }
            wnew[len]=0;

            out = _wfopen(wnew, L"r");
            if(out)
            {
                success = 1;
                fclose(out);
                cout<< "SUCCESS: Already exists!" << endl;
                ++all_success;
            }

            while((fail < min_tries) && !success)
            {
                ss.str("");
                ss<< xpl.get_name() << "\\##vlc_track" << pos << "." << type;
                name = ss.str();

                // Start the child process.
                if( !CreateProcess( NULL,   // No module name (use command line)
                    (LPSTR)cmd.c_str(),        // Command line
                    NULL,           // Process handle not inheritable
                    NULL,           // Thread handle not inheritable
                    FALSE,          // Set handle inheritance to FALSE
                    0,              // No creation flags
                    NULL,           // Use parent's environment block
                    NULL,           // Use parent's starting directory
                    &si,            // Pointer to STARTUPINFO structure
                    &pi )           // Pointer to PROCESS_INFORMATION structure
                )
                {
                    printf( "CreateProcess failed (%d).\n", GetLastError() );
                    return -1;
                }

                // Wait until child process exits.
                WaitForSingleObject( pi.hProcess, INFINITE );

                // Close process and thread handles.
                CloseHandle( pi.hProcess );
                CloseHandle( pi.hThread );


                //out = _wfopen((wchar_t*)u16str, L"r");
                Sleep(1000);
                out = fopen(name.c_str(), "r");
                vsize = 0;
                if(out)
                {
                    fseek(out, 0, SEEK_END);
                    vsize = ftell(out);
                    fclose(out);
                    if (vsize < minimal_size)
                    {
                        vsize = 0;
                        DeleteFileA(name.c_str());
                    }
                }
                if(vsize >= minimal_size)
                {
    //                out = _wfopen(wnew, L"w, ccs=UTF-8");
    //                if(out)
    //                    fclose(out);

                    u8str = name;
                    end_it = utf8::find_invalid(u8str.begin(), u8str.end());
                    if (end_it != u8str.end()) {
                        cout << "Invalid UTF-8 encoding detected" << endl;
                        cout << "This part is fine: " << string(u8str.begin(), end_it) << endl;
                    }
                    utf16line.clear();
                    utf8::utf8to16(u8str.begin(), end_it, back_inserter(utf16line));

                    len = utf16line.size();
                    wold = new wchar_t[len+1];
                    for(c=0; c< len; ++c)
                    {
                        wold[c]=utf16line[c];
                    }
                    wold[len]=0;

                    _wrename(wold, wnew);

                    delete[] wold;
                    success = 1;
                    cout<< "SUCCESS: size = " << vsize/1024 << "KiB" << endl;
                    ++all_success;

                }
                else
                {
                    ++fail;
                    cout<< "FAIL:" << name << "| size=" << vsize << endl;
                    if(fail == min_tries)
                        ++all_fail;
                }
            }

            delete[] wnew;

        }
        ++pos;
    } while(xpl.next());


    cout<< "Results:" << endl << all << " Files" << endl
        << all_success << " succeeded..." << endl
        << all_fail << " failed..." << endl;
    //cin >> pos;

	return 0;

}
