%{
#include <iostream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "pxml.parser.hh"  // to get the token types that we return
#include "definitions.hh"
using namespace std;

// stuff from flex that bison needs to know about:
extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;

const char tag_track[] = "track";
const char tag_title[] = "title";
const char tag_loc[] = "location";






void yyerror(const char *msg);
%}


%union{
	char* str;
	char cval;
}

%token <cval> LETTER
%token <cval> DIGIT
%token <cval> EQ
%token <cval> PARANTH
%token <cval> WHITE
%token <cval> OTHER
%token COMMENT_START COMMENT_END
%token NORMAL_START NORMAL_END
%token NORMAL_END_START
%token EMPTY_END
%token PROCESSING_START PROCESSING_END

%type <str> name
%type <str> content
%type <str> value
%type <str> attribute
%type <str> normal_tag_start
%type <str> whites

%%

xml: processing_tag root

whites: {$$ = (char*)malloc(sizeof(char)); $$[0] = '\0';}
	| whites WHITE {$$=0; char w[2]={0,}; w[0] = $2; str_add(&$$, $1); str_add(&$$, w); if($1) free($1);}
	| WHITE {$$=0; char w[2]={0,}; w[0] = $1; str_add(&$$, w);}
	;


single_tag: whites comment_tag whites
	| whites empty_tag whites
	| whites processing_tag whites
	;

root: whites normal_tag_start nodes normal_tag_end whites{
		if(strcmp($2, tag_track) == 0)
		{
            char* tit = (char*)malloc(xtrack.title.length()+1);
            char* loc = (char*)malloc(xtrack.location.length()+1);
            decode_html_entities_utf8(tit, xtrack.title.c_str());
            decode_html_entities_utf8(loc, xtrack.location.c_str());
            xpl.add_track(tit, loc);
            free(tit);
            free(loc);
		}
	}
	| whites normal_tag_start content normal_tag_end whites{
        if(strcmp($2, tag_title) == 0)
            xtrack.title = $3;
        else if(strcmp($2, tag_loc) == 0)
            xtrack.location = $3;
	}
	| whites normal_tag_start normal_tag_end whites{
        if(strcmp($2, tag_title) == 0)
            xtrack.title = "";
        else if(strcmp($2, tag_loc) == 0)
            xtrack.location = "";
	}
	;


nodes: nodes single_tag
	| nodes root
	| single_tag
	| root
	;

name: name LETTER {$$=0; char w[2]={0,}; w[0] = $2; str_add(&$$, $1); str_add(&$$, w); if($1) free($1);}
	| name DIGIT {$$=0; char w[2]={0,}; w[0] = $2; str_add(&$$, $1); str_add(&$$, w); if($1) free($1);}
	| name OTHER {$$=0; char w[2]={0,}; w[0] = $2; str_add(&$$, $1); str_add(&$$, w); if($1) free($1);}
	| LETTER {$$=0; char w[2]={0,}; w[0] = $1; str_add(&$$, w);}
	| OTHER {$$=0; char w[2]={0,}; w[0] = $1; str_add(&$$, w);}
	;

content: content LETTER {$$=0; char w[2]={0,}; w[0] = $2; str_add(&$$, $1); str_add(&$$, w); if($1) free($1);}
	| content DIGIT {$$=0; char w[2]={0,}; w[0] = $2; str_add(&$$, $1); str_add(&$$, w); if($1) free($1);}
	| content EQ {$$=0; char w[2]={0,}; w[0] = $2; str_add(&$$, $1); str_add(&$$, w); if($1) free($1);}
	| content PARANTH {$$=0; char w[2]={0,}; w[0] = $2; str_add(&$$, $1); str_add(&$$, w); if($1) free($1);}
	| content WHITE {$$=0; char w[2]={0,}; w[0] = $2; str_add(&$$, $1); str_add(&$$, w); if($1) free($1);}
	| content OTHER {$$=0; char w[2]={0,}; w[0] = $2; str_add(&$$, $1); str_add(&$$, w); if($1) free($1);}
	| LETTER {$$=0; char w[2]={0,}; w[0] = $1; str_add(&$$, w);}
	| DIGIT {$$=0; char w[2]={0,}; w[0] = $1; str_add(&$$, w);}
	| EQ {$$=0; char w[2]={0,}; w[0] = $1; str_add(&$$, w);}
	| PARANTH {$$=0; char w[2]={0,}; w[0] = $1; str_add(&$$, w);}
	| WHITE {$$=0; char w[2]={0,}; w[0] = $1; str_add(&$$, w);}
	| OTHER {$$=0; char w[2]={0,}; w[0] = $1; str_add(&$$, w);}
	;

attribute: attribute whites name EQ PARANTH value PARANTH whites{
		xctx.curr_str="";
		xctx.curr_str+=$1;
		xctx.curr_str+=$2;
		xctx.curr_str+=$3;
		xctx.curr_str+=$4;
		xctx.curr_str+=$5;
		xctx.curr_str+=$6;
		xctx.curr_str+=$7;
		xctx.curr_str+=$8;
		if($1)
			free($1);
		if($2)
			free($2);
		if($3)
			free($3);
		if($6)
			free($6);
		if($8)
			free($8);
		$1=$2=$3=$6=$8=0;
		$$ = (char*)malloc(xctx.curr_str.length()+1);
		strcpy($$, xctx.curr_str.c_str());
	}
	| whites name EQ PARANTH value PARANTH whites{
		xctx.curr_str="";
		xctx.curr_str+=$1;
		xctx.curr_str+=$2;
		xctx.curr_str+=$3;
		xctx.curr_str+=$4;
		xctx.curr_str+=$5;
		xctx.curr_str+=$6;
		xctx.curr_str+=$7;
		if($1)
			free($1);
		if($2)
			free($2);
		if($5)
			free($5);
		if($7)
			free($7);
		$1=$2=$5=$7=0;
		$$ = (char*)malloc(xctx.curr_str.length()+1);
		strcpy($$, xctx.curr_str.c_str());
	}
	;

value: value LETTER {$$=0; char w[2]={0,}; w[0] = $2; str_add(&$$, $1); str_add(&$$, w); if($1) free($1);}
	| value DIGIT {$$=0; char w[2]={0,}; w[0] = $2; str_add(&$$, $1); str_add(&$$, w); if($1) free($1);}
	| value EQ {$$=0; char w[2]={0,}; w[0] = $2; str_add(&$$, $1); str_add(&$$, w); if($1) free($1);}
	| value WHITE {$$=0; char w[2]={0,}; w[0] = $2; str_add(&$$, $1); str_add(&$$, w); if($1) free($1);}
	| value OTHER {$$=0; char w[2]={0,}; w[0] = $2; str_add(&$$, $1); str_add(&$$, w); if($1) free($1);}
	| LETTER {$$=0; char w[2]={0,}; w[0] = $1; str_add(&$$, w);}
	| DIGIT {$$=0; char w[2]={0,}; w[0] = $1; str_add(&$$, w);}
	| EQ {$$=0; char w[2]={0,}; w[0] = $1; str_add(&$$, w);}
	| WHITE {$$=0; char w[2]={0,}; w[0] = $1; str_add(&$$, w);}
	| OTHER {$$=0; char w[2]={0,}; w[0] = $1; str_add(&$$, w);}
	;

comment_tag: COMMENT_START content COMMENT_END {
		//cout<< "[" << xctx.line << "]	" << "Found comment tag::"<< $2 << endl;
		if($2)
			free($2);
		$2=0;
	}
	;

empty_tag: NORMAL_START name EMPTY_END {
		//cout<< "[" << xctx.line << "]	" << "Found empty tag:: name = "<< $2 << endl;
		if($2)
			free($2);
		$2 = 0;

	}
	| NORMAL_START name attribute EMPTY_END {
        //cout<< "[" << xctx.line << "]	" << "Found empty tag:: name = <" << $2 << "> attributes =" << $3 << endl;
		if($2)
			free($2);
		if($3)
			free($3);
		$2=$3=0;

	}
	;

processing_tag: PROCESSING_START content PROCESSING_END {
		//cout<< "[" << xctx.line << "]	" << "Found processing tag:: "<< $2 << endl;
		if($2)
			free($2);
		$2=0;
	}
	;

normal_tag_start: NORMAL_START name NORMAL_END {
		//cout<< "[" << xctx.line << "]	" << "Found normal tag start:: name = <" << $2 << ">" << endl;
		push_stack(&xctx.stack, $2);

		$$=$2;
	}
	| NORMAL_START name attribute NORMAL_END {
		//cout<< "[" << xctx.line << "]	" << "Found normal tag start:: name = <" << $2 << "> attributes = " << $3 <<  endl;
		push_stack(&xctx.stack, $2);

		if($3)
			free($3);
		$3=0;
		$$=$2;
	}
	;

normal_tag_end:	NORMAL_END_START name NORMAL_END {
		if(!(close_valid(&xctx.stack, $2) == 1))
		{
			free($2); $2=0;
			yyerror("Tags not properly nested!");
		}
		//cout<< "[" << xctx.line << "]	" << "Found normal tag end name = <" << $2 << ">" << endl;
		if($2)
			free($2);
		$2=0;
	}
	;

%%

void yyerror(const char *msg) {
	cerr << "Error<" << xctx.line << ">" << "<" << xctx.curr_str << ">"<< " " << msg << endl;
	exit(-1);
}

