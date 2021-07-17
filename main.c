#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "farbfeld.h"

enum {
	LineSize = 512,
};

struct Color {
	char *c;
	Pixel p;
} ;
typedef struct Color Color;


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
wrongfmt(void)
{
	fprintf(stderr, "%s: wrong format\n", argv0);
	exit(1);
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
	pbuf = 0 ;
		
	return buf ;
}

int
main(int argc, char *argv[])
{
	char *pstr;
	char tokbuf[64];
	int n;
	argv0 = argv[0] ;
	if(argc>1) usage() ;

	infile = stdin ;

	if(!fgets( curline, sizeof(curline), infile) )
		wrongfmt();
	width = atoi(gettok(tokbuf, curline, 0)) ;
	height = atoi(gettok(tokbuf, curline, 1)) ;
	nchars = atoi(gettok(tokbuf, curline, 2)) ;
	ncolors = atoi(gettok(tokbuf, curline, 3)) ;

	if(!(width && height && nchars && ncolors))
		wrongfmt();

	printf("%d %d %d %d\n", width, height, nchars, ncolors);
	
	return 0 ;
}

