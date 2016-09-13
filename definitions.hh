#ifndef __DEFINITIONS__
#define __DEFINITIONS__

#define _UNICODE
#include <string>
#include <cstring>
#include <cstdlib>
#include <entities.hpp>
#include <utf8.h>
#include "playlist.hpp"
#include "pxml.parser.hh"

using namespace std;

typedef struct tag_stack_elem
{
	char* name;
	struct tag_stack_elem* prev;
} tag_stack_elem_t;

typedef struct tag_stack
{
	int count;
	tag_stack_elem_t* top;
} tag_stack_t;



typedef struct xml_ctx {
	string curr_str;
	int line;
	tag_stack_t stack;
} xml_ctx_t;

typedef struct track {
    string title;
    string location;
} track_t;



int str_add(char** dst, char* add);
void construct_valid_filename(string& name);

void init_stack(tag_stack_t* stack);
void clean_stack(tag_stack_t* stack);
tag_stack_elem_t* pop_stack(tag_stack_t* stack);
int close_valid(tag_stack_t* stack, char* name);
int push_stack(tag_stack_t* stack, char* name);
void free_stack_elem(tag_stack_elem_t* elem);

void init_xml_ctx(xml_ctx_t* ctx);
void reset_xml_ctx(xml_ctx_t* ctx);


extern "C" FILE *yyin;
extern "C" int yyparse();
extern "C" xml_ctx_t xctx;
extern "C" track_t xtrack;
extern "C" Playlist xpl;

#endif // __DEFINITIONS__
