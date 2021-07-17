/* Simple check for byte order in numbers. */

int
islittleendian(void)
{
	int n = 1 ;
	if( *(char*)&n == 1) return 1 ;
	else return 0 ;
}

void
swapendian(void *data, unsigned int size)
{
	unsigned int i, j;
	char buf;
	for( i=0, j=size-1 ; i<j; ++i, --j ){
		buf = ((char*)data)[i] ;
		((char*)data)[i] = ((char*)data)[j] ;
		((char*)data)[j] = buf ;
	}
}

