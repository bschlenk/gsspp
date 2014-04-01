#include "gsspp/gssbuffer.h"
#include <string>
#include <sstream>
#include <cstring>
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
	GSSBuffer buff;
	assert( buff.size() == 0 );

	const char * cTestStr = "hello test";
	buff.assign( cTestStr );

	assert( buff.size() == strlen( cTestStr ) );
	assert( 0 == memcmp( buff.bytes(), cTestStr, buff.size() ) );
	// assert( strcmp( buff.get<char *>(), testStr ) );
	
	string testStr( cTestStr );

	GSSBuffer testStrBuff( testStr );
	assert( buff == testStrBuff );

	testStr += " number 2";
	testStrBuff = testStr;
	assert( buff < testStrBuff );

	stringstream ss( testStr );
	GSSBuffer ssBuff( ss, testStr.size() / 2 );
	assert( ssBuff == testStr.substr( 0, testStr.size() / 2 ) );

	cout << ssBuff << endl;
	return 0;
}
