#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ff/ff.h>

#define DEBUG 1
#define D if(DEBUG)

enum {
	LineSize = 512,
};

struct Color {
	char *c;
	FFPixel p;
} ;
typedef struct Color Color;

static Color *colors;

static char *argv0;
static FILE *infile;
static char curline[LineSize];
static int width, height, nchars, ncolors;

void
usage(void)
{
	fprintf(stderr, "usage: %s\n", argv0);
	exit(1);
}

void
wrongfmt(char *s)
{
	fprintf(stderr, "%s: wrong format: %s\n", argv0, s);
	exit(1);
}

char *
rdline(void)
{
	char *ret = fgets(curline, sizeof(curline), infile);
	printf("\"%s\", toknum = %d\n", curline, toknum(curline));
	return ret ;
}

char *
gettok(char *buf, char *s, const int n)
{
	int i;
	char *pbuf = buf ;

	while(isspace(*s))
		++s;

	for(i=0 ; i<n ; ++i){
		while(!isspace(*s))
			++s;
		while(isspace(*s))
			++s;
	}

	while(!isspace(*s))
		*pbuf++ = *s++ ;
	*pbuf = 0 ;
		
	return buf ;
}

int
toknum(char *s)
{
	int n = 0 ;
	char *ps = s ;

	while(1){
		while(isspace(*ps))
			++ps;
		if(!*ps) break;
		while(!isspace(*ps) && *ps)
			++ps;
		++n;
	}

	return n ;
}

int
main(int argc, char *argv[])
{
	char *pstr;
	char tokbuf[64];
	int i;
	argv0 = argv[0] ;
	if(argc>1) usage() ;

	infile = stdin ;

	if(!rdline() || toknum(curline) != 4)
		wrongfmt("header");

	width = atoi(gettok(tokbuf, curline, 0)) ;
	height = atoi(gettok(tokbuf, curline, 1)) ;
	nchars = atoi(gettok(tokbuf, curline, 2)) ;
	ncolors = atoi(gettok(tokbuf, curline, 3)) ;

	if(!(width && height && nchars && ncolors))
		wrongfmt("header values");

	colors = malloc(sizeof(Color) * ncolors) ;
	for(i=0 ; i<ncolors ; ++i){
		if(!rdline() || toknum(curline) != 3){
			wrongfmt("color");
		}
		pstr = curline + nchars ;
		colors[i].c = malloc(nchars) ;
		ff_colorname_to_pixel(&colors[i].p, gettok(tokbuf, curline, 2));
		D printf("%d %d %d %d\n",
			colors[i].p.r,
			colors[i].p.g,
			colors[i].p.b,
			colors[i].p.a);
	}
	
	return 0 ;
}

