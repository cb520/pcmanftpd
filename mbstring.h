#ifndef __PCMAN_MBSTRING_INCLUDED__
#define __PCMAN_MBSTRING_INCLUDED__
#include <mbstring.h>

inline char mbslastchr( const char* pstr )
{
	while( *pstr )
	{
		if( !*(pstr+1) )
			return *pstr;
		pstr = (const char*)_mbsinc((unsigned char*)pstr);
	}
	return 0;
}

#endif