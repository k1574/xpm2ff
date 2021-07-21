#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ff/ff.h>

int toknum(char *s);

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
static u16 width, height, nchars, ncolors;

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
clridx(char *s)
{
	int i;
	for(i=0 ; i<ncolors ; ++i){
		if(!strcmp(colors[i].c, s))
			return i;
	}

	return -1 ;
}

int
main(int argc, char *argv[])
{
	char *pstr;
	u32 ww, wh;
	char tokbuf[64];
	int i, j, len, idx;
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

	ww = width ;
	wh = height ;

	colors = malloc(sizeof(Color) * ncolors) ;
	for(i=0 ; i<ncolors ; ++i){
		if(!rdline() || toknum(curline) != 3){
			wrongfmt("color");
		}
		pstr = curline + nchars ;

		colors[i].c = malloc(nchars+1) ;
		memcpy(colors[i].c, curline, nchars);
		colors[i].c[nchars] = 0 ;

		ff_colorname_to_pixel(&colors[i].p, gettok(tokbuf, curline, 2));
	}

	if(ff_is_little_endian()){
		ff_swap_endian(&ww, 4);
		ff_swap_endian(&wh, 4);
		for(i=0 ; i<ncolors ; ++i)
			ff_swap_pixel_endian(&colors[i].p);
	}

	printf("farbfeld");
	fwrite(&ww, sizeof(ww), 1, stdout);
	fwrite(&wh, sizeof(wh), 1, stdout);

	for(i=0 ; i<height ; ++i){
		if(!rdline())
			wrongfmt("unexpected EOF");

		len = strlen(curline) ;
		if(len > nchars * width && curline[len -1]!='\n')
			wrongfmt("pixel line");

		for(j=0 ; j<width ; ++j){
			memcpy(tokbuf, &curline[j*nchars], nchars);
			tokbuf[nchars] = 0 ;
			if((idx = clridx(tokbuf)) < 0)
				wrongfmt("pixel");
			fwrite(&colors[idx].p, sizeof(colors[idx].p), 1, stdout);
		}
	}
	
	return 0 ;
}

