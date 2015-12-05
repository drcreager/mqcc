//---------------------------------------------------------------------+
//  File:     isdec.cpp
//  Author:   Chris A. Hamilton
//  Written:  January 1999
//
//  Desc:     Contains a function to see if a string is numeric
//  
//  Dev Log:  Chris Hamilton had this embedded in a class, but I 
//            decided to pull it out so I could use it in several
//            classes.  (DMT)
//  History: 
//  01/15/99 CAH Created original version.
//  02/20/99 DMT Added standard comment blocks
//---------------------------------------------------------------------+
#include "isdec.h"

//---------------------------------------------------------------------+
// A global C style function to check for a real decimal only string.
//---------------------------------------------------------------------+
extern "C"
{
BOOL isdec(const char* str)
{
	if (NULL == str) {
		return FALSE;
	}
	else {
		return strlen(str) == strspn(str, "0123456789");
	}
}
} // End extern "C"